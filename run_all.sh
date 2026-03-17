#!/bin/bash
# Compile and run all Basilisk test cases
# Usage: ./run_all.sh

set -e

# --- Configuration ---
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
export BASILISK="$SCRIPT_DIR/basilisk/src"
export PATH="$PATH:$BASILISK"

CC=qcc
CFLAGS="-O2 -Wall -disable-dimensions"
LIBS="-lm"

# --- Check installation ---
if ! command -v qcc &> /dev/null; then
    echo "[FAIL] qcc not found. Check BASILISK path: $BASILISK"
    exit 1
fi
echo "[OK] qcc found: $(which qcc)"
echo ""

# --- Case 1: Laplace equilibrium ---
echo "===== Case 1: Laplace Equilibrium ====="
cd "$SCRIPT_DIR/cases/01-laplace"
echo "Compiling..."
$CC $CFLAGS laplace.c -o laplace $LIBS
echo "Running..."
./laplace
echo ""

# --- Case 2: Drop impact ---
echo "===== Case 2: Drop Impact ====="
cd "$SCRIPT_DIR/cases/02-drop-impact"
echo "Compiling..."
$CC $CFLAGS impact.c -o impact $LIBS
echo "Running..."
./impact
echo ""

# --- Case 3: Coalescence ---
echo "===== Case 3: Coalescence ====="
cd "$SCRIPT_DIR/cases/03-coalescence"
echo "Compiling..."
$CC $CFLAGS coalescence.c -o coalescence $LIBS
echo "Running..."
./coalescence
echo ""

echo "===== All cases completed! ====="
echo "Output files in each cases/ subdirectory."
