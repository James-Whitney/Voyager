#include "include/BitMap.h"

void BitMap::load(std::string path) {
   std::ifstream input_file;
   input_file.open(path, std::ios::in | std::ios::binary | std::ios::ate);

   if (input_file.fail()) {
      std::cerr << "Failed to load heightmap: " << path << std::endl;
      exit(1);
   }

   std::streampos file_size = input_file.tellg();

   // Read the file header
   BitMap::fileHeader file_header;
   input_file.seekg(0);
   input_file.read(reinterpret_cast<char*>(&file_header), sizeof(BitMap::fileHeader));

   // Read the info header
   BitMap::infoHeader info_header;
   input_file.seekg(sizeof(BitMap::fileHeader));
   input_file.read(reinterpret_cast<char*>(&info_header), sizeof(BitMap::infoHeader));

   this->width = info_header.biWidth;
   this->height = info_header.biHeight;

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

   // Close the file
   input_file.close();
}

std::vector<BitMap::pixel> BitMap::getPixels() {
   return pixels;
}

BitMap::pixel BitMap::getPixel(int x, int y) {
   int i = (y * this->width + x);
   return pixels.at(i);
}