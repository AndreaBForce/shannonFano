#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "io.h"
#include "newfano.h"

/**
 * Programma principale di avvio
 * Tramite il comando di richiamo e gli argomenti giusti si avvia il programma
 * 
 *  ./compressor -c fileDaComprimere fileOut
 *  ./compressor -d filedDaDecomprimere fileOut
 *
 */
int main(int argc, char const* argv[]) {

	if(argc != 4){

		//Gestione argomeni < 4

		printf("Errore: Numero argomenti non valido.\n");
		printf("Esempio di avvio programma: \n");
		printf("./nome_prog -opzione file_input file_output\n");
		printf("Opzioni disponibili: \n");
		printf("-c : compressione \n");
		printf("-d : decompressione \n");
	}else{
		if(strcmp(argv[1],"-c") == 0){

			//Parte di compressione

			printf("Notifica: Avvio compressione del file.\n");

			unsigned char* data = readIO(argv[2]);
			
			countFrequency(data,lunghezzafile);
			
			writeBinEncode(data,lunghezzafile,argv[3]);
			
			printf("Notifica: Compressione avvenuta con successo.\n");
	    	

		}else if(strcmp(argv[1],"-d") == 0){

			//Parte di decompressione

			printf("Notifica: Avvio decompressione del file.\n");
			shannon_decode(argv[2],argv[3]);
			printf("Notifica: Decompressione avvenuta con successo.\n");

		}else{

			//Gestione argomenti non validi

			printf("Errore: Opzione inserita non valida.\n");
			printf("Opzioni disponibili: \n");
			printf("-c : compressione \n");
			printf("-d : decompressione \n");
		}
	}

	return 0;
}