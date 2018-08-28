set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto 			# set ytics automatically
set title "On board True detection"
set xlabel "Time (Min)"
set ylabel "Detected (%)"
set grid 
show grid 
funcT(x) = x / 60
func3(x,y) = y<=0  ? 0 : 100*x / y

plot "../AppV1.dat" using (funcT($2)):(func3($26,$27)) title 'SOTA' with lines lw 3 lc rgb "magenta" , \
"../AppV2.dat" using (funcT($2)):(func3($26,$27)) title 'SCA lib' with lines lw 3 lc rgb "#009000", \

