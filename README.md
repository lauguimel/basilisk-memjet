# Basilisk Tutorial — Droplet Simulations

Three progressively complex test cases for [Basilisk](http://basilisk.fr),
focused on **droplet physics and surface tension**.

| Case | Description | Difficulty | Time |
|------|------------|------------|------|
| `01-laplace` | Static droplet, Laplace pressure check | Beginner | ~10 s |
| `02-drop-impact` | Axisymmetric drop impact on a surface | Intermediate | ~2 min |
| `03-coalescence` | Two droplets merging | Advanced | ~3 min |

---

## 1. Prerequisites

| Tool | Required | Note |
|------|----------|------|
| `gcc` or `clang` | Yes | C99 compiler |
| `make` | Yes | Build system |
| `gnuplot` | Optional | Visualization |
| `darcs` | Optional | To clone Basilisk source (alternative: tarball) |

### Linux (Debian/Ubuntu)

```bash
sudo apt-get install build-essential darcs gnuplot
```

### macOS

```bash
xcode-select --install
brew install gnuplot
```

---

## 2. Install Basilisk

### Option A — From tarball (recommended for beginners)

```bash
cd $HOME
curl -O http://basilisk.fr/basilisk/basilisk.tar.gz
tar xzf basilisk.tar.gz
cd basilisk/src
```

### Option B — From darcs (latest version)

```bash
cd $HOME
darcs clone http://basilisk.fr/basilisk
cd basilisk/src
```

### Compile Basilisk

**Linux:**
```bash
ln -s config.gcc config
make -k
```

**macOS:**
```bash
ln -s config.osx config
make -k
```

> **macOS troubleshooting:** If compilation fails with `-fopenmp` errors,
> edit the `config` file and remove or comment out the `-fopenmp` flag.
> Basilisk will still work, just without OpenMP parallelism.

### Set environment variables

Add these lines to your `~/.bashrc` (Linux) or `~/.zshrc` (macOS):

```bash
export BASILISK=$HOME/basilisk/src
export PATH=$PATH:$BASILISK
```

Then reload:
```bash
source ~/.bashrc   # or source ~/.zshrc
```

### Verify installation

```bash
# Quick check
qcc --version

# Or use the provided script
./scripts/check_install.sh
```

---

## 3. Run the test cases

### All at once

```bash
make all
```

### One at a time

```bash
make case1    # Laplace equilibrium
make case2    # Drop impact
make case3    # Coalescence
```

Each command will **compile** the `.c` file and then **run** the simulation.
Output data files are written in each case directory.

### Clean up

```bash
make clean
```

---

## 4. Test cases description

### Case 1 — Laplace Equilibrium (`cases/01-laplace/`)

A circular droplet at rest in a square domain. The pressure jump across
the interface should satisfy the **Young-Laplace equation**:

$$\Delta P = \frac{\sigma}{R}$$

This is the simplest possible two-phase Basilisk simulation. If this
compiles and the pressure error is small (< 1%), your installation works.

**Output:** `pressure.dat` — pressure field, `log` on stderr — convergence.

### Case 2 — Drop Impact (`cases/02-drop-impact/`)

An axisymmetric droplet falling onto a solid surface under gravity.
Uses adaptive mesh refinement (`adapt_wavelet`) for efficiency.

- Weber number ≈ 250
- Reynolds number ≈ 1000

**Output:** `interface-*.dat` — interface shape at different times.

### Case 3 — Coalescence (`cases/03-coalescence/`)

Two droplets placed close together merge under the action of surface
tension. The capillary bridge radius grows as $r \sim t^{1/2}$ in the
viscous regime.

**Output:** `interface-*.dat` — interface snapshots, `bridge.dat` — bridge radius vs time.

---

## 5. Visualization

Each case includes a `plot.gp` gnuplot script:

```bash
cd cases/01-laplace
gnuplot plot.gp
# Opens a plot window (or generates a PNG with plot.gp)
```

### Jupyter notebook

A complete visualization notebook is provided in `notebooks/visualize.ipynb`.
It renders directly on GitHub and can also be run locally:

```bash
pip install numpy matplotlib jupyter
cd notebooks
jupyter notebook visualize.ipynb
```

Alternatively, the `.dat` files are plain text and can be loaded in Python:
```python
import numpy as np
data = np.loadtxt("pressure.dat")
```

---

## Resources

- [Basilisk website](http://basilisk.fr)
- [Basilisk tutorial](http://basilisk.fr/Tutorial)
- [Basilisk examples](http://basilisk.fr/src/examples/)
- [Basilisk source headers](http://basilisk.fr/src/)
