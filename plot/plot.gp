set terminal png
set key left
set logscale x 2

set output 'ss2o_100.png'
set title 'Strong Scalability 100*100*100'

set xlabel 'number of threads'
set ylabel 'average speed-up'


f(x) = x * 4.2155

plot 'scal_100_o.dat' with points title 'real speed-up', \
     f(x) with lines title 'theoretic speed-up'



set output 'ss2o_500.png'
set title 'Strong Scalability 500*500*500'

set xlabel 'number of threads'
set ylabel 'average speed-up'


f(x) = x * 2.5496

plot 'scal_500_o.dat' with points title 'real speed-up', \
     f(x) with lines title 'theoretic speed-up'



set output 'ss2o_1000.png'
set title 'Strong Scalability 1000*1000*1000'

set xlabel 'number of threads'
set ylabel 'average speed-up'


f(x) = x * 3.1445

plot 'scal_1000_o.dat' with points title 'real speed-up', \
     f(x) with lines title 'theoretic speed-up'