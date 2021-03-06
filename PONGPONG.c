#include <C8051F020.h>
#include <lcd.h>

//variables
//=====================================================
unsigned char pot, pot2;
//functions
//=====================================================
void init_game();


void main(void) {

	WDTCN = 0xde;  // disable watchdog
    WDTCN = 0xad;
    XBR2 = 0x40;   // enable port output
    XBR0 = 4;      // enable uart 0
	REF0CN = 7;
	OSCXCN = 0x67; // turn on external crystal
	TMOD = 0x20;   // wait 1ms using T1 mode 2



	init_lcd();
	init_game();

	for(;;){
		refresh_screen();


	}

}


//============================================================================
//init_game- inits the game (bounds, net,
//============================================================================
void init_game() {

	//------------------
	//BOUNDS
	//------------------
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




