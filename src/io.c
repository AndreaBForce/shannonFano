#include "io.h"
#include "newfano.h"
#include <stdio.h>
#include <stdlib.h>


// Variabile dice la lunghezza di unsigned char del file
int lunghezzafile = 0;

// Metodo che converte da Byte a unsigned char
unsigned char toRealByte(Byte byte) {
  unsigned char value = 0;
  value = value + (byte.bit0 << 0);
  value = value + (byte.bit1 << 1);
  value = value + (byte.bit2 << 2);
  value = value + (byte.bit3 << 3);
  value = value + (byte.bit4 << 4);
  value = value + (byte.bit5 << 5);
  value = value + (byte.bit6 << 6);
  value = value + (byte.bit7 << 7);
  return value;
}

// Metodo che crea un byte da un unsigned char
Byte createByte(unsigned char value) {
  Byte byte;
  byte.bit0 = value >> 0;
  byte.bit1 = value >> 1;
  byte.bit2 = value >> 2;
  byte.bit3 = value >> 3;
  byte.bit4 = value >> 4;
  byte.bit5 = value >> 5;
  byte.bit6 = value >> 6;
  byte.bit7 = value >> 7;
  return byte;
}

// Funzione che permette la lettura in input del file
// Restituisce un puntatore ad unsigned char
// Essa riempe anche la variabile lunghezzafile
unsigned char *readIO(const char *n_file) {
  unsigned char *data;
  int tmp, sztemp = 0;

  FILE *file = fopen(n_file, "r");
  if (file == NULL) {
    //Gestione errori
    printf("Errore: Problemi nell'aprire il file\n");
    printf("Controllare che il file esista o che il percoso specificato sia corretto\n");
    exit(1);
  }

  // Sposto il puntatore per determinare la grandezza del file
  fseek(file, 0L, SEEK_END);
  lunghezzafile = ftell(file);
  rewind(file);

  // Check in lettura se e' vuoto o meno
  if (lunghezzafile == 0) {
    //Gestione errori
    printf("Errore: Il file \"%s\" vuoto.\n", n_file);
    printf("Chiusura del programma in corso.\n");
    exit(1);
  }

  // Alloca char nel malloc della dimensione giusta
  //Andro a ritornare data che contiene tutti i dati letti dal file
  data = (unsigned char *)malloc(sizeof(unsigned char) * lunghezzafile + 1);
  do {
    tmp = fgetc(file);
    data[sztemp] = tmp;
    sztemp++;

    if (feof(file)) {
      break;
    }
  } while (1);

  fclose(file);
  
  //assegno lunghezza del file (caratteri letti)
  lunghezzafile = sztemp;
  printf("Notifica: Lettura file avvenuta con successo\n");
  return data;
}

//Metodo che si occupa nella parte di compressione di salvare su file gli argomeni necessari
//Salva: larghezza del dizionario
//       tutti i caratteri che ci sono nel dizionario attuale
//       la lunghezza del file inizialmente presa
//       tutti i byte dei dati codificati
void writeBinEncode(unsigned char *inputData, int lenght,const char *n_file_o) {
  FILE *outfile;
  outfile = fopen(n_file_o, "wb");

  // Metto il numero di Caratteri presenti all'inizio del file
  fwrite(&larghezzaDizionario, sizeof(int), 1, outfile);

  // Metto sul file le strutture Carattere necessarie per ricreare l'albero dopo
  for (int i = 0; i < larghezzaDizionario; i++) {
    fwrite(&mastroFreq[i], sizeof(Carattere), 1, outfile);
  }

  // Inserisce nella parolacodificata le codifiche di valore 0 o 1
  int volte = 0, contatore = 0;

  // Avviene al gestione per salvare bit a bit su file
  //Creo un file temporaneo su cui salvo tutti i dati codificati
  //Poi li leggo byte per byte e li vado a salvare sul file di output
  //Il file temporaneo verra eliminato dopo
  FILE *tmpfile;
  tmpfile = fopen("t_m_p_f_i_l_e", "w");
  unsigned char valore;

  //Per ogni dato presente nel file iniziale
  for (int i = 0; i < lenght; i++) {
    
    //Viene salvato bit per bit 
    for (int j = 1; j < (codifiche[*(inputData)][0] + 1); j++) {
      valore = codifiche[*(inputData)][j];

      fwrite(&valore, sizeof(unsigned char), 1, tmpfile);
      
      volte++;
    }
    inputData++;
    
  }

  fclose(tmpfile);
  tmpfile = fopen("t_m_p_f_i_l_e", "r");

  // Qua salvo la lunghezza totale del file, ovvero i caratteri letti inizialmente
  fwrite(&lenght, sizeof(int), 1, outfile);


  //Qua vado a leggere i bit dal file temporaneo
  //Creo una struttura Byte con questi bit letti
  //Creo un unsigned char e lo salvo sul file iniziale
  unsigned char cardamettere;
  Byte byteCode;
  while (contatore < volte) {
    byteCode.bit7 = fgetc(tmpfile);
    byteCode.bit6 = fgetc(tmpfile);
    byteCode.bit5 = fgetc(tmpfile);
    byteCode.bit4 = fgetc(tmpfile);
    byteCode.bit3 = fgetc(tmpfile);
    byteCode.bit2 = fgetc(tmpfile);
    byteCode.bit1 = fgetc(tmpfile);
    byteCode.bit0 = fgetc(tmpfile);

    contatore = contatore + 8;
    cardamettere = toRealByte(byteCode);
    
    fputc(cardamettere, outfile);
  }

  printf("Notifica: Scrittura su file binario compresso avvenuta con successo\n");
  fclose(tmpfile);
  remove("t_m_p_f_i_l_e");
  fclose(outfile);
}
