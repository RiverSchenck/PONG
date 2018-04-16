#include <C8051F020.h>
#include <lcd.h>
#define H_PONG_DRIVER


unsigned char put_char (unsigned char row, unsigned char col, char ch);
void init_game();
void init_text();


void main(void)
{

//===========================================================================
//Init
//===========================================================================
	WDTCN = 0xde;  // disable watchdog
    WDTCN = 0xad;
    XBR2 = 0x40;   // enable port output
    XBR0 = 4;      // enable uart 0
	REF0CN = 7;
	OSCXCN = 0x67; // turn on external crystal
	TMOD = 0x20;   // wait 1ms using T1 mode 2

	P2   = 	  0xFF;
	P1   = 	  0xFF;


	//initializing game
	init_lcd();
	init_text();


	while(1) {
		refresh_screen();
	}
}

	
//==============================================================================
//put_char
//inserts characters
//==============================================================================
unsigned char put_char (unsigned char row, unsigned char col, char ch)
{
	int loc = row * 128 + col;
	int font = (ch - ' ') * 5;
	char i;
	for (i=0; i<5; i++)
	{
		screen[loc +i] = font5x8[font+i];
	}
	return (0);
}



//============================================================================
//init_game
//inits the game (bounds, net,
//============================================================================
void init_game ()
{
unsigned int i;
		//TOP BOUNDARY
		for(i=0; i<127; i++) {
			screen[i] = 0x0C; 
		}
		//BOTTOM BOUNDARY
		for(i=0; i<127; i++) {
			screen[896+i] = 0xC0; 
		}
		//NET
		for(i=63; i<960; i++) {
			screen[i+1] = 0xCC;
			screen[i] = 0xCC; 
			i= i+127;
		}
}

void init_text(void) {

//Display Test to Start Game

	put_char(1,0, 'H'); put_char(1,6, 'E'); put_char(1,12,'L');	put_char(1,18,'L'); //displays HELLO
	put_char(1,24,'O'); put_char(1,32,' ');

	put_char(1,38,'P'); put_char(1,44,'R'); put_char(1,50,'E'); put_char(1,56,'S');	//displays PRESS
	put_char(1,62,'S'); put_char(1,68,' '); 
	
	put_char(1,74,'S'); put_char(1,80,'T'); put_char(1,84,'A'); put_char(1,90,'R');	//displays START TO
	put_char(1,96,'T'); put_char(1,102,' '); put_char(1,108,'T'); put_char(1,114,'O'); 

				put_char(3,52, 'P');
				put_char(3,58, 'L');
				put_char(3,64, 'A');
				put_char(3,70, 'Y');
				put_char(3,76,'!');
				put_char(6,30,' ');
				put_char(6,36,' ');
				put_char(6,42,' ');




}


//============================================================================
//move_paddle
//moves the paddles
//============================================================================


