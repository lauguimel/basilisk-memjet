# Gnuplot script for Laplace pressure test
# Usage: cd cases/01-laplace && gnuplot plot.gp

set terminal pngcairo size 1200,500 enhanced font 'Arial,12'
set output 'laplace.png'

set multiplot layout 1,2 title "Laplace Pressure Equilibrium" font 'Arial,14'

# Left: pressure field
set title "Pressure field"
set xlabel "x"
set ylabel "y"
set size ratio 1
set palette defined (0 "blue", 0.5 "white", 1 "red")
set cbrange [*:*]
plot 'pressure.dat' using 1:2:3 with points pt 5 ps 0.3 palette notitle

# Right: VOF fraction (interface)
set title "VOF fraction (interface)"
set palette defined (0 "white", 1 "steelblue")
set cbrange [0:1]
plot 'pressure.dat' using 1:2:4 with points pt 5 ps 0.3 palette notitle

unset multiplot
set output

print "Plot saved to laplace.png"
