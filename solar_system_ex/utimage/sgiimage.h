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
#ifndef _SGI_IMAGE_H_
#define _SGI_IMAGE_H_

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

extern GLubyte *readImage(char *imagefile, GLsizei *xsize, GLsizei *ysize, GLsizei *zsize);
extern int  writeImage(char *imagefile, GLubyte *image, GLsizei xsize, GLsizei ysize, GLsizei zsize);
extern int  writeFastImage(char *imagefile, GLubyte *image, GLsizei xsize, GLsizei ysize, GLsizei zsize);

extern IMAGE *readImageFile(char *imagefile);
extern int writeImageFile(char *imagefile, IMAGE *image);
extern int writeFastImageFile(char *imagefile, IMAGE *image);

extern int getImageSize(char *imagefile, int *xsize, int *ysize, int *zsize);

extern int loadImage(char *imagefile, GLubyte *image, GLsizei xsize, GLsizei ysize, GLsizei zsize);
extern int loadImageFile(IMAGE *image, char *imagefile);

#ifdef __cplusplus
}
#endif

#endif /*_SGI_IMAGE_H_*/
