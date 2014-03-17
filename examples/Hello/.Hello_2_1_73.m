## Copyright (C) 2006-2007 Javier Fernández Baldomero
##
## This program is free software
## -----------------------------

function Hello_2_1_73
%  Example:
%
%  mpirun -c 2 octave-2.1.73 -q --funcall Hello_2_1_73	# 1, 2 or more copies
%
%	Hello_2_1_73 calls "Hello" and then "exit", since
%	--funcall in 2.1.73 didn't exit
%	(--eval in 2.9.12 does exit)
%
%	also, if you want to run this 2.1.73 version of Hello
%	remember to link .octaverc -> s2.1.73 instead to startup...
%	since s2.1.73 uses the old LOADPATH variable (and var=system())
%	(new 2.9.12 startup uses modern addpath and [stat var]=system())
%

## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es
## Keywords: parallel MPI
## ----------------------------------------------------

Hello
exit

