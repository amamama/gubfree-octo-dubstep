#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define field_index 4


enum _DIR {
	ANNUL = -1,
	LEFT = 0,
	DOWN,
	RIGHT,
	UP,
};

int R90deg(int *x, int *y) {
	int loc_x = *x,loc_y = *y;
	*x = -loc_y;
	*y = loc_x;
	return 0;
}

int rotate (int field[][field_index]) {
	int tmp = 0, offset = (field_index * 2 - 1) / 2, step = 2;
	for (int  i = 0, x = 0, ox = 0;i < step * field_index / 2;i+=step) {
		for (int j = 0, y = 0, oy = 0;j < step * field_index / 2;j+=step) {
				x = i - offset, y = j - offset;
			tmp = field[(x + offset)/step][(y + offset)/step];
			ox = x, oy = y;
			R90deg(&x, &y);
			field[(ox + offset)/step][(oy + offset)/step] = field[(x + offset)/step][(y + offset)/step];
			ox = x, oy = y;
			R90deg(&x, &y);
			field[(ox + offset)/step][(oy + offset)/step] = field[(x + offset)/step][(y + offset)/step];
			ox = x, oy = y;
			R90deg(&x, &y);
			field[(ox + offset)/step][(oy + offset)/step] = field[(x + offset)/step][(y + offset)/step];
			field[(x + offset)/step][(y + offset)/step] = tmp;
		}
	}

	return 0;
}

int shift (int field[][field_index], enum _DIR dir) {
	for (int i = 0;i < dir;i++) {
		rotate(field);
	}

	int flag = 1,cnt = -1, doubled[field_index][field_index] = {{0}};
	for (;flag;cnt++) {
		flag = 0;
		for (int i = 0;i < field_index;i++) {
			for ( int j = 1;j < field_index;j++) {
				if (field[i][j] != 0 && field[i][j - 1] == 0) {
					field[i][j - 1] = field[i][j];
					field[i][j] = 0;
					flag = 1;
				}
			}
		}
		for (int i = 0;i < field_index;i++) {
			for ( int j = 0;j < field_index - 1;j++) {
				if (field[i][j] != 0 && doubled[i][j] == 0 && doubled[i][j + 1] == 0 && field[i][j] == field[i][j + 1]) {
					field[i][j] += field[i] [j + 1];
					field[i][j + 1] = 0;
					doubled[i][j] = 1;
					flag = 1;
				}
			}
		}
	}
	for (int i = 0;i < (4 - dir)%4;i++) {
		rotate(field);
	}
	return cnt;
}

int search_recursion (int field[][field_index], int x, int y, int num, int flag[][field_index]) {
	if (x < 0 || field_index <= x || y < 0 || field_index <= y) {
		return 0;
	}
	if (flag[y][x] == 1){
		return 0;
	} else if (field[y][x] == 0) {
		field[y][x] = num;
		return 1;
	} else {
		flag[y][x] = 1;
		return search_recursion(field, x - 1, y - 1, num, flag) ||
		search_recursion(field, x - 1, y + 1, num, flag) ||
		search_recursion(field, x + 1, y - 1, num, flag) ||
		search_recursion(field, x + 1, y + 1, num, flag);
	}

	return 0;
}

int update (int field[][field_index], enum _DIR dir) {
	if (!shift(field, dir)) {
		return 0;
	}
	int x = (rand() >> 4) % field_index, y = (rand() >> 4) % field_index, num = (rand() >> 4) % 4 ? 2 : 4;
	int flag[field_index][field_index] = {{0}};
	search_recursion(field, x, y, num, flag);
	return 1;
}

int init (int field[][field_index]) {
	for (int i = 0;i < field_index;i++) { 
		for (int j = 0;j < field_index;j++) { 
			field[i][j] = 0;
		}
	}
	int x = ((rand() >> 4) % (field_index - 1)) + 1, y = (rand() >> 4) % field_index, num = (rand() >> 4) % 4 ? 2 : 4;
	field[y][x] = num;
	update(field, LEFT);
	return 0;
}

int count_bits(unsigned long i) {
	i = (i & 0x55555555) + (i >> 1 & 0x55555555);
	i = (i & 0x33333333) + (i >> 2 & 0x33333333);
	i = (i & 0x0f0f0f0f) + (i >> 4 & 0x0f0f0f0f);
	i = (i & 0x00ff00ff) + (i >> 8 & 0x00ff00ff);
	return (i & 0x0000ffff) + (i >> 16 & 0x0000ffff);
}


int NTZ(unsigned long x) {
	return count_bits((x & -x) -1);
}

int draw (int field[][field_index]) {
	char *tile[] = {
		"       ",
		"  % 4d ",
		"       ",
	};
	char *waku[] = {
		"       ",
		"       ",
		"       ",
	};
	short colors = 0, color[] = {
		235,225,226,215,216,210,209,222,223,224,220,221,59,59,
	};
	for (int i = 0;i < field_index;i++) { 
		for (int j = 0;j < field_index;j++) { 
			colors = NTZ((unsigned long)field[i][j]);
			attrset(COLOR_PAIR(color[colors==32?0:colors]));
			for (int k = 0; k < (int)(sizeof(waku)/sizeof(waku[0]));k++) {
				mvprintw(i * (int)(sizeof(waku)/sizeof(waku[0])) + k, j * strlen(waku[0]), field[i][j]?tile[k]:waku[k], field[i][j]);
			}
			standend();
		}
	}
	return 0;
}

int isgameover (int field[][field_index]) {
	for (int rot = 0;rot < 4;rot++) {
		for (int i = 0;i < field_index;i++) {
			for (int j = 0; j < field_index;j++) {
				if (field[i][j] == 0 || (j < field_index - 1 && field[i][j] == field[i][j + 1])) {
					return 0;
				}
			}
		}
		rotate(field);
	}
	rotate(field);
	return 1;
}

enum _DIR choose_dir(int i) {
	enum _DIR dir = LEFT;
	switch (i) {
		case 'h' :
			dir = LEFT;
			break;
		case 'j' :
			dir = DOWN;
			break;
		case 'k' :
			dir = UP;
			break;
		case 'l' :
			dir = RIGHT;
			break;
		default :
			dir = ANNUL;
			break;
	}
	return dir;
}


int game (void) {
	int field[field_index][field_index] = {{0}};
	int restart = 0, c = 0;
	enum _DIR dir = LEFT;
	for (init(field),draw(field);!isgameover(field) && restart == 0;draw(field)){
		c = getch();
		if (c == 'r') {
			restart = 1;
		} else if (c == 'q') {
			return 0;
		}

		dir = choose_dir(c);
		if (dir == ANNUL) {
			continue;
		}
		update(field, dir);
	}
	return 1;
}

int register_colors(void) {
	if (!can_change_color()) {
		return 0;
	}
	start_color();
	int colors = 16;
	for (int r = (1000 % 5); r <= 1000; r += (1000 / 5)) {
		for (int g = (1000 % 5); g <= 1000; g += (1000 / 5)) {
			for (int b = (1000 % 5); b <= 1000; b += (1000 / 5)) {
				init_color(colors, r, g, b);
				colors++;
				}
			}
		}
	for (int i = 0; i <= 1000; i += (1000 / 23)) {
		init_color(colors, i, i, i);
		colors++;
	}
	return 1;
}

int register_pair(void) {
	for (int i = 0;i < 256;i++) {
		init_pair(i, 0, i);
		attrset(COLOR_PAIR(i));
	}
	return 0;
}

int main (//int argc, char *argv[]) {
void) {
	srand(9);
	initscr();
	cbreak();
	noecho();
	register_colors();
	register_pair();
	for (;game();) {
		mvprintw(6, 6, "Game Over");
		getch();
	}
	endwin();
	return 0;
}
