%LAST_STAMP	Annotate final timestamp, and print global time spent
%
%	last_stamp ()
%
%  instr(struc)	instrumentation data, with fields
%	 names	annotated labels
%	 times	corresponding timestamps
%

function last_stamp(instr)
 global stamp_names   stamp_times    stamp_index
        stamp_names(++stamp_index  )='END'    ;
        stamp_times(  stamp_index  )=MPI_Wtime;

	printf('TOTAL TIME:\t- %7.3fs\n',...
	stamp_times(stamp_index)-...
	stamp_times(          1) );

	printf('Annotated : %d entries\n', stamp_index);

endfunction
