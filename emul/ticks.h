#ifndef	_TICKS_H_
#define	_TICKS_H_

#include "common.h"

static inline
tick_t eol_ticks(int width, int height)
{
    return 127 * width * height * 16;
}

static inline
tick_t lman_ticks(int index, int eating)
{
    return eating ? 137 : 127;
}

static inline
tick_t ghost_ticks(int index, int fright)
{
    const uint32_t nticks[4] = { 130, 132, 134, 136 };
    const uint32_t fticks[4] = { 195, 198, 201, 204 };

    return fright ? fticks[index % 4] : nticks[index % 4];
}

static inline
tick_t fruit_ticks(int index, int expire)
{
    const uint32_t aticks[2] = { 127 * 200, 127 * 400 };
    const uint32_t eticks[2] = { 127 * 280, 127 * 480 };

    return expire ? eticks[index % 2] : aticks[index % 2];
}

static inline
tick_t fright_ticks(void)
{
    return 127 * 20;
}


#endif
