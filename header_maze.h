struct Coords {
        short int x;
        short int y;
};

struct Directions {
	unsigned short int up;
	unsigned short int down;
	unsigned short int left;
	unsigned short int right;
};

struct b_Directions {
	bool b_up;
	bool b_down;
	bool b_left;
	bool b_right;
};

void init_ncurses (void);
void init (struct Coords *view_coords, struct Coords *arr_coords, bool *built, bool *build_mode, bool *run);
void draw_board (void);
void build_control (struct Coords *view_coords, struct Coords *arr_coords, bool *built, bool *build_mode, bool *run);
void draw_path (struct Coords *view_coords, int color);
void draw_row (void);
bool clear_path (struct Coords *view_coords, struct Coords *arr_coords, bool built, bool run);
bool build_path (struct Coords *view_coords, struct Coords *arr_coords, bool built);
bool delete_path (struct Coords *arr_coords, bool built);
void move_up (struct Coords *view_coords,  struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run);
void move_down (struct Coords *view_coords, struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run);
void move_right (struct Coords *view_coords, struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run);
void move_left (struct Coords *view_coords, struct Coords *arr_coords, bool build_mode, bool *built, int color, bool run);
void init_arr (void);
void write_info (void);
bool change_build_mode (bool build_mode);
void write_build_mode (bool build_mode);
void run_escape (struct Coords *view_coords, struct Coords *arr_coords, bool *run);
int kbhit(void);
void init_escape (struct Coords *view_coords, struct Coords *arr_coords);
void prepare_view (struct Coords *view_coords, struct Coords *arr_coords);
void write_run_info (void);
unsigned short int write_moves (unsigned short int moves);
void check_directions (struct Coords *arr_coords, struct b_Directions *b_directions, struct Directions *directions);
void check_up (struct Coords *arr_coords, unsigned short int *up, bool *b_up);
void check_down (struct Coords *arr_coords, unsigned short int *down, bool *b_down);
void check_left (struct Coords *arr_coords, unsigned short int *left, bool *b_left);
void check_right (struct Coords *arr_coords, unsigned short int *right, bool *b_right);
short int choose_direction (struct b_Directions *b_directions, struct Directions *directions);
short int choose_up (short int direction, int *value, bool *chose, unsigned short int up);
short int choose_right (short int direction, int *value, bool *chose, unsigned short int right);
short int choose_left (short int direction, int *value, bool *chose, unsigned short int left);
void move_to_direction (int direction, struct Coords *view_coords, struct Coords *arr_coords, bool run);
bool check_finish (struct Coords *arr_coords, bool finish);
bool finish (bool b_finish);
void write_coords(struct Coords *arr_coords);