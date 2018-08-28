set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto 			# set ytics automatically
set title "On board Report Accuracy"
set xlabel "Time (Min)"
set ylabel "Falty Reports (%)"
set y2label
set grid 
show grid 
funcAcc(x,y) = 100* x/(x+y)
funcT(x) = x/60

#TP
#FP
#FN
#TN

plot "../AppV1.dat" using (funcT($2)):(funcAcc($26,$14)) title 'SOTA F-score' with lines lw 3 lc rgb "magenta" , \
"../AppV1Inst.dat" using (funcT($2)):(funcAcc($26,$14)) title 'SOTA F-score Inst' ps 0.5 lc rgb "magenta" , \
"../AppV2.dat" using (funcT($2)):(funcAcc($26,$14)) title 'SCA lib F-score' with lines lw 3 lc rgb "blue" , \
"../AppV2Inst.dat" using (funcT($2)):(funcAcc($26,$14)) title 'SCA lib F-score Inst' ps 0.5 lc rgb "blue" 
