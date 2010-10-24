#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <memory.h>
#include <jpeglib.h>
#include <jerror.h>
#include "jpegcodec.h"

/*---------------------*/
/* function for memory */
/*---------------------*/
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

typedef struct {
  struct jpeg_destination_mgr pub;
  size_t * output_size;
  unsigned char ** output;
  JOCTET * buffer;
} memory_destination_mgr;
typedef memory_destination_mgr * mem_dest_ptr;

typedef struct {
  struct jpeg_source_mgr pub;
  unsigned char * inbuf;		
  JOCTET * buffer;		
  boolean start_of_file;	
  int	len;
  int	resbyte;
} memory_source_mgr;
typedef memory_source_mgr * mem_src_ptr;

#define INPUT_BUF_SIZE  40960

METHODDEF(void) init_destination (j_compress_ptr cinfo);
METHODDEF(boolean) empty_output_buffer (j_compress_ptr cinfo);
METHODDEF(void) term_destination (j_compress_ptr cinfo);
METHODDEF(void) term_destination_inplace (j_compress_ptr cinfo);

int
_saveJPEGmemory(unsigned char *dst, size_t *size, unsigned char *src,
		int xsize, int ysize, int zsize, int quality)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];
  int row_stride;
  long cont;
  mem_dest_ptr mem_dest;

  if(dst == NULL) {
    fprintf(stderr, "memory error (this function needs allocated memory)\n");
    return 0;
  }
		
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  if(xsize != (GLsizei)cinfo.image_width || ysize != (GLsizei)cinfo.image_height
     || zsize != (GLsizei)cinfo.input_components){
      fprintf(stderr, "Coundn't load to memory");
      return 0;
  }
  if(zsize ==3) cinfo.in_color_space = JCS_RGB;
  else if(zsize ==1) cinfo.in_color_space = JCS_GRAYSCALE;
  else {
    fprintf(stderr, "can't support %s componets\n", (char*)cinfo.input_components);
    return 0;
  }
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE);

  if (cinfo.dest == NULL) {
    cinfo.dest = (struct jpeg_destination_mgr *)
      (*cinfo.mem->alloc_small) ((j_common_ptr) &cinfo, JPOOL_PERMANENT,sizeof(memory_destination_mgr));
  }

  mem_dest = (mem_dest_ptr) cinfo.dest;
  mem_dest->pub.init_destination = init_destination;
  mem_dest->pub.empty_output_buffer = empty_output_buffer;
  mem_dest->pub.term_destination = term_destination;
  mem_dest->output_size = size;
  mem_dest->output = &dst;

  jpeg_start_compress(&cinfo, TRUE);

  row_stride = xsize * zsize;	/* JSAMPLEs per row in image_buffer */
  cont = cinfo.image_height -1;
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = src + row_stride * cont;
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    cont--;
  }

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  return 1;
}

int
_saveJPEGflipMemory(unsigned char *dst, size_t *size, unsigned char *src,
		   int xsize, int ysize, int zsize, int quality)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];
  int row_stride;
  mem_dest_ptr mem_dest;

  if(dst == NULL) {
    fprintf(stderr, "memory error (this function needs allocated memory)\n");
    return 0;
  }
		
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  if(xsize != (GLsizei)cinfo.image_width || ysize != (GLsizei)cinfo.image_height
     || zsize != (GLsizei)cinfo.input_components){
      fprintf(stderr, "Coundn't load to memory");
      return 0;
  }
  if(zsize ==3) cinfo.in_color_space = JCS_RGB;
  else if(zsize ==1) cinfo.in_color_space = JCS_GRAYSCALE;
  else {
    fprintf(stderr, "can't support %s componets\n", (char*)cinfo.input_components);
    return 0;
  }
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE);

  if (cinfo.dest == NULL) {
    cinfo.dest = (struct jpeg_destination_mgr *)
      (*cinfo.mem->alloc_small) ((j_common_ptr) &cinfo, JPOOL_PERMANENT,sizeof(memory_destination_mgr));
  }

  mem_dest = (mem_dest_ptr) cinfo.dest;
  mem_dest->pub.init_destination = init_destination;
  mem_dest->pub.empty_output_buffer = empty_output_buffer;
  mem_dest->pub.term_destination = term_destination;
  mem_dest->output_size = size;
  mem_dest->output = &dst;

  jpeg_start_compress(&cinfo, TRUE);

  row_stride = xsize * zsize;	/* JSAMPLEs per row in image_buffer */
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = &src[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  return 1;
}

int
saveJPEGmemory(GLubyte *dst, size_t *size, GLubyte *src,
		GLsizei xsize, GLsizei ysize, GLsizei zsize)
{ /* default quality = 75 */
    return _saveJPEGmemory(dst, size, src, xsize, ysize, zsize, 75);
}

int
saveJPEGflipMemory(GLubyte *dst, size_t *size, GLubyte *src,
		  GLsizei xsize, GLsizei ysize, GLsizei zsize)
{ /* default quality = 75 */
    return _saveJPEGflipMemory(dst, size, src, xsize, ysize, zsize, 75);
}

int
_saveJPEGimageMemory(GLubyte *dst, size_t *size, IMAGE *image, int quality)
{
    if((image->type & 0x10) == IMAGE_DATA_UBYTE) {
	return _saveJPEGmemory(dst, size, image->data, 
			       image->width, image->height, image->depth, quality);
    } else {
	return _saveJPEGflipMemory(dst, size, image->data, 
				  image->width, image->height, image->depth, quality);
    }
}

int
saveJPEGimageMemory(GLubyte *dst, size_t *size, IMAGE *image)
{
    return _saveJPEGimageMemory(dst, size, image, 75);
}

int
_saveJPEGflipImageMemory(GLubyte *dst, size_t *size, IMAGE *image, int quality)
{
    return _saveJPEGimageMemory(dst, size, image, quality);
}

int
saveJPEGflipImageMemory(GLubyte *dst, size_t *size, IMAGE *image)
{
    return saveJPEGimageMemory(dst, size, image);
}

int
_writeJPEGmemory(unsigned char **dst, size_t *size, unsigned char *src,
		 int xsize, int ysize, int zsize, int quality)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];
  int row_stride;
  long cont;
  mem_dest_ptr mem_dest;
		
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  cinfo.image_width = xsize;
  cinfo.image_height = ysize;
  cinfo.input_components = zsize;
  if(zsize ==3) cinfo.in_color_space = JCS_RGB;
  else if(zsize ==1) cinfo.in_color_space = JCS_GRAYSCALE;
  else {
    fprintf(stderr, "can't support %s componets\n", (char*)cinfo.input_components);
    return 0;
  }
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE);

  if (cinfo.dest == NULL) {
    cinfo.dest = (struct jpeg_destination_mgr *)
      (*cinfo.mem->alloc_small) ((j_common_ptr) &cinfo, JPOOL_PERMANENT,sizeof(memory_destination_mgr));
  }

  mem_dest = (mem_dest_ptr) cinfo.dest;
  mem_dest->pub.init_destination = init_destination;
  mem_dest->pub.empty_output_buffer = empty_output_buffer;
  mem_dest->pub.term_destination = term_destination_inplace;
  mem_dest->output_size = size;
  mem_dest->output = dst;

  jpeg_start_compress(&cinfo, TRUE);

  row_stride = xsize * zsize;	/* JSAMPLEs per row in image_buffer */
  cont = cinfo.image_height -1;
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = src + row_stride * cont;
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    cont--;
  }

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  return 1;
}

int
_writeJPEGflipMemory(unsigned char **dst, size_t *size, unsigned char *src,
		    int xsize, int ysize, int zsize, int quality)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];
  int row_stride;
  mem_dest_ptr mem_dest;
		
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  cinfo.image_width = xsize;
  cinfo.image_height = ysize;
  cinfo.input_components = zsize;
  if(zsize ==3) cinfo.in_color_space = JCS_RGB;
  else if(zsize ==1) cinfo.in_color_space = JCS_GRAYSCALE;
  else {
    fprintf(stderr, "can't support %s componets\n", (char*)cinfo.input_components);
    return 0;
  }
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE);

  if (cinfo.dest == NULL) {
    cinfo.dest = (struct jpeg_destination_mgr *)
      (*cinfo.mem->alloc_small) ((j_common_ptr) &cinfo, JPOOL_PERMANENT,sizeof(memory_destination_mgr));
  }

  mem_dest = (mem_dest_ptr) cinfo.dest;
  mem_dest->pub.init_destination = init_destination;
  mem_dest->pub.empty_output_buffer = empty_output_buffer;
  mem_dest->pub.term_destination = term_destination_inplace;
  mem_dest->output_size = size;
  mem_dest->output = dst;

  jpeg_start_compress(&cinfo, TRUE);

  row_stride = xsize * zsize;	/* JSAMPLEs per row in image_buffer */
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = &src[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  return 1;
}

int
writeJPEGmemory(GLubyte **dst, size_t *size, GLubyte *src,
		GLsizei xsize, GLsizei ysize, GLsizei zsize)
{ /* default quality = 75 */
    return _writeJPEGmemory(dst, size, src, xsize, ysize, zsize, 75);
}

int
writeJPEGflipMemory(GLubyte **dst, size_t *size, GLubyte *src,
		  GLsizei xsize, GLsizei ysize, GLsizei zsize)
{ /* default quality = 75 */
    return _writeJPEGflipMemory(dst, size, src, xsize, ysize, zsize, 75);
}

int
_writeJPEGimageMemory(GLubyte **dst, size_t *size, IMAGE *image, int quality)
{
    if(image->type == IMAGE_DATA_UBYTE) {
	return _writeJPEGmemory(dst, size, image->data, 
			       image->width, image->height, image->depth, quality);
    } else if(image->type == IMAGE_FLIPDATA_UBYTE) {
	return _writeJPEGflipMemory(dst, size, image->data, 
				  image->width, image->height, image->depth, quality);
    } else {
	fprintf(stderr, "writeJPEGimageMemory: data type is not ubyte!\n");
	return 0;
    }
}

int
writeJPEGimageMemory(GLubyte **dst, size_t *size, IMAGE *image)
{
    return _writeJPEGimageMemory(dst, size, image, 75);
}

int
_writeJPEGflipImageMemory(GLubyte **dst, size_t *size, IMAGE *image, int quality)
{
    return _writeJPEGimageMemory(dst, size, image, quality);
}

int
writeJPEGflipImageMemory(GLubyte **dst, size_t *size, IMAGE *image)
{
    return writeJPEGimageMemory(dst, size, image);
}

METHODDEF(void) init_destination (j_compress_ptr cinfo)
{
  mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;

  int buffer_size = cinfo->image_width * cinfo->image_height * cinfo->input_components;
  dest->buffer = (JOCTET *)(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_IMAGE, buffer_size);

  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = buffer_size;
}

METHODDEF(boolean) empty_output_buffer (j_compress_ptr cinfo)
{
  ERREXIT(cinfo, JERR_NOTIMPL);
  return FALSE;
}

METHODDEF(void) term_destination (j_compress_ptr cinfo)
{
  mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;
  int buffer_size = cinfo->image_width*cinfo->image_height*cinfo->input_components;
  *dest->output_size = buffer_size - dest->pub.free_in_buffer;
  //*dest->output = dest->buffer;
  //  *dest->output = (unsigned char *)malloc(*dest->output_size);
  memcpy(*dest->output, dest->buffer, *dest->output_size);
#ifdef DEBUG
  printf("ADDRESS *dest->output: %d\n", *dest->output);
#endif
}

METHODDEF(void) term_destination_inplace (j_compress_ptr cinfo)
{
  mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;
  int buffer_size = cinfo->image_width*cinfo->image_height*cinfo->input_components;
  *dest->output_size = buffer_size - dest->pub.free_in_buffer;
  //*dest->output = dest->buffer;
  *dest->output = (unsigned char *)malloc(*dest->output_size);
  memcpy(*dest->output, dest->buffer, *dest->output_size);
#ifdef DEBUG
  printf("ADDRESS *dest->output: %d\n", *dest->output);
#endif
}

METHODDEF(void) my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


METHODDEF(void) init_source (j_decompress_ptr cinfo);
METHODDEF(boolean) fill_input_buffer (j_decompress_ptr cinfo);
METHODDEF(void) skip_input_data (j_decompress_ptr cinfo, long num_bytes);
METHODDEF(void) term_source (j_decompress_ptr cinfo);

int
loadJPEGflipMemory(unsigned char *dst, unsigned char *src, size_t size, 
		  GLsizei xsize, GLsizei ysize, GLsizei zsize)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  mem_src_ptr mem_src  = NULL;
  
  JSAMPARRAY js;
  int row_stride;
  long count;

  if(dst == NULL) {
    fprintf(stderr, "memory error (this function needs allocated memory)\n");
    return 0;
  }

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    return 0;
  }
  jpeg_create_decompress(&cinfo);

  if (cinfo.src == NULL) {	/* first time for this JPEG object? */
    cinfo.src = (struct jpeg_source_mgr *)
      (cinfo.mem->alloc_small) ((j_common_ptr) &cinfo, JPOOL_PERMANENT,
				((size_t) sizeof(memory_source_mgr)));
    mem_src = (mem_src_ptr) cinfo.src;
    mem_src->buffer = (JOCTET *)
      (cinfo.mem->alloc_small) ((j_common_ptr) &cinfo, JPOOL_PERMANENT,
				  INPUT_BUF_SIZE * ((size_t) sizeof(JOCTET)));
  }
  mem_src = (mem_src_ptr) cinfo.src;
  mem_src->pub.init_source = init_source;
  mem_src->pub.fill_input_buffer = fill_input_buffer;
  mem_src->pub.skip_input_data = skip_input_data;
  mem_src->pub.resync_to_restart = jpeg_resync_to_restart;
  mem_src->pub.term_source = term_source;
  mem_src->inbuf = src;
  mem_src->len   = (int)size;
  mem_src->pub.bytes_in_buffer = 0;
  mem_src->pub.next_input_byte = NULL; 

  (void) jpeg_read_header(&cinfo, TRUE);
  if(xsize != (GLsizei)cinfo.image_width || ysize != (GLsizei)cinfo.image_height) {
      fprintf(stderr, "Incorrect Size:%d,%d(%d,%d)\n", 
	      cinfo.image_width, cinfo.image_height, xsize, ysize);
      return 0;
  }
  //  *dst = (unsigned char *) malloc(cinfo.image_width*cinfo.image_height*3);
  //  jpeg_destroy_decompress(&cinfo);

  (void) jpeg_start_decompress(&cinfo);
  if(zsize != cinfo.output_components){
      fprintf(stderr, "Incorrect Components:%d(%d)\n", 
	      cinfo.output_components, zsize);
      (void) jpeg_finish_decompress(&cinfo);
      return 0;
  }

  (void) jpeg_start_decompress(&cinfo);
  row_stride = cinfo.output_width * cinfo.output_components;
  js = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  count=0;
  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, js, 1);
	memcpy(dst+(count*row_stride), js[0], row_stride);
    count++;
  }

  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  return 1;
}

int
loadJPEGmemory(unsigned char *dst, unsigned char *src, size_t size, 
	       GLsizei xsize, GLsizei ysize, GLsizei zsize)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  mem_src_ptr mem_src  = NULL;
  
  JSAMPARRAY js;
  int row_stride;
  long count;

  if(dst == NULL) {
    fprintf(stderr, "memory error (this function needs allocated memory)\n");
    return 0;
  }

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    return 0;
  }
  jpeg_create_decompress(&cinfo);

  if (cinfo.src == NULL) {	/* first time for this JPEG object? */
    cinfo.src = (struct jpeg_source_mgr *)
      (cinfo.mem->alloc_small) ((j_common_ptr) &cinfo, JPOOL_PERMANENT,
				((size_t) sizeof(memory_source_mgr)));
    mem_src = (mem_src_ptr) cinfo.src;
    mem_src->buffer = (JOCTET *)
      (cinfo.mem->alloc_small) ((j_common_ptr) &cinfo, JPOOL_PERMANENT,
				  INPUT_BUF_SIZE * ((size_t) sizeof(JOCTET)));
  }
  mem_src = (mem_src_ptr) cinfo.src;
  mem_src->pub.init_source = init_source;
  mem_src->pub.fill_input_buffer = fill_input_buffer;
  mem_src->pub.skip_input_data = skip_input_data;
  mem_src->pub.resync_to_restart = jpeg_resync_to_restart;
  mem_src->pub.term_source = term_source;
  mem_src->inbuf = src;
  mem_src->len   = (int)size;
  mem_src->pub.bytes_in_buffer = 0;
  mem_src->pub.next_input_byte = NULL; 

  (void) jpeg_read_header(&cinfo, TRUE);
  if(xsize != (GLsizei)cinfo.image_width || ysize != (GLsizei)cinfo.image_height) {
      fprintf(stderr, "Incorrect Size:%d,%d(%d,%d)\n", 
	      cinfo.image_width, cinfo.image_height, xsize, ysize);
      return 0;
  }
  //  *dst = (unsigned char *) malloc(cinfo.image_width*cinfo.image_height*3);
  //  jpeg_destroy_decompress(&cinfo);

  (void) jpeg_start_decompress(&cinfo);
  if(zsize != cinfo.output_components){
      fprintf(stderr, "Incorrect Components:%d(%d)\n", 
	      cinfo.output_components, zsize);
      (void) jpeg_finish_decompress(&cinfo);
      return 0;
  }

  row_stride = cinfo.output_width * cinfo.output_components;
  js = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
  count=cinfo.output_height-1;
  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, js, 1);
	memcpy(dst+(count*row_stride), js[0], row_stride);
    count--;
  }

  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  return 1;
}

int
loadJPEGimageMemory(IMAGE *image, unsigned char *src, size_t size)
{
    /* read data */
    if(loadJPEGmemory(image->data, src, size,
		     image->width, image->height, image->depth) == 0){
	fprintf(stderr,"JPEG image memory read Error!!\n");
	return 0;
    }
    return 1;
}

int
loadJPEGflipImageMemory(IMAGE *image, unsigned char *src, size_t size)
{
    /* read data */
    if(loadJPEGflipMemory(image->data, src, size,
			image->width, image->height, image->depth) == 0){
	fprintf(stderr,"JPEG image memory read Error!!\n");
	return 0;
    }
    return 1;
}

GLubyte *
readJPEGflipMemory(unsigned char *src, size_t size, int *xsize, int *ysize, int *zsize)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  GLubyte *dst = NULL;
  mem_src_ptr mem_src  = NULL;

  JSAMPARRAY js;
  int row_stride;
  long count;
  
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    return (GLubyte *)NULL;
  }
  jpeg_create_decompress(&cinfo);

  if (cinfo.src == NULL) {	/* first time for this JPEG object? */
    cinfo.src = (struct jpeg_source_mgr *)
      (cinfo.mem->alloc_small) ((j_common_ptr) &cinfo, JPOOL_PERMANENT,
				((size_t) sizeof(memory_source_mgr)));
    mem_src = (mem_src_ptr) cinfo.src;
    mem_src->buffer = (JOCTET *)
      (cinfo.mem->alloc_small) ((j_common_ptr) &cinfo, JPOOL_PERMANENT,
				  INPUT_BUF_SIZE * ((size_t) sizeof(JOCTET)));
  }
  mem_src = (mem_src_ptr) cinfo.src;
  mem_src->pub.init_source = init_source;
  mem_src->pub.fill_input_buffer = fill_input_buffer;
  mem_src->pub.skip_input_data = skip_input_data;
  mem_src->pub.resync_to_restart = jpeg_resync_to_restart;
  mem_src->pub.term_source = term_source;
  mem_src->inbuf = src;
  mem_src->len   = (int)size;
  mem_src->pub.bytes_in_buffer = 0;
  mem_src->pub.next_input_byte = NULL; 

  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);
  *xsize = cinfo.image_width;
  *ysize = cinfo.image_height;
  *zsize = cinfo.output_components;
  dst = (unsigned char *)malloc(cinfo.image_width*cinfo.image_height
				*cinfo.output_components);

  row_stride = cinfo.output_width * cinfo.output_components;
  js = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
  count=0;
  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, js, 1);
	memcpy(dst+(count*row_stride), js[0], row_stride);
    count++;
  }

  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  return (GLubyte *)dst;
}

GLubyte *
readJPEGmemory(unsigned char *src, size_t size, int *xsize, int *ysize, int *zsize)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  GLubyte *dst = NULL;
  mem_src_ptr mem_src  = NULL;

  JSAMPARRAY js;
  int row_stride;
  long count;
  
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    return (GLubyte *)NULL;
  }
  jpeg_create_decompress(&cinfo);

  if (cinfo.src == NULL) {	/* first time for this JPEG object? */
    cinfo.src = (struct jpeg_source_mgr *)
      (cinfo.mem->alloc_small) ((j_common_ptr) &cinfo, JPOOL_PERMANENT,
				((size_t) sizeof(memory_source_mgr)));
    mem_src = (mem_src_ptr) cinfo.src;
    mem_src->buffer = (JOCTET *)
      (cinfo.mem->alloc_small) ((j_common_ptr) &cinfo, JPOOL_PERMANENT,
				  INPUT_BUF_SIZE * ((size_t) sizeof(JOCTET)));
  }
  mem_src = (mem_src_ptr) cinfo.src;
  mem_src->pub.init_source = init_source;
  mem_src->pub.fill_input_buffer = fill_input_buffer;
  mem_src->pub.skip_input_data = skip_input_data;
  mem_src->pub.resync_to_restart = jpeg_resync_to_restart;
  mem_src->pub.term_source = term_source;
  mem_src->inbuf = src;
  mem_src->len   = (int)size;
  mem_src->pub.bytes_in_buffer = 0;
  mem_src->pub.next_input_byte = NULL; 

  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);

  *xsize = cinfo.image_width;
  *ysize = cinfo.image_height;
  *zsize = cinfo.output_components;
  dst = (unsigned char *)malloc(cinfo.image_width*cinfo.image_height
				*cinfo.output_components);
  //  jpeg_destroy_decompress(&cinfo);

  row_stride = cinfo.output_width * cinfo.output_components;
  js = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
  count=cinfo.output_height-1;
  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, js, 1);
	memcpy(dst+(count*row_stride), js[0], row_stride);
    count--;
  }

  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  return (GLubyte *)dst;
}

IMAGE *
readJPEGimageMemory(unsigned char *src, size_t size)
{
    GLsizei  height, width, depth;
    GLenum format;
    IMAGE *image;
    
    /* read data */
    image = (IMAGE *)malloc(sizeof(IMAGE));
    if((image->data = readJPEGmemory(src, size, &width, &height, &depth)) == NULL){
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

IMAGE *
readJPEGflipImageMemory(unsigned char *src, size_t size)
{
    GLsizei  height, width, depth;
    GLenum format;
    IMAGE *image;
    
    /* read data */
    image = (IMAGE *)malloc(sizeof(IMAGE));
    if((image->data = readJPEGflipMemory(src, size, &width, &height, &depth)) == NULL){
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
    image->type = IMAGE_FLIPDATA_UBYTE;

/*    printf("image data width =%d, height =%d, depth =%d \n",width,height,depth);*/

    return (IMAGE *) image;
}

METHODDEF(void) init_source (j_decompress_ptr cinfo) {
  mem_src_ptr src = (mem_src_ptr) cinfo->src;
  src->start_of_file = TRUE;
}

METHODDEF(boolean) fill_input_buffer (j_decompress_ptr cinfo) {
  mem_src_ptr src = (mem_src_ptr) cinfo->src;
  size_t nbytes;
  unsigned char *cptr;

  if(src->start_of_file)src->resbyte=src->len;
  if(src->resbyte-INPUT_BUF_SIZE<0)nbytes=src->resbyte;
  else nbytes=INPUT_BUF_SIZE;

  if (nbytes <= 0) {
    if (src->start_of_file) printf("jerror\n");//ERREXIT(cinfo, JERR_INPUT_EMPTY);
    //WARNMS(cinfo, JWRN_JPEG_EOF);
    src->buffer[0] = (JOCTET) 0xFF;
    src->buffer[1] = (JOCTET) JPEG_EOI;
    nbytes = 2;
  } else {
    cptr=src->inbuf+src->len-src->resbyte;
    memcpy(src->buffer, cptr, nbytes);
	src->resbyte -= nbytes;
  }
  src->pub.next_input_byte = src->buffer;
  src->pub.bytes_in_buffer = nbytes;
  src->start_of_file = FALSE;

#ifdef DEBUG
  fprintf(stderr,"len:%d resbyte:%d nbytes:%d\n",src->len,src->resbyte,nbytes);
  {
    int i;
    for(i=0;i<24;i++)printf("%d ", src->buffer[i]);
    printf("\n");
  }
#endif
  return TRUE;
}

METHODDEF(void) skip_input_data (j_decompress_ptr cinfo, long num_bytes) {
  mem_src_ptr src = (mem_src_ptr) cinfo->src;

  if (num_bytes > 0) {
    while (num_bytes > (long) src->pub.bytes_in_buffer) {
      num_bytes -= (long) src->pub.bytes_in_buffer;
      (void) fill_input_buffer(cinfo);
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}

METHODDEF(void) term_source (j_decompress_ptr cinfo) {
  /* no work necessary here */
}
