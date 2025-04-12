#include "std_lib_facilities.h"
#include <sys/mman.h>
#include <cstring>
#define RTS 0xc3

typedef char _byte;
typedef int (*fp)(int);


fp restoreFromFile(string fileName, int bytes)  {
    ifstream inputFile(fileName, ifstream::binary);    // array on heap  
    _byte *dataArray = (_byte *)malloc(bytes);           // is filled 
    inputFile.read(dataArray, bytes);                  // with code, 
    _byte *exe = (_byte *) mmap ( 0, bytes,              // aligned 
       PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANON, -1, 0 );
    memcpy(exe, dataArray, bytes);  free(dataArray);    // to page boundary
    mprotect(exe, bytes, PROT_READ | PROT_EXEC); // and flagged executable 
    return (fp)exe;
}

int main(int argc, char **argv) {
    if (argc != 4)  { 
       cerr << "Usage: " << argv[0] << " <file_name>"\
               " <file_size> <input>" << endl; 
       return -1;    }    
    fp plugin = restoreFromFile(argv[1], stoi(argv[2]));
    int resultValue, originalValue = stoi(argv[3]);
    cout << "read " << stoi(argv[2]) << " bytes" << endl;
    resultValue = (*plugin)(originalValue);  
    cout << "the result of applying " << argv[1] 
         << " to " << originalValue << " is " << resultValue << endl;
    return 0;  }
