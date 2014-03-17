## Copyright (C) 2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

% MPITB_CLEAN	Clean MPITB MEX files from memory
%
%  Most probably used in the following sequence:
%
%  MPI_Init
%  ...
%  <MPITB code>
%  ...
%  MPI_Finalize;
%  MPITB_Clean;		% required to avoid
%  MPI_Init;		% octave crash due to MPI re-init
%
%  See MPI_Init help page for more details
%  You can also skip MPI_Finalize, since MPITB_Clean will Finalize for you.
%	MPITB_Clean uses MPI_Finalized to learn what to do.
%  i.e. you can match MPI_Init with MPITB_Clean if you want.
%  you can also match MPITB_Init with MPITB_Clean.
%

% function MPITB_Clean	% can't be, won't work as function
 
%  [M,MEX]=inmem;		% clear all MPI_* MEX files
%   M=MEX(strmatch('MPI_',MEX));
%  clear(M{:})                   % allow for MPI_Init again

  [MPITB_Clean_info MPITB_Clean_flgIni]=MPI_Initialized;
  [MPITB_Clean_info MPITB_Clean_flgFin]=MPI_Finalized;
if MPITB_Clean_flgIni &&...
  ~MPITB_Clean_flgFin
	MPI_Finalize;
end

   clear MPI_* MPITB_*

