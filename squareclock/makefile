all: squareclock_nums.h
	gcc -lm squareclock.c -o squareclock -O3

squareclock_nums.h: squareclock_numgen.sh
	./squareclock_numgen.sh
