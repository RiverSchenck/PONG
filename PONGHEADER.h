	
	
//-------------------------------------------------------------
//variables
//-------------------------------------------------------------	 
unsigned char score1=0, score2=0;		//START SCORE AT 0
char paddle1, paddle2;
char dx = 1;				//SET  BALL ANGLE AT 1/1			   
char dy = 1;	   	   
int start_x = 8; 			//SET STARTING POSITION FOR SERVE		
int start_y = 30;				   
char show_score = 0;		//SET LOOP FOR SHOWING SCORE TO 0
sbit BTN1 = P2^2;			//BTN1 FOR START GAME

char paddle_bottom, paddle_top, paddle_height;
int paddle1height, paddle1top=20, paddle1bottom;
int paddle2height, paddle2top=20, paddle2bottom;
int ball_x, ball_y, ball_left, ball_right, ball_top, ball_bottom;
int soccer;
char switches;	



//-------------------------------------------------------------
//functions
//-------------------------------------------------------------	
void move_comps(void);//DONE
void init_game (void);//DONE
void onemillidelay(unsigned int); //DONE
unsigned char put_char(unsigned char, unsigned char, char);// DONE
void get_switches(void);//DONE
void init_text(void); //DONE
void speed(char);//DONE
void sound(int);//DONE
void winner(void);//DONE
void soccermode(void);
void display_scores (char, char);
void draw_paddle (int, int, char);
void draw_ball (int, int);




//-------------------------------------------------------------
//SOUND
//-------------------------------------------------------------
code unsigned char sine[] = { 176, 217, 244, 254, 244, 217, 176, 128, 80, 39, 12, 2, 12, 39, 80, 128 };
unsigned char phase = sizeof(sine)-1;
unsigned int duration = 0;
char dy_random[] = {-2, 1, -1, 2, 1, -1, -2, -1, 2, -2, 1};
char random_count=1;




