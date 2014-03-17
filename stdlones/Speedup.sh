#!/bin/bash
echo --------------------------------------
echo List of currently booted nodes in LAM
echo --------------------------------------
if ! lamnodes; then
	exit $PIPESTATUS
else
	NC=`lamnodes | awk -F : 'BEGIN{NC=0};{NC+=$2};END{print NC}'`
	echo --------------------------------------
	echo $NC CPUs available for scalability test
	echo --------------------------------------
		mpirun -c 1 Pi | tee $$
	for (( C=2; C<=NC; C++ )); do
		mpirun -c $C Pi | tail -1 | tee -a $$
	done

	tail -n +3 $$ > Speedup.dat
	rm $$
	gnuplot <<-"EOF"
#		set terminal x11
		set terminal postscript eps color
		set output 'Speedup.eps'
		set label "Rebuild this graph using ./Speedup.sh"\
		        at screen 0.5,0.9 center
 
		set multiplot

		set xlabel "# PCs"
		set grid
		set title "Time"
		set ylabel "Run Time (s)"
		set size   0.50,0.80
		set origin 0.00,0.05
		plot "Speedup.dat" title "execution time" with lp 3 1

		T1=`awk '$1 ~ /^1$/ {print $2;}' Speedup.dat`

		set title "Speedup"
		set ylabel "Speedup - wrt. Tpar(1)"
		set size   0.50,0.80
		set origin 0.50,0.05
		set key left
		plot "Speedup.dat" using 1:(T1/$2) title "speedup" with lp

		set nomultiplot

		reset
	EOF
	gv Speedup.eps &
fi
