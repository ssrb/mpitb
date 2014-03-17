function [info,attr_out,flag] = OCTAVE_COPY_FN(OLDcomm,keyval,extra_st,attr_in)
%OCTAVE_COPY_FN		Suggested Octave attribute-copy   procedure -- skeleton
%
%  Every Octave attribute-copy   procedure should follow this prototype
%
%  [info attr_out flag] = COPY_FN (OLDcomm, keyval, extra_st, attr_in)
%
%	where
%   OLDcomm  (int)	communicator in which the attribute is stored
%   keyval   (int)	key value for attribute access
%   extra_st(cell)	extra information (passed when Keyval_create call)
%   attr_in  (var)	attribute value
%   info     (int)	error code (MPI_SUCCESS if no error)
%   attr_out (var)	attribute copy (if flag==1)
%   flag     (int)	0 if attr_out is meaningless, 1 if not
%
%  SEE ALSO: MPI_Keyval_create, MPI_Keyval_free, MPE_Attr_put,
%            MPI_Attr_put, MPI_Attr_get, MPI_Attr_delete
%            MPI_NULL_DELETE_FN, MPI_NULL_COPY_FN, MPI_DUP_FN,
%            caching
%
%  Type this M-file for more information on attribute-copy skeletons
%

printf(
'-----------------------------------------------------------------------\n');
printf(
'For information purposes, this wrapper will not use but wants to print:\n');
printf('0.-Current communicator MPI_COMM_WORLD: %d\n', MPI_COMM_WORLD);
printf('1.- The OLDcomm  communicator argument: %d\n', OLDcomm);
printf('2.- The keyval   integer      argument: %d\n', keyval );
printf('3.- The extra_st octave_value argument: '             );
						       extra_st	% ;
printf('---------------------------\n');
printf('Copying attribute keyed %d:\n', keyval);
disp  (attr_in)
printf('---------------------------\n');

% ------------------------------------------------
% Octave-user method: will work only once
% On the next MPI_Comm_dup, this assignment will overwrite previous one
% and registered LAM/MPI pointers will be meaningless
% ------------------------------------------------
% attr_out = attr_in;
% attr_out = <some other octave_value>;
% ------------------------------------------------

% ------------------------------------------------
% MPITB internal method: works forever
% On each MPI_Comm_dup, attr_out is made to point to the orig. attribute copy
% ------------------------------------------------
% attr_out = 'MPITB_INTERNAL';
% ------------------------------------------------

attr_out = 'MPITB_INTERNAL';
info     =    MPI_SUCCESS  ;
flag     =              1  ;

