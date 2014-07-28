

GHOSTS = $(wildcard *.ghc)
GHOSTS += $(patsubst %.mghc,%.ghc,$(wildcard *.mghc))
GHOSTS += $(patsubst %.pghc,%.ghc,$(wildcard *.pghc))

INCLUDES = $(wildcard *.inc)


ALL:	$(GHOSTS)

clean:

%.ghc : %.mghc mghc2ghc.gawk
	gawk -f mghc2ghc.gawk $< >$@

%.mghc : %.pghc $(INCLUDES)
	./gasp -c\; -o $@ $< 

