#include <C8051F020.h>
#include <lcd.h>
#define H_PONG_DRIVER


unsigned int pot, pot2;
sbit BTN1 = P2^2;
unsigned char paddle_top, paddle_height, paddle_bottom;

unsigned char put_char (unsigned char row, unsigned char col, char ch);
void init_game();
void init_text();
void move_paddle();
void draw_paddle (int x, int y, char pad_type);



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



//----------------------	
//initializing game
//----------------------
	init_lcd();//init screen
	init_text(); //displays text and waits for button press to start

//----------------------	
//Playing game
//----------------------
	while(1) {
		move_paddle();
		refresh_screen();

		draw_paddle(0, pot, 11);
	}
}

	


void draw_paddle (int x, int y, char pad_type) {
	long mask;
	//unsigned char paddle_top, paddle_height, paddle_bottom;
		switch(pad_type)
		{
			case 0:
				if(y>54) y = 54;
				mask = 0xffl;  //assign as long
				paddle_height = 8;
				break;
			case 1:
				if(y>50) y = 50;
				mask = 0xfffl;
				paddle_height = 12;
				break;
			case 2:
				if(y > 46) y = 46;
				mask = 0xffffl; //length of paddles
				paddle_height = 16;
				break;
			case 3:
				while(x<1024)
					{
				    	screen[x] = 0xffl;
					    screen[x+1] = 0xffl;
						x = x+128;
						paddle_top = 2;
						paddle_bottom = 61;
						paddle_height = 60;
					}
				
				return;
		}
	if (y<2) y =2;
	mask = mask <<(y%8);
	x+= (y/8)*128;
	screen[x] |= mask;
	screen[x+1] |= mask;
	screen[x+128] |= mask >> 8;
	screen[x+129] |= mask >> 8;
	screen[x+256] |= mask >> 16;
	screen[x+257] |= mask >> 16;
	paddle_top = y;  // store
	paddle_bottom = y+paddle_height-1; //


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
//inits the game (bounds and net)
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
//inits the text HELLO PRESS START TO PLAY! Waits for button prexs to start
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
void move_paddle() {
	blank_screen();
	init_game();

	AMX0SL = 0x00;  //AIN0
	ADC0CF = 0x40;
	AD0INT = 0; // clear flag
	AD0BUSY = 1;
	while(AD0INT == 0)
	{}
	pot = ADC0H*4;


	AMX0SL = 0x01; //AIN2
	ADC0CF = 0x40;
	AD0INT = 0; //clear flag
	AD0BUSY = 1;
	while(AD0INT == 0)
	{}
	pot2 = ADC0H*4;
			
}


