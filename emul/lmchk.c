
#include <unistd.h>

#include "common.h"
#include "lmc.h"


static const char *cmd_name;

static uint32_t num_entry = 0;
static uint32_t num_limit = 60*3072*1000;

void exec_lmc(lmc_t *lmc);

void exec_lman(uint32_t entry, uint32_t limit)
{
    lmc_t lmc = { 
	.limit = limit,
	.c = entry,
	.d = ALLOC_VAL(TAG_STOP) };

    fprintf(stderr, "\nInitial LMC:\n");
    dump_all(&lmc);

    fprintf(stderr, "\nExecuting LMC ...\n");
    exec_lmc(&lmc);

    fprintf(stderr, "\nFinal LMC:\n");
    dump_all(&lmc);
}


int main(int argc, char *argv[])
{
    extern int optind;
    extern char *optarg;
    int c, err_flag = 0;

    #define VERSION "V1.0"
    #define OPTIONS "hE:L:"

    cmd_name = argv[0];
    while ((c = getopt(argc, argv, OPTIONS)) != EOF) {
	switch (c) {
	case 'h':
	    fprintf(stderr,
		"This is %s " VERSION "\n"
		"options are:\n"
		"-h	   print this help message\n"
		"-E <val>  function entry\n"
		"-L <val>  cycle limit\n"
		, cmd_name);
	    exit(0);
	    break;
	case 'E':
	    num_entry = strtoll(optarg, NULL, 0);
	    break;
	case 'L':
	    num_limit = strtoll(optarg, NULL, 0);
	    break;
	case '?':
	default:
	    err_flag++;
	    break;
	}
    }
    if (err_flag) {
	fprintf(stderr,
	    "Usage: %s -[" OPTIONS "] ...\n"
	    "%s -h for help.\n",
	    cmd_name, cmd_name);
	exit(2);
    }

    exec_lman(num_entry, num_limit);
}
