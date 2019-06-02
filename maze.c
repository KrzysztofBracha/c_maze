#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h> //sleep()
#include "header_maze.h"
// -std=c99

unsigned short int arr[10][10];

int main() {
	struct Coords view_coords;
	struct Coords arr_coords;
	bool built, build_mode, run;
	init_ncurses();
	init (&view_coords, &arr_coords, &built, &build_mode, &run);
	build_control (&view_coords, &arr_coords, &built, &build_mode, &run);

    	endwin();
	return 0;
}

void init_ncurses(void) {
	raw();
        initscr();
        keypad(stdscr, TRUE);
        noecho();
        clear();
        curs_set(0);
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_WHITE); // maze
        init_pair(2, COLOR_RED, COLOR_WHITE); // cursor
        init_pair(3, COLOR_YELLOW, COLOR_WHITE); // path
        init_pair(4, COLOR_WHITE, COLOR_BLACK); // text
	init_pair(5, COLOR_CYAN, COLOR_WHITE); // run
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
	move(4, 37);
	printw("FINISH");
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
	move(25, 18);
	printw("START");
}

void build_control (struct Coords *view_coords, struct Coords *arr_coords, bool *built, bool *build_mode, bool *run) {
	int key;
	while (((key = getch()) != 'q') && (*run == false)) {
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
                                *build_mode = change_build_mode (*build_mode);
                                break;
                        case (KEY_BACKSPACE):
                                *built = delete_path(arr_coords, *built);
                                break;
			case ('r'):
				*run = true;
				run_escape (view_coords, arr_coords, run);
				break;
                }
        }

}

bool clear_path (struct Coords *view_coords, struct Coords *arr_coords, bool built, bool run) {
	if (built == false) {
		if ((run == false) && (arr[arr_coords->y][arr_coords->x] == 0))
			draw_path(view_coords, 1);
                else
			draw_path(view_coords, 3);
       	} else {
		built = false;
	}
	return built;
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

bool build_path (struct Coords *view_coords, struct Coords *arr_coords, bool built) {
	draw_path(view_coords, 3);
        built = true;
        arr[arr_coords->y][arr_coords->x] = 1;
	return built;
}

bool delete_path(struct Coords *arr_coords, bool built) {
	arr[arr_coords->y][arr_coords->x] = 0;
	built = false;
	return built;
}

void init_arr(void) {
	for (int i=0; i<10; ++i)
		for (int j=0; j<10; ++j)
			arr[i][j] = 0;
}

void move_up (struct Coords *view_coords, struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run) {
	if (view_coords->y > 6) {
        	if (build_mode == true)
                	*built = build_path(view_coords, arr_coords, *built);
                *built = clear_path(view_coords, arr_coords, *built, run);
                view_coords->y -= 2;
                draw_path(view_coords, color);
                --(arr_coords->y);
	}
}

void move_down (struct Coords *view_coords, struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run) {
	if (view_coords->y < 24) {
        	if (build_mode == true)
                	*built = build_path(view_coords, arr_coords, *built);
                *built = clear_path(view_coords, arr_coords, *built, run);
                view_coords->y += 2;
                draw_path(view_coords, color);
               	++(arr_coords->y);
	}
}

void move_right (struct Coords *view_coords, struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run) {
	if (view_coords->x < 46) {
        	if (build_mode == true)
                	*built = build_path(view_coords, arr_coords, *built);
                *built = clear_path(view_coords, arr_coords, *built, run);
                view_coords->x += 4;
                draw_path(view_coords, color);
                ++(arr_coords->x);
	}
}

void move_left (struct Coords *view_coords, struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run) {
	if (view_coords->x > 12) {
        	if (build_mode == true)
                	*built = build_path(view_coords, arr_coords, *built);
                *built = clear_path(view_coords, arr_coords, *built, run);
                view_coords->x -= 4;
                draw_path(view_coords, color);
                --(arr_coords->x);
	}
}

bool change_build_mode (bool build_mode) {
	build_mode = (build_mode == true) ? false : true;
	write_build_mode (build_mode);
	return build_mode;
}

void write_build_mode (bool build_mode) {
	attrset(COLOR_PAIR(4));
	move(21, 60);
	if (build_mode == true)
        	printw("Build mode: ON\t");
	else
		printw("Build mode: OFF");
}

int kbhit(void) {
	int ch = getch();
	if (ch != ERR) {
		ungetch(ch);
		return 1;
	} else {
		return 0;
	}
}

void run_escape (struct Coords *view_coords, struct Coords *arr_coords, bool *run) {
	struct Directions directions;
	struct b_Directions b_directions;
	short int direction; // up = 0, down = 1, left = 2, right = 3
	bool b_finish = false;
	unsigned short int moves = 0;
	nodelay(stdscr, TRUE);
	init_escape (view_coords, arr_coords);
	while (b_finish == false) {
		sleep(1);
		check_directions (arr_coords, &b_directions, &directions);
		direction = choose_direction (&b_directions, &directions);
		move_to_direction (direction, view_coords, arr_coords, *run);
		b_finish = check_finish (arr_coords, b_finish);
		moves = write_moves(moves);
		if (kbhit()) {
			if (getch() == 'q')
				break;
		}
	}
}

void init_escape (struct Coords *view_coords, struct Coords *arr_coords) {
	prepare_view(view_coords, arr_coords);
	view_coords->x = 18;
        view_coords->y = 24;
        arr_coords->x = 2;
        arr_coords->y = 9;
        draw_path(view_coords, 5);
        write_run_info();
}

void prepare_view (struct Coords *view_coords, struct Coords *arr_coords) {
	if (arr[arr_coords->x][arr_coords->y] == 0) {
		draw_path (view_coords, 1);
	} else {
		draw_path (view_coords, 3);
	}
}

void write_run_info (void) {
	attrset(COLOR_PAIR(4));
	int y = 5;
	move(y, 60);
        printw("PROGRAM RUN");
	move(y+=2, 60);
	printw("MOVES: 0\t\t\t\t\t");
	move(y+=2, 60);
	printw("PRESS Q TO QUIT\t");
	for (int i=0; i<7; ++i) {
		move(y+=2, 60);
		printw("\t\t\t\t\t\t");
	}
	refresh();
}

void write_coords(struct Coords *arr_coords) {
	attrset(COLOR_PAIR(4));
	move(11, 60);
	printw("UP: %d", arr[arr_coords->y][arr_coords->x]);
	move(13, 60);
	printw("Y: %d", arr_coords->y);
	move(15, 60);
	printw("X: %d", arr_coords->x);
}

unsigned short int write_moves (unsigned short int moves) {
	attrset(COLOR_PAIR(4));
        move(7, 60);
	++moves;
        printw("MOVES: %u", moves);
	refresh();
	return moves;
}

void check_directions (struct Coords *arr_coords, struct b_Directions *b_directions, struct Directions *directions) {
        check_up (arr_coords, &(directions->up), &(b_directions->b_up));
	check_down (arr_coords, &(directions->down), &(b_directions->b_down));
	check_left (arr_coords, &(directions->left), &(b_directions->b_left));
	check_right (arr_coords, &(directions->right), &(b_directions->b_right));
}

void check_up (struct Coords *arr_coords, unsigned short int *up, bool *b_up) {
	if ((arr_coords->y > 0) && (arr[arr_coords->y - 1][arr_coords->x] > 0)) {
       		*up = arr[arr_coords->y - 1][arr_coords->x];
              	*b_up = true;
        } else {
              *b_up = false;
        }
}

void check_down (struct Coords *arr_coords, unsigned short int *down, bool *b_down) {
	if ((arr_coords->y < 9) && (arr[arr_coords->y + 1][arr_coords->x] > 0)) {
		*down = arr[arr_coords->y + 1][arr_coords->x];
                *b_down = true;
        } else {
              *b_down = false;
        }
}

void check_left (struct Coords *arr_coords, unsigned short int *left, bool *b_left) {
	if ((arr_coords->x > 0) && (arr[arr_coords->y][arr_coords->x - 1] > 0)) {
              *left = arr[arr_coords->y][arr_coords->x - 1];
              *b_left = true;
        } else {
              *b_left = false;
        }
}

void check_right (struct Coords *arr_coords, unsigned short int *right, bool *b_right) {
	if ((arr_coords->x < 9) && (arr[arr_coords->y][arr_coords->x + 1] > 0)) {
              *right = arr[arr_coords->y][arr_coords->x + 1];
              *b_right = true;
        } else {
              *b_right = false;
        }
}

short int choose_direction (struct b_Directions *b_directions, struct Directions *directions) {
	int value;
	bool chose = false;
	short int direction;
	if (b_directions->b_up == true) {
		direction = choose_up (direction, &value, &chose, directions->up);
	}
	if ((b_directions->b_right == true) && ((directions->right < value) || (chose == false))) {
		direction = choose_right (direction, &value, &chose, directions->right);
	}
	if ((b_directions->b_left == true) && ((directions->left < value) || (chose == false))) {
		direction = choose_left (direction, &value, &chose, directions->left);
	}
	if ((b_directions->b_down == true) && ((directions->down < value) || (chose == false))) {
		direction = 1;
	}
	return direction;
}

short int choose_up (short int direction, int *value, bool *chose, unsigned short int up) {
	direction = 0;
        *value = up;
        *chose = true;
	return direction;
}

short int choose_right (short int direction, int *value, bool *chose, unsigned short int right) {
        direction = 3;
        *value = right;
        *chose = true;
	return direction;
}

short int choose_left (short int direction, int *value, bool *chose, unsigned short int left) {
        direction = 2;
        *value = left;
        *chose = true;
	return direction;
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

bool check_finish (struct Coords *arr_coords, bool b_finish) {
	if ((arr_coords->y == 0) && (arr_coords->x == 7)) {
		b_finish = finish(b_finish);
	}
	return b_finish;
}

bool finish (bool b_finish) {
	b_finish = true;
        attrset(COLOR_PAIR(4));
        move(5, 60);
        printw("PROGRAM FINISHED");
	refresh();
	return b_finish;
}
