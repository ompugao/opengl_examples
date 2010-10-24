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
#ifndef _UT_IMAGE_H_
#define _UT_IMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <GL/glut.h>

#include "sgiimage.h"
#include "jpegimage.h"
#include "jpegcodec.h"

#define RLUM    (0.3086)
#define GLUM    (0.6094)
#define BLUM    (0.0820)

#define RINTLUM (79)
#define GINTLUM (156)
#define BINTLUM (21)

#define ILUM(r,g,b)     ((RINTLUM*(r)+GINTLUM*(g)+BINTLUM*(b))>>8)
#define LUM(r,g,b)      (RLUM*(r)+GLUM*(g)+BLUM*(b))

#define DOTOBW(optr,rptr,gptr,bptr)     *(optr) = ILUM(*(rptr),*(gptr),*(bptr))

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
    GLsizei	type; /* 0: GLubyte; 1: GLfloat; 4:JPEG; */
    GLenum      format;
    size_t	size;
    GLubyte     *data;
    GLfloat	*float_data;
} IMAGE;

#endif /*_IMAGE_S_*/

extern GLubyte getImagePixel(IMAGE *image, int x, int y, int z);
extern int putImagePixel(IMAGE *image, unsigned char buf, int x, int y, int z);

extern GLfloat getImageFloatPixel(IMAGE *image, int x, int y, int z);
extern int putImageFloatPixel(IMAGE *image, GLfloat buf, int x, int y, int z);

extern IMAGE *mallocImage(GLsizei width, GLsizei height, GLsizei depth);
extern IMAGE *mallocFloatImage(GLsizei width, GLsizei height, GLsizei depth);
extern int freeImage(IMAGE *image);
extern int freeFloatImage(IMAGE *image);

extern int copyImage(IMAGE *newimage, IMAGE *orgimage, int xoff, int yoff);
extern IMAGE *duplicateImage(IMAGE *image);
extern IMAGE *cutImage(IMAGE *image, int rs, int re, int cs, int ce);

extern IMAGE *convertFloatImage(IMAGE *image); 
extern int convertFloatImageSelf(IMAGE *image); 
extern IMAGE *invertFloatImage(IMAGE *image);
extern int invertFloatImageSelf(IMAGE *image);

extern int multipleFloatImage(IMAGE *newimage, IMAGE *image, GLfloat k);
extern int multipleFloatImageSelf(IMAGE *image, GLfloat k);

extern IMAGE *negativeImage(IMAGE *image);
extern int negativeImageSelf(IMAGE *image);
extern IMAGE *negativeFloatImage(IMAGE *image);
extern int negativeFloatImageSelf(IMAGE *image);

extern IMAGE *flipImage(IMAGE *image);
extern int flipImageSelf(IMAGE *image);
extern IMAGE *flipFloatImage(IMAGE *image);
extern int flipFloatImageSelf(IMAGE *image);

extern IMAGE *decodeJPEGimage(IMAGE *image);
extern int decodeJPEGimageSelf(IMAGE *image);
extern IMAGE *encodeJPEGimage(IMAGE *image);
extern int encodeJPEGimageSelf(IMAGE *image);
extern IMAGE *encodeJPEGflipImage(IMAGE *image);
extern int encodeJPEGflipImageSelf(IMAGE *image);

extern IMAGE *convImageRGBtoBW(IMAGE *image);

extern IMAGE *getMatchImage(IMAGE *image1, IMAGE *image2, float threshold);
extern IMAGE *getDiffImage(IMAGE *image1, IMAGE *image2, float threshold);

extern int convRGBtoYCbCr(GLubyte r, GLubyte g, GLubyte b,
                          GLubyte *Y, GLubyte *Cb, GLubyte *Cr);
extern int convYCbCrtoRGB(GLubyte Y, GLubyte Cb, GLubyte Cr,
                          GLubyte *r, GLubyte *g, GLubyte *b);
extern IMAGE *convImageRGBtoYCbCr(IMAGE *image);
extern IMAGE *convImageYCbCrtoRGB(IMAGE *image);

extern int convRGBtoXYZ(float R, float G, float B,
                        float *X, float *Y, float *Z);
extern int convXYZtoRGB(float X, float Y, float Z,
                        float *R, float *G, float *B);
extern IMAGE *convImageRGBtoXYZ(IMAGE *image);
extern IMAGE *convImageXYZtoRGB(IMAGE *image);

extern int defineNewImageSize(GLsizei *w0, GLsizei *h0, GLsizei *w1, GLsizei *h1);

#ifdef __cplusplus
}
#endif

#endif /*_UT_IMAGE_H_*/
/* utImage.h */

