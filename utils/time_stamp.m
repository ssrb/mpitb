%TIME_STAMP	Annotate timestamp label (with walltime, of course)
function time_stamp(name)
 global	stamp_names   stamp_times    stamp_index
	stamp_names(++stamp_index  )=name ;
	stamp_times(  stamp_index  )=MPI_Wtime;

%	useful when debugging instrumented code
%	disp(name); fflush(stdout);
%
endfunction
