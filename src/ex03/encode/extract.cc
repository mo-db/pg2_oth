#include "std_lib_facilities.h"
#include <cstring>
#define RTS 0xc3

int process(int input)  {
	input += 3;
	input *= 2;
	return input;
}


typedef int (*fp)(int);

typedef unsigned char _byte;


int copyToArray(fp source, _byte **dest)  {
    _byte rs = RTS;  int p = 0;  _byte *code = (_byte *)source;
   while (memcmp((_byte *)(code + p), &rs, 1)) {
	   p++;
   }
   p = p + 1;
   *dest = (_byte *)malloc(p);
   memcpy(*dest, code, p);
   return p; 
}

int main(int argc, char **argv)  {
	ofstream file;   _byte *arrayOfBytes;
	if (argc < 2)  {
		cerr << "Usage: " << argv[0] << " <outfile>" << endl;
		return -1;
	}
	file.open(argv[1], fstream::out);
	int size = copyToArray(process, &arrayOfBytes);
        cout << "process has " << size
             << " bytes" << endl;
	const char *data = (const char *)arrayOfBytes;
        file.write(data, size);
	file.close( );
	cout << "and was written to " << argv[1] << endl;
	return 0;
}



