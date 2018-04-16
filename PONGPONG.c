#include <C8051F020.h>
#include <lcd.h>
#define H_PONG_DRIVER


unsigned int pot, pot2;
sbit BTN1 = P2^2;


unsigned char put_char (unsigned char row, unsigned char col, char ch);
void init_game();
void init_text();
void get_pot();
void move_paddle(unsigned int paddle1, unsigned int paddle2);


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
	TH1 = -167;    // 2MHz clock, 167 counts - 1ms
	TR1 = 1;
	while ( TF1 == 0 ) { }          // wait 1ms
	while ( !(OSCXCN & 0x80) ) { }  // wait till oscillator stable
	OSCICN = 8;    // switch over to 22.1184MHz


	T2CON  =  0x08; //Set the timer auto-reload   
	RCAP2H = -180 ; //22.1184E6/12/1024 = 1800
	RCAP2L = 3;
	TR2    =  1;
	//IE     =  0x80; //0x80; //do not enable timer 2 interrupt
	EIE2   =  0x02; //enable ADC interrupt
	ADC0CF =  0x40; //set conversion clock [(22.184Hz/2.5MHz) - 1] = 8
	ADC0CN =  0x8C; //enable ADC, starts conversion when T2 overfows
	REF0CN =  0x07; //set reference voltage
	AMX0SL =  0x01; 



	
//initializing game
//-------------------------------------------------------------------------------
	init_lcd();
	init_text(); //waits for start to continue


	while(1) {
		get_pot();
		move_paddle(pot, pot2);
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

			pot = ADC0H/2;	//*4 to be 64


			AMX0SL = 0x00;
			ADC0CF = 0x41;
			AD0BUSY = 1;
			while (AD0BUSY)
			{}

			pot2 = ADC0H/2;	//*4 to be 64
			
	}




	void move_paddle (unsigned int paddle1, unsigned int paddle2)
	{			
				blank_screen();
				init_game();
				screen[paddle1*128] = 0xFF;
				screen[(paddle1*128)+1] = 0xFF;

				screen[paddle2*127] = 0xFF;
				screen[(paddle2*127)+1] = 0xFF;
				
	

	}
