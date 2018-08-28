
set term x11 dashed background '#000000'
set border lc rgb '#00bb00'
set key tc rgb '#00bb00'
set xtic tc rgb '#00bb00'                        # set xtics automatically
set ytics tc rgb '#00bb00'                       
set y2tics tc rgb '#00bb00'		
set title tc rgb '#00bb00' 
set xlabel tc rgb '#00bb00'
set ylabel tc rgb '#00bb00'
set y2label tc rgb '#00bb00'
set grid lc rgb '#00bb00'
show grid 

while (1) {
    load 'multiPlot.p'
    pause 1
}
