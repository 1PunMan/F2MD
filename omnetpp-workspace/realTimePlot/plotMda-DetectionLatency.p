set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                        # set xtics automatically
set ytics auto                        # set ytics automatically
set y2tics auto		# set ytics automatically
set title "Detection Latency" 
set xlabel "Time (Min)"
set ylabel "Density (Active Cars)" 
set y2label "Detection Latency (s)"
set grid 
show grid 
funcT(x) = x / 60
func3(x,y) = y<=0  ? 0 : 100*x / y


plot "../mdaV1.dat" using (funcT($2)):($8)  title 'SOTA Latency' with lines lw 3 lt rgb "blue"axes x1y2,\
"../mdaV2.dat" using (funcT($2)):($8)  title 'SCA lib Latency' with lines lw 3 lt rgb "magenta" axes x1y2,\

