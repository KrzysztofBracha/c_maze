#include <ncurses.h>
#include <stdbool.h> // bool type
#include <unistd.h> // sleep
// -std=c99

void init_ncurses (void);
void init (int *x, int *y, int *arr_x, int *arr_y, bool *built, bool *build_mode);
void draw_board (void);
void control (int *y, int *x, int *arr_y, int *arr_x, bool *built, bool *build_mode);
void draw_path (int y, int x, int color);
void clear_path (int y, int x, int arr_y, int arr_x, bool *built);
void build_path (int y, int x, int arr_y, int arr_x, bool *built);
void delete_path (int y, int x, int arr_y, int arr_x, bool *built);
void move_up (int *y, int x, int *arr_y, int arr_x, bool build_mode, bool *built, int color);
void move_down (int *y, int x, int *arr_y, int arr_x, bool build_mode, bool *built, int color);
void move_right (int y, int *x, int arr_y, int *arr_x, bool build_mode, bool *built, int color);
void move_left (int y, int *x, int arr_y, int *arr_x, bool build_mode, bool *built, int color);
void init_arr (void);
void write_info (void);
void change_build_mode (bool *build_mode);
void write_build_mode (bool *build_mode);
void run_escape (int *y, int *x, int *arr_y, int *arr_x);
void check_directions (int arr_y, int arr_x, bool *b_up, bool *b_down, bool *b_left, bool *b_right, int *up, int *down, int *left, int *right);
void check_up (int arr_y,int arr_x, int *up, bool *b_up);
void check_down (int arr_y, int arr_x, int *down, bool *b_down);
void check_left (int arr_y, int arr_x, int *left, bool *b_left);
void check_right (int arr_y, int arr_x, int *right, bool *b_right);
void choose_direction (int *direction, bool b_up, bool b_down, bool b_left, bool b_right, int up, int down, int left, int right);
void move_to_direction (int direction, int *y, int *x, int *arr_y, int *arr_x);
int arr[10][10];

void info (int arr_y, int arr_x, int  up, int down, int left, int right, bool b_up, bool b_right);

int main() {
	int x, y, arr_x, arr_y;
	bool built, build_mode;
	init_ncurses();
	init (&x, &y, &arr_x, &arr_y, &built, &build_mode);
	control (&y, &x, &arr_y, &arr_x, &built, &build_mode);

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
        init_pair(1, COLOR_BLUE, COLOR_WHITE);
        init_pair(2, COLOR_RED, COLOR_WHITE);
        init_pair(3, COLOR_YELLOW, COLOR_WHITE);
        init_pair(4, COLOR_WHITE, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_CYAN);
}

void init (int *x, int *y, int *arr_x, int *arr_y, bool *built, bool *build_mode) {
        *built = true;
        *build_mode = false;
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

void control (int *y, int *x, int *arr_y, int *arr_x, bool *built, bool *build_mode) {
	int key;
	while ((key = getch()) != 'q') {
                switch (key) {
                        case (KEY_UP):
                                move_up (y, *x, arr_y, *arr_x, *build_mode, built, 2);
                                break;
                        case (KEY_DOWN):
                                move_down (y, *x, arr_y, *arr_x, *build_mode, built, 2);
                                break;
                        case (KEY_RIGHT):
                                move_right (*y, x, *arr_y, arr_x, *build_mode, built, 2);
                                break;
                        case (KEY_LEFT):
                               move_left (*y, x, *arr_y, arr_x, *build_mode, built, 2);
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
				run_escape (y, x, arr_x, arr_y);
				break;
                }
        }

}

void clear_path (int y, int x, int arr_y, int arr_x, bool *built) {
	if (*built == false) {
		if (arr[arr_y][arr_x] == 0)
			draw_path(y, x, 3);
                else
			draw_path(y, x, 1);
       	} else
		*built = false;
	// todo
        move(y, x);
        printw("%d", arr[arr_y][arr_x]);

}

void draw_path(int y, int x, int color) {
	move(y, x);
	attrset(COLOR_PAIR(color));
	for (int i=0; i<4; ++i)
		addch(ACS_CKBOARD);
        move(--y, x);
	for (int i=0; i<4; ++i)
        	addch(ACS_CKBOARD);
	move(--y, x);
	refresh();
}

void build_path(int y, int x, int arr_y, int arr_x, bool *built) {
	draw_path(y, x, 3);
        *built = true;
        arr[arr_y][arr_x] = 0;
	// todo
	move(y, x);
        printw("%d", arr[arr_y][arr_x]);
}

void delete_path(int y, int x, int arr_y, int arr_x, bool *built) {
	arr[arr_y][arr_x] = -1;
	*built = false;
	// todo
        move(y, x);
        printw("%d", arr[arr_y][arr_x]);
}

void init_arr(void) {
	for (int i=0; i<10; ++i)
		for (int j=0; j<10; ++j)
			arr[i][j] = -1;
}

void move_up (int *y, int x, int *arr_y, int arr_x, bool build_mode, bool *built, int color) {
	if (*y > 6) {
        	if (build_mode == true)
                	build_path(*y, x, *arr_y, arr_x, built);
                clear_path(*y, x, *arr_y, arr_x, built);
                (*y) -= 2;
                draw_path(*y, x, color);
                --(*arr_y);
	}
}

void move_down (int *y, int x, int *arr_y, int arr_x, bool build_mode, bool *built, int color) {
	if (*y < 24) {
        	if (build_mode == true)
                	build_path(*y, x, *arr_y, arr_x, built);
                clear_path(*y, x, *arr_y, arr_x, built);
                (*y) += 2;
                draw_path(*y, x, color);
               	++(*arr_y);
	}
}

void move_right (int y, int *x, int arr_y, int *arr_x, bool build_mode, bool *built, int color) {
	if (*x < 46) {
        	if (build_mode == true)
                	build_path(y, *x, arr_y, *arr_x, built);
                clear_path(y, *x, arr_y, *arr_x, built);
                (*x) += 4;
                draw_path(y, *x, color);
                ++(*arr_x);
	}
}

void move_left (int y, int *x, int arr_y, int *arr_x, bool build_mode, bool *built, int color) {
	if (*x > 12) {
        	if (build_mode == true)
                	build_path(y, *x, arr_y, *arr_x, built);
                clear_path(y, *x, arr_y, *arr_x, built);
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

void run_escape (int *y, int *x, int *arr_y, int *arr_x) {
	move(15, 60);
        printw("program ran");
	int up, down, left, right;
	bool b_up, b_down, b_left, b_right;
	bool run = true;
	int direction; // up = 0, down = 1, left = 2, right = 3
	*x = 18;
        *y = 24;
        *arr_x = 2;
        *arr_y = 9;
	draw_path(*y, *x, 5);
	info(*arr_y, *arr_x, up, down, left, right, b_up, b_right);
	while (run == true) {
		info(*arr_y, *arr_x, up, down, left, right, b_up, b_right);
		sleep(1);
		check_directions (*arr_y, *arr_x, &b_up, &b_down, &b_left, &b_right, &up, &down, &left, &right);
		choose_direction (&direction, b_up, b_down, b_left, b_right, up, down, left, right);
		move_to_direction (direction, y, x, arr_y, arr_x);
	}
}

void info (int arr_y, int arr_x, int  up, int down, int left, int right, bool b_up, bool b_right) {
	move(12, 60);
        printw("value of up: %d\t\t", up);
        move(13, 60);
        printw("value of arr_X: %d\t\t", arr_x);
        move(14, 60);
        printw("value of arr_y: %d\t\t", arr_y);
	move(15, 60);
	printw("value of down: %d\t\t", down);
	move(16, 60);
	printw("value of down: %d\t\t", arr[arr_x][++arr_y]);
	move(17, 60);
        printw("bool up: %d\t\t", b_up);
	move(18, 60);
        printw("bool right: %d\t\t", b_right);


}

void check_directions (int arr_y, int arr_x, bool *b_up, bool *b_down, bool *b_left, bool *b_right, int *up, int *down, int *left, int *right) {
        check_up (arr_y, arr_x, up, b_up);
	check_down (arr_y, arr_x, down, b_down);
	check_left (arr_y, arr_x, left, b_left);
	check_right (arr_y, arr_x, right, b_right);
}

void check_up (int arr_y, int arr_x, int *up, bool *b_up) {
	if ((arr_y > 0) && (arr[--arr_y][arr_x] > -1)) {
       		*up = arr[--arr_y][arr_x];
              	*b_up = true;
        } else {
              *b_up = false;
        }
}

void check_down (int arr_y, int arr_x, int *down, bool *b_down) {
	if ((arr_y < 9) && (arr[++arr_y][arr_x] > -1)) {
		*down = arr[++arr_y][arr_x];
                *b_down = true;
        } else {
              *b_down = false;
        }
}

void check_left (int arr_y, int arr_x, int *left, bool *b_left) {
	if ((arr_x > 0) && (arr[arr_y][--arr_x] > -1)) {
              *left = arr[arr_y][--arr_x];
              *b_left = true;
        } else {
              *b_left = false;
        }
}

void check_right (int arr_y, int arr_x, int *right, bool *b_right) {
	if ((arr_x < 9) && (arr[arr_y][++arr_x] > -1)) {
              *right = arr[arr_y][++arr_x];
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

void move_to_direction (int direction, int *y, int *x, int *arr_y, int *arr_x) {
	bool built = false;
	switch (direction) {
		case 0:
			move_up (y, *x, arr_y, *arr_x, false, &built, 5);
			break;
		case 1:
			move_down (y, *x, arr_y, *arr_x, false, &built, 5);
			break;
		case 2:
			move_left (*y, x, *arr_y, arr_x, false, &built, 5);
			break;
		case 3:
			move_right (*y, x, *arr_y, arr_x, false, &built, 5);
			break;
	}
	++arr[*arr_y][*arr_x];
}
