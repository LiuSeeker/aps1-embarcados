/**
 * 5 semestre - Eng. da Computa��o - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Projeto 0 para a placa SAME70-XPLD
 *
 * Objetivo :
 *  - Introduzir ASF e HAL
 *  - Configuracao de clock
 *  - Configuracao pino In/Out
 *
 * Material :
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 */

/**
 * Entrega realizada em parceria com: 
 * 	Vitor Liu
 * 	Vitor Satyro
 * 
 * 	https://youtu.be/ioLHXsXvSEk
 * 
 *  Música 1 - Tema "Piratas do Caribe", Klaus Badelt e Hans Zimmer
 * 		Transcrito por Xitang - https://github.com/xitangg/-Pirates-of-the-Caribbean-Theme-Song/blob/master/Pirates_of_the_Caribbean_-_Theme_Song.ino
 * 	Música 2 - Castelo Forte (Ein feste burg ist unser Gott), Martinho Lutero
 */

/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

#define LED_PIO           PIOC
#define LED_PIO_ID        12
#define LED_PIO_IDX       8u
#define LED_PIO_IDX_MASK  (1u << LED_PIO_IDX)

#define BUT_PIO           PIOA
#define BUT_PIO_ID        10
#define BUT_PIO_IDX       3u
#define BUT_PIO_IDX_MASK  (1u << BUT_PIO_IDX)

#define BUT2_PIO           PIOA
#define BUT2_PIO_ID        10
#define BUT2_PIO_IDX       19u
#define BUT2_PIO_IDX_MASK  (1u << BUT2_PIO_IDX)

#define BUZ_PIO           PIOA
#define BUZ_PIO_ID        10
#define BUZ_PIO_IDX       0u
#define BUZ_PIO_IDX_MASK  (1u << BUZ_PIO_IDX)

#define NOTE_B3b 233   //Si
#define NOTE_B3  246   //Si
#define NOTE_C4  262   //Do
#define NOTE_D4  294   //R�
#define NOTE_E4  330   //Mi
#define NOTE_F4  349   //Fa
#define NOTE_F4s 369   //Fa sustenido
#define NOTE_G4  392   //Sol
#define NOTE_A4  440   //La
#define NOTE_A4s 466   //La sustenido
#define NOTE_B4  494   //Si
#define NOTE_C5  523   //Do
#define NOTE_D5  587   //R�
#define NOTE_E5  659   //Mi
#define NOTE_F5  698   //Fa
#define NOTE_G5  784   //Sol
#define NOTE_A5  880   //La
#define NOTE_B5  988   //Si

/************************************************************************/
/* constants                                                            */
/************************************************************************/

const int songspeed = 2;

int notes1[] = {       //Note of the song, 0 is a rest/pulse  //203 notas
	NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
	NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
	NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
	NOTE_A4, NOTE_G4, NOTE_A4, 0,
	
	NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
	NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
	NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
	NOTE_A4, NOTE_G4, NOTE_A4, 0,
	
	NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
	NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0,
	NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
	NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,
	
	NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
	NOTE_D5, NOTE_E5, NOTE_A4, 0,
	NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
	NOTE_C5, NOTE_A4, NOTE_B4, 0,

	NOTE_E5, 0, 0, NOTE_F5, 0, 0,
	NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
	NOTE_D5, 0, 0, NOTE_C5, 0, 0,
	NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4,

	NOTE_E5, 0, 0, NOTE_F5, 0, 0,
	NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
	NOTE_D5, 0, 0, NOTE_C5, 0, 0,
	NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4
};

int duration1[] = {         //duration of each note (in ms) Quarter Note is set to 250 ms
	125, 125, 250, 125, 125,
	125, 125, 250, 125, 125,
	125, 125, 250, 125, 125,
	125, 125, 375, 125,
	
	125, 125, 250, 125, 125,
	125, 125, 250, 125, 125,
	125, 125, 250, 125, 125,
	125, 125, 375, 125,
	
	125, 125, 250, 125, 125,
	125, 125, 250, 125, 125,
	125, 125, 250, 125, 125,
	125, 125, 125, 250, 125,

	125, 125, 250, 125, 125,
	250, 125, 250, 125,
	125, 125, 250, 125, 125,
	125, 125, 375, 375,
		
	250, 125, 375, 250, 125, 375,
	125, 125, 125, 125, 125, 125, 125, 125, 375,
	250, 125, 375, 250, 125, 375,
	125, 125, 125, 125, 125, 500,

	250, 125, 375, 250, 125, 375,
	125, 125, 125, 125, 125, 125, 125, 125, 375,
	250, 125, 375, 250, 125, 375,
	125, 125, 125, 125, 125, 500
};

int notes2[] = { //71 notas
	NOTE_C5, NOTE_C5, NOTE_C5, NOTE_G4, NOTE_A4, NOTE_B4,
	NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4,
	NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4,
	NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4,
	
	NOTE_C5, NOTE_C5, NOTE_C5, NOTE_G4, NOTE_A4, NOTE_B4,
	NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4,
	NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4,
	NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4,
	
	NOTE_C4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4s,
	NOTE_G4, NOTE_C4,
	NOTE_G4, NOTE_G4, NOTE_A4, NOTE_B4,
	NOTE_C5, NOTE_B4,
	NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4,
	NOTE_G4, NOTE_A4,
	NOTE_A4, NOTE_G4, NOTE_A4, NOTE_F4,
	NOTE_E4, NOTE_C5,
	NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4,
	NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4
};

int duration2[] = {
	645, 645, 645, 322, 322, 645,
	322, 322, 645, 3225,
	645, 645, 645, 645, 645,
	322, 322, 645, 3225,
	
	645, 645, 645, 322, 322, 645,
	322, 322, 645, 3225,
	645, 645, 645, 645, 645,
	322, 322, 645, 3225,
	645, 645, 645, 645, 645,
	1935,
	645, 645, 645, 645, 645,
	1935,
	645, 645, 645, 645, 645,
	1935,
	645, 645, 645, 645, 645,
	1935,
	645, 645, 645, 645, 645,
	322, 322, 645, 1935
};




/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

int n_mus = 0;

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

void init(void);




/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/

// Fun��o de inicializa��o do uC
void init(void)
{
	sysclk_init();
	
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	pmc_enable_periph_clk(LED_PIO_ID);
	pmc_enable_periph_clk(BUT_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pmc_enable_periph_clk(BUZ_PIO_ID);
	
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
	pio_set_output(BUZ_PIO, BUZ_PIO_IDX_MASK, 0, 0, 0);
	
	pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_DEFAULT);
	
	pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, 1);
	pio_pull_up(BUT2_PIO, BUT2_PIO_IDX_MASK, 1);
}

void tone(int nota, int duracao){
	long delayValue = (100000/nota)/2;
	int ciclos = (nota*duracao)/1000;
	for (int i = 0; i < ciclos; i++){
		if(!pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK)){
			break;
		}
		pio_set(PIOA, BUZ_PIO_IDX_MASK);
		delay_us(delayValue);
		pio_clear(PIOA, BUZ_PIO_IDX_MASK);
		delay_us(delayValue);
	}
	pio_clear(PIOA, BUZ_PIO_IDX_MASK);
}

	
	

/************************************************************************/
/* Main                                                                 */
/************************************************************************/

// Funcao principal chamada na inicalizacao do uC.
int main(void)
{
  init();

  // super loop
  // aplicacoes embarcadas n�o devem sair do while(1).
  
  
  while (1){	
	
	  
	if(!pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK)){
		
		n_mus += 1;
		if (n_mus == 2) {
			n_mus = 0;
		}
		for(int i = 0; i <= n_mus; i++){
			pio_clear(PIOC, LED_PIO_IDX_MASK);
			delay_ms(100);
			pio_set(PIOC, LED_PIO_IDX_MASK);
			delay_ms(100);
		}
		delay_ms(300);
	}
	 
	
	if(n_mus == 0){
		if(!pio_get(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK)){
			for (int i=0;i< sizeof(notes1)/sizeof(notes1[0]) ;i++){
				if(!pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK)){
					break;
				}
				pio_clear(PIOC, LED_PIO_IDX_MASK);
				tone(notes1[i], duration1[i]);
				pio_set(PIOC, LED_PIO_IDX_MASK);
				int pauseBetweenNotes = duration1[i];
				delay_ms(pauseBetweenNotes);
			}
		}
		n_mus = 0;
	}
	if(n_mus == 1){
		if(!pio_get(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK)){
			for (int i=0;i<sizeof(notes2)/sizeof(notes2[0]);i++){
				if(!pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK)){
					break;
				}
				pio_clear(PIOC, LED_PIO_IDX_MASK);
				tone(notes2[i], duration2[i]);
				pio_set(PIOC, LED_PIO_IDX_MASK);
				int pauseBetweenNotes = duration2[i]*0.8;
				delay_ms(pauseBetweenNotes);
			}
		}
		n_mus = 1;
	}
	pio_clear(PIOA, BUZ_PIO_IDX_MASK);
	pio_set(PIOC, LED_PIO_IDX_MASK);      // Coloca 1 no pino LED
  }
  return 0;
}
