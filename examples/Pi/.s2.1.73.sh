#!/bin/bash
	SOFT=octave
	CMD="$SOFT -q --funcall"
	PROG=Pi_2_1_73

if [ -f $PROG.log ]; then
	echo --------------------------------------
	echo Reusing already existing $PROG.log file
	echo --------------------------------------
else
	echo --------------------------------------
	echo List of currently booted nodes in LAM
	echo --------------------------------------
	if ! lamnodes; then exit $PIPESTATUS; fi
	NC=` lamnodes | awk -F : 'BEGIN{NC=0};{NC+=$2};END{print NC}'`
	echo --------------------------------------
	echo $NC CPUs available for scalability test
	echo --------------------------------------

	for (( C=1; C<=NC; C++ )); do
	  for (( T=1; T<=3; T++ )); do
		echo ___ $C processes ___ turn $T ____
		mpirun -c $C $CMD $PROG
		sleep 1
	  done
	done | tee -a $$
	mv $$ $PROG.log
fi   #  [  -f $PROG.log ];

	grep time $PROG.log | cut -d= -f2 > $PROG.dat

$SOFT -q <<-END
	      hnam =system("hostname");
	load $PROG.dat
	sz=fliplr(size($PROG)) .* [3,1/3];
	time=min(reshape($PROG,sz));
	
	__gnuplot_set__ terminal postscript	% remorselessly copied
	__gnuplot_set__ output "$PROG.ps"	% from "help print"
	
	subplot(121), plot(         time), grid("on")
			title(["$PROG runtime on " hnam])
			xlabel("processors"), ylabel("seconds")
	subplot(122), plot(time(1)./time), grid("on")
			title(["$PROG speedup on " hnam])
			xlabel("processors"), ylabel("gain")

	%print("$PROG.eps","-depsc2")		% known bug & workaround
	oneplot					% for octave-2.1.73
	__gnuplot_set__ terminal x11
	__gnuplot_set__ output
END
	echo --------------------------------------
	echo REMOVE $PROG.log to repeat time measurements
	echo --------------------------------------
	rm $PROG.dat
	gv $PROG.ps &
