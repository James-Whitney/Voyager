#pragma once
#ifndef _BITMAP_H
#define _BITMAP_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>

class BitMap {

public:

   typedef struct {
      unsigned char r;
      unsigned char g;
      unsigned char b;
   } pixel;

   void load(std::string path);

   std::vector<pixel> getPixels();
   pixel getPixel(int x, int y);

   int width = 0;
   int height = 0;

private:

   #pragma pack(push, 1)
   struct fileHeader {
      uint16_t bfType;
      uint32_t bfSize;
      uint16_t bfReserved1;
      uint16_t bfReserved2;
      uint32_t bfOffBits;
   };
   #pragma pack(pop)

   #pragma pack(push, 1)
   struct infoHeader {
      uint32_t biSize;
      uint32_t biWidth;
      uint32_t biHeight;
      uint16_t biPlanes;
      uint16_t biBitCount;
      uint32_t biCompression;
      uint32_t biSizeImage;
      uint32_t biXPelsPerMeter;
      uint32_t biYPelsPerMeter;
      uint32_t biClrUsed;
      uint32_t biClrImportant;
   };
   #pragma pack(pop)

   std::vector<pixel> pixels;

};

#endif