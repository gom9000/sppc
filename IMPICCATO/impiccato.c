/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 * File..........: impiccato.c
 * Autore........: Alessandro Fraschetti
 * Funzione......: Gioco dell'impiccato
 * Descrizione...: Indovinare una parola in un numero prestabilito di mosse.
 * Note..........: 
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mul.h"


/* costanti */
#define MOSSE   6                                    /* di mosse consentite */
#define USCR   '_'        /* carattere di mascheratura parola da indovinare */
#define PTO    '.'
#define FILEDATI "parole.dat"      /* file contenente l'elenco delle parole */


/* prototipi delle funzioni utilizzate */
int   loadStringa(char*, char*);
void  fillStringa(char*, char, int);
void  displayForca(char*, char*, int);
void  inputLettera(char*);
int   containsLettera(char*, char, int);
void  clearscreen(void);


int main(void)
{
	char parola[31];                                /* parola da indovinare */
    char lettere[MOSSE+1];                       /* lettere inserite errate */
    char appoggio[31];                           /* lettere inserite valide */
    char ch;
    int  errori = 0;
    int  index;


    /* inizializzazione */
    if ( loadStringa(parola, FILEDATI) ) {
		printf("Errore nell'apertura del file dati.");
		exit(1);
	}
	fillStringa(appoggio, USCR, strlen(parola));
	fillStringa(lettere, PTO, MOSSE);

    /* ciclo principale */
	do {
        MUL_SYSTEM_cls();
        displayForca(lettere, appoggio, errori);
        inputLettera(&ch);
        if ( !containsLettera(lettere, ch, 0) ) {
            if( (index = containsLettera(parola, ch, 0)) )
                do
                    appoggio[index - 1] = ch;
                while ( (index = containsLettera(parola, ch, index)) );
            else
                lettere[errori++] = ch;
		}
    } while ( errori < MOSSE  &&  containsLettera(appoggio, USCR, 0) );

    MUL_SYSTEM_cls();
    displayForca(lettere, appoggio, errori);

	if ( errori< MOSSE )
        printf("\n\nHai indovinato!\n\n");
    else
        printf("\n\nHai perso! La parola era: %s.\n\n", parola);

    return 0;
}


/*
 * Visualizza la forca e le informazioni di gioco
 */
void displayForca(char* s1, char* s2, int n)
{
	char tab[] = "\n\t\t\t\t\t\t\0";
	char testa[4]   = "  '\0";
	char braccia[4] = "   \0";
	char busto[4]   = "   \0";
	char gambe[4]   = "   \0";
	char panca[4]   = "---\0";
	char base[4]    = "   \0";

    if (n>0&&n<6)  testa[1] = 'o';
	if (n==6)      testa[1] = 'O';
    if (n>1)       braccia[1] = '|';
    if (n>2)       braccia[0] = '/';
    if (n>=4)      braccia[2] = '\\';
	if (n>=5)      gambe[0] = '/';
	if (n==6)    { gambe[2] = '\\';
	               panca[0] = ' '; panca[1] = ' '; panca[2] = ' ';
				   base[0] = '-'; base[1] = '-'; base[2] = '-';   }

    printf(MUL_PGM_logo("  ", 6));
    printf("\n\t\t  G I O C O   D E L L '   I M P I C C A T O\n\n");

	printf("%s    ,-+-.", tab);
	printf("%s  %s  \\|", tab, testa);
	printf("%s  %s   |", tab, braccia);
	printf("%s  %s   |", tab, busto);
	printf("%s  %s   |", tab, gambe);
	printf("%s _%s___|", tab, panca);
	printf("%s| %s   |", tab, base);

    printf("\n\nErrori: %d su %d", n, MOSSE);
    printf("\n\nLettere errate: %s", s1);
    printf("\n\nParola: %s", s2);
    printf("\n\nInserire una lettera: ");
}


void fillStringa(char *s, char ch, int len)
{
	s[len] = '\0';
    while (len--) s[len] = ch;
}


void inputLettera(char *ch)
{
    scanf("%c", ch);
    fflush(stdin);
}


int containsLettera(char *str, char ch, int idx)
{
   for ( ; str[idx]; idx++ )
      if ( str[idx] == ch ) return idx+1;
   return 0;
}


int loadStringa(char *s, char *file)
{
	long parole, n;
	time_t t;
    FILE *fp;

    /* tentativo di apertura del file dati */
	if ( (fp=fopen(file, "r")) == '\0' )
		return -1;

	/* estrazione del numero di parole contenute nel file   */
	/* (il dato è scritto nella prima riga del file stesso) */
    fscanf(fp, "%ld\n", &parole);

    /* generazione di un numero random che rappresenta */
	/* l'ordinale della parola da indovinare           */
	time(&t);
	srand((unsigned)t);
    n = parole * rand()/RAND_MAX;

    /* recupero della parola dal file */
    while (n--) fscanf(fp, "%s", s);

	fclose(fp);

    return 0;
}
