%
% MPITB Utilities
%
% Protocol:
%
% LAM_Init	lamboot (if required) and MPI_Init (if required)
%		LAM_Init [ ( nslaves [, rpi [, hosts] ] ) ]
%
% OCTAVE_SPAWN	Spawn an (optionally) given number of slave Octaves
%		[children] = Octave_Spawn [ ( nOctaves [, infokey] ) ]
%
% NUMCMDS_INIT	NumCmds protocol initialization
%		NumCmds_Init [ ( ncmd [, qflg [, tagn [, dbug] ] ] ) ]
%
% NUMCMDS_SEND	NumCmds protocol - send named variables
%		NumCmds_Send (nams, vals)
%
% LAM_CLEAN     Clean MPITB MEX files from memory
%
% RPI_STR	Full LAM SSI RPI string given initial letter(s)
%		rpi = rpi_str (c)
%
% Instrumentation:
%
% INIT_STAMP	Initializes timestamp array, labels cell, and index
%		init_stamp (nlabels)
%
% TIME_STAMP	Annotate timestamp label (with walltime, of course)
%		time_stamp(name)
%
% SAVE_INSTR	Save (to file) and return instrumentation struct
%		instr = save_instr (series,index)
%
% LOAD_INSTR	Load instrumentation data from file
%		instr = load_instr('series', index);
%
% PRINTSTAMP	Pretty-print timestamp labels and timespent
%		printstamp (instr)
%
% LAST_STAMP	Print just last timestamp label and global time spent
%		last_stamp (instr)
%
% CLASSSTAMP	Classify in levels, then browse, timestamp labels and timespent
%		clinstr = classstamp (instr)
%
% DISCRIPAR	Discriminate sequential/parallel time in instrumentation array
%		[seq, par] = discripar (instr, parname [, seqtime])
%
