#ifndef _JPEG_IMAGE_H_
#define _JPEG_IMAGE_H_

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

extern GLubyte *readJPEGimage(char *imagefile, GLsizei *xsize, GLsizei *ysize, GLsizei *zsize);
extern IMAGE *readJPEGimageFile(char *imagefile);
extern int readJPEGimageFile2(IMAGE *image, char *imagefile);

extern GLubyte *readJPEGflipImage(char *imagefile, GLsizei *xsize, GLsizei *ysize, GLsizei *zsize);
extern IMAGE *readJPEGflipImageFile(char *imagefile);
extern int readJPEGflipImageFile2(IMAGE *image, char *imagefile);

extern GLubyte *readJPEGrawImage(char *imagefile, GLsizei *xsize, GLsizei *ysize, GLsizei *zsize,
				 size_t *size);
extern IMAGE *readJPEGrawImageFile(char *imagefile);
extern int readJPEGrawImageFile2(IMAGE *image, char *imagefile);

extern int loadJPEGimage(char *imagefile, GLubyte *imagedata,
			 GLsizei xsize, GLsizei ysize, GLsizei zsize);
extern int loadJPEGimageFile(IMAGE *image, char *imagefile);
extern int loadJPEGflipImage(char *imagefile, GLubyte *imagedata,
			 GLsizei xsize, GLsizei ysize, GLsizei zsize);
extern int loadJPEGflipImageFile(IMAGE *image, char *imagefile);

extern int loadJPEGrawImage(char *imagefile, GLubyte *imagedata, size_t size,
			    GLsizei xsize, GLsizei ysize, GLsizei zsize);
extern int loadJPEGrawImageFile(IMAGE *image, char *imagefile);

extern int _writeJPEGimage(char *imagefile, GLubyte *imagedata,
                           GLsizei xsize, GLsizei ysize, GLsizei zsize, int quality);
extern int writeJPEGimage(char *imagefile, GLubyte *imagedata, /* default quality = 75 */
                          GLsizei xsize, GLsizei ysize, GLsizei zsize);
extern int _writeJPEGflipImage(char *imagefile, GLubyte *imagedata,
			      GLsizei xsize, GLsizei ysize, GLsizei zsize, int quality);
extern int writeJPEGflipImage(char *imagefile, GLubyte *imagedata, /* default quality = 75 */
			     GLsizei xsize, GLsizei ysize, GLsizei zsize);
extern int _writeJPEGrawImage(char *imagefile, GLubyte *imagedata, size_t size,
			      GLsizei xsize, GLsizei ysize, GLsizei zsize, int quality);
extern int writeJPEGrawImage(char *imagefile, GLubyte *imagedata, size_t size,
			     GLsizei xsize, GLsizei ysize, GLsizei zsize);

extern int _writeJPEGimageFile(char *imagefile, IMAGE *image, int quality);
extern int writeJPEGimageFile(char *imagefile, IMAGE *image); /* default quality = 75 */
extern int _writeJPEGflipImageFile(char *imagefile, IMAGE *image, int quality);
extern int writeJPEGflipImageFile(char *imagefile, IMAGE *image); /* default quality = 75 */
extern int _writeJPEGrawImageFile(char *imagefile, IMAGE *image, int quality);
extern int writeJPEGrawImageFile(char *imagefile, IMAGE *image); /* default quality = 75 */

#ifdef __cplusplus
}
#endif

#endif /*_JPEG_IMAGE_H_*/
