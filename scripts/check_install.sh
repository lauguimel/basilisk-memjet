#!/bin/bash
# Basilisk installation checker
# Usage: ./scripts/check_install.sh

set -e

echo "=== Basilisk Installation Check ==="
echo ""

# Check BASILISK env variable
if [ -z "$BASILISK" ]; then
    echo "[FAIL] BASILISK environment variable is not set."
    echo "       Add to your shell config:"
    echo "         export BASILISK=\$HOME/basilisk/src"
    echo "         export PATH=\$PATH:\$BASILISK"
    exit 1
else
    echo "[OK]   BASILISK = $BASILISK"
fi

# Check qcc
if command -v qcc &> /dev/null; then
    echo "[OK]   qcc found: $(which qcc)"
else
    echo "[FAIL] qcc not found in PATH."
    echo "       Make sure \$BASILISK is in your PATH."
    exit 1
fi

# Try compiling a minimal test
TMPDIR=$(mktemp -d)
cat > "$TMPDIR/test.c" << 'EOF'
#include "grid/multigrid.h"
#include "utils.h"

int main() {
  init_grid(16);
  scalar s[];
  foreach()
    s[] = x*y;
  stats st = statsf(s);
  fprintf(stdout, "sum = %g\n", st.sum);
}
EOF

echo ""
echo "Compiling minimal test..."
if qcc -O2 "$TMPDIR/test.c" -o "$TMPDIR/test" -lm 2>/dev/null; then
    echo "[OK]   Compilation successful."
    OUTPUT=$("$TMPDIR/test" 2>/dev/null)
    echo "[OK]   Execution successful: $OUTPUT"
else
    echo "[FAIL] Compilation failed."
    echo "       Try recompiling Basilisk:"
    echo "         cd \$BASILISK && make clean && make -k"
    rm -rf "$TMPDIR"
    exit 1
fi

rm -rf "$TMPDIR"

# Check gnuplot (optional)
echo ""
if command -v gnuplot &> /dev/null; then
    echo "[OK]   gnuplot found (optional): $(which gnuplot)"
else
    echo "[WARN] gnuplot not found (optional, needed for plot.gp scripts)"
fi

echo ""
echo "=== All checks passed! You can now run: make all ==="
