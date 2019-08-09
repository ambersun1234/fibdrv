reset

set ylabel 'time(nsec)'
set xlabel 'number'
set xtics 0,10
set style fill solid
set title 'Transfer time between kernel space and user space'
set term png enhanced font 'Verdana, 10'
set output 'time.png'
set format y

plot[:][:]'time' \
using 2:xtic(10) with linespoints linewidth 2 title 'User to Kernel', \
'' using 3:xtic(10) with linespoints linewidth 2 title 'Kernel to User'
