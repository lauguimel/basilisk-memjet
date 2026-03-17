# Gnuplot script for drop impact visualization
# Usage: cd cases/02-drop-impact && gnuplot plot.gp

set terminal pngcairo size 1200,800 enhanced font 'Arial,12'
set output 'impact.png'

set multiplot layout 2,3 title "Drop Impact — Interface Evolution" font 'Arial,14'

set xlabel "x (axial)"
set ylabel "r (radial)"
set size ratio 1
set xrange [0:4]
set yrange [0:2]
set style line 1 lc rgb "steelblue" lw 2

times = "0.00 0.50 1.00 1.50 2.00 2.50"

do for [i=1:words(times)] {
    t = word(times, i)
    file = sprintf("interface-%s.dat", t)
    set title sprintf("t = %s", t)
    plot file using 1:2 with lines ls 1 notitle
}

unset multiplot
set output

print "Plot saved to impact.png"
