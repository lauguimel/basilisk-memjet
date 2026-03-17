# Basilisk Tutorial — Makefile
# Requires: qcc in PATH (set BASILISK env variable first)
#
# Usage:
#   make all     — compile and run all cases
#   make case1   — Laplace pressure equilibrium
#   make case2   — Drop impact
#   make case3   — Coalescence
#   make clean   — remove binaries and output files

CC = qcc
CFLAGS = -O2 -Wall -disable-dimensions
LIBS = -lm

.PHONY: all case1 case2 case3 clean

all: case1 case2 case3

# --- Case 1: Laplace equilibrium ---
cases/01-laplace/laplace: cases/01-laplace/laplace.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

case1: cases/01-laplace/laplace
	cd cases/01-laplace && ./laplace

# --- Case 2: Drop impact ---
cases/02-drop-impact/impact: cases/02-drop-impact/impact.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

case2: cases/02-drop-impact/impact
	cd cases/02-drop-impact && ./impact

# --- Case 3: Coalescence ---
cases/03-coalescence/coalescence: cases/03-coalescence/coalescence.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

case3: cases/03-coalescence/coalescence
	cd cases/03-coalescence && ./coalescence

# --- Clean ---
clean:
	rm -f cases/01-laplace/laplace cases/01-laplace/*.dat cases/01-laplace/*.png
	rm -f cases/02-drop-impact/impact cases/02-drop-impact/*.dat cases/02-drop-impact/*.png
	rm -f cases/03-coalescence/coalescence cases/03-coalescence/*.dat cases/03-coalescence/*.png
