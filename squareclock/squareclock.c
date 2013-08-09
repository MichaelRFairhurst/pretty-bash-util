#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include "squareclock_nums.h"
#define true 1
#define false 0
#define ANSINL "\033[5A\033[7C"

char nums[10][280] = { NUMS0, NUMS1, NUMS2, NUMS3, NUMS4, NUMS5, NUMS6, NUMS7, NUMS8, NUMS9 };

void printNumber(int n) {
	fputs(nums[n], stdout);
	fputs(ANSINL, stdout);
}

char* repeatChar(int size, char c) {
	char *returnv = malloc(size);
	int i;
	for(i = 0; i < size; i++)
		returnv[i] = c;
	returnv[size] = '\0';

	return returnv;
}

char* randOnesZeroes(int size) {
	struct timeval time;
	gettimeofday(&time, NULL);
	srand(time.tv_usec);
	char *returnv = malloc(size);
	int i;
	for(i = 0; i < size; i++)
	switch(rand() % 10) {
		case 0:
		case 1:
		case 2: returnv[i] = '0';break;
		case 3:
		case 4:
		case 5: returnv[i] = '1';break;
		case 6: returnv[i] = ' ';break;
		case 7: returnv[i] = '[';break;
		case 8: returnv[i] = ']';break;
		case 9: returnv[i] = '?';break;
	}
	returnv[size] = '\0';

	return returnv;
}

void printLabel(char* text) {
	fprintf(stdout, "\033[37;44m[----%s----]\033[0m", text);
}

void printDashes(int size) {
	char* zeroes = randOnesZeroes(size);
	fputs("\033[34;40m", stdout);
	fputs(zeroes, stdout);
	fputs("\033[0m", stdout);
	free(zeroes);
}

void printLabels(int offset) {
	printDashes(offset);
	printLabel("HH");
	printDashes(2);
	printLabel("MM");
	printDashes(2);
	printLabel("SS");
	printDashes(offset);
}

	struct winsize ws;
	int rows;
	int cols;

void setTermSize() {
	ioctl(0, TIOCGWINSZ, &ws);
	rows = ws.ws_row;
	cols = ws.ws_col;
}

void header() {
	int offset = floor((cols - 40) / 2);
	int y;

	puts("\033[2J\033[0;0H --- \033[1;33mNuclear countdown commencing...\033[0m\n");

	printLabels(offset);
	for(y = 4; y < 9; y++)
		printDashes(cols);

	printLabels(offset);

	fprintf(stdout, "\033[4;%dH", offset + 1);
}

void footer() {
	char* xes = repeatChar(cols, 'X');
	int y;
	fputs("\033[10;0H", stdout);
	for(y = 10; y < 14; y++) puts(xes);
	printDashes(floor((cols - 28) / 2));
	fputs("\033[31mALL YOUR BASE R BELONG TO US", stdout);
	printDashes(floor((cols - 28) / 2));
	for(y = 16; y <= rows; y++) puts(xes);
	fputs(xes, stdout);
	fputs("\033[0;0H", stdout);
	free(xes);
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

int main() {
	while(true) {
		setTermSize();
		header();
		printTime();
		footer();
		fflush(stdout);
		usleep(280000);
	}
}
