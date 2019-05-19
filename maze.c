#include <ncurses.h>
#include <stdbool.h> // bool type
#include <unistd.h> // sleep
// -std=c99

void init_ncurses (void);
void init (int *x, int *y, int *arr_x, int *arr_y, bool *built, bool *build_mode, bool *run);
void draw_board (void);
void control (int *y, int *x, int *arr_y, int *arr_x, bool *built, bool *build_mode, bool *run);
void draw_path (int y, int x, int color);
void draw_row (void);
void clear_path (int y, int x, int arr_y, int arr_x, bool *built, bool run);
void build_path (int y, int x, int arr_y, int arr_x, bool *built);
void delete_path (int y, int x, int arr_y, int arr_x, bool *built);
void move_up (int *y, int x, int *arr_y, int arr_x, bool build_mode, bool *built, int color, bool run);
void move_down (int *y, int x, int *arr_y, int arr_x, bool build_mode, bool *built, int color, bool run);
void move_right (int y, int *x, int arr_y, int *arr_x, bool build_mode, bool *built, int color, bool run);
void move_left (int y, int *x, int arr_y, int *arr_x, bool build_mode, bool *built, int color, bool run);
void init_arr (void);
void write_info (void);
void change_build_mode (bool *build_mode);
void write_build_mode (bool *build_mode);
void run_escape (int *y, int *x, int *arr_y, int *arr_x, bool *run);
void check_directions (int arr_y, int arr_x, bool *b_up, bool *b_down, bool *b_left, bool *b_right, int *up, int *down, int *left, int *right);
void check_up (int arr_y,int arr_x, int *up, bool *b_up);
void check_down (int arr_y, int arr_x, int *down, bool *b_down);
void check_left (int arr_y, int arr_x, int *left, bool *b_left);
void check_right (int arr_y, int arr_x, int *right, bool *b_right);
void choose_direction (int *direction, bool b_up, bool b_down, bool b_left, bool b_right, int up, int down, int left, int right);
void move_to_direction (int direction, int *y, int *x, int *arr_y, int *arr_x, bool run);
int arr[10][10];

int main() {
	int x, y, arr_x, arr_y;
	bool built, build_mode, run;
	init_ncurses();
	init (&x, &y, &arr_x, &arr_y, &built, &build_mode, &run);
	control (&y, &x, &arr_y, &arr_x, &built, &build_mode, &run);

    	endwin();                  // zakończenie tryby curses
	return 0;
}

void init_ncurses(void) {
	raw();
        initscr();                 // rozpoczęcie tryby curses
        keypad(stdscr, TRUE);
        noecho();
        clear();
        curs_set(0);
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_WHITE); // maze color
        init_pair(2, COLOR_RED, COLOR_WHITE); // cursor color
        init_pair(3, COLOR_YELLOW, COLOR_WHITE); // path color
        init_pair(4, COLOR_WHITE, COLOR_BLACK);
	init_pair(5, COLOR_CYAN, COLOR_WHITE); // run color 
}

void init (int *x, int *y, int *arr_x, int *arr_y, bool *built, bool *build_mode, bool *run) {
        *built = true;
        *build_mode = false;
	*run = false;
        *x = 18;
        *y = 24;
        *arr_x = 2;
        *arr_y = 9;
	init_arr();
        write_info();
        draw_board();
        draw_path(*y, *x, 3);
	write_build_mode (build_mode);
	arr[*arr_y][*arr_x] = 0;
}

void draw_board (void) {
        attrset(COLOR_PAIR(1));
	move(10, 10);
        for (int i=5; i<25; ++i) {
                for (int j=5; j<25; ++j) {
                        move(i, j*2);
                        addch(ACS_CKBOARD);
                        addch(ACS_CKBOARD);
                }
        }
}

void write_info (void) {
	move(5, 60);
	printw("CONTROL:");
	move(7, 60);
	printw("Move: KEY UP, KEY DOWN, KEY LEFT, KEY RIGHT");
	move(9, 60);
	printw("Build path: SPACE");
	move(11, 60);
	printw("on/off build mode: TAB");
	move(13, 60);
	printw("delete path: BACKSPACE\n ");
	move(15, 60);
	printw("run: ENTER");
	move(17, 60);
	printw("quit: Q");
	move(19, 60);
	for (int i=0; i<22; ++i)
		addch(ACS_HLINE);
}

void control (int *y, int *x, int *arr_y, int *arr_x, bool *built, bool *build_mode, bool *run) {
	int key;
	while ((key = getch()) != 'q') {
                switch (key) {
                        case (KEY_UP):
                                move_up (y, *x, arr_y, *arr_x, *build_mode, built, 2, *run);
                                break;
                        case (KEY_DOWN):
                                move_down (y, *x, arr_y, *arr_x, *build_mode, built, 2, *run);
                                break;
                        case (KEY_RIGHT):
                                move_right (*y, x, *arr_y, arr_x, *build_mode, built, 2, *run);
                                break;
                        case (KEY_LEFT):
                               move_left (*y, x, *arr_y, arr_x, *build_mode, built, 2, *run);
                                break;
                        case (' '):
                                build_path (*y, *x, *arr_y, *arr_x, built);
                                break;
                        case ('\t'):
                                change_build_mode (build_mode);
                                break;
                        case (KEY_BACKSPACE):
                                delete_path(*y, *x, *arr_y, *arr_x, built);
                                break;
			case ('r'):
				*run = true;
				run_escape (y, x, arr_x, arr_y, run);
				break;
                }
        }

}

void clear_path (int y, int x, int arr_y, int arr_x, bool *built, bool run) {
	if (*built == false) {
		if (run == false)
			draw_path(y, x, 1);
                else
			draw_path(y, x, 3);
       	} else
		*built = false;
}

void draw_path (int y, int x, int color) {
	move(y, x);
	attrset(COLOR_PAIR(color));
	draw_row();
        move(--y, x);
	draw_row();
	move(--y, x);
	refresh();
}

void draw_row (void) {
	for (int i=0; i<4; ++i)
        	addch(ACS_CKBOARD);
}

void build_path (int y, int x, int arr_y, int arr_x, bool *built) {
	draw_path(y, x, 3);
        *built = true;
        arr[arr_y][arr_x] = 0;
}

void delete_path(int y, int x, int arr_y, int arr_x, bool *built) {
	arr[arr_y][arr_x] = -1;
	*built = false;
}

void init_arr(void) {
	for (int i=0; i<10; ++i)
		for (int j=0; j<10; ++j)
			arr[i][j] = -1;
}

void move_up (int *y, int x, int *arr_y, int arr_x, bool build_mode, bool *built, int color, bool run) {
	if (*y > 6) {
        	if (build_mode == true)
                	build_path(*y, x, *arr_y, arr_x, built);
                clear_path(*y, x, *arr_y, arr_x, built, run);
                (*y) -= 2;
                draw_path(*y, x, color);
                --(*arr_y);
	}
}

void move_down (int *y, int x, int *arr_y, int arr_x, bool build_mode, bool *built, int color, bool run) {
	if (*y < 24) {
        	if (build_mode == true)
                	build_path(*y, x, *arr_y, arr_x, built);
                clear_path(*y, x, *arr_y, arr_x, built, run);
                (*y) += 2;
                draw_path(*y, x, color);
               	++(*arr_y);
	}
}

void move_right (int y, int *x, int arr_y, int *arr_x, bool build_mode, bool *built, int color, bool run) {
	if (*x < 46) {
        	if (build_mode == true)
                	build_path(y, *x, arr_y, *arr_x, built);
                clear_path(y, *x, arr_y, *arr_x, built, run);
                (*x) += 4;
                draw_path(y, *x, color);
                ++(*arr_x);
	}
}

void move_left (int y, int *x, int arr_y, int *arr_x, bool build_mode, bool *built, int color, bool run) {
	if (*x > 12) {
        	if (build_mode == true)
                	build_path(y, *x, arr_y, *arr_x, built);
                clear_path(y, *x, arr_y, *arr_x, built, run);
                (*x) -= 4;
                draw_path(y, *x, color);
                --(*arr_x);
	}
}

void change_build_mode (bool *build_mode) {
	*build_mode = (*build_mode == true) ? false : true;
	write_build_mode (build_mode);
}

void write_build_mode (bool *build_mode) {
	attrset(COLOR_PAIR(4));
	move(21, 60);
	if (*build_mode == true)
        	printw("Build mode: ON\t");
	else
		printw("Build mode: OFF");
}

void run_escape (int *y, int *x, int *arr_y, int *arr_x, bool *run) {
	int up, down, left, right;
	bool b_up, b_down, b_left, b_right;
	int direction; // up = 0, down = 1, left = 2, right = 3
	*x = 18;
        *y = 24;
        *arr_x = 2;
        *arr_y = 9;
	draw_path(*y, *x, 5);
	while (*run == true) {
		sleep(1);
		check_directions (*arr_y, *arr_x, &b_up, &b_down, &b_left, &b_right, &up, &down, &left, &right);
		choose_direction (&direction, b_up, b_down, b_left, b_right, up, down, left, right);
		move_to_direction (direction, y, x, arr_y, arr_x, *run);
	}
}

void check_directions (int arr_y, int arr_x, bool *b_up, bool *b_down, bool *b_left, bool *b_right, int *up, int *down, int *left, int *right) {
        check_up (arr_y, arr_x, up, b_up);
	check_down (arr_y, arr_x, down, b_down);
	check_left (arr_y, arr_x, left, b_left);
	check_right (arr_y, arr_x, right, b_right);
}

void check_up (int arr_y, int arr_x, int *up, bool *b_up) {
	if ((arr_y > 0) && (arr[arr_y-1][arr_x] > -1)) {
       		*up = arr[arr_y-1][arr_x];
              	*b_up = true;
        } else {
              *b_up = false;
        }
}

void check_down (int arr_y, int arr_x, int *down, bool *b_down) {
	if ((arr_y < 9) && (arr[arr_y+1][arr_x] > -1)) {
		*down = arr[arr_y+1][arr_x];
                *b_down = true;
        } else {
              *b_down = false;
        }
}

void check_left (int arr_y, int arr_x, int *left, bool *b_left) {
	if ((arr_x > 0) && (arr[arr_y][arr_x-1] > -1)) {
              *left = arr[arr_y][arr_x-1];
              *b_left = true;
        } else {
              *b_left = false;
        }
}

void check_right (int arr_y, int arr_x, int *right, bool *b_right) {
	if ((arr_x < 9) && (arr[arr_y][arr_x+1] > -1)) {
              *right = arr[arr_y][arr_x+1];
              *b_right = true;
        } else {
              *b_right = false;
        }
}

void choose_direction (int *direction, bool b_up, bool b_down, bool b_left, bool b_right, int up, int down, int left, int right) {
	int value;
	bool chose = false;
	if (b_up == true) {
		*direction = 0;
		value = up;
		chose = true;
	}
	if ((b_right == true) && ((right < value) || (chose == false))) {
		*direction = 3;
		value = right;
		chose = true;
	}
	if ((b_left == true) && ((left < value) || (chose == false))) {
		*direction = 2;
		value = left;
		chose = true;
	}
	if ((b_down == true) && ((down < value) || (chose == false))) {
		*direction = 1;
	}
}

void move_to_direction (int direction, int *y, int *x, int *arr_y, int *arr_x, bool run) {
	bool built = false;
	switch (direction) {
		case 0:
			move_up (y, *x, arr_y, *arr_x, false, &built, 5, run);
			break;
		case 1:
			move_down (y, *x, arr_y, *arr_x, false, &built, 5, run);
			break;
		case 2:
			move_left (*y, x, *arr_y, arr_x, false, &built, 5, run);
			break;
		case 3:
			move_right (*y, x, *arr_y, arr_x, false, &built, 5, run);
			break;
	}
	++(arr[*arr_y][*arr_x]);
}
