#include <C8051F020.h>
#include <stdlib.h>
#include <lcd.h>
#include <pongheader.h>



sbit BTN1 = P2^2;			//BTN1 FOR START GAME
sbit BTN2 = P2^3;
unsigned char LED18;
sbit LED9 = P2^4;
sbit LED10 = P2^5;


void main(void)
{

//----------------------------------
//Init
//-----------------------------------
	WDTCN = 0xde;// disable watch dog
	WDTCN = 0xad;
	XBR2 = 0x40; // enable port output
	XBR0 = 4;
	OSCXCN = 0x67; // TURN ON EXTERNAL CRYSTAL
	TMOD = 0x22; // wait 1ms using t1 mode2
	TH1 = -167; // 2MHZ CLOCK, 167 COUNTS - 1MS
	TR1 = 1;
	while (TF1 == 0) {} //wait 1ms
	while (!(OSCXCN & 0x80) ) {} //wait till oscillator stable
	OSCICN = 8; // switch over to 22.1184mhz
	SCON0 = 0x50; // 8-BIT, VARIABLE BAUD, RECEIVE ENABLE
	TH1 = -6;  // 9600 BAUD
	REF0CN = 0x07; // ref0 voltage
	ADC0CF = 0xf4; // GAIN
	ADC0CN = 0x80; //ENALBE AD
	IE = 0xA2;
	IP = 0x20;
	DAC0CN = 0x9C;
	T2CON = 0x00;
	TH1 = 0; //set timer 1 for a two bit timer
	TH0 = 0;
	TR0 = 1;


//----------------------	
//initializing game
//----------------------			
		init_lcd();
		blank_screen();
		init_text();
		get_switches();
		init_game();
		display_scores(score1, score2);

//----------------------	
//Playing game
//----------------------
	while(BTN2 == 1) {


		//SHOW SCORE LOOP. IE AFTER A POINT IS SCORED
		while(show_score == 0) {
				display_scores(score1, score2);
				onemillidelay(2000);//DELAY TO SHOW THE SCORE OF THE GAME
				get_switches();
				draw_ball(ball_x, ball_y);
				show_score = 1;
			}
			//PLAYING GAME LOOP
			while(show_score == 1 && BTN2 == 1)
			{
				if(TMR3CN == 0x84)//If the overflow flag is raised
				{
					//CHECKS TO SEE IF SOMEONE HAS WON
					if(score1 < 11 && score2 < 11) {

					blank_screen();				
					init_game();			
					get_switches(); 
					move_comps();
					draw_ball(ball_x, ball_y);
					refresh_screen();	
					TMR3CN &= 0x7F;	
					}									 
					else if(score1 == 11 || score2 == 11)
					{
						winner();
						return;
					}//end else if

				}//end if
	
			}//end while
	
	}//end for

}//end main




//==============================================================================
//TIMERS
//
//==============================================================================

void timer_0(void) interrupt 1
{
	if(count < 3)
	{	
		rise = random[count];
		ball_x = serve[count];
		count++;			
	}
	else if(count == 3)
	{
		rise = random[count];
		count = 0;			
	}


	if(ball_x ==110) {
		run = -1;
	}
	else{
		run = 1;
	}
	ball_y = 30;

}



void timer2(void) interrupt 5
{

		TF2 = 0;
		DAC0H = sine[phase];
		if ( phase < sizeof(sine)-1 )	// if mid-cycle
		{				// complete it
			phase++;
		}
		else if ( duration > 0 )	// if more cycles left to go
		{				// start a new cycle
			phase = 0;
			duration--;
		}
		if ( duration == 0)
		{
			TR2 = 0;
		}
	
}



//==============================================================================
//ONEMILLIDELAY
//dealy of i time, from the book
//==============================================================================
void onemillidelay(unsigned int time) 
{
unsigned int i;	
unsigned int j;	
for(i=0; i<time; i++)	
for(j=0; j<1843; j++);
}



//==============================================================================
//PUT_CHAR
//inserts characters
//==============================================================================
unsigned char put_char (unsigned char row, unsigned char col, char ch) {
		int loc = row * 128 + col;
		int font = (ch - ' ') * 5;
		char i;
		for (i=0; i<5; i++)
		{
			screen[loc +i] = font5x8[font+i];
		}
		return (0);
}



//==============================================================================
//INIT_GAME
//draws the bounds for the game
//==============================================================================
void init_game () {

		unsigned int i;
		//TOP BOUNDARY
		for(i=0; i<127; i++) {
			screen[i] = 0x03; 
		}
		//BOTTOM BOUNDARY
		for(i=0; i<127; i++) {
			screen[896+i] = 0xC0; 
		}

		//-----------------------
		//SOCCER MODE
		//-----------------------
		if (soccer == 0) {
			soccermode();


		}
		else {
			//NET
			screen[63] = 0xCF;
			screen[64] = 0xCF;
			for(i=191; i<960; i++) {
				screen[i+1] = 0xCC;
				screen[i] = 0xCC; 
				i= i+127;
			}

		}
}



//==============================================================================
//INIT_TEXT
//Prints HELLO PRESS START TO PLAY!
//==============================================================================
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


		TR0 = 0;
		get_switches();
		TMR3CN = 0x04;	//Turn on the refresh rate timer	
}



//==============================================================================
//GET_SWITCHES
//gets the input of the switches and passes them to the paddle function
//==============================================================================
void get_switches(){
			int check = 0;
			switches = ~P3;
			switches &= 0x03;
			paddle1 = switches;
			draw_paddle (0, paddle1top, paddle1); 
			paddle1height = paddle_height;
		
			switches = ~P3;
			switches &= 0x0c;
			paddle2 = switches >> 2;		
			draw_paddle (126, paddle2top, paddle2);
			paddle2height = paddle_height;

			switches = P3;			
			switches &= 0xc0;	
			switches = switches >> 6;			
			check = switches;		
			speed(switches);		

			soccer = ~P3;
			soccer &= 0x10; 
			soccer = soccer >> 4;

}



//==============================================================================
//SOUND
//produces a noise based on where the ball hits
//==============================================================================
void sound(int sound){

	if(sound == 1) {
		RCAP2H = (-307)>>8; RCAP2L = -307; TR2 = 1; duration = 60;
	}
	
	if(sound == 2) {
		RCAP2H = (-230)>>8; RCAP2L = -230; TR2 = 1; duration = 80;
	}

	if(sound ==3) {		
		RCAP2H = (-460)>>8; RCAP2L = -460; TR2 = 1; duration = 40;
	}
}



//==============================================================================
//MOVE_COMPS
//gets input from the pots and moves the ball
//==============================================================================
void move_comps(void) {

	//BALL COORDINATES
	char bally, ballx;
	//LEFT 4 PADDLE HIT SPOTS
	char left_hit1 = (paddle1height/4)+paddle1top;
	char left_hit2 = (paddle1height/4*2)+paddle1top;
	char left_hit3 = (paddle1height/4*3)+paddle1top;
	char left_hit4 = paddle1height+paddle1top;
	//RIGHT 4 PADDLE HIT SPOTS
	char right_hit1 = (paddle2height/4)+paddle2top;
	char right_hit2 = (paddle2height/4*2)+paddle2top;
	char right_hit3 = (paddle2height/4*3)+paddle2top;
	char right_hit4 = paddle2height+paddle2top;

	//-------------------
	//GET POT INPUT
	//-------------------
	AMX0SL = 0x00;  //Select ain0
	onemillidelay(4);
	ADC0CF = 0x40;
	AD0INT = 0; 	//clear flag
	AD0BUSY = 1; 			
	while(AD0INT == 0){}	
	paddle1top = ADC0H*4; //so we get value out of 64	
								
	AMX0SL = 0x01; //select ain1	
	onemillidelay(4);
	ADC0CF = 0x40;
	AD0INT = 0;    //clear flag
	AD0BUSY = 1; 
	while(AD0INT == 0) 
	{}
	paddle2top = ADC0H*4;//so we get value out of 64	
	

		//---------------------
		//MOVE THE BALL
		//---------------------
		ball_x += run;		
		ball_y += rise;	
		
		ballx = ball_x+2; //so we know where the edges of the ball are
		bally = ball_y+2; //ball_x and ball_y are the center of the ball

		//----------------------------------	  
		//HITTING TOP OR BOTTOM
		//----------------------------------
		if (bally < 2 || bally > 61) {
		rise = -rise; 
		sound(1); //INVERT TO MOVE OPPOSITE
		}  
		else{ 
		rise = rise;
		}	
						
	//---------------------------------		  
	//LEFT PADDLE
	//---------------------------------
	if (ballx < 3){ 

		//-----FULL PADDLE-----
		if (paddle1 == 3){
		run = -run; //INVERT DIRECTION AUTOMATICALLY BECAUSE NO MATTER WHERE IT HITS IT WILL BOUNCE BACK
		sound(1); 
		} 
		//-----TOP QUAD-----									
		else if (bally >= paddle1top && bally <= left_hit1) {
		rise = -2; //STEEP ANGLE UP
		run = 1; 
		sound(2);
		}
		//-----THIRD QUAD-----
		else if (bally >= left_hit1 && bally <= left_hit2){
		rise = -1; //SHALLOW ANGLE UP
		run = 2; 
		sound(2);
		}
		//-----SECOND QUAD-----
		else if (bally >= left_hit2 && bally <= left_hit3){
		rise = 1; //SHALLOW ANGLE DOWN
		run = 2; 
		sound(2);
		}
		//-----FIRST QUAD-----
		else if (bally >= left_hit3 && bally <= left_hit4){
		rise = 2; //STEEP ANGLE DOWN
		run = 1; 
		sound(2);
		}
		//-----MISS-----
		else {
		score2++; 
		show_score = 0; 
		ball_x = 110; 
		ball_y = 30;
		run = -1; 
		rise = 1; 
		P1 = 0x00;
		LED9 = 0;
		LED10 = 0;
		sound(3);
		onemillidelay(500);
		P1 = 0xFF;
		LED9 = 1;
		LED10 = 1;
		}//end else
	refresh_screen();
	}//end if 

	//---------------------------------		  
	//RIGHT PADDLE
	//---------------------------------
	if (ballx > 123)
	{	
		//-----FULL PADDLE-----
		if (paddle2 == 3){
		run = -run; //INVERT DIRECTION AUTOMATICALLY BECAUSE NO MATTER WHERE IT HITS IT WILL BOUNCE BACK
		sound(1);
		}
		//-----TOP QUAD-----
		else if (bally >= paddle2top && bally <= right_hit1){
		rise = -2; //STEEP ANGLE UP
		run = -1; 
		sound(2);
		}
		//-----THIRD QUAD-----
		else if (bally >= right_hit1 && bally <= right_hit2) {
		rise = -1; //SHALLOW ANGLE UP
		run = -2; 
		sound(2);
		}
		//-----SECOND QUAD-----
		else if (bally >= right_hit2 && bally <= right_hit3){
		rise = 1; //SHALLOW ANGLE DOWN
		run = -2; 
		sound(2);
		}
		//-----FULL PADDLE-----
		else if (bally >= right_hit3 && bally <= right_hit4){
		rise = 2; 
		run = -1;
		sound(2);
		} 
		//-----MISS-----
		else {
		score1++; 
		show_score = 0; 
		ball_x = 8; 
		ball_y = 30;
		run = 1; 
		rise = 1; 
		P1 = 0x00;
		LED9 = 0;
		LED10 = 0;
		sound(3);
		onemillidelay(500);
		P1 = 0xFF;
		LED9 = 1;
		LED10 = 1;
		}
	}

	
}


//==============================================================================
//DISPLAY SCORES
//
//==============================================================================
void display_scores (char score1, char score2) 
{

		int font = (score1)*5;
		int font2;
		char i;
			if(score1 >=10){
				font2 = 5;
				font = (((font/5)-10)*5);
			}
			else {
				font2 = 0;
			}


				for (i=0; i<5; i++)
				{
					screen[304+i] = score5x8[font+i];
					screen[298+i] = score5x8[font2+i];
						//FOR SOCCER MODE
						if(soccer==1) {
							put_char(1,42,'P');
							put_char(1,48,'1');
						}
						else if(soccer==0){
							put_char(1,42, 'U');
							put_char(1,48, 'S');
							put_char(1,54, 'A');
						}
				}
		font = (score2)*5;
			if(score2 >=10){
				font2 = 5;
				font = (((font/5)-10)*5);
			}
			else {
				font2 = 0;
			}
				for (i=0; i<5; i++)
				{
					screen[336+i] = score5x8[font+i];
					screen[330+i] = score5x8[font2+i];
						//FOR SOCCER MODE
						if(soccer==1) {
							put_char(1,74,'P');
							put_char(1,80,'2');
						}
						else if(soccer==0){
							put_char(1,70, 'I');
							put_char(1,76, 'T');
							put_char(1,82, 'A');
							put_char(1,88, 'L');
							put_char(1,94, 'Y');
						}

				}//end for
		refresh_screen();		
}




//==============================================================================
//WINNER
//
//==============================================================================
void winner(void) {
			int i;
			blank_screen();
			init_game();
			display_scores(score1, score2);
			put_char(4,48, 'W'); put_char(4,54, 'I'); put_char(4,60,'N');	put_char(4,66,'N'); //displays WINNER
			put_char(4,72,'E'); put_char(4,78,'R'); put_char(4,84, '!');
			refresh_screen();
			

			for(i=0;i<5;i++) {

				P1 = 0x00;
				LED9 = 0;
				LED10 = 0;
				sound(3);
				onemillidelay(500);
				P1 = 0xFF;
				LED9 = 1;
				LED10 = 1;

			}
			onemillidelay(2000);
}



//==============================================================================
//SOCCERMODE
//
//==============================================================================
void soccermode(void) {
	int i; 

	for(i=63; i<960; i++) {
		screen[i+1] = 0xFF; 
		i= i+127;
	}

	//LEFT GOAL
	for(i=256; i<276; i++) {
		screen[i] = 0x1;
	}
	for(i=768; i<788; i++) {
		screen[i] = 0x1;
	}
	for(i=275; i<768; i++) {
		screen[i] = 0xFF;
		i = i+127;
	}

	//RIGHT GOAL
	for(i=363; i<383; i++) {
		screen[i] = 0x1;
	}
	for(i=875; i<895; i++) {
		screen[i] = 0x1;
	}
	for(i=363; i<875; i++) {
		screen[i] = 0xFF;
		i = i+127;
	}

	//CENTER 
	for(i=449;i<459;i++){
	screen[i] = 0x01;
	screen[i+256] = 0x01;
	}
	for(i=438;i<448;i++){
	screen[i] = 0x01;
	screen[i+256] = 0x01;
	}
	for(i=438;i<689;i++){
	screen[i] = 0xFF;
	screen[i+20] = 0xFF;
	i=i+127;
	}

}












//==============================================================================
//SPEED
//gets input from the pots and moves the ball
//==============================================================================
void speed(char speed)
{
		if(speed==0) {
			TMR3RLH = (-18432)>>8; //100 pixels per second
			TMR3RLL = -18432;
		}
		else if(speed==1) {
			TMR3RLH = (-50000)>>8;//120 pixels per second
			TMR3RLL = -50000;
		}
		else if(speed==2) {
			TMR3RLH = (-12288)>>8;//150 pixels per second
			TMR3RLL = -12288;
		}
		else if(speed==3) {
			TMR3RLH = (-9216)>>8;//200 pixels per second
			TMR3RLL = -9216;
		}
	
}




void draw_paddle (int x, int y, char paddle)
{
	long mask;

			if(paddle == 0) {
				if(y>54) y = 54;  	//Case 0 sets the paddle
				mask = 0xffl;  		//as 8 pixels tall and 
				paddle_height = 8;	//assigns it as a long
			}
			else if(paddle == 1) {
				if(y>50) y = 50;	//Case 1 sets the paddle
				mask = 0xfffl;		//as 12 pixels tall and
				paddle_height = 12;	//assigns it as a long
			}
			else if(paddle == 2) {
				if(y > 46) y= 46;	//Case 2 sets the paddle
				mask = 0xffffl; 	//as 16 pisels tall and 
				paddle_height = 16;	//assigns it as a long
			}
			else if(paddle == 3) {
				while(x<1024)			//Case 3 is where the
				{						//the paddle can be 
				   	screen[x] = 0xffl;	//set as 64 pixels tall
				    screen[x+1] = 0xffl;//which creates the one
					x = x+128;			//player mode.
					paddle_top = 2;
					paddle_bottom = 60;
					paddle_height = 60;
				}
			}
		
	if (y<2) y =2;					//In this section of the
	mask = mask <<(y%8);			//draw_paddle we apply the 
	x += (y/8)*128;					//mask from the top of the screen
	screen[x] |= mask;				//to the bottom of the screen
	screen[x+1] |= mask;			//y modulus 8 is how the correct
	screen[x+128] |= mask >> 8;		//page is selected and than we span
	screen[x+129] |= mask >> 8;		//at least 3 pages so as to cover 
	screen[x+256] |= mask >> 16;	//all the paddle sizes with the exception
	screen[x+257] |= mask >> 16;	//of the full length paddle
	paddle_top = y;
	paddle_bottom = y+paddle_height-1;
}


void draw_ball (int x, int y)
{
	long mask = 0x1fl << (y%8); // 1f gives a 5 pixel tall mask
							    // it is cast as a long and than
								// shifted left by the remainder of
								// y divided by 8 so as to get the
								// mask in the right place vertically
	int j = (y/8)*128+x; 	  // divide by 8 and multiply by 128 and add
							  // the x value to get on the correct page
	int i;
	for (i = 0; i<5; ++i) // loop 5 times to make it 5 pixels wide
	{ 
		screen[j] |= mask; // or with the mask to not overwrite anything 
		screen[j+128] |= mask >>8;//shift right span two pages.
		j++; 
	}
}



