function Pi_2_1_73
%  Example:
%
%  mpirun -c 5 octave-2.1.73 -q --funcall Pi_2_1_73	# parallel run, 5 ranks
%
%	Pi_2.1.73 calls "Pi(...)" and then "exit", since
%	--funcall in 2.1.73 didn't exit		and didn't allow arguments
%	(--eval in 2.9.12 does exit)		(and does allow args)
%
%	also, if you want to run this 2.1.73 version of Pi
%	remember to link .octaverc -> s2.1.73 instead to startup...
%	since s2.1.73 uses the old LOADPATH variable (and var=system())
%	(new 2.9.12 startup uses modern addpath and [stat var]=system())
%

Pi(1E7,'s')
exit
