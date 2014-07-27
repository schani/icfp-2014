
	// C Version of lmc.gcc

#include "gcc.h"

#define gcc_GOTO(lmc) do {		\
	uint32_t pc = lmc->c;		\
					\
	switch(pc) {			\
		case 5: goto L5;	\
		case 6: goto L6;	\
		case 10: goto L10;	\
		case 20: goto L20;	\
		case 12: goto L12;	\
		case 16: goto L16;	\
	}				\
} while(0)

void exec_lmc(lmc_t *lmc) {
		if (limit(lmc)) return; gcc_DUM(lmc, 2);
		if (limit(lmc)) return; gcc_LDF(lmc, 6);
		if (limit(lmc)) return; gcc_LDF(lmc, 16);
		if (limit(lmc)) return; gcc_LDF(lmc, 16);
		if (limit(lmc)) return; gcc_RAP(lmc, 2); if (lmc->fault) return; gcc_GOTO(lmc);
	L5:	if (limit(lmc)) return; gcc_STOP(lmc); return;
	L6:	if (limit(lmc)) return; gcc_LDC(lmc, 1);
		if (limit(lmc)) return; gcc_LDC(lmc, 1);
		if (limit(lmc)) return; gcc_CEQ(lmc); if (lmc->fault) return;
		if (limit(lmc)) return; if (gcc_TSEL(lmc)) goto L12; else goto L10; if (lmc->fault) return;
	L10:	if (limit(lmc)) return; gcc_LDC(lmc, 3141592);
		if (limit(lmc)) return; gcc_RTN(lmc); if (lmc->fault) return; gcc_GOTO(lmc);
	L12:	if (limit(lmc)) return; gcc_LD(lmc, 0, 0); if (lmc->fault) return;
		if (limit(lmc)) return; gcc_LD(lmc, 0, 1); if (lmc->fault) return;
		if (limit(lmc)) return; gcc_ADD(lmc); if (lmc->fault) return;
		if (limit(lmc)) return; gcc_RTN(lmc); if (lmc->fault) return; gcc_GOTO(lmc);
	L16:	if (limit(lmc)) return; gcc_LDC(lmc, 1);
		if (limit(lmc)) return; gcc_LDC(lmc, 2);
		if (limit(lmc)) return; gcc_LD(lmc, 0, 0); if (lmc->fault) return;
		if (limit(lmc)) return; gcc_TAP(lmc, 2); if (lmc->fault) return; gcc_GOTO(lmc);
	L20:	if (limit(lmc)) return; 
	EXIT:	return;
}

