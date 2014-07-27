#ifndef	_STATE_H_
#define	_STATE_H_

#include "common.h"

#include "map.h"
#include "lman.h"
#include "ghost.h"
#include "fruit.h"


typedef
struct event_ {
    uint32_t eol;
    uint32_t eof;

    uint32_t fright;
    uint32_t eating;
    uint32_t ghosts;
} event_t;


typedef
struct state_ {
    map_t map;

    lman_t lman[2];
    int lnum;

    ghost_t ghost[256];
    int gnum;

    fruit_t fruit[2];
    int fnum;

    event_t event;

    int pcnt;

    uint32_t ticks;
} state_t;


static inline
void state_init(state_t *state)
{
    map_init(&state->map);

    for (int i=0; i<2; i++)
	lman_init(&state->lman[i], i);
    for (int i=0; i<256; i++)
	ghost_init(&state->ghost[i], i);
    for (int i=0; i<2; i++)
	fruit_init(&state->fruit[i], i);

    state->ticks = 1;
}

#endif
