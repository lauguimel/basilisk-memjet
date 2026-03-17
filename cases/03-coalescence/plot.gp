# Gnuplot script for coalescence visualization
# Usage: cd cases/03-coalescence && gnuplot plot.gp

set terminal pngcairo size 1200,800 enhanced font 'Arial,12'

# --- Plot 1: Bridge radius vs time ---
set output 'bridge.png'
set title "Capillary Bridge Radius vs Time" font 'Arial,14'
set xlabel "t"
set ylabel "r_b"
set logscale xy
set grid
set style line 1 lc rgb "steelblue" lw 2 pt 7 ps 0.5
set style line 2 lc rgb "red" lw 1.5 dt 2

# Inertial scaling: r_b ~ t^(1/2)
plot 'bridge.dat' using 1:2 with linespoints ls 1 title "Simulation", \
     '' using 1:(0.8*sqrt($1)) with lines ls 2 title "~ t^{1/2} scaling"

set output

# --- Plot 2: Interface snapshots ---
set output 'coalescence.png'
set terminal pngcairo size 1200,800 enhanced font 'Arial,12'

unset logscale
set multiplot layout 2,3 title "Coalescence — Interface Evolution" font 'Arial,14'

set xlabel "x"
set ylabel "y"
set size ratio 1
set xrange [-0.6:0.6]
set yrange [-0.4:0.4]
set style line 1 lc rgb "steelblue" lw 2

times = "0.000 0.050 0.100 0.200 0.300 0.500"

do for [i=1:words(times)] {
    t = word(times, i)
    file = sprintf("interface-%s.dat", t)
    set title sprintf("t = %s", t)
    plot file using 1:2 with lines ls 1 notitle
}

unset multiplot
set output

print "Plots saved to bridge.png and coalescence.png"
