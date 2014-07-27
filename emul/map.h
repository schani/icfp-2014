#ifndef	_MAP_H_
#define	_MAP_H_

#include "common.h"

typedef
struct map_ {
    uint16_t w;
    uint16_t h;
    square_t data[256][256];
    square_t over[256][256];
    uint8_t class[256][256];
} map_t;


static inline
uint8_t map_data(map_t *map, uint8_t x, uint8_t y)
{
    if ((x < map->w) && (y < map->h))
	return map->data[x][y];
    else
	return SQ_WALL;
}

static inline
uint8_t map_over(map_t *map, uint8_t x, uint8_t y)
{
    if ((x < map->w) && (y < map->h))
	return map->over[x][y];
    else
	return SQ_EMPTY;
}

static inline
uint8_t map_class(map_t *map, uint8_t x, uint8_t y)
{
    if ((x < map->w) && (y < map->h))
	return map->class[x][y];
    else
	return 0;
}


static inline
void map_init(map_t *map)
{
    map->w = 0;
    map->h = 0;

    for (int x=0; x<256; x++) {
	for (int y=0; y<256; y++) {
	    map->data[x][y] = SQ_WALL;
	    map->over[x][y] = SQ_EMPTY;
	    map->class[x][y] = 0;
	}
    }
}

static inline
void map_load(map_t *map, FILE *file)
{
    uint8_t x = 0, y = 0;
    int c;

    while ((c = getc(file)) != EOF) {
	if (c == '\n') {
	    if (x > map->w)
		map->w = x;
	    x = 0; y++;
	    if (y > map->h)
		map->h = y;
	} else {
	    map->data[x][y] = sq_dec(c);
	    x++;
	}
    }
}


static inline
void map_dump(map_t *map, FILE *file)
{
    for (int y=0; y<map->h; y++) {
	for (int x=0; x<map->w; x++) {
	    putc(sq_enc(map->data[x][y]), file);
	}
	putc('\n', file);
    }
}

static inline
void map_dump_ansi(map_t *map, FILE *file)
{

    for (int y=0; y<map->h; y++) {
	int at[3] = { 0, 0, 0 };
	for (int x=0; x<map->w; x++) {
	    int nat[3] = { at[0], at[1], at[2] };
	    int o = map_over(map, x, y);
	    int c = sq_ansi((o == SQ_EMPTY) ? map_data(map, x, y) : o, nat);

	    if ((nat[0] != at[0]) || (nat[1] != at[1]) || (nat[2] != at[2])) {
		fputs("\e[0", file);
		for (int i=0; i<3; i++) {
		    if (nat[i] > 0)
			fprintf(file, ";%d", nat[i]);
		    at[i] = nat[i];
		}
		putc('m', file);
	    }

	    /* FIXME: conditional?
	    if (map_class(map, x, y))
		c = '0' + map_class(map, x, y); */

	    putc(c, file);
	}
	fputs("\e[0m\n", file);
    }
}


#endif
