#ifndef _JPEG_CODEC_H_
#define _JPEG_CODEC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <GL/glut.h>

#ifndef _IMAGE_S_
#define _IMAGE_S_

#define IMAGE_DATA_UBYTE	0
#define IMAGE_DATA_FLOAT	1
#define IMAGE_FLIPDATA_UBYTE	2
#define IMAGE_FLIPDATA_FLOAT	3
#define IMAGE_JPEG_UBYTE	4

typedef struct image_s
{
    GLsizei     width;
    GLsizei     height;
    GLsizei     depth;
    GLsizei	type; /* 0: GLubyte; 1: GLfloat; 4: JPEG; */
    GLenum      format;
    size_t	size;
    GLubyte     *data;
    GLfloat	*float_data;
} IMAGE;
#endif /*_IMAGE_S_*/

extern GLubyte *readJPEGmemory(unsigned char *src, size_t size,
			       GLsizei *xsize, GLsizei *ysize, GLsizei *zsize);
extern IMAGE *readJPEGimageMemory(unsigned char *src, size_t size);

extern GLubyte *readJPEGflipMemory(GLubyte *src, size_t size,
				  GLsizei *xsize, GLsizei *ysize, GLsizei *zsize);
extern IMAGE *readJPEGflipImageMemory(unsigned char *src, size_t size);


extern int loadJPEGmemory(unsigned char *dst, unsigned char *src, size_t size, 
			  GLsizei xsize, GLsizei ysize, GLsizei zsize);
extern int loadJPEGimageMemory(IMAGE *image, unsigned char *src, size_t size);
extern int loadJPEGflipMemory(unsigned char *dst, unsigned char *src, size_t size, 
			     GLsizei xsize, GLsizei ysize, GLsizei zsize);
extern int loadJPEGflipImageMemory(IMAGE *image, unsigned char *src, size_t size);

extern int _writeJPEGmemory(GLubyte **dst, size_t *size, GLubyte *src,
			    GLsizei xsize, GLsizei ysize, GLsizei zsize, int quality);
extern int _writeJPEGflipMemory(GLubyte **dst, size_t *size, GLubyte *src,
			       GLsizei xsize, GLsizei ysize, GLsizei zsize, int quality);
extern int _writeJPEGimageMemory(GLubyte **dst, size_t *size, IMAGE *image, int quality);
extern int _writeJPEGflipImageMemory(GLubyte **dst, size_t *size, IMAGE *image, int quality);
 /* default quality = 75 */
extern int writeJPEGmemory(GLubyte **dst, size_t *size, GLubyte *src,
			   GLsizei xsize, GLsizei ysize, GLsizei zsize);
extern int writeJPEGflipMemory(GLubyte **dst, size_t *size, GLubyte *src,
			      GLsizei xsize, GLsizei ysize, GLsizei zsize);
extern int writeJPEGimageMemory(GLubyte **dst, size_t *size, IMAGE *image);
extern int writeJPEGflipImageMemory(GLubyte **dst, size_t *size, IMAGE *image);

extern int _saveJPEGmemory(GLubyte *dst, size_t *size, GLubyte *src,
                           GLsizei xsize, GLsizei ysize, GLsizei zsize, int quality);
extern int _saveJPEGflipMemory(GLubyte *dst, size_t *size, GLubyte *src,
			      GLsizei xsize, GLsizei ysize, GLsizei zsize, int quality);
extern int _saveJPEGimageMemory(GLubyte *dst, size_t *size, IMAGE *image, int quality);
extern int _saveJPEGflipImageMemory(GLubyte *dst, size_t *size, IMAGE *image, int quality);
 /* default quality = 75 */
extern int saveJPEGmemory(GLubyte *dst, size_t *size, GLubyte *src,
			   GLsizei xsize, GLsizei ysize, GLsizei zsize);
extern int saveJPEGflipMemory(GLubyte *dst, size_t *size, GLubyte *src,
			     GLsizei xsize, GLsizei ysize, GLsizei zsize);
extern int saveJPEGimageMemory(GLubyte *dst, size_t *size, IMAGE *image);
extern int saveJPEGflipImageMemory(GLubyte *dst, size_t *size, IMAGE *image);

#ifdef __cplusplus
}
#endif

#endif /*_JPEG_CODEC_H_*/
