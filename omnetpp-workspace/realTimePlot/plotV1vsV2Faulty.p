set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto      	                   # set xtics automatically
set ytic auto						   # set ytics automatically
set title "On board Faulty detection"
set xlabel "Time (Min)"
set ylabel "Detected (%)"
set y2label "Detected Num"
set grid 
show grid 
funcT(x) = x / 60
func3(x,y) = y<=0  ? 0 : 100*x / y

plot "../AppV1.dat" using (funcT($2)):(func3($14,$15)) title 'SOTA' with lines lw 3 lc rgb "red" axis x1y1, \
"../AppV2.dat" using (funcT($2)):(func3($14,$15)) title 'SCA lib' with lines lw 3 lc rgb "blue" axis x1y1, \
"../AppV1.dat" using (funcT($2)):($14) title 'SOTA Num' with linespoints lw 0.1 ps 0.1 lt rgb "red" axis x1y2, \
"../AppV2.dat" using (funcT($2)):($14) title 'SCA lib Num' with linespoints lw 0.1 ps 0.1 lt rgb "blue" axis x1y2, \

