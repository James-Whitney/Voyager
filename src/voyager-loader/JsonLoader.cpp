#include "include/JsonLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#define _JSONLOADER_SHOW_IO_CONTENTS 0 // set to 1 to print the file read/writes

using namespace rapidjson;
using namespace std;

Document loadDocument(string path) {

   ifstream in_file;
   in_file.open(path);
   if (!in_file) {
      cerr << "Failed to open " << path << " for reading." << endl;
      exit(1);
   }

   string in_file_contents { istreambuf_iterator<char>(in_file), istreambuf_iterator<char>() };

#if _JSONLOADER_SHOW_IO_CONTENTS
   stringstream ss;
   ss << "+-< reading >----------[ " << path << " ]----------+";
   cout << ss.str() << endl;
   cout << in_file_contents << endl;
   int len = ss.str().size();
   ss = stringstream();
   ss << "+";
   while (len-- > 2) ss << "-";
   ss << "+";
   cout << ss.str() << endl;
#endif

   Document result;
   result.Parse(in_file_contents.c_str());

   in_file.close();
   return result;

}

void storeDocument(Document document, string path) {

   ofstream out_file;
   out_file.open(path);
   if (!out_file) {
      cerr << "Failed to open " << path << " for writing." << endl;
      exit(1);
   }

   StringBuffer buffer;
   Writer<StringBuffer> writer(buffer);
   document.Accept(writer);

   string out_file_contents = document.GetString();

#if _JSONLOADER_SHOW_IO_CONTENTS
   stringstream ss;
   ss << "+--< writing >---------[ " << path << " ]----------+";
   cout << ss.str() << endl;
   cout << out_file_contents << endl;
   int len = ss.str().length();
   ss = stringstream();
   ss << '+';
   while (len-- > 2) ss << '-';
   ss << '+';
   cout << ss.str() << endl;
#endif

   out_file << out_file_contents << endl;

   out_file.close();

}
