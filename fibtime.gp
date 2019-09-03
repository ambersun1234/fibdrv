reset

set ylabel 'time(nsec)'
set xlabel 'number'
set xtics 0,10
set style fill solid
set title 'Fibonacci with fast doubling and arbitrary precision arithmetic'
set term png enhanced font 'Verdana, 10'
set output 'fibtime.png'
set format y

plot[:][:]'result.txt' \
using 1:3 with linespoints linewidth 2 title 'Fibonacci execution time'
