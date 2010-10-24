/*
 *  readImage(), writeImage()
 *
 *  readImage: Modified a bit from texture.c
 *    Read an RGB(SGI Format) image file and return a pointer to loaded image.
 *  writeImage: Rewritten from ORIGINAL
 *    Write an RGB(SGI Format) image to a file.
 *
 *  Once upon a time    Written by someone in the IHL
 *  June 9, 1995        Modified a bit by MIYATA Ryosuke
 *  Nov 20, 1997        Modified by TANIKAWA Tomohiro
 *  Feb 21, 1998        Rewritten for many platform (IRIX, WinNT and etc.)
 *                      by TANIKAWA Tomohiro
 *
 *
 *  texture.c  -  Wrtten by David Blythe, SGI
 *    texload is a simplistic routine for reading an SGI .rgb image file.
 *
 *  ORIGINAL:
 *    readimg, writeimg -  example code using image library on IRIX
 *    iopen, getrow, putrow, iclose - image library on IRIX
 *
 *				Paul Haeberli - 1984
 *
 */

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include "sgiimage.h"

typedef struct _ImageRec {
    unsigned short imagic;
    unsigned short type;
    unsigned short dim;
    unsigned short xsize, ysize, zsize;
    unsigned int min, max;
    unsigned int wasteBytes;
    char name[80];
    unsigned long colorMap;
    FILE *file;
    unsigned char *tmp;
    unsigned long rleEnd;
    unsigned int *rowStart;
    int *rowSize;
} ImageRec;

#define IMAGIC			0732

#define TYPEMASK		0xff00
#define BPPMASK			0x00ff
#define ITYPE_VERBATIM		0x0000
#define ITYPE_RLE		0x0100
#define ISRLE(type)		(((type) & 0xff00) == ITYPE_RLE)
#define ISVERBATIM(type)	(((type) & 0xff00) == ITYPE_VERBATIM)
#define BPP(type)		((type) & BPPMASK)
#define RLE(bpp)		(ITYPE_RLE | (bpp))
#define VERBATIM(bpp)		(ITYPE_VERBATIM | (bpp))

void
rgbtorgb(unsigned char *r,unsigned char *g,unsigned char *b,unsigned char *l,int n) {
    while(n--) {
        l[0] = r[0];
        l[1] = g[0];
        l[2] = b[0];
        l += 3; r++; g++; b++;
    }
}

static void
ConvertGetShort(unsigned short *array, unsigned int length) {
    unsigned short b1, b2;
    unsigned char *ptr;

    ptr = (unsigned char *)array;
    while (length--) {
        b1 = *ptr++;
        b2 = *ptr++;
        *array++ = (b1 << 8) | (b2);
    }
}

static void
ConvertGetUint(unsigned *array, unsigned int length) {
    unsigned int b1, b2, b3, b4;
    unsigned char *ptr;

    ptr = (unsigned char *)array;
    while (length--) {
        b1 = *ptr++;
        b2 = *ptr++;
        b3 = *ptr++;
        b4 = *ptr++;
        *array++ = (b1 << 24) | (b2 << 16) | (b3 << 8) | (b4);
    }
}

static void
ConvertPutShort(unsigned short *array, unsigned int length) {
    unsigned short b1, b2;
    unsigned char *ptr;

    ptr = (unsigned char *)array;
    while (length--) {
        b1 = *array >> 8;
        b2 = *array;
        *ptr++ = b1; *ptr++ = b2;
        *array++;
    }
}

static void
ConvertPutUint(unsigned *array, unsigned int length) {
    unsigned int b1, b2, b3, b4;
    unsigned char *ptr;

    ptr = (unsigned char *)array;
    while (length--) {
        b1 = *array >> 24;
        b2 = *array >> 16;
        b3 = *array >> 8;
        b4 = *array;
        *ptr++ = b1; *ptr++ = b2; *ptr++ = b3; *ptr++ = b4;
        *array++;
    }
}

static ImageRec *
ImageReadOpen(char *fileName)
{
    union {
        int testWord;
        char testByte[4];
    } endianTest;
    ImageRec *image;
    int swapFlag;
    int x;

    endianTest.testWord = 1;
    if (endianTest.testByte[0] == 1) {
        swapFlag = 1;
    } else {
        swapFlag = 0;
    }

    image = (ImageRec *)malloc(sizeof(ImageRec));
    if (image == NULL) {
        fprintf(stderr, "Out of memory!\n");
        exit(1);
    }
    if ((image->file = fopen(fileName, "rb")) == NULL) {
	return NULL;
    }

    fread(image, 1, 12, image->file);

    if (swapFlag) {
        ConvertGetShort(&image->imagic, 6);
    }

    image->tmp = (unsigned char *)malloc(image->xsize*256);
    if (image->tmp == NULL) {
        fprintf(stderr, "\nOut of memory!\n");
        exit(1);
    }

    if ((image->type & 0xFF00) == 0x0100) {
        x = image->ysize * image->zsize * (int) sizeof(unsigned);
        image->rowStart = (unsigned *)malloc(x);
        image->rowSize = (int *)malloc(x);
        if (image->rowStart == NULL || image->rowSize == NULL) {
            fprintf(stderr, "\nOut of memory!\n");
            exit(1);
        }
        image->rleEnd = 512 + (2 * x);
        fseek(image->file, 512, SEEK_SET);
        fread(image->rowStart, 1, x, image->file);
        fread(image->rowSize, 1, x, image->file);
        if (swapFlag) {
            ConvertGetUint(image->rowStart, x/(int) sizeof(unsigned));
            ConvertGetUint((unsigned *)image->rowSize, x/(int) sizeof(int));
        }
    }
    return image;
}

static void
ImageReadClose(ImageRec *image) {
    fclose(image->file);
    free(image->tmp);
    if ((image->type & 0xFF00) == 0x0100) {
	free(image->rowStart);
	free(image->rowSize);
    }
    free(image);
}

static ImageRec *
ImageWriteOpen(char *fileName, GLsizei type, GLsizei dim, GLsizei xsize, GLsizei ysize, GLsizei zsize)
{
    union {
        int testWord;
        char testByte[4];
    } endianTest;
    ImageRec *image;
    int swapFlag;
    int x, max;

    endianTest.testWord = 1;
    if (endianTest.testByte[0] == 1) {
        swapFlag = 1;
    } else {
        swapFlag = 0;
    }

    image = (ImageRec *)malloc(sizeof(ImageRec));
    if (image == NULL) {
        fprintf(stderr, "Out of memory!\n");
        exit(1);
    }

    image->imagic = IMAGIC;
    image->type  = type;
    image->dim   = dim;
    image->xsize = xsize;
    image->ysize = ysize;
    image->zsize = zsize;
    image->min   = 0;
    image->max   = 255;
    if (swapFlag) {
        ConvertPutShort(&image->imagic, 6);
        ConvertPutUint(&image->min, 2);
    }
    strncpy(image->name,fileName,80);

    if ((image->file = fopen(fileName, "wb")) == NULL) {
	return NULL;
    }
/*    fwrite(image, 1, 12, image->file);*/
    fwrite(image, 1, sizeof(ImageRec), image->file);

    if (swapFlag) {
        image->imagic = IMAGIC;
        image->type  = type;
        image->dim   = dim;
        image->xsize = xsize;
        image->ysize = ysize;
        image->zsize = zsize;
        image->min   = 0;
        image->max   = 255;
    }
    image->tmp = (unsigned char *)malloc(image->xsize*256);
    if (image->tmp == NULL) {
        fprintf(stderr, "\nOut of memory!\n");
        exit(1);
    }

    if ((image->type & 0xFF00) == 0x0100) {
        x = image->ysize * image->zsize * (int) sizeof(unsigned);
        image->rowStart = (unsigned *)malloc(x);
        image->rowSize = (int *)malloc(x);
        if (image->rowStart == NULL || image->rowSize == NULL) {
            fprintf(stderr, "\nOut of memory!\n");
            exit(1);
        }
        image->rleEnd = 512 + (2 * x);
	max = image->ysize*image->zsize;
	for(x=0; x<max; x++){
	    image->rowStart[x] = 0;
	    image->rowSize[x] = -1;
	}
    }
    return image;
}

static void
ImageWriteClose(ImageRec *image) {
    union {
        int testWord;
        char testByte[4];
    } endianTest;
    int swapFlag;
    int x;

    endianTest.testWord = 1;
    if (endianTest.testByte[0] == 1) {
        swapFlag = 1;
    } else {
        swapFlag = 0;
    }

    if ((image->type & 0xFF00) == 0x0100) {
        x = image->ysize * image->zsize * (int) sizeof(unsigned);
        if (swapFlag) {
            ConvertPutUint(image->rowStart, x/(int) sizeof(unsigned));
            ConvertPutUint((unsigned *)image->rowSize, x/(int) sizeof(int));
        }
        fseek(image->file, 512, SEEK_SET);
	fwrite(image->rowStart, 1, x, image->file);
	fwrite(image->rowSize, 1, x, image->file);
    }

    fclose(image->file);
    free(image->tmp);
    if ((image->type & 0xFF00) == 0x0100) {
	free(image->rowStart);
	free(image->rowSize);
    }
    free(image);
}

static void
ImageGetRow(ImageRec *image, unsigned char *buf, int y, int z) {
    unsigned char *iPtr, *oPtr, pixel;
    int count;

    if ((image->type & 0xFF00) == 0x0100) {
        fseek(image->file, (long) image->rowStart[y+z*image->ysize], SEEK_SET);
        fread(image->tmp, 1, (unsigned int)image->rowSize[y+z*image->ysize],
              image->file);

        iPtr = image->tmp;
        oPtr = buf;
        for (;;) {
            pixel = *iPtr++;
            count = (int)(pixel & 0x7F);
            if (!count) {
                return;
            }
            if (pixel & 0x80) {
                while (count--) {
                    *oPtr++ = *iPtr++;
                }
            } else {
                pixel = *iPtr++;
                while (count--) {
                    *oPtr++ = pixel;
                }
            }
        }
    } else {
        fseek(image->file, 512+(y*image->xsize)+(z*image->xsize*image->ysize),
              SEEK_SET);
        fread(buf, 1, image->xsize, image->file);
    }
}

static void
ImagePutRow(ImageRec *image, unsigned char *buf, int y, int z) {
    unsigned char *iPtr, *oPtr;
    unsigned char *sPtr;
    unsigned char *ibufend;
    short todo, cc;
    int count, bufsize;

    if ((image->type & 0xFF00) == 0x0100) {
        iPtr = buf;
	ibufend = iPtr +image->xsize;
        oPtr = image->tmp;

	while(iPtr<ibufend) {
	    sPtr = iPtr;
	    iPtr += 2;
	    while((iPtr<ibufend)&&((iPtr[-2]!=iPtr[-1])||(iPtr[-1]!=iPtr[0]))) iPtr++;
	    iPtr -= 2;
	    count = iPtr-sPtr;
	    while(count) {
		todo = count>126 ? 126: count;
		count -= todo;
		*oPtr++ = 0x80|todo;
		while(todo--) *oPtr++ = *sPtr++;
	    }
	    sPtr = iPtr;
	    cc = *iPtr++;
	    while((iPtr<ibufend)&&(*iPtr==cc)) iPtr++;
	    count = iPtr-sPtr;
	    while(count) {
		todo = count>126 ? 126: count;
		count -= todo;
		*oPtr++ = todo;
		*oPtr++ = cc;
	    }
	}
	*oPtr++ = 0;
	bufsize = oPtr - image->tmp;
	image->rowSize[y+z*image->ysize] = bufsize;
	image->rowStart[y+z*image->ysize] = image->rleEnd;
	image->rleEnd += bufsize;
	fseek(image->file, (long) image->rowStart[y+z*image->ysize], SEEK_SET);
	fwrite(image->tmp, 1, (unsigned int)image->rowSize[y+z*image->ysize],
	       image->file);
    } else {
        fseek(image->file, 512+(y*image->xsize)+(z*image->xsize*image->ysize),
              SEEK_SET);
        fwrite(buf, 1, image->xsize, image->file);
    }
}


GLubyte *
readImage(char *imagefile, GLsizei *xsize, GLsizei *ysize, GLsizei *zsize)
{
    ImageRec *image;
    GLubyte *base, *lptr;
    GLubyte *rbuf, *gbuf, *bbuf, *abuf;
    GLsizei  x, y;

    if( (image = ImageReadOpen(imagefile)) == NULL ) {
	fprintf(stderr,"readImage: Can't open image file '%s'.\n", imagefile);
	return(NULL);
    }
  
    *xsize = image->xsize;
    *ysize = image->ysize;
    *zsize = image->zsize;
    
    base = (GLubyte *)malloc(image->xsize * image->ysize * image->zsize *
			     sizeof(GLubyte));
    rbuf = (GLubyte *)malloc(image->xsize * sizeof(GLubyte));
    gbuf = (GLubyte *)malloc(image->xsize * sizeof(GLubyte));
    bbuf = (GLubyte *)malloc(image->xsize * sizeof(GLubyte));
    abuf = (GLubyte *)malloc(image->xsize * sizeof(GLubyte));
    if (!base || !rbuf || !gbuf || !bbuf || !abuf) {
	fprintf(stderr,"readImage: Can't malloc enough memory\n");
	return(NULL);
    }
    lptr = base;

    if (image->zsize >= 4) {
	for (y = 0; y < image->ysize; y++) {
            ImageGetRow(image,rbuf,y,0);
            ImageGetRow(image,gbuf,y,1);
            ImageGetRow(image,bbuf,y,2);
            ImageGetRow(image,abuf,y,3);  /* Discard. */
	    for (x = 0; x < image->xsize; x++) {
		*lptr++ = (GLubyte)*rbuf++;
		*lptr++ = (GLubyte)*gbuf++;
		*lptr++ = (GLubyte)*bbuf++;
		*lptr++ = (GLubyte)*abuf++;
	    }
	    rbuf -= image->xsize;
	    gbuf -= image->xsize;
	    bbuf -= image->xsize;
	    abuf -= image->xsize;
	}
    } else if (image->zsize == 3) {
	for (y = 0; y < image->ysize; y++) {
            ImageGetRow(image,rbuf,y,0);
            ImageGetRow(image,gbuf,y,1);
            ImageGetRow(image,bbuf,y,2);
	    for (x = 0; x < image->xsize; x++) {
		*lptr++ = (GLubyte)*rbuf++;
		*lptr++ = (GLubyte)*gbuf++;
		*lptr++ = (GLubyte)*bbuf++;
	    }
	    rbuf -= image->xsize;
	    gbuf -= image->xsize;
	    bbuf -= image->xsize;
	}
    } else {
	for (y = 0; y < image->ysize; y++) {
            ImageGetRow(image,rbuf,y,0);
	    for (x = 0; x < image->xsize; x++) {
		*lptr++ = (GLubyte)*rbuf++;
	    }
	    rbuf -= image->xsize;
	}
    }

    ImageReadClose(image);
    free(rbuf);
    free(gbuf);
    free(bbuf);
    free(abuf);

    return (GLubyte *) base;
}

int
writeImage(char *imagefile, GLubyte *image, GLsizei xsize, GLsizei ysize, GLsizei zsize)
{
    GLsizei x, y;
    ImageRec *outimage;
    GLubyte *rbuf, *gbuf, *bbuf, *abuf;
    GLubyte *lptr;

    rbuf = (GLubyte *)malloc(xsize * sizeof(GLubyte));
    gbuf = (GLubyte *)malloc(xsize * sizeof(GLubyte));
    bbuf = (GLubyte *)malloc(xsize * sizeof(GLubyte));
    abuf = (GLubyte *)malloc(xsize * sizeof(GLubyte));
    if (!rbuf || !gbuf || !bbuf || !abuf) {
	fprintf(stderr,"writeImage: Can't malloc enough memory\n");
	return 0;
    }
    
    if((outimage = ImageWriteOpen(imagefile, RLE(1), 3, xsize, ysize, zsize)) == NULL ) {
	fprintf(stderr,"writeImage: Can't open image file '%s'.\n", imagefile);
	return 0;
    }
    lptr = image;

    if (zsize >= 4){
	for(y=0; y<ysize; y++) {
	    for (x = 0; x < xsize; x++) {
		*rbuf++ = (GLubyte)*lptr++;
		*gbuf++ = (GLubyte)*lptr++;
		*bbuf++ = (GLubyte)*lptr++;
		*abuf++ = (GLubyte)*lptr++;
	    }
	    rbuf -= xsize;
	    gbuf -= xsize;
	    bbuf -= xsize;
	    abuf -= xsize;
	    ImagePutRow(outimage,rbuf,y,0);
	    ImagePutRow(outimage,gbuf,y,1);
	    ImagePutRow(outimage,bbuf,y,2);
	    ImagePutRow(outimage,abuf,y,3);
	}
    } else if (zsize == 3) { 
	for(y=0; y<ysize; y++) {
	    for (x = 0; x < xsize; x++) {
		*rbuf++ = (GLubyte)*lptr++;
		*gbuf++ = (GLubyte)*lptr++;
		*bbuf++ = (GLubyte)*lptr++;
	    }
	    rbuf -= xsize;
	    gbuf -= xsize;
	    bbuf -= xsize;
	    ImagePutRow(outimage,rbuf,y,0);
	    ImagePutRow(outimage,gbuf,y,1);
	    ImagePutRow(outimage,bbuf,y,2);
	}
    } else {
	for(y=0; y<ysize; y++) {
	    for (x = 0; x < xsize; x++) {
		*rbuf++ = (GLubyte)*lptr++;
	    }
	    rbuf -= xsize;
	    ImagePutRow(outimage,rbuf,y,0);
	}
    }

    ImageWriteClose(outimage);

    free(rbuf);
    free(gbuf);
    free(bbuf);
    free(abuf);

    return 1;
}


IMAGE *
readImageFile(char *imagefile)
{
    GLsizei  height, width, depth;
    GLenum format;
    IMAGE *image;
    
    /* read data */
    image = (IMAGE *)malloc(sizeof(IMAGE));
    if((image->data = readImage(imagefile, &width, &height, &depth)) == NULL){
	fprintf(stderr,"Image file read Error!!\n");
	exit(0);
    }
    image->width = width;
    image->height= height;
    image->depth = depth;
    if(depth >= 4) format = GL_RGBA;
    else if(depth == 3) format = GL_RGB;
    else format = GL_LUMINANCE;
    image->format = format;
    image->type = IMAGE_DATA_UBYTE;
    image->size = (size_t)(width * height * depth);
/*    printf("image data width =%d, height =%d, depth =%d \n",width,height,depth);*/

    return (IMAGE *) image;
}

int
writeImageFile(char *imagefile, IMAGE *image)
{
    GLsizei  height, width, depth;
    
    /* write data */
    width = image->width;
    height = image->height;
    depth = image->depth;
/*    printf("image data width =%d, height =%d, depth =%d \n",width,height,depth);*/

    return writeImage(imagefile, image->data, width, height, depth);
}


int
writeFastImage(char *imagefile, GLubyte *image, GLsizei xsize, GLsizei ysize, GLsizei zsize)
{
    GLsizei x, y;
    ImageRec *outimage;
    GLubyte *rbuf, *gbuf, *bbuf, *abuf;
    GLubyte *lptr;

    rbuf = (GLubyte *)malloc(xsize * sizeof(GLubyte));
    gbuf = (GLubyte *)malloc(xsize * sizeof(GLubyte));
    bbuf = (GLubyte *)malloc(xsize * sizeof(GLubyte));
    abuf = (GLubyte *)malloc(xsize * sizeof(GLubyte));
    if (!rbuf || !gbuf || !bbuf || !abuf) {
	fprintf(stderr,"writeImage: Can't malloc enough memory\n");
	return 0;
    }
    
    if((outimage = ImageWriteOpen(imagefile, VERBATIM(1), 3, xsize, ysize, zsize)) == NULL ) {
	fprintf(stderr,"writeImage: Can't open image file '%s'.\n", imagefile);
	return 0;
    }
    lptr = image;

    if (zsize >= 4){
	for(y=0; y<ysize; y++) {
	    for (x = 0; x < xsize; x++) {
		*rbuf++ = (GLubyte)*lptr++;
		*gbuf++ = (GLubyte)*lptr++;
		*bbuf++ = (GLubyte)*lptr++;
		*abuf++ = (GLubyte)*lptr++;
	    }
	    rbuf -= xsize;
	    gbuf -= xsize;
	    bbuf -= xsize;
	    abuf -= xsize;
	    ImagePutRow(outimage,rbuf,y,0);
	    ImagePutRow(outimage,gbuf,y,1);
	    ImagePutRow(outimage,bbuf,y,2);
	    ImagePutRow(outimage,abuf,y,3);
	}
    } else if (zsize == 3) { 
	for(y=0; y<ysize; y++) {
	    for (x = 0; x < xsize; x++) {
		*rbuf++ = (GLubyte)*lptr++;
		*gbuf++ = (GLubyte)*lptr++;
		*bbuf++ = (GLubyte)*lptr++;
	    }
	    rbuf -= xsize;
	    gbuf -= xsize;
	    bbuf -= xsize;
	    ImagePutRow(outimage,rbuf,y,0);
	    ImagePutRow(outimage,gbuf,y,1);
	    ImagePutRow(outimage,bbuf,y,2);
	}
    } else {
	for(y=0; y<ysize; y++) {
	    for (x = 0; x < xsize; x++) {
		*rbuf++ = (GLubyte)*lptr++;
	    }
	    rbuf -= xsize;
	    ImagePutRow(outimage,rbuf,y,0);
	}
    }

    ImageWriteClose(outimage);

    free(rbuf);
    free(gbuf);
    free(bbuf);
    free(abuf);

    return 1;
}

int
writeFastImageFile(char *imagefile, IMAGE *image)
{
    GLsizei  height, width, depth;
    
    /* write data */
    width = image->width;
    height = image->height;
    depth = image->depth;
/*    printf("image data width =%d, height =%d, depth =%d \n",width,height,depth);*/

    return writeFastImage(imagefile, image->data, width, height, depth);
}

int
getImageSize(char *imagefile, int *xsize, int *ysize, int *zsize)
{
    ImageRec *image;

    if( (image = ImageReadOpen(imagefile)) == NULL ) {
	fprintf(stderr,"readImage: Can't open image file '%s'.\n", imagefile);
	return 0;
    }
  
    *xsize = image->xsize;
    *ysize = image->ysize;
    *zsize = image->zsize;

    ImageReadClose(image);

    return 1;
}

int
loadImage(char *imagefile, GLubyte *image, GLsizei xsize, GLsizei ysize, GLsizei zsize)
{
    ImageRec *inimage;
    GLubyte *lptr;
    GLubyte *rbuf, *gbuf, *bbuf, *abuf;
    GLsizei  x, y;
    
    /* read data */
    if((inimage = ImageReadOpen(imagefile)) == NULL ) {
	fprintf(stderr,"readImage: Can't open image file '%s'.\n", imagefile);
	return 0;
    }

    if(inimage->xsize != xsize || inimage->ysize != ysize || inimage->zsize != zsize){
	fprintf(stderr,"overreadImage: mismatch image size!!\n");
	return 0;
    }
    rbuf = (GLubyte *)malloc(xsize * sizeof(GLubyte));
    gbuf = (GLubyte *)malloc(xsize * sizeof(GLubyte));
    bbuf = (GLubyte *)malloc(xsize * sizeof(GLubyte));
    abuf = (GLubyte *)malloc(xsize * sizeof(GLubyte));
    if (!rbuf || !gbuf || !bbuf || !abuf) {
	fprintf(stderr,"readImage: Can't malloc enough memory\n");
	return 0;
    }
    lptr = image;
    
    if (zsize >= 4) {
	for (y = 0; y < ysize; y++) {
            ImageGetRow(inimage,rbuf,y,0);
            ImageGetRow(inimage,gbuf,y,1);
            ImageGetRow(inimage,bbuf,y,2);
            ImageGetRow(inimage,abuf,y,3);  /* Discard. */
	    for (x = 0; x < xsize; x++) {
		*lptr++ = (GLubyte)*rbuf++;
		*lptr++ = (GLubyte)*gbuf++;
		*lptr++ = (GLubyte)*bbuf++;
		*lptr++ = (GLubyte)*abuf++;
	    }
	    rbuf -= xsize;
	    gbuf -= xsize;
	    bbuf -= xsize;
	    abuf -= xsize;
	}
    } else if (zsize == 3) {
	for (y = 0; y < ysize; y++) {
            ImageGetRow(inimage,rbuf,y,0);
            ImageGetRow(inimage,gbuf,y,1);
            ImageGetRow(inimage,bbuf,y,2);
	    for (x = 0; x < xsize; x++) {
		*lptr++ = (GLubyte)*rbuf++;
		*lptr++ = (GLubyte)*gbuf++;
		*lptr++ = (GLubyte)*bbuf++;
	    }
	    rbuf -= xsize;
	    gbuf -= xsize;
	    bbuf -= xsize;
	}
    } else {
	for (y = 0; y < ysize; y++) {
            ImageGetRow(inimage,rbuf,y,0);
	    for (x = 0; x < xsize; x++) {
		*lptr++ = (GLubyte)*rbuf++;
	    }
	    rbuf -= xsize;
	}
    }

    ImageReadClose(inimage);
    free(rbuf);
    free(gbuf);
    free(bbuf);
    free(abuf);

    return 1;
}

int
loadImageFile(IMAGE *image, char *imagefile)
{
    /* read data */
    if(loadImage(imagefile, image->data,
		 image->width, image->height, image->depth) == 0){
	fprintf(stderr,"Image file read Error!!\n");
	return 0;
    }

    return 1;
}

