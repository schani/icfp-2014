#ifndef	_LMC_DUMP_H_
#define	_LMC_DUMP_H_

#include "common.h"


static inline
void dump_valp(const char *msg, val_p val, int mode)
{
    fprintf(stderr, "%s", msg);
    switch (val->tag) {
	case TAG_EMPTY:
	    fprintf(stderr, "<EMPTY>");
	    break;
	
	case TAG_INT:
	    fprintf(stderr, "%d", val->num);
	    break;

	case TAG_CONS:
	    dump_valp("(", val->cons.left, mode);
	    dump_valp(", ", val->cons.right, mode);
	    fprintf(stderr, ")");
	    break;

	case TAG_CLOSURE:
	    fprintf(stderr, "{%d, %p}",
		val->closure.addr, val->closure.env);
	    break;

	/* control stack */

	case TAG_ADDR:
	    fprintf(stderr, "@%d", val->addr);
	    break;

	case TAG_RET:
	    fprintf(stderr, "!RET!");
	    break;

	case TAG_JOIN:
	    fprintf(stderr, "!JOIN!");
	    break;

	case TAG_STOP:
	    fprintf(stderr, "!STOP!");
	    break;

	/* frames */

	case TAG_DUM:
	    fprintf(stderr, "?DUM?");
	    break;

	case TAG_FRAME:
	    fprintf(stderr, "?FRAME?");
	    break;

	default:
	    fprintf(stderr, "????");
    }
    if (mode == 1)
	fprintf(stderr, "|%p/%p", val, val->next);
}

static inline
void dump_state(lmc_t *lmc)
{
    fprintf(stderr,
	"STATE %%s=%p, %%d=%p, %%e=%p, %%c=%d\n",
	lmc->s, lmc->d, lmc->e, lmc->c);
}

static inline
void dump_ctrl(lmc_t *lmc, int mode)
{
    char str[256];
    for (val_p this = lmc->d; this; this = this->next) {
	sprintf(str, "\nC[%p]\t", this);
	dump_valp(str, this, mode);
    }
    fprintf(stderr, "\n");
}

static inline
void dump_data(lmc_t *lmc, int mode)
{
    char str[256];
    for (val_p this = lmc->s; this; this = this->next) {
	sprintf(str, "\nV[%p]\t", this);
	dump_valp(str, this, mode);
    }
    fprintf(stderr, "\n");
}



static inline
void dump_framep(const char *msg, frame_p fp, int mode)
{
    char str[256];
    fprintf(stderr, "%s", msg);
    switch (fp->tag) {
	case TAG_DUM:
	    fprintf(stderr, "DUM");
	    break;

	case TAG_FRAME:
	    fprintf(stderr, "FRAME");
	    break;

	default:
	    fprintf(stderr, "????");
    }
    for (int i=0; i<fp->size; i++) {
	sprintf(str, "\n  [%d]\t", i);
	dump_valp(str, &fp->value[i], mode);
    }
}

static inline
void dump_frame(lmc_t *lmc, int mode)
{
    char str[256];
    for (frame_p this = lmc->e; this; this = this->parent) {
	sprintf(str, "\nF[%p]\t", this);
	dump_framep(str, this, mode);
    }
    fprintf(stderr, "\n");
}

static inline
void dump_all(lmc_t *lmc, int mode)
{
    dump_state(lmc);
    dump_frame(lmc, mode);
    dump_ctrl(lmc, mode);
    dump_data(lmc, mode);
}

#endif
