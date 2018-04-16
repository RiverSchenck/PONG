#include <C8051F020.h>
#include <lcd.h>
#define H_PONG_DRIVER


unsigned char pot, pot2;
sbit BTN1 = P2^2;


unsigned char put_char (unsigned char row, unsigned char col, char ch);
void init_game();
void init_text();
void get_pot();
void move_paddle(unsigned int row);


void main(void)
{

//===========================================================================
//Init
//===========================================================================
	WDTCN = 0xde;// disable watch dog
	WDTCN = 0xad;
	XBR2 = 0x40; // enable port output
	//XBR0 = 4;
	OSCXCN = 0x67; // TURN ON EXTERNAL CRYSTAL
	TMOD = 0x22; // wait 1ms using t1 mode2
	TH1 = -167; // 2MHZ CLOCK, 167 COUNTS - 1MS
	TR1 = 1;
	while (TF1 == 0) {} //wait 1ms
	while (!(OSCXCN & 0x80) ) {} //wait till oscillator stable
	OSCICN = 8; // switch over to 22.1184mhz
	
	TH1 = -6;  // 9600 BAUD
	REF0CN = 0x07; // ref0 voltage
	ADC0CF = 0xF1; // GAIN
	ADC0CN = 0x80; //ENALBE AD
	TMR3CN = 4; // turn on timer 3 for ball speed



	
//initializing game
//-------------------------------------------------------------------------------
	init_lcd();
	init_text(); //waits for start to continue
	init_game();


	while(1) {
		get_pot();
		move_paddle(pot);
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





//============================================================================
//init_text
//inits the text HELLO PRESS START TO PLAY!
//============================================================================
void init_text(void) {

		put_char(1,0, 'H'); put_char(1,6, 'E'); put_char(1,12,'L');	put_char(1,18,'L'); //displays HELLO
		put_char(1,24,'O'); put_char(1,32,' ');

		put_char(1,38,'P'); put_char(1,44,'R'); put_char(1,50,'E'); put_char(1,56,'S');	//displays PRESS
		put_char(1,62,'S'); put_char(1,68,' '); 
	
		put_char(1,74,'S'); put_char(1,80,'T'); put_char(1,84,'A'); put_char(1,90,'R');	//displays START TO
		put_char(1,96,'T'); put_char(1,102,' '); put_char(1,108,'T'); put_char(1,114,'O'); 

		put_char(3,52, 'P'); put_char(3,58, 'L'); put_char(3,64, 'A'); put_char(3,70, 'Y'); //PLAY!
		put_char(3,76,'!');	put_char(6,30,' ');	put_char(6,36,' '); put_char(6,42,' ');

		refresh_screen();
		while(BTN1) {

		}
		blank_screen();

}




//============================================================================
//move_paddle
//moves the paddles
//============================================================================
	void get_pot() {

			AMX0SL = 0x00;
			ADC0CF = 0x41;
			AD0BUSY = 1;
			while (AD0BUSY)
			{}

			P7 = ADC0H;	//*4 to be 64
			screen[128] = 0xFF;
			
	}




	void move_paddle (unsigned int row)
	{

				screen[row*128] = 0xFF;

	}
