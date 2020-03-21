reset

set ylabel 'time(nsec)'
set xlabel 'number'
set xtics 0,10
set style fill solid
set title 'fibonacci execution time'
set term png enhanced font 'Verdana, 10'
set output 'time.png'
set format y

plot[:][:]'time' \
using 2:xtic(10) with linespoints linewidth 2 title 'fibonacci calculation time'
