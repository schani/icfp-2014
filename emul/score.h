#ifndef	_SCORE_H_
#define	_SCORE_H_

#include "common.h"

static inline
uint32_t fruit_score(map_t *map)
{
    uint32_t size = map->w * map->h;
    uint32_t lvl = size / 100;

    switch (lvl) {
	case 1:  return 100;
	case 2:  return 300;
	case 3:
	case 4:  return 500;
	case 5:
	case 6:  return 700;
	case 7:
	case 8:  return 1000;
	case 9:
	case 10: return 2000;
	case 11:
	case 12: return 3000;
    }
    return 5000;
}

static inline
uint32_t ghost_score(int ghosts)
{
    switch (ghosts) {
	case 1:  return 200;
	case 2:  return 400;
	case 3:  return 800;
    }
    return 1600;
}

#endif
