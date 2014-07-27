#ifndef	_COMMON_H_
#define	_COMMON_H_

#include <stdint.h>
#include <stdio.h>

#include <unistd.h>
#include <stdlib.h>
#include <assert.h>


typedef
struct pos_ {
    uint8_t x;
    uint8_t y;
} pos_t;

typedef
enum dir_ {
    DIR_UP = 0,
    DIR_RIGHT = 1,
    DIR_DOWN = 2,
    DIR_LEFT = 3
} dir_t;

typedef
enum square_ {
    SQ_WALL = 0,
    SQ_EMPTY = 1,
    SQ_PILL = 2,
    SQ_POWER = 3,
    SQ_FRUIT = 4,
    SQ_LMAN = 5,
    SQ_LMAN2 = 6,
    SQ_GHOST = 7,
    SQ_GHOST2,
    SQ_GHOST3,
    SQ_GHOST4,
} square_t;

typedef uint32_t tick_t;


#define	min(a, b)	(((a) < (b)) ? (a) : (b))


static inline
square_t sq_dec(int c)
{
    switch (c) {
	case '#' : return SQ_WALL;
	case ' ' : return SQ_EMPTY;
	case '.' : return SQ_PILL;
	case 'o' : return SQ_POWER;
	case '%' : return SQ_FRUIT;
	case '\\': return SQ_LMAN;
	case '=' : return SQ_GHOST;
    }
    return -1;
}

static inline
int sq_enc(square_t v)
{
    switch (v) {
	case SQ_WALL   : return '#';
	case SQ_EMPTY  : return ' ';
	case SQ_PILL   : return '.';
	case SQ_POWER  : return 'o';
	case SQ_FRUIT  : return '%';
	case SQ_LMAN   : return '\\';
	case SQ_LMAN2  : return '\\';
	case SQ_GHOST  : return '=';
	case SQ_GHOST2 : return '=';
	case SQ_GHOST3 : return '=';
	case SQ_GHOST4 : return '=';
    }
    return '?';
}


/*	0	Reset all attributes
	1	Bright
	2	Dim
	4	Underscore
	5	Blink
	7	Reverse
	8	Hidden

		Colours
	30  40  Black
	31  41  Red
	32  42  Green
	33  43  Yellow
	34  44  Blue
	35  45  Magenta
	36  46  Cyan
	37  47  White		*/

static inline
int sq_ansi(square_t v, int *at)
{
    switch (v) {
	case SQ_WALL   : at[0] =  2; at[1] = 34; at[2] = 44; return '#';
	case SQ_EMPTY  : at[0] =  0; at[1] = -1; at[2] = -1; return ' ';
	case SQ_PILL   : at[0] = 37; at[1] = -1; at[2] = -1; return '.';
	case SQ_POWER  : at[0] = 31; at[1] =  1; at[2] = -1; return 'o';
	case SQ_FRUIT  : at[0] = 32; at[1] =  1; at[2] = -1; return '%';
	case SQ_LMAN   : at[0] = 33; at[1] =  1; at[2] = -1; return 'C';
	case SQ_LMAN2  : at[0] = 33; at[1] =  0; at[2] = -1; return 'D';
	case SQ_GHOST  : at[0] = 31; at[1] =  0; at[2] = -1; return '0';
	case SQ_GHOST2 : at[0] = 35; at[1] =  1; at[2] = -1; return '1';
	case SQ_GHOST3 : at[0] = 36; at[1] =  0; at[2] = -1; return '2';
	case SQ_GHOST4 : at[0] = 33; at[1] =  0; at[2] = -1; return '3';
    }
    at[0] = 31; at[1] = 40; at[2] = 5; return '?';
}

#endif
