
#include <unistd.h>

#include "common.h"
#include "lmc.h"


void step(lmc_t *lmc) {
    fprintf(stderr, "ICNT=%d\t", lmc->icnt);
    dump_state(lmc);
    dump_frame(lmc, 0);
    dump_data(lmc, 0);
}

static const char *cmd_name;

static uint32_t num_entry = 0;
static uint32_t num_limit = 60*3072*1000;
static uint32_t num_step = 0;
static uint32_t num_mode = 0;

void exec_lmc(lmc_t *lmc);

void exec_lman(uint32_t entry, uint32_t limit)
{
    frame_p env = ALLOC_FRAME(2, NULL);
    FRAME_VALUE(env, 0) = VAL_INT(-1);

    lmc_t lmc = { 
	.limit = limit,
	.step = 1,
	.e = env,
	.c = entry,
	.d = ALLOC_VAL(TAG_STOP) };

    fprintf(stderr, "\nInitial LMC:\n");
    dump_all(&lmc, num_mode);

    fprintf(stderr, "\nExecuting LMC @%d ...\n", entry);
    exec_lmc(&lmc);

    if (!num_step)
	goto done;

    val_t cons = POPF(&lmc.s);
    if (TAG(cons) != TAG_CONS) {
	fprintf(stderr, "main() must return CONS");
	goto done;
    }

    val_t step_cl = *CDRF(cons);
    if (TAG(step_cl) != TAG_CLOSURE) {
	fprintf(stderr, "main() must return CONS(?, CLOSURE)");
	goto done;
    }

    val_t ai = *CARF(cons);

    entry = CAR_CLOSURE(step_cl);
    frame_p step_env = CDR_CLOSURE(step_cl);

    while (num_step) {
	FREE_STACK(&lmc.s);
	FREE_STACK(&lmc.d);
	// FREE_ENV(&lmc.e);

	frame_p env = ALLOC_FRAME(2, step_env);
	FRAME_VALUE(env, 0) = ai;
	FRAME_VALUE(env, 1) = VAL_INT(-1);

	lmc.e = env;
	lmc.c = entry;
	lmc.d = ALLOC_VAL(TAG_STOP);

	lmc.fault = 0;
	lmc.icnt = 0;
	lmc.limit = 3072*1000;

	fprintf(stderr, "\nIntermediate LMC:\n");
	dump_all(&lmc, num_mode);

	fprintf(stderr, "\nExecuting LMC @%d ...\n", entry);
	exec_lmc(&lmc);

	val_t cons = POPF(&lmc.s);
	if (TAG(cons) != TAG_CONS) {
	    fprintf(stderr, "step() must return CONS");
	    goto done;
	}

	ai = *CARF(cons);

	num_step--;
    }

done:
    fprintf(stderr, "\nFinal LMC:\n");
    dump_all(&lmc, num_mode);
}


int main(int argc, char *argv[])
{
    extern int optind;
    extern char *optarg;
    int c, err_flag = 0;

    #define VERSION "V1.1"
    #define OPTIONS "hE:L:N:M:"

    cmd_name = argv[0];
    while ((c = getopt(argc, argv, OPTIONS)) != EOF) {
	switch (c) {
	case 'h':
	    fprintf(stderr,
		"This is %s " VERSION "\n"
		"options are:\n"
		"-h	   print this help message\n"
		"-E <val>  function entry\n"
		"-L <num>  terminate after <num> cycles\n"
		"-N <num>  call step <num> times\n"
		"-M <num>  debug mode\n"
		, cmd_name);
	    exit(0);
	    break;
	case 'E':
	    num_entry = strtoll(optarg, NULL, 0);
	    break;
	case 'L':
	    num_limit = strtoll(optarg, NULL, 0);
	    break;
	case 'N':
	    num_step = strtoll(optarg, NULL, 0);
	    break;
	case 'M':
	    num_mode = strtoll(optarg, NULL, 0);
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
