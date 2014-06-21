#include <stdio.h>
#include <stdlib.h>

#define field_index 4


enum _DIR {
	LEFT,
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

int move (int field[][field_index], enum _DIR dir) {
	for (unsigned int i = 0;i < dir;i++) {
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
			for ( int j = 0;j < field_index;j++) {
				if (field[i][j] != 0 && doubled[i][j] == 0 && doubled[i][j + 1] == 0 && field[i][j] == field[i][j + 1]) {
					field[i][j] += field[i] [j + 1];
					field[i][j + 1] = 0;
					doubled[i][j] = 1;
					flag = 1;
				}
			}
		}
	}
	for (unsigned int i = 0;i < (4 - dir)%4;i++) {
		rotate(field);
	}
	return cnt;
}

int search_recursion (int field[][field_index], int x, int y, int num, int flag[][field_index]) {
	if (x < 0 || field_index <= x || y < 0 || field_index <= y) {
		return 0;
	}
	if (field[y][x] == 0) {
		field[y][x] = num;
	} else if (flag[y][x] == 1){
		return 0;
	} else {
		flag[y][x] = 1;
		search_recursion(field, x - 1, y - 1, num, flag);
		search_recursion(field, x - 1, y + 1, num, flag);
		search_recursion(field, x + 1, y - 1, num, flag);
		search_recursion(field, x + 1, y + 1, num, flag);
	}

	return 0;
}

int update (int field[][field_index], enum _DIR dir) {
	if (!move(field, dir)) {
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
	int x = (rand() >> 4) % field_index, y = (rand() >> 4) % field_index, num = (rand() >> 4) % 4 ? 2 : 4;
	field[y][x] = num;
	update(field, LEFT);
	return 0;
}

int draw (int field[][field_index]) {
	for (int i = 0;i < field_index;i++) { 
		for (int j = 0;j < field_index;j++) { 
			printf("% 4d",field[i][j]);
		}
		puts("");
	}
	puts("");
	return 0;
}

int isgameover (int field[][field_index]) {
	for (int rot = 0;rot < 4;rot++) {
		for (int i = 0;i < field_index;i++) {
			for (int j = 0; j < field_index;j++) {
				if (field[i][j] == 0 || field[i][j] == field[i][j + 1]) {
					return 1;
				}
			}
		}
	}
	return 0;
}

int game (void) {
	int field[field_index][field_index] = {{0}};
	int quit = 0;
	enum _DIR dir = LEFT;
	for (init(field),draw(field);isgameover(field) && quit == 0;draw(field)){
		switch (getchar()) {
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
			case 'q' :
				quit = 1;
			default :
				continue;
				break;
		}
		update(field, dir);
	}
	return 0;
}

int main (int argc, char *argv[]) {
	srand(0);
	for (;argc--;argv[0][0] = 65) {
		game();
	}
	return 0;
}
