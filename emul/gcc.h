#ifndef	_GCC_H_
#define	_GCC_H_

#include "common.h"

#include "lmc.h"


static inline
void gcc_LDC(lmc_t *lmc, uint32_t n)
{
    lmc->s = PUSH(SET_TAG(TAG_INT, VAL_INT(n)), lmc->s);
    lmc->c = lmc->c+1;
}

static inline
void gcc_LD(lmc_t *lmc, uint32_t n, uint32_t i)
{
    frame_p fp = lmc->e;
    if (!fp)
	FAULT(lmc, FLT_NO_ENVIRONMENT, VAL_FRAME(fp));

    while (n > 0) {
	fp = FRAME_PARENT(fp);
	if (!fp)
	    FAULT(lmc, FLT_NO_PARENT, VAL_FRAME(fp));
	n = n-1;
    }

    if (FRAME_TAG(fp) == TAG_DUM)
	FAULT(lmc, FLT_FRAME_MISMATCH_FRAME, VAL_FRAME(fp));
    val_t v = FRAME_VALUE(fp, i);
    lmc->s = PUSH(v, lmc->s);
    lmc->c = lmc->c+1;
}

static inline
void gcc_ADD(lmc_t *lmc)
{
    val_t y = POPF(&lmc->s);
    val_t x = POPF(&lmc->s);
    if (TAG(x) != TAG_INT)
	FAULT(lmc, FLT_TAG_MISMATCH_INT, x);
    if (TAG(y) != TAG_INT)
	FAULT(lmc, FLT_TAG_MISMATCH_INT, y);

    val_t z = ADD(x, y);
    lmc->s = PUSH(SET_TAG(TAG_INT, z), lmc->s);
    lmc->c = lmc->c+1;
}

static inline
void gcc_SUB(lmc_t *lmc)
{
    val_t y = POPF(&lmc->s);
    val_t x = POPF(&lmc->s);
    if (TAG(x) != TAG_INT)
	FAULT(lmc, FLT_TAG_MISMATCH_INT, x);
    if (TAG(y) != TAG_INT)
	FAULT(lmc, FLT_TAG_MISMATCH_INT, y);

    val_t z = SUB(x, y);
    lmc->s = PUSH(SET_TAG(TAG_INT, z), lmc->s);
    lmc->c = lmc->c+1;
}

static inline
void gcc_MUL(lmc_t *lmc)
{
    val_t y = POPF(&lmc->s);
    val_t x = POPF(&lmc->s);
    if (TAG(x) != TAG_INT)
	FAULT(lmc, FLT_TAG_MISMATCH_INT, x);
    if (TAG(y) != TAG_INT)
	FAULT(lmc, FLT_TAG_MISMATCH_INT, y);

    val_t z = MUL(x, y);
    lmc->s = PUSH(SET_TAG(TAG_INT, z), lmc->s);
    lmc->c = lmc->c+1;
}

static inline
void gcc_DIV(lmc_t *lmc)
{
    val_t y = POPF(&lmc->s);
    val_t x = POPF(&lmc->s);
    if (TAG(x) != TAG_INT)
	FAULT(lmc, FLT_TAG_MISMATCH_INT, x);
    if (TAG(y) != TAG_INT)
	FAULT(lmc, FLT_TAG_MISMATCH_INT, y);

    val_t z = DIV(x, y);
    lmc->s = PUSH(SET_TAG(TAG_INT, z), lmc->s);
    lmc->c = lmc->c+1;
}

static inline
void gcc_CEQ(lmc_t *lmc)
{
    val_t y = POPF(&lmc->s);
    val_t x = POPF(&lmc->s);
    if (TAG(x) != TAG_INT)
	FAULT(lmc, FLT_TAG_MISMATCH_INT, x);
    if (TAG(y) != TAG_INT)
	FAULT(lmc, FLT_TAG_MISMATCH_INT, y);

    val_t z;
    if (NUM(x) == NUM(y))
	z = TRUE;
    else
	z = FALSE;
    lmc->s = PUSH(SET_TAG(TAG_INT, z), lmc->s);
    lmc->c = lmc->c+1;
}

static inline
void gcc_CGT(lmc_t *lmc)
{
    val_t y = POPF(&lmc->s);
    val_t x = POPF(&lmc->s);
    if (TAG(x) != TAG_INT)
	FAULT(lmc, FLT_TAG_MISMATCH_INT, x);
    if (TAG(y) != TAG_INT)
	FAULT(lmc, FLT_TAG_MISMATCH_INT, y);

    val_t z;
    if (NUM(x) > NUM(y))
	z = TRUE;
    else
	z = FALSE;
    lmc->s = PUSH(SET_TAG(TAG_INT, z), lmc->s);
    lmc->c = lmc->c+1;
}

static inline
void gcc_CGTE(lmc_t *lmc)
{
    val_t y = POPF(&lmc->s);
    val_t x = POPF(&lmc->s);
    if (TAG(x) != TAG_INT)
	FAULT(lmc, FLT_TAG_MISMATCH_INT, x);
    if (TAG(y) != TAG_INT)
	FAULT(lmc, FLT_TAG_MISMATCH_INT, y);

    val_t z;
    if (NUM(x) >= NUM(y))
	z = TRUE;
    else
	z = FALSE;
    lmc->s = PUSH(SET_TAG(TAG_INT, z), lmc->s);
    lmc->c = lmc->c+1;
}

static inline
void gcc_ATOM(lmc_t *lmc)
{
    val_t x = POPF(&lmc->s);
    val_t y;

    if (TAG(x) == TAG_INT)
	y = TRUE;
    else
	y = FALSE;
    lmc->s = PUSH(SET_TAG(TAG_INT, y), lmc->s);
    lmc->c = lmc->c+1;
}

static inline
void gcc_CONS(lmc_t *lmc)
{
    val_p y = POP(&lmc->s);
    val_p x = POP(&lmc->s);

    val_t z = VAL_CONS(x, y);
    lmc->s = PUSH(SET_TAG(TAG_CONS, z), lmc->s);
    lmc->c = lmc->c+1;
}

static inline
void gcc_CAR(lmc_t *lmc)
{
    val_t x = POPF(&lmc->s);

    if (TAG(x) != TAG_CONS)
	FAULT(lmc, FLT_TAG_MISMATCH_CONS, x);

    val_p y = CARF(x);
    lmc->s = PUSHP(y, lmc->s);
    lmc->c = lmc->c+1;
}

static inline
void gcc_CDR(lmc_t *lmc)
{
    val_t x = POPF(&lmc->s);

    if (TAG(x) != TAG_CONS)
	FAULT(lmc, FLT_TAG_MISMATCH_CONS, x);

    val_p y = CDRF(x);
    lmc->s = PUSHP(y, lmc->s);
    lmc->c = lmc->c+1;
}

static inline
int gcc_SEL(lmc_t *lmc)
{
    val_t x = POPF(&lmc->s);

    if (TAG(x) != TAG_INT)
	FAULTR(lmc, FLT_TAG_MISMATCH_INT, x, 0);
    lmc->d = PUSH(SET_TAG(TAG_JOIN, VAL_ADDR(lmc->c+1)),lmc->d);

    return !IS_ZERO(x);
}

static inline
void gcc_JOIN(lmc_t *lmc)
{
    val_t x = POPF(&lmc->d);

    if (TAG(x) != TAG_JOIN)
	FAULT(lmc, FLT_CONTROL_MISMATCH_JOIN, x);

    lmc->c = ADDR(x);
}

static inline
void gcc_LDF(lmc_t *lmc, uint32_t f)
{
    val_t x = VAL_CLOSURE(f, lmc->e);
    lmc->s = PUSH(SET_TAG(TAG_CLOSURE, x), lmc->s);
    lmc->c = lmc->c+1;
}

static inline
void gcc_AP(lmc_t *lmc, uint32_t n)
{
    val_t x = POPF(&lmc->s);

    if (TAG(x) != TAG_CLOSURE)
	FAULT(lmc, FLT_TAG_MISMATCH_CLOSURE, x);

    addr_t f = CAR_CLOSURE(x);
    frame_p e = CDR_CLOSURE(x);
    frame_p fp = ALLOC_FRAME(n, e);

    int i = n - 1;
    while (i != -1) {
	val_t y = POPF(&lmc->s);
	FRAME_VALUE(fp, i) = y;
	i = i-1;
    }
    lmc->d = PUSH(VAL_FRAME(lmc->e), lmc->d);
    lmc->d = PUSH(SET_TAG(TAG_RET, VAL_ADDR(lmc->c+1)), lmc->d);
    lmc->e = fp;
    lmc->c = f;
}

static inline
void gcc_RTN(lmc_t *lmc)
{
    val_t x = POPF(&lmc->d);

    if (TAG(x) == TAG_STOP)
	MACHINE_STOP(lmc);
    if (TAG(x) != TAG_RET)
	FAULT(lmc, FLT_CONTROL_MISMATCH_RET, x);

    val_t y = POPF(&lmc->d);
    lmc->e = FRAME(y);
    lmc->c = ADDR(x);
}

static inline
void gcc_DUM(lmc_t *lmc, uint32_t n)
{
    frame_p fp = ALLOC_FRAME(n, lmc->e);
    FRAME_TAG(fp) = TAG_DUM;
    lmc->e = fp;
    lmc->c = lmc->c+1;
}

static inline
void gcc_RAP(lmc_t *lmc, uint32_t n)
{
    val_t x = POPF(&lmc->s);

    if (TAG(x) != TAG_CLOSURE)
	FAULT(lmc, FLT_TAG_MISMATCH_CLOSURE, x);

    addr_t f = CAR_CLOSURE(x);
    frame_p fp = CDR_CLOSURE(x);

    if (FRAME_TAG(lmc->e) != TAG_DUM)
	FAULT(lmc, FLT_FRAME_MISMATCH_DUM, x);
    if (FRAME_SIZE(lmc->e) != n)
	FAULT(lmc, FLT_FRAME_MISMATCH_SIZE, x);
    if (lmc->e != fp)
	FAULT(lmc, FLT_FRAME_MISMATCH_SAME, x);

    int i = n-1;
    while (i != -1) {
	val_t y = POPF(&lmc->s);
	FRAME_VALUE(fp, i) = y;
	i = i-1;
    }

    frame_p ep = FRAME_PARENT(lmc->e);
    lmc->d = PUSH(VAL_FRAME(ep), lmc->d);
    lmc->d = PUSH(SET_TAG(TAG_RET, VAL_ADDR(lmc->c+1)), lmc->d);
    FRAME_TAG(fp) = TAG_FRAME;
    lmc->e = fp;
    lmc->c = f;
}

static inline
void gcc_STOP(lmc_t *lmc)
{
}

static inline
int gcc_TSEL(lmc_t *lmc)
{
    val_t x = POPF(&lmc->s);

    if (TAG(x) != TAG_INT)
	FAULTR(lmc, FLT_TAG_MISMATCH_INT, x, 0);

    return !IS_ZERO(x);
}

static inline
void gcc_TAP(lmc_t *lmc, uint32_t n)
{
    val_t x = POPF(&lmc->s);

    if (TAG(x) != TAG_CLOSURE)
	FAULT(lmc, FLT_TAG_MISMATCH_CLOSURE, x);

    addr_t f = CAR_CLOSURE(x);
    frame_p e = CDR_CLOSURE(x);
    frame_p fp = ALLOC_FRAME(n, e);

    int i = n-1;
    while (i != -1) {
	val_t y = POPF(&lmc->s);
	FRAME_VALUE(fp, i) = y;
	i = i-1;
    }

    lmc->e = fp;
    lmc->c = f;
}

static inline
void gcc_TRAP(lmc_t *lmc, uint32_t n)
{
    val_t x = POPF(&lmc->s);

    if (TAG(x) != TAG_CLOSURE)
	FAULT(lmc, FLT_TAG_MISMATCH_CLOSURE, x);

    addr_t f = CAR_CLOSURE(x);
    frame_p fp = CDR_CLOSURE(x);

    if (FRAME_TAG(lmc->e) != TAG_DUM)
	FAULT(lmc, FLT_FRAME_MISMATCH_DUM, x);
    if (FRAME_SIZE(lmc->e) != n)
	FAULT(lmc, FLT_FRAME_MISMATCH_SIZE, x);
    if (lmc->e != fp)
	FAULT(lmc, FLT_FRAME_MISMATCH_SAME, x);

    int i = n-1;
    while (i != -1) {
	val_t y = POPF(&lmc->s);
	FRAME_VALUE(fp, i) = y;
	i = i-1;
    }

    FRAME_TAG(fp) = TAG_FRAME;
    lmc->e = fp;
    lmc->c = f;
}

static inline
void gcc_ST(lmc_t *lmc, uint32_t n, uint32_t i)
{
    frame_p fp = lmc->e;
    if (!fp)
	FAULT(lmc, FLT_NO_ENVIRONMENT, VAL_FRAME(fp));

    while (n > 0) {
	fp = FRAME_PARENT(fp);
	if (!fp)
	    FAULT(lmc, FLT_NO_PARENT, VAL_FRAME(fp));
	n = n-1;
    }

    if (FRAME_TAG(fp) == TAG_DUM)
	FAULT(lmc, FLT_FRAME_MISMATCH_FRAME, VAL_FRAME(fp));

    val_t v = POPF(&lmc->s);
    FRAME_VALUE(fp, i) = v;
    lmc->c = lmc->c+1;
}

static inline
void gcc_DBUG(lmc_t *lmc)
{
    val_t x = POPF(&lmc->s);
    dump_valp("DBUG", &x);
    lmc->c = lmc->c+1;
}

static inline
void gcc_BRK(lmc_t *lmc)
{
    lmc->c = lmc->c+1;
}

#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-label"

#endif
