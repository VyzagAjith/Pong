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

    //MENU SCREEN
    timeout(100);
    if (y < rows && x1 < cols && x2 < cols){
	clear();
	attron(A_BOLD);
	mvprintw(y, x1, "%s", text1);
	attroff(A_BOLD);
	while(1) {
	    mvprintw(y + 1, x2, "Press ");

	    if  (visible)
		mvprintw(y + 1, x2 + 6, "ENTER");
	    else
		mvprintw(y + 1, x2 + 6, "     ");

	    mvprintw(y + 1, x2 + 11, " to play");

	    visible = !visible; 
	    
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
    mvprintw(0, 0, "GAME SCREEN");
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
