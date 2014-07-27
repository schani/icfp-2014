#ifndef	_GHOST_H_
#define	_GHOST_H_

#include "common.h"
#include "ticks.h"

typedef
enum vit_ {
    VIT_STD = 0,
    VIT_FRIGHT = 1,
    VIT_INVIS = 2
} vit_t;

typedef
struct ghost_ {
    const char *id;

    /* GHC related */
    struct reg_ {
	uint8_t pc;

	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t f;
	uint8_t g;
	uint8_t h;
    } reg;
    uint8_t mem[256];

    /* Ghost related */
    uint8_t index;

    pos_t start;
    pos_t pos;
    dir_t dir;
    vit_t vit;

    /* Event related */
    dir_t new_dir;
    tick_t ticks;
} ghost_t;


static inline
void ghost_init(ghost_t *ghost, int index)
{
    ghost->reg.a = 0;
    ghost->reg.b = 0;
    ghost->reg.c = 0;
    ghost->reg.d = 0;
    ghost->reg.e = 0;
    ghost->reg.f = 0;
    ghost->reg.g = 0;
    ghost->reg.h = 0;

    for (int i=0; i<256; i++)
	ghost->mem[i] = 0;

    ghost->index = index;

    ghost->start.x = 0;
    ghost->start.y = 0;

    ghost->pos.x = 0;
    ghost->pos.y = 0;

    ghost->dir = DIR_DOWN;
    ghost->vit = VIT_STD;

    ghost->new_dir = DIR_DOWN;
    ghost->ticks = ghost_ticks(index, 0);
}


#endif
