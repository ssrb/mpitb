#!/bin/bash
## -----------------------------
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------


	SOFT=octave
	CMD="$SOFT -q --eval"
	PROG=PingPong
	HF="lam-bhost.def"

	source ../utils/detect.sh	# sets either LAM / OMPI / UNKNW

if [ -f $PROG.log ]; then
	echo --------------------------------------
	echo Reusing already existing $PROG.log file
	echo --------------------------------------
else
    if [ $LAM ]; then
	echo --------------------------------------
	echo List of currently booted nodes in LAM
	echo --------------------------------------
	if ! lamnodes; then exit $PIPESTATUS; fi
	NN=` lamnodes | wc -l`
	MPIRUN="mpirun"
  fi # $LAM

  if [ $OMPI ]; then
	echo --------------------------------------
	echo Uncommented non-empty lines in bhost file
	echo --------------------------------------
	if ! grep -v "^#" $HF | grep -v "^$" ; then exit $PIPESTATUS; fi
	NN=` grep -v "^#" $HF | grep -v "^$" | nl | tail -1 | cut -f1`
	MPIRUN="mpirun -hostfile $HF"
  fi # $OMPI

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
	echo ___ up to 32MB ___ send ______	| tee -a $$
	$MPIRUN -c 2 $CMD $PROG"(3E7,'s')"	| tee -a $$

	echo ___ up to 32MB ___ reduce ____	| tee -a $$
	$MPIRUN -c 2 $CMD $PROG"(3E7,'r')"	| tee -a $$

  if [ $LAM ]; then
	echo ___ up to 32MB __ snd _ lamd _	| tee -a $$
$MPIRUN -ssi rpi lamd -c 2 $CMD $PROG"(3E7,'s')"| tee -a $$

	echo ___ up to 32MB __ red _ lamd _	| tee -a $$
$MPIRUN -ssi rpi lamd -c 2 $CMD $PROG"(3E7,'r')"| tee -a $$
  fi # $LAM

# if [ $OMPI ]; then
#	echo ___ up to 32MB __ snd _ MCA? _	| tee -a $$
#$MPIRUN -mca btl ??? -c 2 $CMD $PROG"(3E7,'r')"| tee -a $$
# fi # $OMPI

	mv $$ $PROG.log
fi   #  [  -f $PROG.log ];



	NL=(`nl    $PROG.log | grep Size | cut -f1`)
#	N2=`wc -l $PROG.log | cut -d\  -f1`	# last line
	N2=`nl $PROG.log | tail -1 | cut -f1`
#	NL[${#NL[*]}]=$((N2+2))			# fake 2 lines after
#	NL=(${NL[*]} $((N2+2)))			# to normalize next code
	NL=(${NL[@]} $((N2+2)))


	head -$((NL[1]-2)) $PROG.log | tail -n +$((NL[0]+1)) > sndtcp.dat
	head -$((NL[2]-2)) $PROG.log | tail -n +$((NL[1]+1)) > redtcp.dat
  if [ $LAM ]; then
	head -$((NL[3]-2)) $PROG.log | tail -n +$((NL[2]+1)) > sndlam.dat
	head -$((NL[4]-2)) $PROG.log | tail -n +$((NL[3]+1)) > redlam.dat
     #	w/out fake ->  cat $PROG.log | tail -n +$((NL[3]+1)) > redlam.dat

	PROLOG="
	[stat,hnms]=system\
		('lamnodes|head -2|cut -f2|cut -d: -f1|tr [:space:] -');
	load sndtcp.dat,load redtcp.dat,load sndlam.dat,load redlam.dat,
	XSZ=[sndtcp(:,1)     redtcp(:,1)     sndlam(:,1)     redlam(:,1)];
	YTM=[sndtcp(:,2)     redtcp(:,2)     sndlam(:,2)     redlam(:,2)];
	YBW=[sndtcp(:,3)     redtcp(:,3)     sndlam(:,3)     redlam(:,3)];
	LGND={'snd-recv',   'reduce',       's-r/lamd',     'red/lamd'};
	"
  fi # $LAM
  if [ $OMPI ]; then
	PROLOG="
	[stat,hnms]=system\
	(\"grep -v ^# $HF | grep -v ^$ | head -2|cut -f1|tr [:space:] -\");
	load sndtcp.dat,load redtcp.dat
	XSZ=[sndtcp(:,1)     redtcp(:,1)];
	YTM=[sndtcp(:,2)     redtcp(:,2)];
	YBW=[sndtcp(:,3)     redtcp(:,3)];
	LGND={'snd-recv',   'reduce'};
	"
  fi # $OMPI



$SOFT -q <<-END
	$PROLOG
	subplot(221), plot(XSZ,YTM,"-x"), grid("on")
			title(["$PROG one-way time between\n" hnms])
			xlabel("msg size(bytes)"), ylabel("seconds")
			legend(LGND,2)
	subplot(222), plot(XSZ,YBW,"-x"), grid("on")
			title(["$PROG bandwidth between\n" hnms])
			xlabel("msg size(bytes)"), ylabel("MB/s")
			legend(LGND,4)
	subplot(223), loglog(XSZ,YTM,"-x"), grid("on")
			title("one-way time, logarithmic")
			xlabel("msg size(bytes)"), ylabel("seconds")
			legend(LGND,2)
	subplot(224), loglog(XSZ,YBW,"-x"), grid("on")
			title("bandwidth, logarithmic")
			xlabel("msg size(bytes)"), ylabel("MB/s")
			legend(LGND,4)
	print("$PROG.eps","-depsc2")
END

	echo --------------------------------------
	echo REMOVE $PROG.log to repeat time measurements
	echo --------------------------------------
	rm *.dat
	gv $PROG.eps &

