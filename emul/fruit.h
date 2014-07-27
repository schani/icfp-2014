#ifndef	_FRUIT_H_
#define	_FRUIT_H_

#include "common.h"

typedef
struct fruit_ {
    pos_t pos;
    tick_t appear;
    tick_t expire;
} fruit_t;


static inline
void fruit_init(fruit_t *fruit, int index)
{
    fruit->pos.x = 0;
    fruit->pos.y = 0;

    fruit->appear = fruit_ticks(index, 0);
    fruit->expire = fruit_ticks(index, 1);
}

#endif
