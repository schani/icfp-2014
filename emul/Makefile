

CFLAGS = -O3 -g -std=gnu99 -Wall -Wunused-label

HFILES = $(wildcard *.h)

ALL:	engine lmchk

clean:
	$(RM) -f *.o
	$(RM) -f engine
	$(RM) -f lmchk

lmchk : lmchk.o lmc.o

engine : engine.o lmc.o ghost0.o ghost1.o ghost2.o ghost3.o
	$(CC) $(CFLAGS) -o $@ $^

engine.o : engine.c $(HFILES)

%.c : %.ghc ghc.h $(HFILES)
	gawk -v GHOST=$* -f ghc2c.gawk $< >$@

%.c : %.gcc gcc.h $(HFILES)
	gawk -v LMAN=$* -f gcc2c.gawk $< >$@

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<
