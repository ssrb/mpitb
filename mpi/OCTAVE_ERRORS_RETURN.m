function OCTAVE_ERRORS_RETURN (comm, errclass, error, errmsg)
%OCTAVE_ERRORS_RETURN	Suggested OCTAVE ErrorHandler procedure
%
%  Every OCTAVE Errhandler should follow this prototype
%
%  OctaveCMD (comm, errclass, error, errmsg)
%		comm     (int)	communicator origin of the error
%		errclass (int)	error class
%		error    (int)	error code
%		errmsg   (str)	error message
%
%  Type this M-file to have a hint on howto build Errhandlers
%

printf(
"----------------------------------------------------------------------\n");
disp  ('returned error:')
printf('errclass==%d, error==%d, errmsg==%s\n',errclass, error, errmsg);
    if comm==MPI_COMM_WORLD, disp('in MPI_COMM_WORLD')
elseif comm==MPI_COMM_SELF,  disp('in MPI_COMM_SELF')
elseif comm==MPI_COMM_NULL,  disp('in MPI_COMM_NULL')
else,			     disp('in other comm, not WORLD/SELF/NULL')
end
disp(
'won''t take any action on it, except for writing its MPI_Error_string:')
printf(
"----------------------------------------------------------------------\n");
[info msg] = MPI_Error_string(errclass); disp(msg)

