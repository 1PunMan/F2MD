set   autoscale                        # scale axes automatically
set style data points
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title "On board detection"
set xlabel "Time (Min)"
set ylabel "Detected (%)"
set y2label
set grid 
set key
show grid 
funcT(x) = x / 60
func3(x,y) = y<=0  ? 0 : 100*x / y

plot "../AppV1.dat" using (funcT($2)):(func3($14,$15)) title 'SOTA Fault' with lines lw 3 lc rgb "red" , \
 "../AppV1Inst.dat" using (funcT($2)):(func3($14,$15)) title 'SOTA Fault Inst' ps 0.5 lc rgb "red" , \
 "../AppV1.dat" using (funcT($2)):(func3($26,$27)) title 'SOTA True' with lines lc rgb "magenta" lw 3 , \
 "../AppV1Inst.dat" using (funcT($2)):(func3($26,$27)) title 'SOTA True Inst' ps 0.5 lc rgb "magenta", \
 "../AppV2.dat" using (funcT($2)):(func3($14,$15)) title 'SCA lib Fault' with lines lw 3 lc rgb "blue" , \
 "../AppV2Inst.dat" using (funcT($2)):(func3($14,$15)) title 'SCA lib Fault Inst'  ps 0.5 lc rgb "blue" , \
 "../AppV2.dat" using (funcT($2)):(func3($26,$27)) title 'SCA lib True' with lines lc rgb "#009000" lw 3 , \
 "../AppV2Inst.dat" using (funcT($2)):(func3($26,$27)) title 'SCA lib True Inst' ps 0.5 lc rgb "#009000"  


#plot "../AppV1Inst.dat" using (funcT($2)):(func3($14,$15)) title 'SOTA Fault' ps 0.5 lc rgb "red" , \
# "../AppV1Inst.dat" using (funcT($2)):(func3($26,$27)) title 'SOTA True ' ps 0.5 lc rgb "magenta", \
# "../AppV2Inst.dat" using (funcT($2)):(func3($14,$15)) title 'SCA lib Fault' ps 0.5 lc rgb "blue" , \
# "../AppV2Inst.dat" using (funcT($2)):(func3($26,$27)) title 'SCA lib True' ps 0.5 lc rgb "#009000"  


#plot "../AppV1.dat" using (funcT($2)):(func3($14,$15)) title 'Cumulative Faulty V1' with lines lw 3 lc rgb "red" , \
# "../AppV1Inst.dat" using (funcT($2)):(func3($14,$15)) title 'Instantaneous Faulty V1' ps 0.5 lc rgb "red" , \
# "../AppV1.dat" using (funcT($2)):(func3($26,$27)) title 'Cumulative Attacker V1' with lines lc rgb "magenta" lw 3 , \
# "../AppV1Inst.dat" using (funcT($2)):(func3($26,$27)) title 'Instantaneous Attacker V1' ps 0.5 lc rgb "magenta", \
# "../AppV2.dat" using (funcT($2)):(func3($14,$15)) title 'Cumulative Faulty V2' with lines lw 3 lc rgb "blue" , \
# "../AppV2Inst.dat" using (funcT($2)):(func3($14,$15)) title 'Instantaneous Faulty V2' ps 0.5 lc rgb "blue" , \
# "../AppV2.dat" using (funcT($2)):(func3($26,$27)) title 'Cumulative Attacker V2' with lines lc rgb "#009000" lw 3 , \
# "../AppV2Inst.dat" using (funcT($2)):(func3($26,$27)) title 'Instantaneous Attacker V2' ps 0.5 lc rgb "#009000"  

