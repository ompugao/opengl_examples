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
#include <math.h>

#include "utimage.h"

GLubyte
getImagePixel(IMAGE *image, int x, int y, int z)
{
    int width, height, depth;
    GLubyte *lptr;
    
    width = image->width;
    height = image->height;
    depth = image->depth;
    lptr = image->data;

    if(x > width || y > height || z > depth) return (GLubyte)NULL;
    lptr += (y*width + x) * depth +z;

    return (GLubyte) *lptr;
}

int
putImagePixel(IMAGE *image, GLubyte buf, int x, int y, int z)
{
    int width, height, depth;
    GLubyte *lptr;

    width = image->width;
    height = image->height;
    depth = image->depth;
    lptr = (GLubyte *)image->data;
    
    if(x > width || y > height || z > depth) return 0;
    lptr += (y*width + x) * depth +z;

    *lptr = (GLubyte) buf;
    return 1;
}

GLfloat
getImageFloatPixel(IMAGE *image, int x, int y, int z)
{
    int width, height, depth;
    GLfloat *lptr;
    
    width = image->width;
    height = image->height;
    depth = image->depth;
    lptr = (GLfloat *)image->float_data;

    if(x > width || y > height || z > height) return (GLfloat)0.0;
    lptr += (y*width + x) * depth +z;

    return (GLfloat) *lptr;
}

int
putImageFloatPixel(IMAGE *image, GLfloat buf, int x, int y, int z)
{
    int width, height, depth;
    GLfloat *lptr;

    width = image->width;
    height = image->height;
    depth = image->depth;
    lptr = (GLfloat *)image->float_data;
    
    if(x > width || y > height || z > depth) return 0;
    lptr += (y*width + x) * depth +z;

    *lptr = (GLfloat) buf;
    return 1;
}


IMAGE *
mallocImage(GLsizei width, GLsizei height, GLsizei depth)
{
    GLenum format;
    GLubyte *base;
    size_t size;
    IMAGE *image;

    /* read data */
    image = (IMAGE *)malloc(sizeof(IMAGE));
    size = width * height * depth * sizeof(GLubyte);
    base = (GLubyte *)malloc(size);
    if (!base || !image) {
	fprintf(stderr,"mallocImage: Can't malloc enough memory\n");
	return (IMAGE *)NULL;
    }
    
    image->width = width;
    image->height= height;
    image->depth = depth;
    image->size = size;

    if(depth >= 4) format = GL_RGBA;
    else if(depth == 3) format = GL_RGB;
    else format = GL_LUMINANCE;
    image->format = format;
    image->type = IMAGE_DATA_UBYTE;

    image->data = base;
    /*    printf("image data width =%d, height =%d, depth =%d \n",width,height,depth);*/

    return (IMAGE *) image;
}

IMAGE *
mallocFloatImage(GLsizei width, GLsizei height, GLsizei depth)
{
    GLenum format;
    GLfloat *base;
    size_t size;
    IMAGE *image;

    /* read data */
    image = (IMAGE *)malloc(sizeof(IMAGE));
    size = width * height * depth * sizeof(GLfloat);
    base = (GLfloat *)malloc(size);
    if (!base || !image) {
	fprintf(stderr,"mallocImage: Can't malloc enough memory\n");
	return (IMAGE *)NULL;
    }
    
    image->width = width;
    image->height= height;
    image->depth = depth;
    image->size = size;

    if(depth >= 4) format = GL_RGBA;
    else if(depth == 3) format = GL_RGB;
    else format = GL_LUMINANCE;
    image->format = format;
    image->type = IMAGE_DATA_FLOAT;

    image->float_data = base;
    /*    printf("image data width =%d, height =%d, depth =%d \n",width,height,depth);*/

    return (IMAGE *) image;
}

int
freeImage(IMAGE *image)
{
    if((image->type != IMAGE_DATA_FLOAT) && (image->type != IMAGE_FLIPDATA_FLOAT)){
	free(image->data); free(image);
    } else {
	free(image->float_data); free(image);
    }
    return 1;
}

int
freeFloatImage(IMAGE *image)
{
    if((image->type == IMAGE_DATA_FLOAT) || (image->type == IMAGE_FLIPDATA_FLOAT)){
	free(image->float_data); free(image);
    } else {
	free(image->data); free(image);
    }
    return 1;
}


int
copyImage(IMAGE *newimage, IMAGE *orgimage, int xoff, int yoff)
{
    int   x, y, z;
    int   width, height, depth;
    GLubyte  *buf;

    width = orgimage->width;
    height = orgimage->height;
    depth = orgimage->depth;
    buf = (GLubyte *)malloc(sizeof(GLubyte));
    
    for(y=yoff; y<height; y++){
	for(x=xoff; x<width; x++){
	    for(z=0; z<depth; z++){
		*buf = getImagePixel(orgimage, x, y, z);
		putImagePixel(newimage, *buf, x, y, z);
	    }
	}
    }

    return 1;
}    

IMAGE *
duplicateImage(IMAGE *image)
{
    IMAGE *newimage;

    newimage = mallocImage(image->width, image->height, image->depth);

    copyImage(newimage, image, 0, 0);
    return (IMAGE *) newimage;
}

IMAGE *
cutImage(IMAGE *image, int cs, int ce, int rs, int re)
{
    IMAGE *newimage;
    int   width, height, depth;
    int   x, y, z;
    GLubyte  *buf;

    width = ce -cs;
    height = re -rs;
    depth = image->depth;
    newimage = mallocImage(width, height, depth);
    buf = (GLubyte *)malloc(sizeof(GLubyte));
    
    for(y=0; y<height; y++){
	for(x=0; x<width; x++){
	    for(z=0; z<depth; z++){
		*buf = getImagePixel(image, cs+x, rs+y, z);
		putImagePixel(newimage, *buf, x, y, z);
	    }
	}
    }
    return (IMAGE *) newimage;
}

int
convertFloatImageSelf(IMAGE *image)
{
    GLfloat *float_base, *float_buf;
    GLubyte *ubyte_buf, ubyte_max = 0xff;
    int   width, height, depth;
    int   x, y, z;

    if((image->type != IMAGE_DATA_UBYTE) && (image->type != IMAGE_FLIPDATA_UBYTE)) {
	fprintf(stderr, "convertFloatImage: data type is not unsigned byte!\n");
	return 0;
    }
    width = image->width;
    height = image->height;
    depth = image->depth;
    ubyte_buf = image->data;

    float_base = (GLfloat *)malloc(width * height * depth * sizeof(GLfloat));
    if (!float_base) {
	fprintf(stderr,"convertFloatImage: Can't malloc enough memory\n");
	return 0;
    }

    float_buf = float_base; 
    for(y=0; y<height; y++){
	for(x=0; x<width; x++){
	    for(z=0; z<depth; z++){
		*float_buf++ = ((GLfloat)*ubyte_buf++) / (GLfloat)ubyte_max;
	    }
	}
    }
    image->type = image->type ^ 0x1;
    free(image->data);
    image->float_data = float_base;

    return 1;
}

IMAGE *
convertFloatImage(IMAGE *image)
{
    IMAGE	*newimage;
    GLfloat 	*float_buf;
    GLubyte 	*ubyte_buf, ubyte_max = 0xff;
    int   	x, y, z;

    if((image->type != IMAGE_DATA_UBYTE) && (image->type != IMAGE_FLIPDATA_UBYTE)) {
	fprintf(stderr, "convertFloatImage: data type is not unsigned byte!\n");
	return (IMAGE *)NULL;
    }
    ubyte_buf = image->data;

    newimage = (IMAGE *)mallocFloatImage(image->width, image->height, image->depth);

    float_buf = newimage->float_data; 
    for(y=0; y<image->height; y++){
	for(x=0; x<image->width; x++){
	    for(z=0; z<image->depth; z++){
		*float_buf++ = ((GLfloat)*ubyte_buf++) / (GLfloat)ubyte_max;
	    }
	}
    }
    newimage->type = image->type ^ 0x1;

    return (IMAGE *)newimage;
}

int
invertFloatImageSelf(IMAGE *image)
{
    GLfloat *float_buf, float_tmp;
    GLubyte *ubyte_base, *ubyte_buf, ubyte_max = 0xff;
    int   width, height, depth;
    int   x, y, z;

    if((image->type != IMAGE_DATA_FLOAT) && (image->type != IMAGE_FLIPDATA_FLOAT)) {
	fprintf(stderr, "invertFloatImageSelf: data type is not float!\n");
	return 0;
    }
    width = image->width;
    height = image->height;
    depth = image->depth;
    float_buf = image->float_data;

    ubyte_base = (GLubyte *)malloc(width * height * depth * sizeof(GLubyte));
    if (!ubyte_base) {
	fprintf(stderr,"invertFloatImageSelf: Can't malloc enough memory\n");
	return 0;
    }

    ubyte_buf = ubyte_base; 
    for(y=0; y<height; y++){
	for(x=0; x<width; x++){
	    for(z=0; z<depth; z++){
		float_tmp = (GLfloat)*float_buf++;
		if(float_tmp > 1.0) *ubyte_buf++ = (GLubyte)ubyte_max;
		else if(float_tmp < 0.0) *ubyte_buf++ = (GLubyte)0;
		else *ubyte_buf++ = (GLubyte)(float_tmp * (GLfloat)ubyte_max);
	    }
	}
    }
    image->type = image->type ^ 0x1;
    free(image->float_data);
    image->data = ubyte_base;

    return 1;
}

IMAGE *
invertFloatImage(IMAGE *image)
{
    IMAGE	*newimage;
    GLfloat 	*float_buf, float_tmp;
    GLubyte 	*ubyte_buf, ubyte_max = 0xff;
    int   	x, y, z;

    if((image->type != IMAGE_DATA_FLOAT) && (image->type != IMAGE_FLIPDATA_FLOAT)) {
	fprintf(stderr, "invertFloatImage: data type is not float!\n");
	return (IMAGE *)NULL;
    }
    float_buf = image->float_data;

    newimage = (IMAGE *)mallocImage(image->width, image->height, image->depth);

    ubyte_buf = image->data; 
    for(y=0; y<image->height; y++){
	for(x=0; x<image->width; x++){
	    for(z=0; z<image->depth; z++){
		float_tmp = (GLfloat)*float_buf++;
		if(float_tmp > 1.0) *ubyte_buf++ = (GLubyte)ubyte_max;
		else if(float_tmp < 0.0) *ubyte_buf++ = (GLubyte)0;
		else *ubyte_buf++ = (GLubyte)(float_tmp * (GLfloat)ubyte_max);
	    }
	}
    }
    newimage->type = image->type ^ 0x1;

    return (IMAGE *)newimage;
}

int
multipleFloatImage(IMAGE *newimage, IMAGE *image, GLfloat k)
{
    GLfloat	*float_buf, *new_buf, tmp_buf;
    int   	x, y, z;
    
    if((image->type != IMAGE_DATA_FLOAT) && (image->type != IMAGE_FLIPDATA_FLOAT)) {
	fprintf(stderr, "multipleFloatImage: data type is not float!\n");
	return 0;
    }
    if(newimage == image){
	return (int)multipleFloatImageSelf(image, k);
    } else if(newimage == NULL){
	newimage = (IMAGE *)mallocFloatImage(image->width, image->height, image->depth);
    } else if(newimage->float_data == NULL) {
	if((newimage->float_data = (GLfloat *)malloc(image->width * image->height
						     * image->depth * sizeof(GLfloat)))
	   ==NULL) {
	    fprintf(stderr,"multipleFloatImage: Can't malloc enough memory\n");
	    return 0;
	}
    }
    float_buf = image->float_data;
    new_buf = newimage->float_data;

    for(y=0; y<image->height; y++){
	for(x=0; x<image->width; x++){
	    for(z=0; z<image->depth; z++){
		tmp_buf = k * (GLfloat)*float_buf++;
		if(tmp_buf > 1.0) *new_buf++ = (GLfloat)1.0;
		else if(tmp_buf < 0.0) *new_buf++ = (GLfloat)0.0;
		else *new_buf++ = (GLfloat)tmp_buf;
	    }
	}
    }
    return 1;
}

int
multipleFloatImageSelf(IMAGE *image, GLfloat k)
{
    GLfloat	*float_buf, tmp_buf;
    int   	x, y, z;
    
    if((image->type != IMAGE_DATA_FLOAT) && (image->type != IMAGE_FLIPDATA_FLOAT)) {
	fprintf(stderr, "multipleFloatImageSelf: data type is not float!\n");
	return 0;
    }
    float_buf = image->float_data;

    for(y=0; y<image->height; y++){
	for(x=0; x<image->width; x++){
	    for(z=0; z<image->depth; z++){
		tmp_buf = k * ((GLfloat)*float_buf);
		if(tmp_buf > 1.0) *float_buf++ = (GLfloat)1.0;
		else if(tmp_buf < 0.0) *float_buf++ = (GLfloat)0.0;
		else *float_buf++ = (GLfloat)tmp_buf;
	    }
	}
    }
    return 1;
}

IMAGE *
negativeImage(IMAGE *image)
{
    IMAGE 	*newimage;
    GLubyte	*ubyte_buf, *new_buf, ubyte_max = 0xff;
    int   	x, y, z;
    
    if((image->type != IMAGE_DATA_UBYTE) && (image->type != IMAGE_FLIPDATA_UBYTE)) {
	fprintf(stderr, "negativeImage: data type is not ubyte!\n");
	return (IMAGE *)NULL;
    }
    ubyte_buf = image->data;
    
    newimage = (IMAGE *)mallocImage(image->width, image->height, image->depth);

    new_buf = newimage->data;
    for(y=0; y<image->height; y++){
	for(x=0; x<image->width; x++){
	    for(z=0; z<image->depth; z++){
		*new_buf++ = (GLubyte)ubyte_max - (GLubyte)*ubyte_buf++;
	    }
	}
    }

    return (IMAGE *)newimage;
}

int
negativeImageSelf(IMAGE *image)
{
    GLubyte	*ubyte_buf, tmp_buf, ubyte_max = 0xff;
    int   	x, y, z;
    
    if((image->type != IMAGE_DATA_UBYTE) && (image->type != IMAGE_FLIPDATA_UBYTE)) {
	fprintf(stderr, "negativeImageSelf: data type is not ubyte!\n");
	return 0;
    }
    ubyte_buf = image->data;
    
    for(y=0; y<image->height; y++){
	for(x=0; x<image->width; x++){
	    for(z=0; z<image->depth; z++){
		tmp_buf = *ubyte_buf;
		*ubyte_buf++ = (GLubyte)(ubyte_max - tmp_buf);
	    }
	}
    }
    return 1;
}

IMAGE *
negativeFloatImage(IMAGE *image)
{
    IMAGE 	*newimage;
    GLfloat	*float_buf, *new_buf;
    int   	x, y, z;
    
    if((image->type != IMAGE_DATA_FLOAT) && (image->type != IMAGE_FLIPDATA_FLOAT)) {
	fprintf(stderr, "negativeFloatImage: data type is not float!\n");
	return (IMAGE *)NULL;
    }
    float_buf = image->float_data;
    
    newimage = (IMAGE *)mallocFloatImage(image->width, image->height, image->depth);

    new_buf = newimage->float_data;
    for(y=0; y<image->height; y++){
	for(x=0; x<image->width; x++){
	    for(z=0; z<image->depth; z++){
		*new_buf++ = (GLfloat)1.0 - (GLfloat)*float_buf++;
	    }
	}
    }
    return (IMAGE *)newimage;
}

int
negativeFloatImageSelf(IMAGE *image)
{
    GLfloat	*float_buf, tmp_buf;
    int   	x, y, z;
    
    if((image->type != IMAGE_DATA_FLOAT) && (image->type != IMAGE_FLIPDATA_FLOAT)) {
	fprintf(stderr, "negativeFloatImageSelf: data type is not float!\n");
	return 0;
    }
    float_buf = image->float_data;
    
    for(y=0; y<image->height; y++){
	for(x=0; x<image->width; x++){
	    for(z=0; z<image->depth; z++){
		tmp_buf = *float_buf;
		*float_buf++ = (GLfloat)1.0 - tmp_buf;
	    }
	}
    }
    return 1;
}

IMAGE *
flipImage(IMAGE *image)
{
    IMAGE 	*newimage;
    int   	y, row_stride;
    
    if((image->type != IMAGE_DATA_UBYTE) && (image->type != IMAGE_FLIPDATA_UBYTE)) {
	fprintf(stderr, "flipImage: data type is not ubyte!\n");
	return (IMAGE *)NULL;
    }
    newimage = (IMAGE *)mallocImage(image->width, image->height, image->depth);

    row_stride = image->width * image->depth;
    for(y=0; y<image->height; y++){
	memcpy(newimage->data + row_stride *y, 
	       image->data + row_stride *(image->height-y-1), row_stride);
    }
    newimage->type = image->type ^ 0x02;

    return (IMAGE *)newimage;
}

int
flipImageSelf(IMAGE *image)
{
    GLubyte	*base;
    int   	y, row_stride;
    
    if((image->type != IMAGE_DATA_UBYTE) && (image->type != IMAGE_FLIPDATA_UBYTE)) {
	fprintf(stderr, "flipImageSelf: data type is not ubyte!\n");
	return 0;
    }
    
    base = (GLubyte *)malloc(image->width * image->height * image->depth * sizeof(GLubyte));
    if (!base) {
	fprintf(stderr,"flipImageSelf: Can't malloc enough memory\n");
	return 0;
    }
    
    row_stride = image->width * image->depth;
    for(y=0; y<image->height; y++){
	memcpy(base + row_stride *y, 
	       image->data + row_stride *(image->height-y-1), row_stride);
    }
    image->type = image->type ^ 0x02;
    free(image->data); image->data = base;

    return 1;
}

IMAGE *
flipFloatImage(IMAGE *image)
{
    IMAGE 	*newimage;
    int   	y, row_stride;
    
    if((image->type != IMAGE_DATA_FLOAT) && (image->type != IMAGE_FLIPDATA_FLOAT)) {
	fprintf(stderr, "flipFloatImage: data type is not float!\n");
	return (IMAGE *)NULL;
    }
    newimage = (IMAGE *)mallocFloatImage(image->width, image->height, image->depth);

    row_stride = image->width * image->depth;
    for(y=0; y<image->height; y++){
	memcpy(newimage->float_data + row_stride *y, 
	       image->float_data + row_stride *(image->height-y-1), row_stride);
    }
    newimage->type = image->type ^ 0x02;

    return (IMAGE *)newimage;
}

int
flipFloatImageSelf(IMAGE *image)
{
    GLfloat	*base;
    int   	y, row_stride;
    
    if((image->type != IMAGE_DATA_FLOAT) && (image->type != IMAGE_FLIPDATA_FLOAT)) {
	fprintf(stderr, "flipFloatImageSelf: data type is not float!\n");
	return 0;
    }
    base = (GLfloat *)malloc(image->width * image->height * image->depth * sizeof(GLfloat));
    if (!base) {
	fprintf(stderr,"flipFloatImageSelf: Can't malloc enough memory\n");
	return 0;
    }
    
    row_stride = image->width * image->depth;
    for(y=0; y<image->height; y++){
	memcpy(base + row_stride *y,
	       image->float_data + row_stride *(image->height-y-1), row_stride);
    }
    image->type = image->type ^ 0x02;
    free(image->float_data); image->float_data = base;

    return 1;
}

IMAGE *
encodeJPEGimage(IMAGE *image)
{
    IMAGE 	*newimage;
    
    newimage = (IMAGE *)mallocImage(image->width, image->height, image->depth);

    if(image->type == IMAGE_DATA_UBYTE || image->type == IMAGE_FLIPDATA_UBYTE) {
	writeJPEGimageMemory(&(newimage->data), &(newimage->size), image);
	image->type = IMAGE_JPEG_UBYTE;
    } else {
	fprintf(stderr, "encodeJPEGmage: data type is not ubyte!\n");
	return (IMAGE *)NULL;
    }

    return (IMAGE *)newimage;
}

int
encodeJPEGimageSelf(IMAGE *image)
{
    GLubyte	*base;
    size_t	size;
    
    if(image->type == IMAGE_DATA_UBYTE || image->type == IMAGE_FLIPDATA_UBYTE) {
	writeJPEGimageMemory(&base, &size, image);
	image->type = IMAGE_JPEG_UBYTE;
	image->size = size;
	free(image->data); image->data = base;
    } else {
	fprintf(stderr, "encodeJPEGimageSelf: data type is not ubyte!\n");
	return 0;
    }

    return 1;
}

IMAGE *
decodeJPEGimage(IMAGE *image)
{
    IMAGE 	*newimage;
    
    if(image->type != IMAGE_JPEG_UBYTE) {
	newimage = readJPEGimageMemory(image->data, image->size);
    } else {
	fprintf(stderr, "decodeJPEGimage: data type is not jpeg!\n");
	return (IMAGE *)NULL;
    }

    return (IMAGE *)newimage;
}

int
decodeJPEGimageSelf(IMAGE *image)
{
    GLubyte	*base;
    int		width, height, depth;
    
    if(image->type != IMAGE_JPEG_UBYTE) {
	fprintf(stderr, "decodeJPEGimageSelf: data type is not jpeg!\n");
	return 0;
    }
    base = (GLubyte *)readJPEGmemory(image->data, image->size, &width, &height, &depth);
    if(image->width != width || image->height != height || image->depth != depth) {
	fprintf(stderr, "decodeJPEGimageSelf: mismatch image size!\n");
	return 0;
    }
    image->type = IMAGE_DATA_UBYTE;
    image->size = (size_t)(width * height * depth);
    free(image->data); image->data = base;

    return 1;
}

IMAGE *
decodeJPEGflipImage(IMAGE *image)
{
    IMAGE 	*newimage;
    
    if(image->type != IMAGE_JPEG_UBYTE) {
	newimage = readJPEGflipImageMemory(image->data, image->size);
    } else {
	fprintf(stderr, "decodeJPEGimage: data type is not jpeg!\n");
	return (IMAGE *)NULL;
    }

    return (IMAGE *)newimage;
}

int
decodeJPEGflipImageSelf(IMAGE *image)
{
    GLubyte	*base;
    int		width, height, depth;
    
    if(image->type != IMAGE_JPEG_UBYTE) {
	fprintf(stderr, "decodeJPEGimageSelf: data type is not jpeg!\n");
	return 0;
    }
    base = (GLubyte *)readJPEGflipMemory(image->data, image->size, &width, &height, &depth);
    if(image->width != width || image->height != height || image->depth != depth) {
	fprintf(stderr, "decodeJPEGimageSelf: mismatch image size!\n");
	return 0;
    }
    image->type = IMAGE_FLIPDATA_UBYTE;
    image->size = (size_t)(width * height * depth);
    free(image->data); image->data = base;

    return 1;
}

IMAGE *
convImageRGBtoBW(IMAGE *image)
{
    IMAGE *newimage;
    int   width, height, depth;
    int   x, y;

    GLubyte *lptr;
    GLubyte *rbuf, *gbuf, *bbuf, *obuf;

    if(image->depth < 3) return 0;
    width = image->width;
    height = image->height;
    depth = 1;

    newimage = mallocImage(width, height, depth);
    rbuf = (GLubyte *)malloc(sizeof(GLubyte));
    gbuf = (GLubyte *)malloc(sizeof(GLubyte));
    bbuf = (GLubyte *)malloc(sizeof(GLubyte));
    obuf = (GLubyte *)malloc(sizeof(GLubyte));

    lptr = newimage->data;
    for(y=0; y<height; y++){
	for(x=0; x<width; x++){
	    *rbuf = getImagePixel(image, x, y, 0);
	    *gbuf = getImagePixel(image, x, y, 1);
	    *bbuf = getImagePixel(image, x, y, 2);
	    DOTOBW(obuf, rbuf, gbuf, bbuf);
	    *lptr++ = *obuf;
	}
    }
    return (IMAGE *) newimage;
}

IMAGE *
getMatchImage(IMAGE *image1, IMAGE *image2, float threshold)
{
    IMAGE *newimage;
    int   width, height, depth;
    int   x, y, z;
    GLubyte  *l1ptr, *l2ptr, *buf;
    int   diff;
    float sum =0;

    if(image1->width != image2->width || image1->height != image2->height
       || image1->depth != image2->depth) {
	fprintf(stderr, "getMatchImage: mismatch image size!!\n");
	return NULL;
    }
    width = image1->width;
    height = image1->height;
    depth = image1->depth;
    newimage = mallocImage(width, height, depth);
    buf = (GLubyte *)malloc(sizeof(GLubyte));

    l1ptr = image1->data;
    l2ptr = image2->data;
    for(y=0; y<height; y++){
	for(x=0; x<width; x++){
	    sum =0;
	    for(z=0; z<depth; z++){
		diff = *l1ptr++ - *l2ptr++;
		sum += (float) (diff * diff);
	    }
	    if(sqrt(sum) < threshold){
		for(z=0; z<depth; z++){
		    *buf = getImagePixel(image1, x, y, z);
		    putImagePixel(newimage, *buf, x, y, z);
		}
	    }
	}
    }
    return (IMAGE *) newimage;
}

IMAGE *
getDiffImage(IMAGE *image1, IMAGE *image2, float threshold)
{
    IMAGE *newimage;
    int   width, height, depth;
    int   x, y, z;
    GLubyte  *l1ptr, *l2ptr, *buf;
    int   diff;
    float sum =0;

    if(image1->width != image2->width || image1->height != image2->height
       || image1->depth != image2->depth) {
	fprintf(stderr, "getMatchImage: mismatch image size!!\n");
	return NULL;
    }
    width = image1->width;
    height = image1->height;
    depth = image1->depth;
    newimage = mallocImage(width, height, depth);
    buf = (GLubyte *)malloc(sizeof(GLubyte));

    l1ptr = image1->data;
    l2ptr = image2->data;
    for(y=0; y<height; y++){
	for(x=0; x<width; x++){
	    sum =0;
	    for(z=0; z<depth; z++){
		diff = *l1ptr++ - *l2ptr++;
		sum += (float) (diff * diff);
	    }
	    if(sqrt(sum) > threshold){
		for(z=0; z<depth; z++){
		    *buf = getImagePixel(image1, x, y, z);
		    putImagePixel(newimage, *buf, x, y, z);
		}
	    }
	}
    }
    return (IMAGE *) newimage;
}

int
convRGBtoYCbCr(GLubyte r, GLubyte g, GLubyte b, GLubyte *Y, GLubyte *Cb, GLubyte *Cr)
{
    float    colorR, colorG, colorB;
    float    colorY, colorCb, colorCr;

    colorR = (float)r /255.0;
    colorG = (float)g /255.0;
    colorB = (float)b /255.0;

    colorY  =  0.2989*colorR +0.5866*colorG +0.1145*colorB;
    colorCb = -0.1687*colorR -0.3312*colorG +0.5000*colorB +0.5000;
    colorCr =  0.5000*colorR -0.4183*colorG -0.0816*colorB +0.5000;

    *Y  = (GLubyte)(colorY *255);
    *Cb = (GLubyte)(colorCb*255);
    *Cr = (GLubyte)(colorCr*255);

    return 1;
}    

int
convRGBtoXYZ(float R, float G, float B, float *X, float *Y, float *Z)
{
    *X = 0.431*R +0.342*G +0.178*B;
    *Y = 0.222*R +0.707*G +0.071*B;
    *Z = 0.020*R +0.130*G +0.939*B;
    if(*X > 1.0) *X = 1.0;
    if(*Y > 1.0) *Y = 1.0;
    if(*Z > 1.0) *Z = 1.0;

    return 1;
}    

int
convYCbCrtoRGB(GLubyte Y, GLubyte Cb, GLubyte Cr, GLubyte *r, GLubyte *g, GLubyte *b)
{
    float    colorR, colorG, colorB;
    float    colorY, colorCb, colorCr;

    colorY  = (float)Y  /255.0;
    colorCb = (float)Cb /255.0 -0.5;
    colorCr = (float)Cr /255.0 -0.5;

    colorR = colorY +0.0000*colorCb +1.4022*colorCr;
    colorG = colorY -0.3456*colorCb -0.7145*colorCr;
    colorB = colorY +1.7710*colorCb +0.0000*colorCr;
    if(colorR > 1.0) colorR = 1.0;
    if(colorG > 1.0) colorG = 1.0;
    if(colorB > 1.0) colorB = 1.0;
    if(colorR < 0.0) colorR = 0.0;
    if(colorG < 0.0) colorG = 0.0;
    if(colorB < 0.0) colorB = 0.0;

    *r = (GLubyte)(colorR*255);
    *g = (GLubyte)(colorG*255);
    *b = (GLubyte)(colorB*255);

    return 1;
}

int
convXYZtoRGB(float X, float Y, float Z, float *R, float *G, float *B)
{
    *R =  3.063*X -1.393*Y -0.476*Z;
    *G = -0.969*X +1.876*Y +0.042*Z;
    *B =  0.068*X -0.229*Y +1.069*Z;
    if(*R > 1.0) *R = 1.0; if(*R < 0.0) *R = 0.0;
    if(*G > 1.0) *G = 1.0; if(*G < 0.0) *G = 0.0;
    if(*B > 1.0) *B = 1.0; if(*B < 0.0) *B = 0.0;

    return 1;
}

IMAGE *
convImageRGBtoYCbCr(IMAGE *image) 
{
    IMAGE *newimage;
    int   width, height, depth;
    int   x, y;
    float    colorR, colorG, colorB;
    float    colorY, colorCb, colorCr;

    width = image->width;
    height = image->height;
    depth = image->depth;
    if(depth < 3) return (IMAGE *)NULL;
    
    newimage = mallocImage(width, height, depth);

    for(y=0; y<height; y++){
        for(x=0; x<width; x++){
            colorR = (float)getImagePixel(image, x, y, 0) /255.0;
            colorG = (float)getImagePixel(image, x, y, 1) /255.0;
            colorB = (float)getImagePixel(image, x, y, 2) /255.0;

            colorY  =  0.2989*colorR +0.5866*colorG +0.1145*colorB;
            colorCb = -0.1687*colorR -0.3312*colorG +0.5000*colorB +0.5000;
            colorCr =  0.5000*colorR -0.4183*colorG -0.0816*colorB +0.5000;

            putImagePixel(newimage, (GLubyte)(colorY *255), x, y, 0);
            putImagePixel(newimage, (GLubyte)(colorCb*255), x, y, 1);
            putImagePixel(newimage, (GLubyte)(colorCr*255), x, y, 2);
            if(depth == 4) putImagePixel(newimage,
                                         getImagePixel(image, x, y, 3),
                                         x, y, 3);
        }
    }
    return (IMAGE *) newimage;
}

IMAGE *
convImageRGBtoXYZ(IMAGE *image) 
{
    IMAGE *newimage;
    int   width, height, depth;
    int   x, y;
    float    colorR, colorG, colorB;
    float    colorX, colorY, colorZ;

    width = image->width;
    height = image->height;
    depth = image->depth;
    if(depth < 3) return (IMAGE *)NULL;
    
    newimage = mallocImage(width, height, depth);

    for(y=0; y<height; y++){
        for(x=0; x<width; x++){
            colorR = (float)getImagePixel(image, x, y, 0) /255.0;
            colorG = (float)getImagePixel(image, x, y, 1) /255.0;
            colorB = (float)getImagePixel(image, x, y, 2) /255.0;

            convRGBtoXYZ(colorR, colorG, colorB, &colorX, &colorY, &colorZ);

            putImagePixel(newimage, (GLubyte)(colorX *255), x, y, 0);
            putImagePixel(newimage, (GLubyte)(colorY *255), x, y, 1);
            putImagePixel(newimage, (GLubyte)(colorZ *255), x, y, 2);
            if(depth == 4) putImagePixel(newimage,
                                         getImagePixel(image, x, y, 3),
                                         x, y, 3);
        }
    }
    return (IMAGE *) newimage;
}

IMAGE *
convImageYCbCrtoRGB(IMAGE *image) 
{
    IMAGE *newimage;
    int   width, height, depth;
    int   x, y;
    float    colorR, colorG, colorB;
    float    colorY, colorCb, colorCr;

    width = image->width;
    height = image->height;
    depth = image->depth;
    if(depth < 3) return (IMAGE *)NULL;
    
    newimage = mallocImage(width, height, depth);

    for(y=0; y<height; y++){
        for(x=0; x<width; x++){
            colorY  = (float)getImagePixel(image, x, y, 0) /255.0;
            colorCb = (float)getImagePixel(image, x, y, 1) /255.0 -0.5;
            colorCr = (float)getImagePixel(image, x, y, 2) /255.0 -0.5;

            colorR = colorY +0.0000*colorCb +1.4022*colorCr;
            colorG = colorY -0.3456*colorCb -0.7145*colorCr;
            colorB = colorY +1.7710*colorCb +0.0000*colorCr;
            if(colorR > 1.0) colorR = 1.0;
            if(colorG > 1.0) colorG = 1.0;
            if(colorB > 1.0) colorB = 1.0;
            if(colorR < 0.0) colorR = 0.0;
            if(colorG < 0.0) colorG = 0.0;
            if(colorB < 0.0) colorB = 0.0;

            putImagePixel(newimage, (GLubyte)(colorR*255), x, y, 0);
            putImagePixel(newimage, (GLubyte)(colorG*255), x, y, 1);
            putImagePixel(newimage, (GLubyte)(colorB*255), x, y, 2);
            if(depth == 4) putImagePixel(newimage,
                                         getImagePixel(image, x, y, 3),
                                         x, y, 3);
        }
    }
    return (IMAGE *) newimage;
}

IMAGE *
convImageXYZtoRGB(IMAGE *image) 
{
    IMAGE *newimage;
    int   width, height, depth;
    int   x, y;
    float    colorR, colorG, colorB;
    float    colorX, colorY, colorZ;

    width = image->width;
    height = image->height;
    depth = image->depth;
    if(depth < 3) return (IMAGE *)NULL;
    
    newimage = mallocImage(width, height, depth);

    for(y=0; y<height; y++){
        for(x=0; x<width; x++){
            colorX = (float)getImagePixel(image, x, y, 0) /255.0;
            colorY = (float)getImagePixel(image, x, y, 1) /255.0;
            colorZ = (float)getImagePixel(image, x, y, 2) /255.0;

            convXYZtoRGB(colorX, colorY, colorZ, &colorR, &colorG, &colorB);
            
            putImagePixel(newimage, (GLubyte)(colorR*255), x, y, 0);
            putImagePixel(newimage, (GLubyte)(colorG*255), x, y, 1);
            putImagePixel(newimage, (GLubyte)(colorB*255), x, y, 2);
            if(depth == 4) putImagePixel(newimage,
                                         getImagePixel(image, x, y, 3),
                                         x, y, 3);
        }
    }
    return (IMAGE *) newimage;
}

int
defineNewImageSize(GLsizei *w0, GLsizei *h0, GLsizei *w1, GLsizei *h1)
{
    int i = 0;
    GLsizei prev;

    while(*w0 > (prev = (int)pow(2, i))) i++;
    *w1 = prev;

    i = 0;
    while(*h0 > (prev = (int)pow(2, i))) i++;
    *h1 = prev;

    return 1;
}
