#!/bin/bash
## -----------------------------
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

	SOFT=octave
	CMD="$SOFT -q --eval"
	PROG=Pi
	HF="lam-bhost.def"

if [ -f $PROG.log ]; then
	echo --------------------------------------
	echo Reusing already existing $PROG.log file
	echo --------------------------------------
else
	source ../utils/detect.sh	# sets either LAM / OMPI / UNKNW

  if [ $LAM ]; then
	echo --------------------------------------
	echo List of currently booted nodes in LAM
	echo --------------------------------------
	if ! lamnodes; then exit $PIPESTATUS; fi
	NC=` lamnodes | awk -F : 'BEGIN{NC=0};{NC+=$2};END{print NC}'`
	MPIRUN="mpirun"
  fi # $LAM

  if [ $OMPI ]; then
	echo --------------------------------------
	echo Uncommented lines in bhost file
	echo --------------------------------------
	if ! grep -v "^#" $HF; then exit $PIPESTATUS; fi
	NC=` grep -v "^#" $HF | grep -v "^$" | nl | tail -1 | cut -f1`
	MPIRUN="mpirun -hostfile $HF"
  fi # $OMPI

	echo --------------------------------------
	echo $NC CPUs available for scalability test
	echo --------------------------------------

	for (( C=1; C<=NC; C++ )); do
	  for (( T=1; T<=3; T++ )); do
		echo ___ $C processes ___ turn $T ____
		$MPIRUN -c $C $CMD $PROG
		sleep 1
	  done
	done | tee -a $$
	mv $$ $PROG.log
fi   #  [  -f $PROG.log ];

	grep time $PROG.log | cut -d= -f2 > $PROG.dat

$SOFT -q <<-END
	[stat,hnam]=system("hostname");
	load $PROG.dat
	sz=fliplr(size($PROG)) .* [3,1/3];
	time=min(reshape($PROG,sz));
	subplot(121), plot(         time), grid("on")
			title(["$PROG runtime on " hnam])
			xlabel("processors"), ylabel("seconds")
	subplot(122), plot(time(1)./time), grid("on")
			title(["$PROG speedup on " hnam])
			xlabel("processors"), ylabel("gain")
	print("$PROG.eps","-depsc2")
END
	echo --------------------------------------
	echo REMOVE $PROG.log to repeat time measurements
	echo --------------------------------------
	rm $PROG.dat
	gv $PROG.eps &
