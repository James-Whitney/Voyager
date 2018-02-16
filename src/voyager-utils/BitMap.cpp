#include "include/BitMap.h"

void BitMap::load(std::string path) {
   std::ifstream input_file;
   input_file.open(path, std::ios::in | std::ios::binary | std::ios::ate);

   if (input_file.fail()) {
      std::cerr << "Failed to load heightmap: " << path << std::endl;
      exit(1);
   }

   std::streampos file_size = input_file.tellg();
   std::cout << "size: " << file_size << std::endl;
   
   // Read the file header
   BitMap::fileHeader file_header;
   input_file.seekg(0);
   input_file.read(reinterpret_cast<char*>(&file_header), sizeof(BitMap::fileHeader));

   std::cout << "--- File header ---" << std::endl;
   std::cout << "bfType: " << file_header.bfType << std::endl;
   std::cout << "bfSize: " << file_header.bfSize << std::endl;
   std::cout << "bfReserved1: " << file_header.bfReserved1 << std::endl;
   std::cout << "bfReserved2: " << file_header.bfReserved2 << std::endl;
   std::cout << "bfOffBits: " << (std::streampos)file_header.bfOffBits << std::endl;

   // Read the info header
   BitMap::infoHeader info_header;
   input_file.seekg(sizeof(BitMap::fileHeader));
   input_file.read(reinterpret_cast<char*>(&info_header), sizeof(BitMap::infoHeader));

   this->width = info_header.biWidth;
   this->height = info_header.biHeight;

   std::cout << "--- Info header ---" << std::endl;
   std::cout << "biSize: " << info_header.biSize << std::endl;
   std::cout << "biWidth: " << info_header.biWidth << std::endl;
   std::cout << "biHeight: " << info_header.biHeight << std::endl;
   std::cout << "biPlanes: " << info_header.biPlanes << std::endl;
   std::cout << "biBitCount: " << info_header.biBitCount << std::endl;
   std::cout << "biCompression: " << info_header.biCompression << std::endl;
   std::cout << "biSizeImage: " << info_header.biSizeImage << std::endl;
   std::cout << "biXPelsPerMeter: " << info_header.biXPelsPerMeter << std::endl;
   std::cout << "biYPelsPerMeter: " << info_header.biYPelsPerMeter << std::endl;
   std::cout << "biClrUsed: " << info_header.biClrUsed << std::endl;
   std::cout << "biClrImportant: " << info_header.biClrImportant << std::endl;

   // Read the pixel data
   this->pixels.clear();
   std::streampos offset = file_header.bfOffBits;
   char buffer[file_size - offset];
   input_file.seekg(offset);
   input_file.read(buffer, sizeof(buffer));

   int padding = 0;
   int scanlinebytes = width * 3;
	while ((scanlinebytes + padding) % 4 != 0 )
      padding++;

   int i = 0;
   for (int y = 0; y < this->height; y++) {
      for (int x = 0; x < this->width; x++) {
         BitMap::pixel p;
         p.r = buffer[i];
         p.g = buffer[i + 1];
         p.b = buffer[i + 2];

         this->pixels.push_back(p);

         i += 3;
      }

      i += padding;
   }

   for (int x = 0; x < this->width; x++) {
      for (int y = 0; y < this->height; y++) {
         BitMap::pixel p = this->getPixel(x, y);
         std::cout << "(" << x << ", " << y << "): (" << (unsigned int)p.r << ", " << (unsigned int)p.g << ", " << (unsigned int)p.b << ")" << std::endl;
      }
   }

   // Close the file
   input_file.close();
}

std::vector<BitMap::pixel> BitMap::getPixels() {
   return pixels;
}

BitMap::pixel BitMap::getPixel(int x, int y) {
   int i = (y * this->height + x);
   return pixels.at(i);
}