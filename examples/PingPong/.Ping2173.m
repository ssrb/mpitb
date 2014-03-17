function Ping2173
%  Example:
%
%  mpirun -c 2 octave-2.1.73 -q --funcall Ping2173	# 2 ranks required
%
%	Ping2173.1.73 calls "PingPong(...)" and then "exit", since
%	--funcall in 2.1.73 didn't exit		and didn't allow arguments
%	(--eval in 2.9.12 does exit)		(and does allow args)
%
%	it also adjusts some screen paging variable required in 2.1.73
%
%	also, if you want to run this 2.1.73 version of PingPong
%	remember to link .octaverc -> s2.1.73 instead to startup...
%	since s2.1.73 uses the old LOADPATH variable (and var=system())
%	(new 2.9.12 startup uses modern addpath and [stat var]=system())
%

 oldPSO=page_screen_output;
	page_screen_output=0;
PingPong(1E8,'s')
	page_screen_output=oldPSO;
exit
