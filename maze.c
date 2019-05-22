#include <ncurses.h>
#include <stdbool.h> // bool type
#include <unistd.h> // sleep
#include <stdio.h>
// -std=c99

struct Coords {
        int x;
        int y;
};

void init_ncurses (void);
void init (struct Coords *view_coords, struct Coords *arr_coords, bool *built, bool *build_mode, bool *run);
void draw_board (void);
void control (struct Coords *view_coords, struct Coords *arr_coords, bool *built, bool *build_mode, bool *run);
void draw_path (struct Coords *view_coords, int color);
void draw_row (void);
void clear_path (struct Coords *view_coords, bool *built, bool run);
void build_path (struct Coords *view_coords, struct Coords *arr_coords, bool *built);
void delete_path (struct Coords *arr_coords, bool *built);
void move_up (struct Coords *view_coords,  struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run);
void move_down (struct Coords *view_coords, struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run);
void move_right (struct Coords *view_coords, struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run);
void move_left (struct Coords *view_coords, struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run);
void init_arr (void);
void write_info (void);
void change_build_mode (bool *build_mode);
void write_build_mode (bool *build_mode);
void run_escape (struct Coords *view_coords, struct Coords *arr_coords, bool *run);
void check_directions (struct Coords *arr_coords, bool *b_up, bool *b_down, bool *b_left, bool *b_right, int *up, int *down, int *left, int *right);
void check_up (struct Coords *arr_coords, int *up, bool *b_up);
void check_down (struct Coords *arr_coords, int *down, bool *b_down);
void check_left (struct Coords *arr_coords, int *left, bool *b_left);
void check_right (struct Coords *arr_coords, int *right, bool *b_right);
void choose_direction (int *direction, bool b_up, bool b_down, bool b_left, bool b_right, int up, int down, int left, int right);
void move_to_direction (int direction, struct Coords *view_coords, struct Coords *arr_coords, bool run);
int arr[10][10];

int main() {
	struct Coords view_coords;
	struct Coords arr_coords;
	// int arr_x, arr_y;
	bool built, build_mode, run;
	init_ncurses();
	init (&view_coords, &arr_coords, &built, &build_mode, &run);
	control (&view_coords, &arr_coords, &built, &build_mode, &run);

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

void init (struct Coords *view_coords, struct Coords *arr_coords, bool *built, bool *build_mode, bool *run) {
        *built = true;
        *build_mode = false;
	*run = false;
        view_coords->x = 18;
        view_coords->y = 24;
        arr_coords->x = 2;
        arr_coords->y = 9;
	init_arr();
        write_info();
        draw_board();
        draw_path(view_coords, 3);
	write_build_mode (build_mode);
	arr[arr_coords->y][arr_coords->x] = 0;
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

void control (struct Coords *view_coords, struct Coords *arr_coords, bool *built, bool *build_mode, bool *run) {
	int key;
	while ((key = getch()) != 'q') {
                switch (key) {
                        case (KEY_UP):
                                move_up (view_coords, arr_coords, *build_mode, built, 2, *run);
                                break;
                        case (KEY_DOWN):
                                move_down (view_coords, arr_coords, *build_mode, built, 2, *run);
                                break;
                        case (KEY_RIGHT):
                                move_right (view_coords, arr_coords, *build_mode, built, 2, *run);
                                break;
                        case (KEY_LEFT):
                               move_left (view_coords, arr_coords, *build_mode, built, 2, *run);
                                break;
                        case (' '):
                                build_path (view_coords, arr_coords, built);
                                break;
                        case ('\t'):
                                change_build_mode (build_mode);
                                break;
                        case (KEY_BACKSPACE):
                                delete_path(arr_coords, built);
                                break;
			case ('r'):
				*run = true;
				run_escape (view_coords, arr_coords, run);
				break;
                }
        }

}

void clear_path (struct Coords *view_coords, bool *built, bool run) {
	if (*built == false) {
		if (run == false)
			draw_path(view_coords, 1);
                else
			draw_path(view_coords, 3);
       	} else
		*built = false;
}

void draw_path (struct Coords *view_coords, int color) {
	int y = view_coords->y;
	move(y, view_coords->x);
	attrset(COLOR_PAIR(color));
	draw_row();
        move(--y, view_coords->x);
	draw_row();
	move(--y, view_coords->x);
	refresh();
}

void draw_row (void) {
	for (int i=0; i<4; ++i)
        	addch(ACS_CKBOARD);
}

void build_path (struct Coords *view_coords, struct Coords *arr_coords, bool *built) {
	draw_path(view_coords, 3);
        *built = true;
        arr[arr_coords->y][arr_coords->x] = 0;
}

void delete_path(struct Coords *arr_coords, bool *built) {
	arr[arr_coords->y][arr_coords->x] = -1;
	*built = false;
}

void init_arr(void) {
	for (int i=0; i<10; ++i)
		for (int j=0; j<10; ++j)
			arr[i][j] = -1;
}

void move_up (struct Coords *view_coords, struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run) {
	if (view_coords->y > 6) {
        	if (build_mode == true)
                	build_path(view_coords, arr_coords, built);
                clear_path(view_coords, built, run);
                view_coords->y -= 2;
                draw_path(view_coords, color);
                --(arr_coords->y);
	}
}

void move_down (struct Coords *view_coords, struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run) {
	if (view_coords->y < 24) {
        	if (build_mode == true)
                	build_path(view_coords, arr_coords, built);
                clear_path(view_coords, built, run);
                view_coords->y += 2;
                draw_path(view_coords, color);
               	++(arr_coords->y);
	}
}

void move_right (struct Coords *view_coords, struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run) {
	if (view_coords->x < 46) {
        	if (build_mode == true)
                	build_path(view_coords, arr_coords, built);
                clear_path(view_coords, built, run);
                view_coords->x += 4;
                draw_path(view_coords, color);
                ++(arr_coords->x);
	}
}

void move_left (struct Coords *view_coords, struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run) {
	if (view_coords->x > 12) {
        	if (build_mode == true)
                	build_path(view_coords, arr_coords, built);
                clear_path(view_coords, built, run);
                view_coords->x -= 4;
                draw_path(view_coords, color);
                --(arr_coords->x);
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

void run_escape (struct Coords *view_coords, struct Coords *arr_coords, bool *run) {
	int up, down, left, right;
	bool b_up, b_down, b_left, b_right;
	int direction; // up = 0, down = 1, left = 2, right = 3
	view_coords->x = 18;
        view_coords->y = 24;
        arr_coords->x = 2;
        arr_coords->y = 9;
	draw_path(view_coords, 5);
	while (*run == true) {
		sleep(1);
		check_directions (arr_coords, &b_up, &b_down, &b_left, &b_right, &up, &down, &left, &right);
		choose_direction (&direction, b_up, b_down, b_left, b_right, up, down, left, right);
		move_to_direction (direction, view_coords, arr_coords, *run);
	}
}

void check_directions (struct Coords *arr_coords, bool *b_up, bool *b_down, bool *b_left, bool *b_right, int *up, int *down, int *left, int *right) {
        check_up (arr_coords, up, b_up);
	check_down (arr_coords, down, b_down);
	check_left (arr_coords, left, b_left);
	check_right (arr_coords, right, b_right);
}

void check_up (struct Coords *arr_coords, int *up, bool *b_up) {
	if ((arr_coords->y > 0) && (arr[arr_coords->y - 1][arr_coords->x] > -1)) {
       		*up = arr[arr_coords->y - 1][arr_coords->x];
              	*b_up = true;
        } else {
              *b_up = false;
        }
}

void check_down (struct Coords *arr_coords, int *down, bool *b_down) {
	if ((arr_coords->y < 9) && (arr[arr_coords->y + 1][arr_coords->x] > -1)) {
		*down = arr[arr_coords->y + 1][arr_coords->x];
                *b_down = true;
        } else {
              *b_down = false;
        }
}

void check_left (struct Coords *arr_coords, int *left, bool *b_left) {
	if ((arr_coords->x > 0) && (arr[arr_coords->y][arr_coords->x - 1] > -1)) {
              *left = arr[arr_coords->y][arr_coords->x - 1];
              *b_left = true;
        } else {
              *b_left = false;
        }
}

void check_right (struct Coords *arr_coords, int *right, bool *b_right) {
	if ((arr_coords->x < 9) && (arr[arr_coords->y][arr_coords->x + 1] > -1)) {
              *right = arr[arr_coords->y][arr_coords->x + 1];
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

void move_to_direction (int direction, struct Coords *view_coords, struct Coords *arr_coords, bool run) {
	bool built = false;
	switch (direction) {
		case 0:
			move_up (view_coords, arr_coords, false, &built, 5, run);
			break;
		case 1:
			move_down (view_coords, arr_coords, false, &built, 5, run);
			break;
		case 2:
			move_left (view_coords, arr_coords, false, &built, 5, run);
			break;
		case 3:
			move_right (view_coords, arr_coords, false, &built, 5, run);
			break;
	}
	++(arr[arr_coords->y][arr_coords->x]);
}
