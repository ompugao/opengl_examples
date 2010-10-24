#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <memory.h>
#include <jpeglib.h>
#include "jpegimage.h"


/******************** JPEG DECOMPRESSION SAMPLE INTERFACE *******************/

/* This half of the example shows how to read data from the JPEG decompressor.
 * It's a bit more refined than the above, in that we show:
 *   (a) how to modify the JPEG library's standard error-reporting behavior;
 *   (b) how to allocate workspace using the library's memory manager.
 *
 * Just to make this example a little different from the first one, we'll
 * assume that we do not intend to put the whole image into an in-memory
 * buffer, but to send it line-by-line someplace else.  We need a one-
 * scanline-high JSAMPLE array as a work buffer, and we will let the JPEG
 * memory manager allocate it for us.  This approach is actually quite useful
 * because we don't need to remember to deallocate the buffer separately: it
 * will go away automatically when the JPEG object is cleaned up.
 */

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */
  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


/*
 * Sample routine for JPEG decompression.  We assume that the source file name
 * is passed in.  We want to return 1 on success, 0 on error.
 */


GLubyte *
readJPEGimage(char *imagefile, GLsizei *xsize, GLsizei *ysize, GLsizei *zsize)
{
  /* This struct contains the JPEG decompression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   */
  struct jpeg_decompress_struct cinfo;
  /* We use our private extension JPEG error handler.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct my_error_mgr jerr;
  /* More stuff */
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */
  long cont;
  JSAMPLE *imagebuf;
  
  /* In this example we want to open the input file before doing anything else,
   * so that the setjmp() error recovery below can assume the file is open.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to read binary files.
   */

  if ((infile = fopen(imagefile, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", imagefile);
    return NULL;
  }

  /* Step 1: allocate and initialize JPEG decompression object */

  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(jerr.setjmp_buffer)) {
    /* If we get here, the JPEG code has signaled an error.
     * We need to clean up the JPEG object, close the input file, and return.
     */
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return NULL;
  }
  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&cinfo);

  /* Step 2: specify data source (eg, a file) */

  jpeg_stdio_src(&cinfo, infile);

  /* Step 3: read file parameters with jpeg_read_header() */

  (void) jpeg_read_header(&cinfo, TRUE);
  
  /* We can ignore the return value from jpeg_read_header since
   *   (a) suspension is not possible with the stdio data source, and
   *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
   * See libjpeg.doc for more info.
   */

  /* Step 4: set parameters for decompression */

  /* In this example, we don't need to change any of the defaults set by
   * jpeg_read_header(), so we do nothing here.
   */

  /* Step 5: Start decompressor */

  (void) jpeg_start_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* We may need to do some setup of our own at this point before reading
   * the data.  After jpeg_start_decompress() we have the correct scaled
   * output image dimensions available, as well as the output colormap
   * if we asked for color quantization.
   * In this example, we need to make an output work buffer of the right size.
   */ 
  /* JSAMPLEs per row in output buffer */
  row_stride = cinfo.output_width * cinfo.output_components;
  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */

  /* Here we use the library's state variable cinfo.output_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   */

  imagebuf = (JSAMPLE *)malloc(cinfo.image_width *cinfo.image_height
			       *cinfo.output_components);
  if(imagebuf==NULL) {
      fprintf(stderr, "Coundn't allocate memory");
      return NULL;
  }
  *xsize = cinfo.image_width;
  *ysize = cinfo.image_height;
  *zsize = cinfo.output_components;
  
  cont=cinfo.output_height-1;
  while (cinfo.output_scanline < cinfo.output_height) {
    /* jpeg_read_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could ask for
     * more than one scanline at a time if that's more convenient.
     */
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    /* Assume put_scanline_someplace wants a pointer and sample count. */
    /* put_scanline_someplace(buffer[0], row_stride); */
    memcpy(imagebuf + cinfo.image_width *cinfo.output_components *cont,
	   buffer[0], row_stride);
    cont--;
  }

  /* Step 7: Finish decompression */

  (void) jpeg_finish_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);

  /* After finish_decompress, we can close the input file.
   * Here we postpone it until after no more JPEG errors are possible,
   * so as to simplify the setjmp error logic above.  (Actually, I don't
   * think that jpeg_destroy can do an error exit, but why assume anything...)
   */
  fclose(infile);

  /* At this point you may want to check to see whether any corrupt-data
   * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
   */

  /* And we're done! */
  return imagebuf;
}

IMAGE *
readJPEGimageFile(char *imagefile)
{
    GLsizei  height, width, depth;
    GLenum format;
    IMAGE *image;
    
    /* read data */
    image = (IMAGE *)malloc(sizeof(IMAGE));
    if((image->data = readJPEGimage(imagefile, &width, &height, &depth)) == NULL){
	fprintf(stderr,"JPEG image file read Error!!\n");
	return (IMAGE *) NULL;
    }
    image->width = width;
    image->height= height;
    image->depth = depth;
    if(depth >= 4) format = GL_RGBA;
    else if(depth == 3) format = GL_RGB;
    else format = GL_LUMINANCE;
    image->format = format;
    image->size = (size_t)(width * height * depth);
    image->type = IMAGE_DATA_UBYTE;
/*    printf("image data width =%d, height =%d, depth =%d \n",width,height,depth);*/

    return (IMAGE *) image;
}

int
readJPEGimageFile2(IMAGE *image, char *imagefile)
{
    GLsizei  height, width, depth;
    GLenum format;
    
    /* read data */
    if((image->data = readJPEGimage(imagefile, &width, &height, &depth)) == NULL){
	fprintf(stderr,"JPEG image file read Error!!\n");
	return 0;
    }
    image->width = width;
    image->height= height;
    image->depth = depth;
    if(depth >= 4) format = GL_RGBA;
    else if(depth == 3) format = GL_RGB;
    else format = GL_LUMINANCE;
    image->format = format;
    image->size = (size_t)(width * height * depth);
    image->type = IMAGE_DATA_UBYTE;
/*    printf("image data width =%d, height =%d, depth =%d \n",width,height,depth);*/

    return 1;
}

GLubyte *
readJPEGrawImage(char *imagefile, GLsizei *xsize, GLsizei *ysize, GLsizei *zsize, size_t *size)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  FILE * infile;		/* source file */
  GLubyte *imagebuf;
  
  if ((infile = fopen(imagefile, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", imagefile);
    return NULL;
  }
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return NULL;
  }

  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);

  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);

  *xsize = cinfo.image_width;
  *ysize = cinfo.image_height;
  *zsize = cinfo.output_components;

  (void) jpeg_abort_decompress(&cinfo);
//  (void) jpeg_finish_decompress(&cinfo);

  fseek(infile, 0L, SEEK_END);
  *size = (size_t)ftell(infile);
  fseek(infile, 0L, SEEK_SET);

  imagebuf = (GLubyte *)malloc(*size);

  if(imagebuf==NULL) {
      fprintf(stderr, "Coundn't allocate memory");
      return NULL;
  }
  fread((void *)imagebuf, 1, *size, infile);

  jpeg_destroy_decompress(&cinfo);
  fclose(infile);
  return imagebuf;
}

IMAGE *
readJPEGrawImageFile(char *imagefile)
{
    GLsizei  height, width, depth;
    GLenum format;
    size_t size;
    IMAGE *image;
    
    /* read data */
    image = (IMAGE *)malloc(sizeof(IMAGE));
    if((image->data = readJPEGrawImage(imagefile, &width, &height, &depth, &size))
       == NULL){
	fprintf(stderr,"JPEG image file read Error!!\n");
	return (IMAGE *) NULL;
    }
    image->width = width;
    image->height= height;
    image->depth = depth;
    if(depth >= 4) format = GL_RGBA;
    else if(depth == 3) format = GL_RGB;
    else format = GL_LUMINANCE;
    image->format = format;
    image->size = size;
    image->type = IMAGE_JPEG_UBYTE;
/*    printf("image data width =%d, height =%d, depth =%d \n",width,height,depth);*/

    return (IMAGE *) image;
}

int
readJPEGrawImageFile2(IMAGE *image, char *imagefile)
{
    GLsizei  height, width, depth;
    GLenum format;
    size_t size;
    
    /* read data */
    if((image->data = readJPEGrawImage(imagefile, &width, &height, &depth, &size))
       == NULL){
	fprintf(stderr,"JPEG image file read Error!!\n");
	return 0;
    }
    image->width = width;
    image->height= height;
    image->depth = depth;
    if(depth >= 4) format = GL_RGBA;
    else if(depth == 3) format = GL_RGB;
    else format = GL_LUMINANCE;
    image->format = format;
    image->size = size;
    image->type = IMAGE_JPEG_UBYTE;
/*    printf("image data width =%d, height =%d, depth =%d \n",width,height,depth);*/

    return 1;
}

GLubyte *
readJPEGflipImage(char *imagefile, GLsizei *xsize, GLsizei *ysize, GLsizei *zsize)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */
  long cont;
  JSAMPLE *imagebuf;
  
  if ((infile = fopen(imagefile, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", imagefile);
    return NULL;
  }
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return NULL;
  }
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);
  row_stride = cinfo.output_width * cinfo.output_components;
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
  imagebuf = (JSAMPLE *)malloc(cinfo.image_width *cinfo.image_height
			       *cinfo.output_components);
  if(imagebuf==NULL) {
      fprintf(stderr, "Coundn't allocate memory");
      return NULL;
  }
  *xsize = cinfo.image_width;
  *ysize = cinfo.image_height;
  *zsize = cinfo.output_components;
  cont= 0;
  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    memcpy(imagebuf + cinfo.image_width *cinfo.output_components *cont,
	   buffer[0], row_stride);
    cont++;
  }
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);
  return imagebuf;
}

IMAGE *
readJPEGflipImageFile(char *imagefile)
{
    GLsizei  height, width, depth;
    GLenum format;
    IMAGE *image;
    
    /* read data */
    image = (IMAGE *)malloc(sizeof(IMAGE));
    if((image->data = readJPEGflipImage(imagefile, &width, &height, &depth)) == NULL){
	fprintf(stderr,"JPEG image file read Error!!\n");
	return (IMAGE *) NULL;
    }
    image->width = width;
    image->height= height;
    image->depth = depth;
    if(depth >= 4) format = GL_RGBA;
    else if(depth == 3) format = GL_RGB;
    else format = GL_LUMINANCE;
    image->format = format;
    image->type = IMAGE_FLIPDATA_UBYTE;
/*    printf("image data width =%d, height =%d, depth =%d \n",width,height,depth);*/

    return (IMAGE *) image;
}

int
readJPEGflipImageFile2(IMAGE *image, char *imagefile)
{
    GLsizei  height, width, depth;
    GLenum format;
    
    /* read data */
    if((image->data = readJPEGflipImage(imagefile, &width, &height, &depth)) == NULL){
	fprintf(stderr,"JPEG image file read Error!!\n");
	return 0;
    }
    image->width = width;
    image->height= height;
    image->depth = depth;
    if(depth >= 4) format = GL_RGBA;
    else if(depth == 3) format = GL_RGB;
    else format = GL_LUMINANCE;
    image->format = format;
    image->size = (size_t)(width * height * depth);
    image->type = IMAGE_FLIPDATA_UBYTE;
/*    printf("image data width =%d, height =%d, depth =%d \n",width,height,depth);*/

    return 1;
}

int
loadJPEGimage(char *imagefile, GLubyte *imagedata, GLsizei xsize, GLsizei ysize, GLsizei zsize)
{
  /* This struct contains the JPEG decompression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   */
  struct jpeg_decompress_struct cinfo;
  /* We use our private extension JPEG error handler.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct my_error_mgr jerr;
  /* More stuff */
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */
  long cont;
  JSAMPLE *imagebuf;
  
  /* In this example we want to open the input file before doing anything else,
   * so that the setjmp() error recovery below can assume the file is open.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to read binary files.
   */

  if ((infile = fopen(imagefile, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", imagefile);
    return 0;
  }

  /* Step 1: allocate and initialize JPEG decompression object */

  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(jerr.setjmp_buffer)) {
    /* If we get here, the JPEG code has signaled an error.
     * We need to clean up the JPEG object, close the input file, and return.
     */
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return 0;
  }
  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&cinfo);

  /* Step 2: specify data source (eg, a file) */

  jpeg_stdio_src(&cinfo, infile);

  /* Step 3: read file parameters with jpeg_read_header() */

  (void) jpeg_read_header(&cinfo, TRUE);
  
  /* We can ignore the return value from jpeg_read_header since
   *   (a) suspension is not possible with the stdio data source, and
   *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
   * See libjpeg.doc for more info.
   */

  /* Step 4: set parameters for decompression */

  /* In this example, we don't need to change any of the defaults set by
   * jpeg_read_header(), so we do nothing here.
   */

  /* Step 5: Start decompressor */

  (void) jpeg_start_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* We may need to do some setup of our own at this point before reading
   * the data.  After jpeg_start_decompress() we have the correct scaled
   * output image dimensions available, as well as the output colormap
   * if we asked for color quantization.
   * In this example, we need to make an output work buffer of the right size.
   */ 
  /* JSAMPLEs per row in output buffer */
  row_stride = cinfo.output_width * cinfo.output_components;
  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */

  /* Here we use the library's state variable cinfo.output_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   */
/*
  imagebuf = (JSAMPLE *)malloc(cinfo.image_width *cinfo.image_height
			       *cinfo.output_components);
*/
  imagebuf = (JSAMPLE *)imagedata;
  if(imagebuf==NULL) {
      fprintf(stderr, "Coundn't allocate memory");
      return 0;
  }
  if(xsize != (GLsizei)cinfo.image_width || ysize != (GLsizei)cinfo.image_height
     || zsize != (GLsizei)cinfo.output_components){
      fprintf(stderr, "Coundn't load to memory");
      return 0;
  }
  
  cont=cinfo.output_height-1;
  while (cinfo.output_scanline < cinfo.output_height) {
    /* jpeg_read_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could ask for
     * more than one scanline at a time if that's more convenient.
     */
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    /* Assume put_scanline_someplace wants a pointer and sample count. */
    /* put_scanline_someplace(buffer[0], row_stride); */
    memcpy(imagebuf + cinfo.image_width *cinfo.output_components *cont,
	   buffer[0], row_stride);
    cont--;
  }

  /* Step 7: Finish decompression */

  (void) jpeg_finish_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);

  /* After finish_decompress, we can close the input file.
   * Here we postpone it until after no more JPEG errors are possible,
   * so as to simplify the setjmp error logic above.  (Actually, I don't
   * think that jpeg_destroy can do an error exit, but why assume anything...)
   */
  fclose(infile);

  /* At this point you may want to check to see whether any corrupt-data
   * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
   */

  /* And we're done! */
    return 1;
}

int
loadJPEGimageFile(IMAGE *image, char *imagefile)
{
    /* read data */
    if(loadJPEGimage(imagefile, image->data,
		     image->width, image->height, image->depth) == 0){
	fprintf(stderr,"JPEG image file read Error!!\n");
	return 0;
    }

    return 1;
}

int
loadJPEGflipImage(char *imagefile, GLubyte *imagedata, GLsizei xsize, GLsizei ysize, GLsizei zsize)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */
  long cont;
  JSAMPLE *imagebuf;
  
  if ((infile = fopen(imagefile, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", imagefile);
    return 0;
  }
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return 0;
  }
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);
  row_stride = cinfo.output_width * cinfo.output_components;
  buffer = (*cinfo.mem->alloc_sarray)
      ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
/*
  imagebuf = (JSAMPLE *)malloc(cinfo.image_width *cinfo.image_height
			       *cinfo.output_components);
*/
  imagebuf = (JSAMPLE *)imagedata;
  if(imagebuf==NULL) {
      fprintf(stderr, "Coundn't allocate memory");
      return 0;
  }
  if(xsize != (GLsizei)cinfo.image_width || ysize != (GLsizei)cinfo.image_height
     || zsize != (GLsizei)cinfo.output_components){
      fprintf(stderr, "Coundn't load to memory");
      return 0;
  }
  cont= 0;
  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    memcpy(imagebuf + cinfo.image_width *cinfo.output_components *cont,
	   buffer[0], row_stride);
    cont++;
  }
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);
  return 1;
}

int
loadJPEGflipImageFile(IMAGE *image, char *imagefile)
{
    /* read data */
    if(loadJPEGflipImage(imagefile, image->data,
		     image->width, image->height, image->depth) == 0){
	fprintf(stderr,"JPEG image file read Error!!\n");
	return 0;
    }

    return 1;
}

int
loadJPEGrawImage(char *imagefile, GLubyte *imagedata, size_t size,
		 GLsizei xsize, GLsizei ysize, GLsizei zsize)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */
  long cont;
  JSAMPLE *imagebuf;
  
  if ((infile = fopen(imagefile, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", imagefile);
    return 0;
  }
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return 0;
  }
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);
  row_stride = cinfo.output_width * cinfo.output_components;
  buffer = (*cinfo.mem->alloc_sarray)
      ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
/*
  imagebuf = (JSAMPLE *)malloc(cinfo.image_width *cinfo.image_height
			       *cinfo.output_components);
*/
  imagebuf = (JSAMPLE *)imagedata;
  if(imagebuf==NULL) {
      fprintf(stderr, "Coundn't allocate memory");
      return 0;
  }
  if(xsize != (GLsizei)cinfo.image_width || ysize != (GLsizei)cinfo.image_height
     || zsize != (GLsizei)cinfo.output_components){
      fprintf(stderr, "Coundn't load to memory");
      return 0;
  }
  cont= 0;
  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    memcpy(imagebuf + cinfo.image_width *cinfo.output_components *cont,
	   buffer[0], row_stride);
    cont++;
  }
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);
  return 1;
}

int
loadJPEGrawImageFile(IMAGE *image, char *imagefile)
{
    /* read data */
    if(loadJPEGrawImage(imagefile, image->data, image->size,
			image->width, image->height, image->depth) == 0){
	fprintf(stderr,"JPEG image file read Error!!\n");
	return 0;
    }

    return 1;
}

/******************** JPEG COMPRESSION SAMPLE INTERFACE *******************/

/* This half of the example shows how to feed data into the JPEG compressor.
 * We present a minimal version that does not worry about refinements such
 * as error recovery (the JPEG code will just exit() if it gets an error).
 */


/*
 * IMAGE DATA FORMATS:
 *
 * The standard input image format is a rectangular array of pixels, with
 * each pixel having the same number of "component" values (color channels).
 * Each pixel row is an array of JSAMPLEs (which typically are unsigned chars).
 * If you are working with color data, then the color values for each pixel
 * must be adjacent in the row; for example, R,G,B,R,G,B,R,G,B,... for 24-bit
 * RGB color.
 *
 * For this example, we'll assume that this data structure matches the way
 * our application has stored the image in memory, so we can just pass a
 * pointer to our image buffer.  In particular, let's say that the image is
 * RGB color and is described by:
 */

/*
 * Sample routine for JPEG compression.  We assume that the target file name
 * and a compression quality factor are passed in.
 */

int
_writeJPEGimage(char *imagefile, GLubyte *imagedata,
                GLsizei xsize, GLsizei ysize, GLsizei zsize, int quality)
{
  /* This struct contains the JPEG compression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   * It is possible to have several such structures, representing multiple
   * compression/decompression processes, in existence at once.  We refer
   * to any one struct (and its associated working data) as a "JPEG object".
   */
  struct jpeg_compress_struct cinfo;
  /* This struct represents a JPEG error handler.  It is declared separately
   * because applications often want to supply a specialized error handler
   * (see the second half of this file for an example).  But here we just
   * take the easy way out and use the standard error handler, which will
   * print a message on stderr and call exit() if compression fails.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct jpeg_error_mgr jerr;
  /* More stuff */
  FILE * outfile;		/* target file */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  int row_stride;		/* physical row width in image buffer */
  long cont;

  /* Step 1: allocate and initialize JPEG compression object */

  /* We have to set up the error handler first, in case the initialization
   * step fails.  (Unlikely, but it could happen if you are out of memory.)
   * This routine fills in the contents of struct jerr, and returns jerr's
   * address which we place into the link field in cinfo.
   */
  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG compression object. */
  jpeg_create_compress(&cinfo);

  /* Step 2: specify data destination (eg, a file) */
  /* Note: steps 2 and 3 can be done in either order. */

  /* Here we use the library-supplied code to send compressed data to a
   * stdio stream.  You can also write your own code to do something else.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to write binary files.
   */
  if ((outfile = fopen(imagefile, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", imagefile);
    return 0;
  }
  jpeg_stdio_dest(&cinfo, outfile);

  /* Step 3: set parameters for compression */

  /* First we supply a description of the input image.
   * Four fields of the cinfo struct must be filled in:
   */
  cinfo.image_width = xsize; 	/* image width and height, in pixels */
  cinfo.image_height = ysize;
  cinfo.input_components = zsize;	/* # of color components per pixel */
                                /* colorspace of input image */
  if(zsize == 3) cinfo.in_color_space = JCS_RGB;
  else if(zsize == 1) cinfo.in_color_space = JCS_GRAYSCALE;
  else {
    fprintf(stderr, "can't support %s componets\n", (char*)cinfo.input_components);
    return 0;
  }

  /* Now use the library's routine to set default compression parameters.
   * (You must set at least cinfo.in_color_space before calling this,
   * since the defaults depend on the source color space.)
   */
  jpeg_set_defaults(&cinfo);
  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

  /* Step 4: Start compressor */

  /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
  jpeg_start_compress(&cinfo, TRUE);

  /* Step 5: while (scan lines remain to be written) */
  /*           jpeg_write_scanlines(...); */

  /* Here we use the library's state variable cinfo.next_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   * To keep things simple, we pass one scanline per call; you can pass
   * more if you wish, though.
   */
  /* JSAMPLEs per row in image_buffer */
  row_stride = cinfo.image_width * cinfo.input_components;

  cont = cinfo.image_height -1;
  while (cinfo.next_scanline < cinfo.image_height) {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
    row_pointer[0] = imagedata + row_stride * cont;
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    cont--;
  }

  /* Step 6: Finish compression */

  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */
  fclose(outfile);

  /* Step 7: release JPEG compression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

  /* And we're done! */
  return 1;
}

int
_writeJPEGflipImage(char *imagefile, GLubyte *imagedata,
		   GLsizei xsize, GLsizei ysize, GLsizei zsize, int quality)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * outfile;		/* target file */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  int row_stride;		/* physical row width in image buffer */

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  if ((outfile = fopen(imagefile, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", imagefile);
    return 0;
  }
  jpeg_stdio_dest(&cinfo, outfile);
  cinfo.image_width = xsize; 	/* image width and height, in pixels */
  cinfo.image_height = ysize;
  cinfo.input_components = zsize;	/* # of color components per pixel */
                                /* colorspace of input image */
  if(zsize == 3) cinfo.in_color_space = JCS_RGB;
  else if(zsize == 1) cinfo.in_color_space = JCS_GRAYSCALE;
  else {
    fprintf(stderr, "can't support %s componets\n", (char*)cinfo.input_components);
    return 0;
  }
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

  jpeg_start_compress(&cinfo, TRUE);

  row_stride = cinfo.image_width * cinfo.input_components;
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = &imagedata[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }
  jpeg_finish_compress(&cinfo);
  fclose(outfile);

  jpeg_destroy_compress(&cinfo);
  return 1;
}

int
_writeJPEGrawImage(char *imagefile, GLubyte *imagedata, size_t size,
		   GLsizei xsize, GLsizei ysize, GLsizei zsize, int quality)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * outfile;		/* target file */

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  if ((outfile = fopen(imagefile, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", imagefile);
    return 0;
  }
  fwrite(imagedata, 1, size, outfile);
/*
  jpeg_stdio_dest(&cinfo, outfile);
  cinfo.image_width = xsize; 	*//* image width and height, in pixels *//*
  cinfo.image_height = ysize;
  cinfo.input_components = zsize;	*//* # of color components per pixel *//*
					*//* colorspace of input image *//*
  if(zsize == 3) cinfo.in_color_space = JCS_RGB;
  else if(zsize == 1) cinfo.in_color_space = JCS_GRAYSCALE;
  else {
    fprintf(stderr, "can't support %s componets\n", cinfo.input_components);
    return 0;
  }
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE *//* limit to baseline-JPEG values *//*);
  jpeg_start_compress(&cinfo, TRUE);

  row_stride = cinfo.image_width * cinfo.input_components;
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = &imagedata[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }
  jpeg_finish_compress(&cinfo);
*/
  fclose(outfile);

  jpeg_destroy_compress(&cinfo);
  return 1;
}

int
writeJPEGimage(char *imagefile, GLubyte *imagedata, /* default quality = 75 */
               GLsizei xsize, GLsizei ysize, GLsizei zsize)
{
    return _writeJPEGimage(imagefile, imagedata, xsize, ysize, zsize, 75);
}

int
writeJPEGflipImage(char *imagefile, GLubyte *imagedata, /* default quality = 75 */
		  GLsizei xsize, GLsizei ysize, GLsizei zsize)
{
    return _writeJPEGflipImage(imagefile, imagedata, xsize, ysize, zsize, 75);
}

int
writeJPEGrawImage(char *imagefile, GLubyte *imagedata, size_t size, 
		  GLsizei xsize, GLsizei ysize, GLsizei zsize)
{   /* default quality = 75 */
    return _writeJPEGrawImage(imagefile, imagedata, size, xsize, ysize, zsize, 75);
}

int
_writeJPEGimageFile(char *imagefile, IMAGE *image, int quality)
{
    switch(image->type){
    case IMAGE_DATA_UBYTE:
	return _writeJPEGimage(imagefile, image->data, 
			       image->width, image->height, image->depth, quality);
	break;
    case IMAGE_FLIPDATA_UBYTE:
	return _writeJPEGflipImage(imagefile, image->data, 
				   image->width, image->height, image->depth, quality);
	break;
    case IMAGE_JPEG_UBYTE:
	return _writeJPEGrawImage(imagefile, image->data, image->size,
				  image->width, image->height, image->depth, quality);
	break;
    default:
	printf("Error: Illeagal Image Type: %d\n", image->type);
	return -1;
	break;
    }
    return -1;
}

int
writeJPEGimageFile(char *imagefile, IMAGE *image)
{
    return _writeJPEGimageFile(imagefile, image, 75);
}

int
_writeJPEGflipImageFile(char *imagefile, IMAGE *image, int quality)
{
    return _writeJPEGflipImage(imagefile, image->data, 
			       image->width, image->height, image->depth, quality);
}

int
writeJPEGflipImageFile(char *imagefile, IMAGE *image)
{
    return _writeJPEGflipImage(imagefile, image->data, 
			       image->width, image->height, image->depth, 75);
}

int
_writeJPEGrawImageFile(char *imagefile, IMAGE *image, int quality)
{
    return _writeJPEGrawImage(imagefile, image->data, image->size,
			      image->width, image->height, image->depth, quality);
}

int
writeJPEGrawImageFile(char *imagefile, IMAGE *image)
{
    return _writeJPEGrawImage(imagefile, image->data, image->size,
			      image->width, image->height, image->depth, 75);
}

/*
 * SOME FINE POINTS:
 *
 * In the above loop, we ignored the return value of jpeg_write_scanlines,
 * which is the number of scanlines actually written.  We could get away
 * with this because we were only relying on the value of cinfo.next_scanline,
 * which will be incremented correctly.  If you maintain additional loop
 * variables then you should be careful to increment them properly.
 * Actually, for output to a stdio stream you needn't worry, because
 * then jpeg_write_scanlines will write all the lines passed (or else exit
 * with a fatal error).  Partial writes can only occur if you use a data
 * destination module that can demand suspension of the compressor.
 * (If you don't know what that's for, you don't need it.)
 *
 * If the compressor requires full-image buffers (for entropy-coding
 * optimization or a multi-scan JPEG file), it will create temporary
 * files for anything that doesn't fit within the maximum-memory setting.
 * (Note that temp files are NOT needed if you use the default parameters.)
 * On some systems you may need to set up a signal handler to ensure that
 * temporary files are deleted if the program is interrupted.  See libjpeg.doc.
 *
 * Scanlines MUST be supplied in top-to-bottom order if you want your JPEG
 * files to be compatible with everyone else's.  If you cannot readily read
 * your data in that order, you'll need an intermediate array to hold the
 * image.  See rdtarga.c or rdbmp.c for examples of handling bottom-to-top
 * source data using the JPEG code's internal virtual-array mechanisms.
 */

