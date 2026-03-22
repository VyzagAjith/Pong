#include <ncurses.h>
#include <string.h>
#include <unistd.h>

int main() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    printf("\033[?25l");  // hide cursor using ANSI escape sequence 
    
    int rows, cols;
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

    //GAME SCREEN
    int center_y = rows / 2;
    int center_x = cols / 2;
    mvprintw(0, 0, "Press ");
    attron(A_BOLD);
    mvprintw(0, 6, "Q");
    attroff(A_BOLD);
    mvprintw(0, 7, " to Quit");

    for (int y_axis = 0; y_axis != rows + 1; y_axis++) 
	mvprintw(y_axis, center_x, "|");  // Center Line
    
    //Ball 
    attron(A_BOLD);
    mvprintw(center_y, center_x, "O");
    attroff(A_BOLD);

    //Player Paddle
    mvprintw(center_y, 0, "|"); //At start
    int player_paddle_pos = center_y;
    if (player_paddle_pos < rows - 1 && player_paddle_pos > 0){
	mvprintw(player_paddle_pos - 1, 0, "|");
	mvprintw(player_paddle_pos + 1, 0, "|");
    }
    
    //Computer Paddle
    mvprintw(center_y, cols - 1, "|");
    int computer_paddle_pos = center_y;
    if (computer_paddle_pos < rows - 1 && computer_paddle_pos > 0){
	mvprintw(computer_paddle_pos - 1, cols - 1, "|");
	mvprintw(computer_paddle_pos + 1, cols - 1, "|");
    }


    refresh();
    timeout(-1);
    while (1){
	int ch = getch();
	if (ch == 'q' || ch == 'Q')
	    break;
    }
    
    endwin();
    printf("\033[?25h");

return 0;
}
