#ifndef	_LMC_H_
#define	_LMC_H_

#include "common.h"

typedef
enum tag_ {
    TAG_EMPTY = 0,
    TAG_INT = 1,
    TAG_CONS,
    TAG_CLOSURE,

    TAG_ADDR = 16,
    TAG_RET,
    TAG_JOIN,
    TAG_STOP,

    TAG_DUM = 32,
    TAG_FRAME,
} tag_t;

typedef
enum fault_ {
    FLT_UNKNOWN = 0,

    FLT_TAG_MISMATCH_INT = 1,
    FLT_TAG_MISMATCH_CONS,
    FLT_TAG_MISMATCH_CLOSURE,

    FLT_CONTROL_MISMATCH_JOIN,
    FLT_CONTROL_MISMATCH_RET,

    FLT_FRAME_MISMATCH_DUM,
    FLT_FRAME_MISMATCH_FRAME,
    FLT_FRAME_MISMATCH_SIZE,
    FLT_FRAME_MISMATCH_SAME,

    FLT_NO_ENVIRONMENT,
    FLT_NO_PARENT,

    FLT_LAST
} fault_t;


static inline
const char *fault_msg(fault_t id) {
    const char const *msg[FLT_LAST] = {
	[FLT_UNKNOWN] =			    "Unknown Fault",

	[FLT_TAG_MISMATCH_INT] =	    "Not an INT Type",
	[FLT_TAG_MISMATCH_CONS] =	    "Not a CONS Type",
	[FLT_TAG_MISMATCH_CLOSURE] =	    "Not a CLOSURE Type",

	[FLT_CONTROL_MISMATCH_JOIN] =	    "Not a JOIN Type",
	[FLT_CONTROL_MISMATCH_RET] =	    "Not a RET Type",

	[FLT_FRAME_MISMATCH_DUM] =	    "Not a DUM Frame",
	[FLT_FRAME_MISMATCH_FRAME] =	    "Not a FULL Frame",
	[FLT_FRAME_MISMATCH_SIZE] =	    "Frame Size Mismatch",
	[FLT_FRAME_MISMATCH_SAME] =	    "Environment Mismatch",

	[FLT_NO_ENVIRONMENT] =		    "Missing Environment",
	[FLT_NO_PARENT] =		    "Missing Parent",
    };
    return msg[id];
}


typedef uint32_t addr_t;

typedef struct val_ *val_p;

typedef struct frame_ *frame_p;

typedef
struct val_ {
    tag_t tag;
    val_p next;
    union {
	uint32_t num;
	addr_t addr;
	frame_p frame;
	struct {
	    val_p left;
	    val_p right;
	} cons;
	struct {
	    addr_t addr;
	    frame_p env;
	} closure;
    };
} val_t;

typedef
struct frame_ {
    tag_t tag;
    frame_p parent;
    uint32_t size;
    val_t value[];
} frame_t;

typedef
struct lmc_ {
    val_p s;
    val_p d;
    frame_p e;
    addr_t c;

    int fault;
    int step;
    uint32_t icnt;
    uint32_t limit;
} lmc_t;

static inline tag_t TAG(val_t v)	{ return v.tag; }
static inline int NUM(val_t v)		{ return v.num; }
static inline addr_t ADDR(val_t v)	{ return v.addr; }

static inline
val_t SET_TAG(tag_t tag, val_t v)
{
    v.tag = tag;
    return v;
}


static inline
val_t INIT_VAL(tag_t t)
{
    val_t new = { .tag = t };
    return new;
}

static inline
val_p ALLOC_VAL(tag_t t)
{
    val_p new = calloc(1, sizeof(val_t));
    assert(new != NULL);
    new->tag = t;
    return new;
}

static inline
val_t VAL_INT(uint32_t n)
{
    val_t new = INIT_VAL(TAG_INT);
    new.num = n;
    return new;
}

static inline
val_p ALLOC_INT(uint32_t n)
{
    val_p new = ALLOC_VAL(TAG_INT);
    new->num = n;
    return new;
}

static inline
val_t VAL_CONS(val_p x, val_p y)
{
    val_t new = INIT_VAL(TAG_CONS);
    new.cons.left = x;
    new.cons.right = y;
    return new;
}

static inline
val_p ALLOC_CONS(val_p x, val_p y)
{
    val_p new = ALLOC_VAL(TAG_CONS);
    new->cons.left = x;
    new->cons.right = y;
    return new;
}

static inline
val_t VAL_CLOSURE(addr_t f, frame_p fp)
{
    val_t new = INIT_VAL(TAG_CLOSURE);
    new.closure.addr = f;
    new.closure.env = fp;
    return new;
}

static inline
val_p ALLOC_CLOSURE(addr_t f, frame_p fp)
{
    val_p new = ALLOC_VAL(TAG_CLOSURE);
    new->closure.addr = f;
    new->closure.env = fp;
    return new;
}

static inline
val_t VAL_ADDR(addr_t addr)
{
    val_t new = INIT_VAL(TAG_ADDR);
    new.addr = addr;
    return new;
}

static inline
val_t VAL_FRAME(frame_p fp)
{
    val_t new = INIT_VAL(TAG_FRAME);
    new.frame = fp;
    return new;
}

static inline
void FREE_VAL(val_p x)
{
    free(x);
}

static inline
void FREE_VAL_REC(val_p x)
{
    while (x) {
	val_p next = x->next;
	free(x);
	x = next;
    }
}

static inline
void FREE_STACK(val_p *s)
{
    FREE_VAL_REC(*s);
    *s = NULL;
}



#define	TRUE	VAL_INT(1)
#define	FALSE	VAL_INT(0)


#include "lmc_dump.h"



static inline
void handle_fault(const char *func, lmc_t *lmc, fault_t id, val_t val)
{
    fprintf(stderr,
	"FAULT #%d (%s) in %s\n", id, fault_msg(id), func);
    dump_state(lmc);
    lmc->fault = 1;
}

static inline
void handle_stop(lmc_t *lmc)
{
    fprintf(stderr, "STOP\n");
    lmc->fault = -1;
}

#define	FAULT(l, m, v) do {			\
	handle_fault(__func__, l, m, v);	\
	return; } while (0)

#define	FAULTR(l, m, v, r) do {			\
	handle_fault(__func__, l, m, v);	\
	return (r); } while (0)


#define	MACHINE_STOP(l) do {			\
	handle_stop(l);				\
	return; } while (0)


#define	CAR_CLOSURE(c)		((c).closure.addr)
#define	CDR_CLOSURE(c)		((c).closure.env)




static inline
val_p PUSH(val_t v, val_p s)
{
    val_p new = ALLOC_VAL(TAG(v));
    *new = v;
    new->next = s;
    return new;
}

static inline
val_p PUSHP(val_p v, val_p s)
{
    v->next = s;
    return v;
}

static inline
val_p POP(val_p *s)
{
    assert(s != NULL);
    val_p top = *s;
    assert(top != NULL);
    *s = top->next;
    top->next = NULL;
    return top;
}

static inline
val_t POPF(val_p *s)
{
    val_p top = POP(s);
    val_t ret = *top;
    FREE_VAL(top);
    return ret;
}



#define	FRAME_PARENT(f)		((f)->parent)
#define	FRAME_VALUE(f, i)	((f)->value[i])
#define	FRAME_SIZE(f)		((f)->size)
#define	FRAME_TAG(f)		((f)->tag)

static inline
frame_p ALLOC_FRAME(uint32_t s, frame_p fp)
{
    frame_p new = calloc(1, sizeof(frame_t) + sizeof(val_t) * s);
    assert(new != NULL);
    new->parent = fp;
    new->size = s;
    new->tag = TAG_FRAME;
    return new;
}

static inline
void FREE_FRAME(frame_p fp)
{
    free(fp);
}

static inline
void FREE_FRAME_REC(frame_p fp)
{
    while (fp) {
	frame_p parent = FRAME_PARENT(fp);
	free(fp);
	fp = parent;
    }
}

static inline
void FREE_ENV(frame_p *fp)
{
    FREE_FRAME_REC(*fp);
    *fp = NULL;
}



/* static inline
val_t FRAME_VALUE(frame_p fp, uint32_t index)
{
    return fp->value[index];
}	*/



static inline val_t ADD(val_t x, val_t y)
{
    val_t r = VAL_INT(NUM(x) + NUM(y));
    return r;
}

static inline val_t SUB(val_t x, val_t y)
{
    val_t r = VAL_INT(NUM(x) - NUM(y));
    return r;
}

static inline val_t MUL(val_t x, val_t y)
{
    val_t r = VAL_INT(NUM(x) * NUM(y));
    return r;
}

static inline val_t DIV(val_t x, val_t y)
{
    val_t r = VAL_INT(NUM(x) / NUM(y));
    return r;
}

static inline int IS_ZERO(val_t x)
{
    return (NUM(x) == 0);
}



static inline val_p CARF(val_t x)
{
    /* FIXME: collect	FREE_VAL(x.cons.right); */
    return x.cons.left;
}

static inline val_p CDRF(val_t x)
{
    /* FIXME: collect	FREE_VAL(x.cons.left); */
    return x.cons.right;
}

#define	FRAME(v)	((v).frame)


void step(lmc_t *lmc);

static inline
int limit(lmc_t *lmc)
{
    if (lmc->step)
	step(lmc);
    lmc->icnt++;
    return (lmc->icnt > lmc->limit);
}

#endif
