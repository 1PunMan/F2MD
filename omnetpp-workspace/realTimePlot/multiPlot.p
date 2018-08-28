### Start multiplot (2x3 layout)

set multiplot layout 2,3 rowsfirst
#set lmargin 2.5
#set rmargin 2.5
#set tmargin 2.5
#set bmargin 2.5

# --- GRAPH a
load 'plotMdmAll.p'
# --- GRAPH b
load 'plotV1vsV2Faulty.p'
# --- GRAPH c
load 'plotV1vsV2Attack.p'
# --- GRAPH d
load 'plotMda-DetectionLatency.p'
# --- GRAPH e
load 'plotMda-DetectionRate.p'
# --- GRAPH f
#load 'plotHistogram.p'
load 'plotAccuracyRate.p'

unset multiplot
### End multiplot   


