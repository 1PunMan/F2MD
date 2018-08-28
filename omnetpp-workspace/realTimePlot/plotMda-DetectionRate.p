set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytics auto                          # set ytics automatically
set y2tics auto 			# set ytics automatically
set title "Detection Rate"
set xlabel "Time (Min)"
set ylabel "Density (Active Cars)"
set y2label "Detection Rate (%)"
set grid
show grid 
funcT(x) = x / 60
func3(x,y) = y<=0  ? 0 : 100*x / y

plot "../AppV1.dat" using (funcT($2)):($3) title 'Density' with lines lw 2 lc rgb "#CD853F" axes x1y1,\


#plot "../mdaV1.dat" using (funcT($2)):(func3($3,$4)) title 'SOTA Fault' with lines lc rgb "red" axes x1y2,\
#"../mdaV1.dat" using (funcT($2)):(func3($6,$7))  title 'SOTA True' with lines lw 3  lt rgb "blue" axes x1y2,\
#"../mdaV2.dat" using (funcT($2)):(func3($3,$4))  title 'SCA lib Fault' with lines lc rgb "cyan" axes x1y2,\
#"../mdaV2.dat" using (funcT($2)):(func3($6,$7))  title 'SCA lib True' with lines lw 3 lt rgb "magenta" axes x1y2,\
