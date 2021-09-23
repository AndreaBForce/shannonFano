#ifndef __IO__
#define __IO__

//struttura rappresentante il byte
struct byte {
	unsigned char bit0 : 1;
	unsigned char bit1 : 1;
	unsigned char bit2 : 1;
	unsigned char bit3 : 1;
	unsigned char bit4 : 1;
	unsigned char bit5 : 1;
	unsigned char bit6 : 1;
	unsigned char bit7 : 1;
};
typedef struct byte Byte;


Byte createByte(unsigned char value);
extern int lunghezzafile;
unsigned char* readIO(const char* n_file);
void writeBinEncode(unsigned char* inputData,int lenght,const char* n_file_o);
#endif