#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include "squareclock_nums.h"
#define true 1
#define false 0
#define ANSINL "\033[5A\033[7C"

#define MAX_BURN_HEIGHT 80
#define MAX_BURN_WIDTH 130

#define COOLNESS(a) (a < 1 ? (a<0?-50*a:1500) : 0)

char *text = "ALL YOUR BASE ARE BELONG TO US";
char nums[10][300] = { NUMS0, NUMS1, NUMS2, NUMS3, NUMS4, NUMS5, NUMS6, NUMS7, NUMS8, NUMS9 };

unsigned short state[MAX_BURN_HEIGHT][MAX_BURN_WIDTH];
unsigned short laststate[MAX_BURN_HEIGHT][MAX_BURN_WIDTH];
short delta[MAX_BURN_HEIGHT][MAX_BURN_WIDTH];

struct winsize ws;
int rows;
int cols;

void printNumber(int n) {
	fputs(nums[n], stdout);
	fputs(ANSINL, stdout);
}

void recurseDeltaExpand(int i, int b) {
	if(i == 0 || i == MAX_BURN_HEIGHT || b == 0 || b == MAX_BURN_WIDTH) return;
	if(delta[i][b] != 0) return;
	delta[i][b] = (rand() % 3) + 1;
	recurseDeltaExpand(i-1, b-1);
	recurseDeltaExpand(i-1, b);
	recurseDeltaExpand(i-1, b+1);
	recurseDeltaExpand(i, b-1);
	recurseDeltaExpand(i, b+1);
	recurseDeltaExpand(i+1, b-1);
	recurseDeltaExpand(i+1, b);
	recurseDeltaExpand(i+1, b+1);
}

char* repeatChar(int size, char c) {
	char *returnv = malloc(size + 1);
	int i;
	for(i = 0; i < size; i++)
		returnv[i] = c;
	returnv[size] = '\0';

	return returnv;
}

void printLabel(char* text) {
	fprintf(stdout, "\033[37;44m[----%s----]\033[0m", text);
}

void printSpaces(int size) {
	char* spaces = repeatChar(size, ' ');
	fputs("\033[34;40m", stdout);
	fputs(spaces, stdout);
	fputs("\033[0m", stdout);
	free(spaces);
}

void printLabels(int offset) {
	printSpaces(offset);
	printLabel("HH");
	printSpaces(2);
	printLabel("MM");
	printSpaces(2);
	printLabel("SS");
	printSpaces(offset);
}

void setTermSize() {
	ioctl(0, TIOCGWINSZ, &ws);
	rows = ws.ws_row;
	cols = ws.ws_col;
}

void printBackground() {

	puts("\033[34;48m");
	int i, b, heat, cool;

	for(i = 0; i < MAX_BURN_HEIGHT; i++)
	for(b = 0; b < MAX_BURN_WIDTH; b++)
		laststate[i][b] = state[i][b];

	for(i = 1; i < MAX_BURN_HEIGHT - 1 && i < rows - 1; i++) {
		for(b = 1; b < MAX_BURN_WIDTH - 1 && b < cols; b ++) {
			heat = laststate[i-1][b-1]
				+ laststate[i-1][b]
				+ laststate[i-1][b+1]
				+ laststate[i][b-1]
				+ laststate[i][b-1]
				+ laststate[i+1][b-1]
				+ laststate[i+1][b]
				+ laststate[i+1][b+1];

			cool = COOLNESS(delta[i-1][b-1])
				+ COOLNESS(delta[i-1][b])
				+ COOLNESS(delta[i-1][b+1])
				+ COOLNESS(delta[i][b-1])
				+ COOLNESS(delta[i][b-1])
				+ COOLNESS(delta[i+1][b-1])
				+ COOLNESS(delta[i+1][b])
				+ COOLNESS(delta[i+1][b+1])
				+ 2;

			heat = 1 + (heat*heat / 8);

			if(rand() < 100000 * heat * heat  * heat * heat * heat * heat || state[i][b] > 0) {
				state[i][b] += delta[i][b];
			}

			if(state[i][b] >= 30) {
				state[i][b] = 30;
				if(rand() < 10000 * cool * cool * cool * cool * cool * cool * cool * cool * cool) delta[i][b] = -delta[i][b];
			}

			if(state[i][b] <= 0) {
				state[i][b] = 0;
				if(delta[i][b] < 0) {
					delta[i][b] = 0;
					if(rand() < 500000000) recurseDeltaExpand(i,b);
				}
			}

			switch((int) state[i][b] / 6) {
				case 0: putchar('0'); break;
				case 1: putchar('['); break;
				case 2: putchar('?'); break;
				case 3: putchar('1'); break;
				case 4: putchar(':'); break;
				case 5: putchar(' '); break;
			}

		}
		if(i < MAX_BURN_HEIGHT - 2) putchar('\n');
	}
}

void header() {
	int offset = floor((cols - 40) / 2);
	int y;

	puts("\033[2J\033[0;0H");

	printLabels(offset);
	printBackground();

	fprintf(stdout, "\033[8;%dH", offset + 1);
}

void footer() {
	int offset = floor((cols - strlen(text)) / 2);
	fprintf(stdout, "\033[5;%dH%s", offset + 1, text);
	fprintf(stdout, "\033[%d;0H", rows);
	printLabels(floor((cols - 40) / 2));
}

void printTime() {
	time_t current;
	struct tm * localtm;
	current = time(NULL);
	localtm = localtime(&current);
	printNumber((int) floor(localtm->tm_hour / 10));
	printNumber(localtm->tm_hour % 10);
	printNumber((int) floor(localtm->tm_min / 10));
	printNumber(localtm->tm_min % 10);
	printNumber((int) floor(localtm->tm_sec / 10));
	printNumber(localtm->tm_sec % 10);
}

int main(int argc, char** argv) {
	int i, b;

	if(argc > 1) text = argv[1];

	srand(time(NULL));

	for(i = 0; i < MAX_BURN_HEIGHT; i++)
	for(b = 0; b < MAX_BURN_WIDTH; b++) {
		state[i][b] = 0;
		laststate[i][b] = 0;
		delta[i][b] = (rand() % 3) + 1;
	}

	while(true) {
		setTermSize();
		header();
		printTime();
		footer();
		fflush(stdout);
		usleep(80000);
	}
}
