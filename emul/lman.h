#ifndef	_LMAN_H_
#define	_LMAN_H_

#include "common.h"
#include "ticks.h"

typedef
struct lman_ {
    pos_t start;
    pos_t pos;
    dir_t dir;

    int32_t vit;

    int32_t lives;
    int32_t score;

    /* Event related */
    tick_t ticks;
} lman_t;


static inline
void lman_init(lman_t *lman, int index)
{
    lman->pos.x = 0;
    lman->pos.y = 0;

    lman->dir = DIR_DOWN;

    lman->vit = 0;

    lman->lives = 3;
    lman->score = 0;

    lman->ticks = lman_ticks(index, 0);
}


#endif
