function info = OCTAVE_DEL_FN (comm, keyval, attr, extra_st)
%OCTAVE_DEL_FN		Suggested Octave attribute-delete procedure -- skeleton
%
%  Every Octave attribute-delete procedure should follow this prototype
%
%  info = DEL_FN (comm, keyval, attr, extra_st)
%
%	where
%   comm     (int)	communicator in which the attribute is stored
%   keyval   (int)	key value for attribute access
%   attr     (var)	attribute value to delete
%   extra_st(cell)	extra information (passed when Keyval_create call)
%   info     (int)	error code (MPI_SUCCESS if no error)
%
%  SEE ALSO: MPI_Keyval_create, MPI_Keyval_free, MPE_Attr_put,
%            MPI_Attr_put, MPI_Attr_get, MPI_Attr_delete
%            MPI_NULL_DELETE_FN, MPI_NULL_COPY_FN, MPI_DUP_FN,
%            caching
%
%  Type this M-file for more information on attribute-delete skeletons
%

printf(
'-----------------------------------------------------------------------\n');
printf(
'For information purposes, this wrapper will not use but wants to print:\n');
printf('0.-Current communicator MPI_COMM_WORLD: %d\n', MPI_COMM_WORLD);
printf('1.- Passed comm  communicator argument: %d\n', comm  );
printf('2.- Passed keyval integer     argument: %d\n', keyval);
printf('3.- Passed attr. octave_value argument: '             );
							attr % ;
printf('4.- The extra_st octave_value argument: '             );
							extra_st % ;
printf(
"----------------------------------------------------------------------\n");
disp(
'With the current MPITB design, only MPI_Keyval_free.oct should destroy attr')
disp(
'No work left for an Octave-M-file callback (see Keyval.cc source code)')
printf(
"----------------------------------------------------------------------\n");

attr =          [];		% unneeded, uneffective, useless
info = MPI_SUCCESS;

