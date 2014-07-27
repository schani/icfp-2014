
#include "common.h"
#include "state.h"
#include "ticks.h"
#include "score.h"
#include "lmc.h"
#include "lmc_dump.h"

static state_t state;

void step(lmc_t *lmc) {
    return;
}

void scan_map(map_t *map, state_t *state)
{
    uint8_t gnum = 0;
    uint8_t lnum = 0;

    for (int y=0; y<map->h; y++) {
	for (int x=0; x<map->w; x++) {
	    if (map->data[x][y] == SQ_GHOST) {
		state->ghost[gnum].start.x = x;
		state->ghost[gnum].start.y = y;
		state->ghost[gnum].pos.x = x;
		state->ghost[gnum].pos.y = y;
		state->ghost[gnum].index = gnum;
		map->data[x][y] = SQ_EMPTY;
		gnum++;
	    } else if (map->data[x][y] == SQ_LMAN) {
		state->lman[lnum].start.x = x;
		state->lman[lnum].start.y = y;
		state->lman[lnum].pos.x = x;
		state->lman[lnum].pos.y = y;
		map->data[x][y] = SQ_EMPTY;
		lnum++;
	    } else if (map->data[x][y] == SQ_FRUIT) {
		state->fruit[0].pos.x = x;
		state->fruit[0].pos.y = y;
		state->fruit[1].pos.x = x;
		state->fruit[1].pos.y = y;
		map->data[x][y] = SQ_EMPTY;
	    } else if (map->data[x][y] == SQ_PILL) {
		state->pcnt++;
	    } else if (map->data[x][y] == SQ_POWER) {
	    }
	}
    }

    state->gnum = gnum;
    state->lnum = lnum;
    state->fnum = 2;
}


void class_map(map_t *map)
{
    for (int y=0; y<map->h; y++) {
	for (int x=0; x<map->w; x++) {
	    if (map_data(map, x, y) != SQ_WALL) {
		int exits = 0;

		if (map_data(map, x, y-1) != SQ_WALL) {
		    map->class[x][y] |= (1 << 0);
		    exits++;
		}
		if (map_data(map, x+1, y) != SQ_WALL) {
		    map->class[x][y] |= (1 << 1);
		    exits++;
		}
		if (map_data(map, x, y+1) != SQ_WALL) {
		    map->class[x][y] |= (1 << 2);
		    exits++;
		}
		if (map_data(map, x-1, y) != SQ_WALL) {
		    map->class[x][y] |= (1 << 3);
		    exits++;
		}

		if (exits >= 3)
		    map->class[x][y] |= (1 << 4);
	    }
	}
    }
}


void prep_event(event_t *event, state_t *state)
{
    event->eol = eol_ticks(state->map.w, state->map.h);
}




void move(pos_t *pos, dir_t dir)
{
    switch (dir) {
	case DIR_UP:	pos->y--; break;
	case DIR_RIGHT:	pos->x++; break;
	case DIR_DOWN:	pos->y++; break;
	case DIR_LEFT:	pos->x--; break;
    }
}


void ghost_reset(state_t *state, ghost_t *ghost, vit_t vit)
{
    /* remove ghost */
    state->map.over[ghost->pos.x][ghost->pos.y] = SQ_EMPTY;

    ghost->vit = vit;
    ghost->dir = DIR_DOWN;
    ghost->pos = ghost->start;
}


void lman_die(state_t *state, lman_t *lman)
{
    for (int i=0; i<state->gnum; i++)
	ghost_reset(state, &state->ghost[i], VIT_STD);

    /* remove lman */
    state->map.over[lman->pos.x][lman->pos.y] = SQ_EMPTY;

    lman->dir = DIR_DOWN;
    lman->pos = lman->start;
    lman->lives--;
}



void exec_ghost0(state_t *state, ghost_t *ghost);
void exec_ghost1(state_t *state, ghost_t *ghost);
void exec_ghost2(state_t *state, ghost_t *ghost);
void exec_ghost3(state_t *state, ghost_t *ghost);

void exec_ghost(state_t *state, int index)
{
    fprintf(stderr,
	"%10d:\texec_ghost[%d] ...\n",
	state->ticks, index);
    switch (index % 4) {
	case 0: return exec_ghost0(state, &state->ghost[index]);
	case 1: return exec_ghost1(state, &state->ghost[index]);
	case 2: return exec_ghost2(state, &state->ghost[index]);
	case 3: return exec_ghost3(state, &state->ghost[index]);
    }
}




void move_ghost(state_t *state, int index)
{
    ghost_t *ghost = &state->ghost[index];
    pos_t pos = ghost->pos;
    dir_t dir = ghost->dir;
    uint8_t class = map_class(&state->map, pos.x, pos.y);

    if (class & (1 << 4)) {			/* junction */
	dir_t new_dir = ghost->new_dir;
	dir_t inv = (dir + 2) % 4;

	if ((new_dir != inv) &&
	    (class & (1 << new_dir))) {		/* legal */
	    dir = new_dir;
	} else if (class & (1 << dir)) {	/* continue */
	    dir = dir;
	} else {
	    for (int d=0; d<4; d++) {
		if ((d != inv) &&
		    (class & (1 << d))) {
		    dir = d;
		    break;
		}
	    }
	}
    } else {					/* no junction */
	if (class & (1 << dir)) {		/* continue */
	    dir = dir;
	} else {
	    dir_t ccw = (dir - 1) % 4;
	    dir_t cw = (dir + 1) % 4;
	    dir_t inv = (dir + 2) % 4;

	    if (class & (1 << cw))		/* turn right */
		dir = cw;
	    else if (class & (1 << ccw))	/* turn left */
		dir = ccw;
	    else if (class & (1 << inv))	/* turn back */
		dir = inv;
	    else
		exit(3);
	}
    }

    /* remove ghost */
    state->map.over[pos.x][pos.y] = SQ_EMPTY;
    move(&ghost->pos, dir);
    ghost->dir = dir;
}



void exec_lmc(lmc_t *lmc);

void init_lman(state_t *state, int index)
{
    frame_p env = ALLOC_FRAME(2, NULL);
    FRAME_VALUE(env, 0) = VAL_INT(-1);

    lmc_t lmc = { 
	.limit = 3072*1000*60,
	.step = 1,
	.e = env,
	.d = ALLOC_VAL(TAG_STOP) };

    fprintf(stderr, "\nExecuting LMC main() ...\n");
    exec_lmc(&lmc);

    val_t cons = POPF(&lmc.s);
    if (TAG(cons) != TAG_CONS) {
	fprintf(stderr, "main() must return CONS");
	return;
    }

    dump_valp("MAIN:", &cons, 0);

    state->ai = *CARF(cons);
    state->step = *CDRF(cons);
    if (TAG(state->step) != TAG_CLOSURE) {
	fprintf(stderr, "main() must return CONS(?, CLOSURE)");
	return;
    }
}

void exec_lman(state_t *state, int index)
{
    fprintf(stderr,
	"%10d:\texec_lman[%d] ...\n",
	state->ticks, index);

    FREE_STACK(&state->lmc.s);
    FREE_STACK(&state->lmc.d);
    // FREE_ENV(&state->lmc.e);

    state->lmc.c = CAR_CLOSURE(state->step);
    state->lmc.e = CDR_CLOSURE(state->step);

    frame_p env = ALLOC_FRAME(2, state->lmc.e);
    FRAME_VALUE(env, 0) = state->ai;
    FRAME_VALUE(env, 1) = VAL_INT(-1);

    state->lmc.e = env;
    state->lmc.d = ALLOC_VAL(TAG_STOP);

    state->lmc.fault = 0;
    state->lmc.icnt = 0;
    state->lmc.limit = 3072*1000;

    fprintf(stderr, "\nExecuting LMC step() ...\n");
    exec_lmc(&state->lmc);

    val_t cons = POPF(&state->lmc.s);
    if (TAG(cons) != TAG_CONS) {
	fprintf(stderr, "step() must return CONS");
	return;
    }

    dump_valp("STEP", &cons, 0);

    state->ai = *CARF(cons);
    state->lman[index].dir = NUM(*CDRF(cons));
}


















void move_lman(state_t *state, int index)
{
    lman_t *lman = &state->lman[index];
    pos_t pos = lman->pos;
    dir_t dir = lman->dir;
    uint8_t class = map_class(&state->map, pos.x, pos.y);

    /* remove lman */
    state->map.over[pos.x][pos.y] = SQ_EMPTY;

    if (class & (1 << dir))				/* possible? */
	move(&lman->pos, dir);

    switch (map_data(&state->map, lman->pos.x, lman->pos.y)) {
	case SQ_PILL:
	case SQ_POWER:
	case SQ_FRUIT:
	    state->event.eating = 1;
	    break;

	default:
	    state->event.eating = 0;
    }
}
















tick_t mech_emove(state_t *state, tick_t this)
{
    tick_t next = ~0;

    /* advance ticks and check for move */
    for (int i=0; i<state->gnum; i++) {
	state->ghost[i].ticks -= this;

	if (state->ghost[i].ticks == 0) {
	    exec_ghost(state, i);			/* execute */
	    move_ghost(state, i);			/* move */
	}

	if (state->ghost[i].ticks == 0)			/* update */
	    state->ghost[i].ticks =
		ghost_ticks(i, state->event.fright);

	if (state->ghost[i].ticks < next)		/* next event */
	    next = state->ghost[i].ticks;
    }

    /* advance ticks and check for move */
    for (int i=0; i<state->lnum; i++) {
	state->lman[i].ticks -= this;

	if (state->lman[i].ticks == 0) {
	    exec_lman(state, i);			/* execute */
	    move_lman(state, i);			/* move */
	}

	if (state->lman[i].ticks == 0)			/* execute */
	    state->lman[i].ticks =
		lman_ticks(i, state->event.eating);

	if (state->lman[i].ticks < next)		/* next event */
	    next = state->lman[i].ticks;
    }

    return next;
}

tick_t mech_actions(state_t *state, tick_t this)
{
    tick_t next = ~0;

    /* advance ticks */
    if (state->event.fright > 0) {
	state->event.fright -= this;

	if (state->event.fright == 0) {

	} else {
	    if (state->event.fright < next)
		next = state->event.fright;
	}
    }

    /* advance ticks and check for fruit */
    for (int i=0; i<state->fnum; i++) {
	if (state->fruit[i].appear > 0) {
	    state->fruit[i].appear -= this;

	    if (state->fruit[i].appear == 0) {
		pos_t pos = state->fruit[i].pos;

		/* add fruit */
		state->map.data[pos.x][pos.y] = SQ_FRUIT;
	    } else {
		if (state->fruit[i].appear < next)
		    next = state->fruit[i].appear;
	    }
	}

	if (state->fruit[i].expire > 0) {
	    state->fruit[i].expire -= this;

	    if (state->fruit[i].expire == 0) {
		pos_t pos = state->fruit[i].pos;

		/* remove fruit */
		state->map.data[pos.x][pos.y] = SQ_EMPTY;
	    } else {
		if (state->fruit[i].expire < next)
		    next = state->fruit[i].expire;
	    }
	}
    }
    return next;
}

void mech_lman(state_t *state)
{
    for (int i=0; i<state->lnum; i++) {
	pos_t pos = state->lman[i].pos;

	switch (map_data(&state->map, pos.x, pos.y)) {
	    case SQ_PILL:
		state->lman[i].score += 10;
		state->pcnt--;
		break;

	    case SQ_POWER:
		state->event.fright = fright_ticks();
		state->event.ghosts = 0;
		state->lman[i].score += 50;
		break;

	    case SQ_FRUIT:
		state->lman[i].score += fruit_score(&state->map);
		break;
	}

	state->map.data[pos.x][pos.y] = SQ_EMPTY;
    }
}


void mech_ghost(state_t *state)
{
    for (int i=0; i<state->gnum; i++) {
	pos_t pos = state->ghost[i].pos;

	for (int j=0; j<state->lnum; j++) {
	    if ((pos.x == state->lman[j].pos.x) &&
		(pos.y == state->lman[j].pos.y)) {
		if (state->event.fright > 0) {
		    ghost_reset(state, &state->ghost[i], VIT_INVIS);
		    state->event.ghosts++;
		    state->lman[i].score +=
			ghost_score(state->event.ghosts);

		} else {
		    lman_die(state, &state->lman[j]);
		}
	    }
	}
    }
}


void mech_over(state_t *state)
{
    for (int i=0; i<state->lnum; i++) {
	pos_t pos = state->lman[i].pos;

	/* add lman */
	state->map.over[pos.x][pos.y] = SQ_LMAN + i;
    }

    for (int i=0; i<state->gnum; i++) {
	pos_t pos = state->ghost[i].pos;

	/* add ghost */
	state->map.over[pos.x][pos.y] = SQ_GHOST + (i % 4);
    }
}




int mech_win(state_t *state)
{
    return (state->pcnt == 0);
}

int mech_lose(state_t *state)
{
    return (state->lman[0].lives == 0);
}


int main(int argc, char *argv[])
{
    state_init(&state);
    fprintf(stdout, "\e[2J");

    FILE *mapfd = fopen(argv[1], "r");

    map_load(&state.map, mapfd);
    fprintf(stderr,
	"map %dx%d loaded.\n",
	state.map.w, state.map.h);

    scan_map(&state.map, &state);
    class_map(&state.map);

    prep_event(&state.event, &state);

    init_lman(&state, 0);

    tick_t this = 0;

    while (state.ticks < state.event.eol) {
	tick_t next, nact;
	int win, lose;

	next = mech_emove(&state, this);
	nact = mech_actions(&state, this);

	mech_lman(&state);
	mech_ghost(&state);
	mech_over(&state);

	win = mech_win(&state);
	lose = mech_lose(&state);

	state.ticks += this;
	this = min(next, nact);
	// fprintf(stderr, "next=%d, nact=%d\n", next, nact);
	

	fprintf(stdout, "\e[0;0H");
	map_dump_ansi(&state.map, stdout);
	
	fprintf(stdout,
	    "\nScore: %d Lives: %d Ticks: %d\t%d/%3d\n",
	    state.lman[0].score, state.lman[0].lives, state.ticks,
	    state.event.eating, state.event.fright);

	for (int i=0; i<4; i++) {
	    pos_t pos = state.ghost[i].pos;
	    fprintf(stdout,
		"ghost%d (%2d,%2d,%2x) [%d/%d] ticks=%3d\n",
		i, pos.x, pos.y, state.map.class[pos.x][pos.y],
		state.ghost[i].dir, state.ghost[i].new_dir,
		state.ghost[i].ticks);
	}

	if (win) {
	    fprintf(stdout, "win\n");
	    exit(0);
	}

	if (lose) {
	    fprintf(stdout, "lose\n");
	    exit(1);
	}

	getchar();
    }
    exit(2);
}
