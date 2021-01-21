/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 * File..........: tris.c
 * Autore........: Alessandro Fraschetti
 * Funzione......: Simulazione del gioco del 'tris' o 'tic tac toe'
 * Descrizione...:
 * Note..........: 
 * (con livelli di difficoltà: facile (pesi uguali),
 *                             intermedio (come è),
 *                             difficile(2 mosse di ant.))
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/


#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "mul.h"


/* costanti */
#define FLAG0     'x'
#define FLAG1     'o'
#define BLANK     ' '
#define TRUE      1
#define FALSE     0
#define TAG      -1                                /* la casella è occupata */  
#define OX_ROW    0                     /* Costanti segnaindice del vettore */
#define OX_COL    3
#define OX_DIAG1  6
#define OX_DIAG2  7
#define OX_END    8


/* prototipi delle funzioni utilizzate */
void  inputGiocatori(void);
void  displayHeader(void);
void  displayCampo(void);
void  displayEsito(int);
int   complementare(int);
int   esitoMossa(void);
int   doMossa(int);
void  inputMossa(int);
void  elaboraMossa(int);
void  elaboraCoordinate(int, int);
int   updateCampo(int);       /* ret: 0=update valido  1=update non valido */
void  update_ox_status(int);
void  update_c_status(int);


char campo[3][3] = {{' ', ' ', ' '},                      /* campo di gioco */
                    {' ', ' ', ' '},
	                {' ', ' ', ' '}};

char init_c_status[3][3] = {{3, 2, 3},         /* potenziali campo iniziali */
                            {2, 4, 2},
	                        {3, 2, 3}};

char bonus_c_status[3][3] = {{2, 0, 0},                 /* bonus attaccante */
                             {0, 0, 0},
                             {0, 0, 0}};

struct {                              /* struttura con i dati del giocatore */
    char nome[20];                                    /* nome del giocatore */
    int  human;                                      /* 0=computer, 1=umano */
    char flag;                       /* simbolo del giocatore: FLAG0, FLAG1 */
    char c_status[3][3];                                /* potenziali campo */
    char ox_status[8];                                  /* vettore di stato */
} s[2];                                       /* 0=giocatore1, 1=giocatore2 */

struct {
    int row;
    int col;
} coord;


int main(void)
{
    int player = 0;      /* indicatore di turno: 0=giocatore1, 1=giocatore2 */
    int mosse = 0;                  /* numero delle mosse corrette eseguite */
    int esito = 0;                      /* esito dell'ultima mossa eseguita */

    /* input giocatori ed inizializzazione strutture */
	displayHeader();
    inputGiocatori();

    /* ciclo principale */
    displayCampo();
    do {

		if( !doMossa(player) ) {
	        player = complementare(player);
	        mosse++;
        } else {
	        printf("\nMossa non valida. Premere INVIO per continuare.");
	        MUL_SYSTEM_pause();
        }

		displayCampo();
		if ( (esito = esitoMossa()) ) break;

	} while ( mosse < 9 );

	displayEsito(esito);

    return 0;
}


void inputGiocatori(void)
{
    int i, j;

    displayHeader();
    printf("\nNome del giocatore 1 ('c' per computer): ");
    scanf("%s", s[0].nome);
    printf("\nNome del giocatore 2 ('c' per computer): ");
    scanf("%s", s[1].nome);

    /* inizializzazione delle strutture dati dei giocatori */
	for( i=0; i<2; i++ )
        if ( !strcmp(s[i].nome, "c") || !strcmp(s[i].nome, "C") ) {
            char ch[2] = {('1' + i)};
            strcpy(s[i].nome, "Computer");
            strcat(s[i].nome, ch);
		} else
	        s[i].human = 1;
    s[0].flag = FLAG0;
    s[1].flag = FLAG1;
    for( i=0; i<3; i++ )
        for( j=0; j<3; j++ ) {
	        s[0].c_status[i][j] = init_c_status[i][j] + bonus_c_status[i][j];
	        s[1].c_status[i][j] = init_c_status[i][j];
        }
}


/* visualizza logo, titolo del programma e si occupa di pulire lo schermo */
void displayHeader(void)
{
    MUL_SYSTEM_cls();
    printf(MUL_PGM_logo("\t", 6));
    printf("\t\t\t  G I O C O   D E L   T R I S\n\n");
}


/* visualizza il campo da gioco ed i nomi dei giocatori */
void displayCampo(void)
{
    int i = 0, j = 0;

    displayHeader();
    printf("\n\n\t  %20s  vs  %.20s\n\n", s[0].nome,s[1].nome);
    while( (i + j) < 3 ) {
        printf("\n\t\t\t      |     |      ");
        printf("\n\t\t\t   %c  |  %c  |  %c  ", campo[i][j], campo[i][j+1],
                                                              campo[i][j+2] );
        j=0; i++;
        printf("\n\t\t\t      |     |      ");
        if ( i < 3 ) printf("\n\t\t\t- - - - - - - - - -");
   }
   printf("\n\n");
}


/* ritorna il giocatore complementare */
int complementare(int player)
{
    return player = player? 0 : 1 ;
}


/* ritorna: 0 = mossa valida, 1 = mossa non valida */
int doMossa(int player)
{
    if( s[player].human )
        inputMossa(player);
    else
        elaboraMossa(player);

	if( updateCampo(player) )
        return 1;

	update_ox_status(player);
    update_c_status(complementare(player));

	return 0;
}


void inputMossa(int player)
{
    printf("\nGiocatore %s, digita la tua mossa (riga, colonna): ",
                                                        s[player].nome);
    scanf("%d%*c%d", &coord.row, &coord.col);
    coord.row--;
    coord.col--;
}


void elaboraMossa(int player)
{
    int i, j, val = TAG;
    int other = complementare(player);

    /* se c'è una mossa che determina la vittoria va eseguita */
    for ( i=0; i < OX_END; i++ )
        if( (s[player].ox_status[i] == 2) && (s[other].ox_status[i] == 0) ) {
	        elaboraCoordinate(i, player);
	        return;
        }

    /* se c'è una mossa che evita la sconfitta va eseguita */
	for ( i=0; i < OX_END; i++ )
        if ( (s[other].ox_status[i] == 2) && (s[player].ox_status[i] == 0) ) {
	        elaboraCoordinate(i,other);
	        return;
        }

    /* ricerca di una mossa che porti al maggior numero di 2 non bilanciati */
            /* nel vettore s[player].ox_status[], considerando che la mossa */
         /* dell'avversario (other) non dovrebbe portare a 2 non bilanciati */
	                                                /* nel vettore s[other] */

    for ( i=0; i < 3; i++ ) /* esegue la prima mossa di potenziale maggiore */
        for ( j=0; j < 3; j++ )
	        if( s[player].c_status[i][j] > val ) {
	            val = s[player].c_status[i][j];
                coord.row = i;
                coord.col = j;
	 }
}

void elaboraCoordinate(int ind, int player)
{
    int i, j;

    if( ind < OX_COL )
        for ( j=0; j < 3; j++ )
	    if( campo[ind][j] == BLANK ) {
	        coord.row=ind;
	        coord.col=j;
	        return;
	    }
    if ( ind < OX_DIAG1 )
      for(i=0;i<3;i++)
	 if(campo[i][ind-OX_COL]==BLANK) {
	    coord.row=i;
	    coord.col=ind-OX_COL;
	    return;
	 }
   if(ind==OX_DIAG1)
      for(i=0;i<3;i++)
	 if(campo[i][i]==BLANK) {
	    coord.row=coord.col=i;
	    return;
	 }
   if(ind==OX_DIAG2)
      for(i=0;i<3;i++)
	 if(campo[i][2-i]==BLANK) {
	    coord.row=i;
	    coord.col=2-i;
	    return;
	 }
}

int updateCampo(int player)
{
    if( (coord.row >= 0) && (coord.row < 3) )
        if( (coord.col >= 0) && (coord.col < 3) )
	        if( campo[coord.row][coord.col] == BLANK ) {
	            campo[coord.row][coord.col] = s[player].flag;
	            return 0;
	 }

	 return 1;
}

void update_ox_status(int player)
{
   s[player].ox_status[OX_ROW+coord.row]+=1;
   s[player].ox_status[OX_COL+coord.col]+=1;
   if(coord.row==coord.col)   s[player].ox_status[OX_DIAG1]+=1;
   if(coord.row+coord.col==2) s[player].ox_status[OX_DIAG2]+=1;
}

void update_c_status(int player)
{
   int i, j, k;
   int other=complementare(player);

   s[player].c_status[coord.row][coord.col] = TAG;
   s[other].c_status[coord.row][coord.col]  = TAG;
   for(i=0,k=0;i<3;i++,k++)  /* aggiornamento righe */
      for(j=0;j<3;j++)
	 if(s[player].c_status[i][j]>0 && s[player].ox_status[k]>0)
	    s[player].c_status[i][j] = init_c_status[i][j] - 1;
   for(i=0;i<3;i++,k++)      /* aggiornamento colonne */
      for(j=0;j<3;j++)
	 if(s[player].c_status[j][i]>0 && s[player].ox_status[k]>0)
	    s[player].c_status[j][i] -= 1;
      for(i=0;i<3;i++)       /* aggiornamento diagonale 1 */
	 if(s[player].c_status[i][i]>0 && s[player].ox_status[k]>0)
	    s[player].c_status[i][i] -= 1;
      for(i=0,k++;i<3;i++)   /* aggiornamento diagonale 2 */
	 if(s[player].c_status[i][2-i]>0 && s[player].ox_status[k]>0)
	    s[player].c_status[i][2-i] -= 1;
}

/* ritorna: 0=game, 1=player1 win, -1=player2 win */
int esitoMossa(void)
{
    int i;

    for( i=0; i < OX_END; i++ ) {
        if ( s[0].ox_status[i] == 3 ) return +1;
        if ( s[1].ox_status[i] == 3 ) return -1;
    }

	return 0;
}


void displayEsito(int esito)
{
    if ( !esito )
        printf("\n\nLa partita e' terminata patta.\n\n");
    else
        printf("\n\nHa vinto il giocatore: %s\n\n",
               esito == +1 ? s[0].nome : s[1].nome );
}
