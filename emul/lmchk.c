
#include "common.h"
#include "lmc.h"

void exec_lmc(lmc_t *lmc);

void exec_lman(int limit)
{
    lmc_t lmc = { .limit = limit, .d = ALLOC_VAL(TAG_STOP) };

    exec_lmc(&lmc);
    dump_state(&lmc);
    dump_data(&lmc);
}


int main(int argc, char *argv[])
{
    exec_lman(3072*1000);
}
