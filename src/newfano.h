#ifndef __NEWFANO__
#define __NEWFANO__

//Struttura di tipo carattere
typedef struct carattere{
    int carattere;
    double frequenza;
    int binCount;
    unsigned char* binary;
}Carattere;

//Struttura di tipo nodo dell'albero
struct nodeAl {
	Carattere* valore;
	struct nodeAl* left;
	struct nodeAl* right;
};

void shannon_decode(const char* file_comp,const char* file_out);
unsigned char codifiche[256][256];
double media(Carattere* datiInput, int lenght);
int findDivisioneDx(Carattere* datiInput, int lenght, double media);
void splitFrequenze(Carattere* datiInput, int lenght, struct nodeAl* radice);
extern int larghezzaDizionario;
extern Carattere mastroFreq[256];
void countFrequency(unsigned char* inputData,int lenght);
#endif