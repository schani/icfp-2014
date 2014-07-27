
#include "common.h"
#include "lmc.h"

void exec_lmc(lmc_t *lmc);

void exec_lman(int limit)
{
    lmc_t lmc = { .limit = limit, .d = ALLOC_VAL(TAG_STOP) };

    fprintf(stderr, "\nInitial LMC:\n");
    dump_all(&lmc);
    fprintf(stderr, "\nExecuting LMC ...\n");
    exec_lmc(&lmc);
    fprintf(stderr, "\nFinal LMC:\n");
    dump_all(&lmc);
}


int main(int argc, char *argv[])
{
    exec_lman(3072*1000);
}
