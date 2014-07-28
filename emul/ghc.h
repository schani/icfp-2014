#ifndef	_GHC_H_
#define	_GHC_H_

#include "common.h"

#include "state.h"
#include "ghost.h"


static inline
void int_0(state_t *state, ghost_t *ghost)
{
    ghost->new_dir = ghost->reg.a;
}

static inline
void int_1(state_t *state, ghost_t *ghost)
{
    ghost->reg.a = state->lman[0].pos.x;
    ghost->reg.b = state->lman[0].pos.y;
}

static inline
void int_2(state_t *state, ghost_t *ghost)
{
    ghost->reg.a = state->lman[1].pos.x;
    ghost->reg.b = state->lman[1].pos.y;
}

static inline
void int_3(state_t *state, ghost_t *ghost)
{
    ghost->reg.a = ghost->index;
}

static inline
void int_4(state_t *state, ghost_t *ghost)
{
    uint8_t idx = ghost->reg.a;
    if (idx < state->gnum) {
	ghost->reg.a = state->ghost[idx].start.x;
	ghost->reg.b = state->ghost[idx].start.y;
    }
}

static inline
void int_5(state_t *state, ghost_t *ghost)
{
    uint8_t idx = ghost->reg.a;
    if (idx < state->gnum) {
	ghost->reg.a = state->ghost[idx].pos.x;
	ghost->reg.b = state->ghost[idx].pos.y;
    }
}

static inline
void int_6(state_t *state, ghost_t *ghost)
{
    uint8_t idx = ghost->reg.a;
    if (idx < state->gnum) {
	ghost->reg.a = state->ghost[idx].vit;
	ghost->reg.b = state->ghost[idx].dir;
    }
}

static inline
void int_7(state_t *state, ghost_t *ghost)
{
    uint8_t x = ghost->reg.a;
    uint8_t y = ghost->reg.b;
    ghost->reg.a = map_data(&state->map, x, y);
}

static inline
void int_8(state_t *state, ghost_t *ghost)
{
    fprintf(stderr,
	"trace %s: %d %d %d %d %d %d %d %d %d\n",
	ghost->id, ghost->reg.pc,
	ghost->reg.a, ghost->reg.b, ghost->reg.c, ghost->reg.d,
	ghost->reg.e, ghost->reg.f, ghost->reg.g, ghost->reg.h);
    return;
}

#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-label"

#endif
