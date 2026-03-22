#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL) ^ getpid());
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    printf("\033[?25l");  // hide cursor using ANSI escape sequence 
    
    float rows, cols;
    getmaxyx(stdscr, rows, cols);
    char text1[] = "PING PONG GAME";
    char text2[] = "Press ENTER to play";
    int y = rows / 2;
    int x1 = (cols - strlen(text1)) / 2;
    int x2 = (cols - strlen(text2)) / 2;
    int visible = 1;
    int counter = 0; 

    //MENU SCREEN
    timeout(100); // ~10 frames per second 
    if (y < rows && x1 < cols && x2 < cols){
	while(1) {
	    clear();
	    attron(A_BOLD);
	    mvprintw(y, x1, "%s", text1);
	    attroff(A_BOLD);
	    mvprintw(y + 1, x2, "Press ");

	    if  (visible)
		mvprintw(y + 1, x2 + 6, "ENTER");
	    else
		mvprintw(y + 1, x2 + 6, "     ");

	    mvprintw(y + 1, x2 + 11, " to play");
	    
	    //Blink Speed
	    counter++;
	    if (counter >= 5){
	    visible = !visible;
	    counter = 0;
	    }
	    refresh();
	    int ch = getch();
	    if(ch == '\n' || ch == KEY_ENTER)
		break;
	}
	flushinp();
    }
    else {
	    endwin();
	    printf("Terminal window is too small to be playable/n");
	    return(1);
	}

    //SWITCH TO GAME
    clear();
    refresh();

    int center_y = rows / 2;
    int center_x = cols / 2;
	
    //Ball variables
    int ball_pos_x = cols / 2;
    int ball_pos_y = rows / 2;
    int ball_dx, ball_dy; 
    
    int player_paddle_pos = center_y;
    int computer_paddle_pos = center_y;
    int player_dy, computer_dy;    
    int player_score = 0, comp_score = 0;
    timeout(0);

    //GAME LOOP
    clear();
    float speed = 1.0 + (rand() % 3);
    ball_dx = (rand() % 2) ? 1 : -1;
    ball_dy = (rand() % 3) - 1; 
    
    if (ball_dy == 0)
	ball_dy = (rand() % 2) ? 1 : -1;

    while (1) {
	//GAME AREA
	clear();
	mvprintw(0, 0, "Press ");
	attron(A_BOLD);
	mvprintw(0, 6, "Q");
	attroff(A_BOLD);
	mvprintw(0, 7, " to Quit");
    
	//Center Line
	for (int y_axis = 1; y_axis != rows - 1; y_axis++) 
	    mvprintw(y_axis, center_x, "|");  
	//Borders
	for (int x = 0; x != cols; x++){
	    mvprintw(1, x, "-");
	    mvprintw(rows - 2, x, "-");
	}

	//Score Board
	mvprintw(rows - 1, cols/2 - 11, "Player = %d  Computer = %d", player_score, comp_score); 

	//Computer Paddle
	static int ai_frame = 0;
	static int error = 0;
	static int error_timer = 0;

	ai_frame++;
	error_timer++;

	if (error_timer > 20) {
	    error = (rand() % 3) - 1;   // small error
	    error_timer = 0;
	}

	if (ai_frame % 2 == 0) {

	    if (ball_dx > 0) {
		int target = ball_pos_y + error;

		if (computer_paddle_pos < target && computer_paddle_pos < rows - 3) {
		    computer_paddle_pos++;
		}
		else if (computer_paddle_pos > target && computer_paddle_pos > 2) {
		    computer_paddle_pos--;
		}
	    }
	    else {
	    // return to center slowly
		if (computer_paddle_pos < rows / 2) {
		    computer_paddle_pos++;
		}
		else if (computer_paddle_pos > rows / 2) {
		    computer_paddle_pos--;
		}
	    }
    }	



	mvprintw(computer_paddle_pos, cols - 1, "|");
	mvprintw(computer_paddle_pos - 1, cols - 1, "|");
	mvprintw(computer_paddle_pos + 1, cols - 1, "|");

	int command = getch();
	switch (command) {
		    
	    case KEY_UP:
		if (player_paddle_pos > 2){
		    player_paddle_pos--;
		    player_dy = -1;
		}
		break;
	    case KEY_DOWN:
		if (player_paddle_pos < rows - 3){
		    player_paddle_pos++;
		    player_dy = 1;
		}
	    break;
	    case 'q':
	    case 'Q':
		endwin();
		printf("\033[?25h");
		return 0;

	}
	mvprintw(player_paddle_pos, 0, "|");
	mvprintw(player_paddle_pos - 1, 0, "|");
	mvprintw(player_paddle_pos + 1, 0, "|");
	
	//Ball Movement
	
	static int frame = 0; //Frame skip logic
	frame++;

	int ball_speed = 3;
	if (frame >= ball_speed){
	    ball_pos_x += ball_dx;
	    ball_pos_y += ball_dy;
	    frame = 0;
	}
	
	//Left/Right Bounce
	if (ball_pos_x <= 0){
	    ball_pos_x = 0;
	    ball_dx = 1;
	}
	else if (ball_pos_x >= cols - 1){
	    ball_pos_x = cols - 1;
	    ball_dx = -1;
	}
	
	//Top/Bottom Bounce
	if (ball_pos_y <= 1){
	    ball_pos_y = 1;
	    ball_dy = 1;
	}
	else if (ball_pos_y >= rows - 2){
	    ball_pos_y = rows - 2;
	    ball_dy = -1;
	}
	attron(A_BOLD);
	mvprintw(ball_pos_y,ball_pos_x, "O");
	attroff(A_BOLD);

	if (ball_dx < 0 && ball_pos_x <= 1){
	    if (abs(ball_pos_y - player_paddle_pos) <= 1){
		ball_dx = 1;
		ball_dy = player_dy;
		int diff = ball_pos_y - player_paddle_pos;
		ball_dy = diff;

		if (ball_dy > 1) ball_dy = 1;
		if (ball_dy < -1) ball_dy = -1;
	    }
	    else{
		attron(A_BOLD);
		mvprintw(ball_pos_y,ball_pos_x, "X");
		attroff(A_BOLD);
		comp_score++;
	    	napms(1000);
		ball_pos_x = cols / 2;
		ball_pos_y = rows / 2;
		player_paddle_pos = rows / 2;
		computer_paddle_pos = rows / 2;
		flushinp();
		ball_dx = -1;
		if (ball_dy == 0)
		
		ball_dy = (rand() % 3) - 1;

		if (ball_dy == 0)
		    ball_dy = (rand() % 2) ? 1 : -1 ;
	    }
	}
	else if (ball_dx > 0 && ball_pos_x == cols - 2){
	    if (abs(ball_pos_y - computer_paddle_pos) <= 1){
		ball_dx = -1;
		int diff = ball_pos_y - player_paddle_pos;
		ball_dy = diff;

		if (ball_dy > 1) ball_dy = 1;
		if (ball_dy < -1) ball_dy = -1;	
	    }
	    else{
		attron(A_BOLD);
		mvprintw (ball_pos_y, ball_pos_x, "X");
		attroff(A_BOLD);
		player_score++;
		napms(1000);
		ball_pos_x = cols / 2;
		ball_pos_y = rows / 2;
		ball_dx = 1;
		player_paddle_pos = rows / 2;
		computer_paddle_pos = rows / 2;
		flushinp();
	    	if (ball_dy == 0){
		    ball_dy = (rand() % 2) ? 1 : -1;
		}

	    }
	}

	if (player_score > 5 && player_score - comp_score >= 2){
	    char win_str1[] = "PLAYER WINS!";
	    int winx = (cols - strlen(win_str1)) / 2;
	    int winy = center_y;
	    clear();
	    attron(A_BOLD);
	    mvprintw(winy, winx,"%s", win_str1);
	    attroff(A_BOLD);
	    refresh();
	    napms(1000);
	    mvprintw(center_y + 1, center_x - 11, "Press any key to exit");
	    timeout(-1);
	    getch();
	    endwin();
	    printf("\033[?25h");
	    return 0;

	}

	else if(comp_score > 5 && comp_score - player_score >= 2){
	    char win_str2[] = "COMPUTER WINS ;-;";
	    int winx = (cols - strlen(win_str2)) / 2;
	    int winy = center_y;
	    clear();
	    mvprintw(winy, winx,"%s", win_str2);
	    refresh();
	    napms(1000);
	    mvprintw(center_y + 1, center_x - 11, "Press any key to exit");
	    timeout(-1);
	    getch();
	    endwin();
	    printf("\033[?25h");
	    return 0;

	}

	refresh();
	
	usleep(16000);   // ~60 FPS
    }
    
    endwin();
    printf("\033[?25h");

    return 0;
}
