#include "newfano.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>

//Array dei caratteri
Carattere mastroFreq[256];

//Array delle codifiche in binario ricavate dopo l'esecuzione di shannon fano
unsigned char codifiche[256][256];

//Dimensione del dizionario dei caratteri on frequenza != 0 (viene modificato dopo)
int larghezzaDizionario = 256;

//Metodo che crea nodo albero binario
struct nodeAl* creaNode(Carattere* carat)
{
	//Metto in memoria il nodo
	struct nodeAl* node = (struct nodeAl*)malloc(sizeof(struct nodeAl));

	//Assegno i valori per il nodo
	node->valore = carat;
	node->left = NULL;
	node->right = NULL;
	return(node);
}

//Metodo che usato per il sort dei caratteri dal quicksort
int ordinaDecrescente(const void* x, const void* y)
{
	Carattere* a = (Carattere*)x;
	Carattere* b = (Carattere*)y;
	if ((a->frequenza - b->frequenza) < 0) {
		return 1;
	}
	else if ((a->frequenza - b->frequenza) == 0) {
		return 0;
	}
	else {
		return -1;
	}
}

//Metodo che calcola la media tra i caratteri
double media(Carattere* datiInput, int lenght) {
	double totale = 0;
	for (int i = 0; i < lenght; i++) {
		totale = totale + datiInput[i].frequenza;
	}
	return totale / 2;
}

//Metodo che trova la divisione bilanciata per effetuare shannon-fano 
int findDivisioneDx(Carattere* datiInput, int lenght, double media) {
	double sommaDx = 0;
	int divisione = 0;

	//Trova dove dividere da Dx usando la media
	//come valore aleatorio
	for (int i = (lenght - 1); i > 0; i--) {
		sommaDx = sommaDx + datiInput->frequenza;
		if (sommaDx >= media) {
			return i - 1;
		}
	}
	//Else return 1
	return 1;
}

//Metodo che splitta gli array bilanciando in sotto array creando l'albero binario
//Esso segue il concetto di ShannonFano ovvero della divisione bilanciata
//Alla fine avremo come output l'albero creato,
//ma andremo a salvare le nostre codifiche in un array chiamato "codifiche", 
//che verra comodo per salvare dopo i dati tramite la ricerca per indice
void splitFrequenze(Carattere* datiInput, int lenght, struct nodeAl* radice) {
	int divisione, dimS1 = 0, dimS2 = 0;
	static int volteEntr = 0;

	//Check della grandezza array
	if (lenght > 1) {
		volteEntr++;
		divisione = findDivisioneDx(datiInput, lenght, media(datiInput, lenght));

		//Dimensioni dei due sotto array
		dimS2 = divisione + 1;
		dimS1 = (lenght - 1) - (dimS2 - 1);

		//Ogni volta crea dei sotto split1 che sono degli array di Carattere
		Carattere* split1 = (Carattere*)calloc(dimS1, sizeof(Carattere));
		Carattere* split2 = (Carattere*)calloc(dimS2, sizeof(Carattere));


		//For che assegnano i due sottoarray
		for (int k = 0; k < dimS1; k++) {
			split1[k] = datiInput[k];
			split1[k].binary[split1[k].binCount] = 0; //Asssegna il valore 0 nell'index giusto di ogni lettera
			split1[k].binCount++; //Incremento
			
		}

		for (int k = dimS1, i = 0; k < lenght; k++) {
			split2[i] = datiInput[k];
			split2[i].binary[split2[i].binCount] = 1; //Assegna il valore 1 al branch dell'index socio
			split2[i].binCount++; //Incremento
			i++;
		}

		//assegno alla parte dx e sx la nuova testa
		radice->left = creaNode(split1);
		radice->right = creaNode(split2);

		//Splitto i due in sotto sequenze e ricorsivamente richiamo la parte dopo
		splitFrequenze(split1, dimS1, radice->left);
		splitFrequenze(split2, dimS2, radice->right);
		
	}else {
		if (volteEntr == 0) {
			datiInput->binary[0] = 0;
			datiInput->binCount = 1;
			radice->left = creaNode(datiInput);
			volteEntr++;
		}

		//Arriva qui significa che ha creato lo split giusto con le codifiche adatte
		//Blocco di codice che assegna all'array codifiche le codifiche in binario
		//Viene usato dopo per ricercare velocemente i valori iniziali

		//Qua assegno al posto 0 dell'indice voluto, il numero di bit per la codifica del socio
		codifiche[radice->valore->carattere][0] = radice->valore->binCount;

		//Qua assegno dopo lo 0 tutta la codifica in binario del valore
		for(int i=1;i<radice->valore->binCount+1;i++){
			codifiche[radice->valore->carattere][i] = radice->valore->binary[i-1];
		}

	}
}

//Metodo che avvia il processo di codifica di shannon fano
//Esso crea il nodo radice sul quale si basera l'albero binario 
//Avvia la procedura ricorsiva di splitFrequenze 
void shannonFano(){

    struct nodeAl* radice = creaNode(NULL);

	//Funzione ricorsiva che splitta e crea albero binario
	splitFrequenze(mastroFreq, larghezzaDizionario, radice);

}


//Metodo che conta le frequenze di ogni file in input e le assegna ad una struttura
//Essa popola l'array di Caratteri mastroFreq che poi verra usato dopo nella funzione di shannonFano
void countFrequency(unsigned char* inputData,int lenght){

    //Inizializzo l'array delle frequenze a 0 e assegno i caratteri
    for(int i=0;i<256;i++){
        mastroFreq[i].carattere = i;
        mastroFreq[i].frequenza = 0;
    }

    //Conta la frequenza dei numeri presente
    for(int i=0;i<=lenght;i++){
        mastroFreq[*(inputData)].frequenza++;
        inputData++;
    }

    //Calcola la frequenza in double facendo la divisione
	//Se la frequenza é 0 il carattere sara rimosso dal dizionario cosi da risparmiare spazio dopo
    for(int i=0;i<=256;i++){
        if(mastroFreq[i].frequenza != 0){
            mastroFreq[i].frequenza = mastroFreq[i].frequenza / lenght;
			mastroFreq[i].binary = (unsigned char*)malloc(32 * sizeof(unsigned char)); 
			mastroFreq[i].binCount = 0;
        }else{
            larghezzaDizionario--;
        }
    }

    //dim +1 per non avere problemi con i <= negli if
    larghezzaDizionario++;

    //Sorting in ordine decrescente
    qsort(mastroFreq, 256, sizeof(Carattere), ordinaDecrescente);
    
    //Funzione che avvia la codifica di shannonfano
    shannonFano();
}





//////////////////////////////////////////////
//Parte di decode
/////////////////////////////////////////////

//Metodo che avvia la decompressione dei file
void shannon_decode(const char* file_comp,const char* outfile_name){

	FILE *comp_file;
	Carattere car_letto;
	
	comp_file = fopen(file_comp,"rb");

	//Lettura della dimensione del dizionario letto
	fread(&larghezzaDizionario,sizeof(int),1,comp_file);

	//Lettura in input delle strutture Carattere
	for(int i=0;i<larghezzaDizionario;i++){
		fread(&car_letto,sizeof(Carattere),1,comp_file);
		car_letto.binary = (unsigned char*)malloc(32 * sizeof(unsigned char));
		car_letto.binCount = 0;
		mastroFreq[i] = car_letto;
		
	}
	
	//Creo nodo radice per ricreare l'albero binario
	struct nodeAl* radiceDecode = creaNode(NULL);

	splitFrequenze(mastroFreq, larghezzaDizionario, radiceDecode);
	
	//Avvio lettura caratteri da decodificare
	int car_da_trovare = 0;
	int car_trovati = 0;
	
	//Leggo il numero di caratteri da trovare(lunghezza file)
	fread(&car_da_trovare,sizeof(int),1,comp_file);

	Byte byteLetto;
	unsigned char lettura;

	//Apro outfile
	FILE *outfile;
	outfile = fopen(outfile_name,"w");
	
	//Salvo tutti i bit letti in un contenutoBinary che useremo dopo per esplorare l'albero binario
	int* contenutoBinary = (int*)malloc(car_da_trovare * 30 * sizeof(int));

	int i = 0;
	while (((fread(&lettura, sizeof(unsigned char), 1, comp_file)) != 0)) {
			//Leggo il byte letto e lo salvo in una struttura
			byteLetto = createByte(lettura);
			
			//Salvo bit per bit nella contenutoBinary
			contenutoBinary[i] = byteLetto.bit7;
			contenutoBinary[i + 1] = byteLetto.bit6;
			contenutoBinary[i + 2] = byteLetto.bit5;
			contenutoBinary[i + 3] = byteLetto.bit4;
			contenutoBinary[i + 4] = byteLetto.bit3;
			contenutoBinary[i + 5] = byteLetto.bit2;
			contenutoBinary[i + 6] = byteLetto.bit1;
			contenutoBinary[i + 7] = byteLetto.bit0;

			//usato per saltare di 8 in 8
			i = i + 8;
			
		}

	//Chiudo il file di input compresso
	fclose(comp_file);


	//Nodo per tenere la radice dell'albero
	struct nodeAl* ripristina = radiceDecode;
	unsigned char bit;

	int contatore = 0;

	//Ciclo che esplora l'albero a dx e sx a seconda del bit letto
	//Una volta trovato una dead-end sa che dovra salvare il carattere su file
	while(1){

		//leggo bit
		bit = contenutoBinary[contatore];


		if(car_trovati < (car_da_trovare-1)){

			if(bit == 255){
				//Evitare problemi di lettura
				break;
			}

			//Gestione parte left dell'albero o del nodo
			if(bit == 0){
				if(radiceDecode->left == NULL && radiceDecode->right == NULL){
					//Trovato la fine salvo 
					fputc(radiceDecode->valore->carattere,outfile);
					
					//Ripristino radice e aumento contatore
					radiceDecode = ripristina;
					car_trovati++;
				}else{
					//Altrimenti continuo ad esplorare e aumento contatore					
					radiceDecode = radiceDecode->left;
					contatore++;
				}

			//Gestione parte right dell'albero o del nodo
			}else if(bit == 1){
				if(radiceDecode->left == NULL && radiceDecode->right == NULL){
					//Trovato la fine salvo
					fputc(radiceDecode->valore->carattere,outfile);
					
					//Ripristino e aumento
					radiceDecode = ripristina;
					car_trovati++;
				}else{
					//Altrimenti aumento e continuo a cercare
					contatore++;
					radiceDecode = radiceDecode->right;
				}
			}else{
				//In caso di bit problematici
				break;
			}
		}else{
			//Fine char trovati
			break;
		}
	}

	//Libero memoria e chiudo il file
	free(contenutoBinary);
	fclose(outfile);
}