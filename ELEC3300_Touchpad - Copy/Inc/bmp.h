#ifndef __BMP_H
#define __BMP_H

#include "stm32f1xx_hal.h"
typedef __packed struct
{
    uint16_t bfType;  /* specifies the file type */
    uint32_t bfSize;  /* specifies the size in bytes of the bitmap file */
    uint16_t bfReserved1;  /* reserved : must be 0 */
    uint16_t bfReserved2;  /* reserved : must be 0 */
    uint32_t bOffBits;  /* species the offset in bytes from the bitmapfileheader to the bitmap bits */

} BitMapFileHeader;

typedef __packed struct
{
  uint32_t biSize;  /* specifies the number of bytes required by the struct */
  long biWidth;  /* specifies width in pixels */
  long biHeight;  /* species height in pixels */
  uint16_t biPlanes; /* specifies the number of color planes, must be 1 */
  uint16_t biBitCount; /* specifies the number of bit per pixel */
  uint32_t biCompression; /* specifies the type of compression */
  uint32_t biSizeImage;  /* size of image in bytes */
	long biXPelsPerMeter;  /* number of pixels per meter in x axis */
	long biYPelsPerMeter;  /* number of pixels per meter in y axis */
	uint32_t biClrUsed;  /* number of colors used by the bitmap */
	uint32_t biClrImportant;  /* number of colors that are important */

} BitMapFileInfoHeader;

typedef __packed struct
{
	uint8_t rgbBlue;
	uint8_t rgbGreen;
	uint8_t rgbRed;
	uint8_t rgbReserved;
}rgbQuad;
typedef rgbQuad * LPRGBQUAD;
typedef __packed struct 
{  
	BitMapFileHeader bmfHeader; 
	BitMapFileInfoHeader bmiHeader; 
	uint32_t RGB_MASK [3] ; 
} BITMAPINFO ; 

#endif
