#!/bin/bash
	SOFT=octave
	CMD="$SOFT -q --funcall"
	PROG=Ping2173
	PROr=Ping2173r

if [ -f $PROG.log ]; then
	echo --------------------------------------
	echo Reusing already existing $PROG.log file
	echo --------------------------------------
else
	echo --------------------------------------
	echo List of currently booted nodes in LAM
	echo --------------------------------------
	if ! lamnodes; then exit $PIPESTATUS; fi
	NN=` lamnodes | wc -l`
   if ((NN < 2)); then
	echo --------------------------------------
	echo $NN nodes available for ping-pong test - too few
	echo --------------------------------------
	exit
   fi
   if ((NN > 2)); then
	echo --------------------------------------
	echo $NN nodes available for ping-pong test - will use first two
	echo --------------------------------------
   fi
	echo ___ up to 64MB ___ send ______	| tee -a $$
	mpirun -c 2 $CMD $PROG			| tee -a $$

	echo ___ up to 64MB ___ reduce ____	| tee -a $$
	mpirun -c 2 $CMD $PROr			| tee -a $$

	echo ___ up to 64MB __ snd _ lamd _	| tee -a $$
mpirun -ssi rpi lamd -c 2 $CMD $PROG		| tee -a $$

	echo ___ up to 64MB __ red _ lamd _	| tee -a $$
mpirun -ssi rpi lamd -c 2 $CMD $PROr		| tee -a $$

	mv $$ $PROG.log
fi   #  [  -f $PROG.log ];

#	NL=(` nl    $PROG.log | grep Size | cut -f1`)
	NL=(` nl Ping2173.log | grep Size | cut -f1`)
echo $NL
	head -$((NL[1]-2)) $PROG.log | tail -n +$((NL[0]+1)) > sndtcp.dat
	head -$((NL[2]-2)) $PROG.log | tail -n +$((NL[1]+1)) > redtcp.dat
	head -$((NL[3]-2)) $PROG.log | tail -n +$((NL[2]+1)) > sndlam.dat
#	head -$((NL[4]-2)) $PROG.log | tail -n +$((NL[3]+1)) > redlam.dat
		       cat $PROG.log | tail -n +$((NL[3]+1)) > redlam.dat

$SOFT -q <<-END
	      hnms =system\
		("lamnodes|head -2|cut -f2|cut -d: -f1|tr [:space:] -");
	load sndtcp.dat,load redtcp.dat,load sndlam.dat,load redlam.dat
	XSZ=[sndtcp(:,1)     redtcp(:,1)     sndlam(:,1)     redlam(:,1)];
	YTM=[sndtcp(:,2)     redtcp(:,2)     sndlam(:,2)     redlam(:,2)];
	YBW=[sndtcp(:,3)     redtcp(:,3)     sndlam(:,3)     redlam(:,3)];
	LGND={"snd-recv",   "reduce",       "s-r/lamd",     "red/lamd"};
	
	__gnuplot_set__ terminal postscript	% remorselessly copied
	__gnuplot_set__ output "$PROG.ps"	% from "help print"
	
	subplot(221), plot(XSZ,YTM,"-x"), grid("on")
			title(["$PROG one-way time between\n" hnms])
			xlabel("msg size(bytes)"), ylabel("seconds")
	#		legend(LGND,2)
	subplot(222), plot(XSZ,YBW,"-x"), grid("on")
			title(["$PROG bandwidth between\n" hnms])
			xlabel("msg size(bytes)"), ylabel("MB/s")
	#		legend(LGND,4)
	subplot(223), loglog(XSZ,YTM,"-x"), grid("on")
			title("one-way time, logarithmic")
			xlabel("msg size(bytes)"), ylabel("seconds")
	#		legend(LGND,2)
	subplot(224), loglog(XSZ,YBW,"-x"), grid("on")
			title("bandwidth, logarithmic")
			xlabel("msg size(bytes)"), ylabel("MB/s")
	#		legend(LGND,4)
	
	%print("$PROG.eps","-depsc2")		% known bug & workaround
	oneplot					% for octave-2.1.73
	__gnuplot_set__ terminal x11
	__gnuplot_set__ output

END
	echo --------------------------------------
	echo REMOVE $PROG.log to repeat time measurements
	echo --------------------------------------
	rm *.dat
	gv $PROG.ps &

