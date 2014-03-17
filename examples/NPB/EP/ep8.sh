#!/bin/bash
#
# ep8	run ep scalability test from 8 to 1 PCs
#	compacting the last 2 tests as soon as 1 - 2 PCs are left free
#
# Usage: ep8 <logfile> <soft> <class>
# where:      soft = F | M | O	(FORTRAN | MATLAB | Octave)
#	      class= S | W | A | B | C	(2^24 | 25 | 28 | 30 | 32)
# screen output saved to log file <logfile>.<soft>.ep.<class>.8-1.log
#
#	#PCs	8  7  6   5    4      3         2               1
#       time  1/81/71/6 1/5  1/4    1/3       1/2             1/1
#	       ._._.__.___.____.______. ... ._________._______________.
# nodes	  n0   | | |  |   |    |      |     |         |_______________|
#	  n1   | | |  |   |    |      | ... |_________|
#	  n2   | | |  |   |    |______|
#	  n3   | | |  |   |____|
#	  n4   | | |  |___|_____.          1/4+1/3 = 0.58 > 0.5
#	  n5   | | |__|         |      1/5+1/4     = 0.45 ~ 0.5
#	  n6   | |_|__|_________|____. 1/5+1/4+1/3 = 0.78 > 0.5
#	  n7   |_|___________________| 1/7+...+1/3 = 1.09 > 1.0
#
# for a perfectly scalable application in which runtime = seq.time / #PCs
# whole runtime would be 1/8+1/7+...        +1/2+1 = 2.72
# it can be reduced to   1/8+1/7+...+1/4+1/3       = 1.22 < 2.72/2
# 			 less than half, slightly more than seq.time (20%, 1/5)
# since the sequential can be run as soon as node n7 is freed (after 8 PC's)
#		  during the 7/6/5/4/3 PC's runtime (1/7+...+1/3 = 1.09 > 1.0)
# and the 2 PC's test can be run as soon as 2 more nodes are freed (n5-6)
#		  during the     5/4/3 PC's runtime (1/5+...+1/3 = 0.78 > 0.5)

if [ "$#" -ne 3 ]; then
	echo $"Usage: ep8 <logfile> <soft> <class>"
	echo $"where: soft = F | M | O		(FORTRAN | MATLAB | Octave)"
	echo $"       class= S | W | A | B | C	(2^24 | 25 | 28 | 30 | 32)"
	echo $"	screen output appended to <logfile>.<soft>.ep.<class>.8-1.log"
	exit 4
fi

case $3 in
  S|W|A|B|C) class=$3	;;
  *)	echo $"$3 not a valid class"
	exit 3	;;
esac
case $2 in
  F)	soft="../bin/ep.$class.1"
			;;
  M)	soft="matlab -nodisplay -nosplash -nojvm -r \"ep('$class');quit\""
			;;
  O)	soft="octave -q --eval ep('$class')"
			;;
  *)	echo $"$2 not a valid software"
	exit 2	;;
esac

	logfile="$1.$2.ep.$3.8-1.log"
	LF="$1"


echo $"mpirun     n0-7   alone"
eval  "mpirun     n0-7"  $soft  "> $LF.8"

echo $"                  there is one free PC"
echo $"mpirun       n7   in the background till end"
eval  "mpirun -nw   n7"  $soft  "> $LF.1"

echo $"mpirun     n0-6"
eval  "mpirun     n0-6"  $soft  "> $LF.7"

#cho $"                  there is one free PC"
#cho $"mpirun       n6   in the background till end"
#val  "mpirun -nw   n6"  $soft  "> $LF.1"

echo $"mpirun     n0-5"
eval  "mpirun     n0-5"  $soft  "> $LF.6"

echo $"                  there are two more free PCs"
echo $"mpirun     n5-6   in the background till end"
eval  "mpirun -nw n5-6"  $soft  "> $LF.2"

echo $"mpirun     n0-4"
eval  "mpirun     n0-4"  $soft  "> $LF.5"

#cho $"                  there are two more free PCs"
#cho $"mpirun     n4-5   in the background till end"
#val  "mpirun -nw n4-5"  $soft  "> $LF.2"

echo $"mpirun     n0-3"
eval  "mpirun     n0-3"  $soft  "> $LF.4"

echo $"mpirun     n0-2"
eval  "mpirun     n0-2"  $soft  "> $LF.3"

cat $LF.8 $LF.7 $LF.6 $LF.5 $LF.4 $LF.3 $LF.2 $LF.1 >> $logfile
rm  $LF.8 $LF.7 $LF.6 $LF.5 $LF.4 $LF.3 $LF.2 $LF.1
