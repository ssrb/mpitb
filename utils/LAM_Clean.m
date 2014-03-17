## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

% LAM_CLEAN	Clean MPITB MEX files from memory
%
%  Most probably used in the following sequence:
%
%  MPI_Init
%  ...
%  <MPITB code>
%  ...
%  MPI_Finalize;
%  LAM_Clean;		% required to avoid
%  MPI_Init;		% matlab crash due to MPI re-init
%
%  See MPI_Init help page for more details
%  If you init with LAM_Init, you can match it with LAM_Clean
%	and skip MPI_Finalize if you want.
%	LAM_Clean uses MPI_Finalized to learn what to do.
%
%  This utility is deprecated to adapt to other implementations (like OMPI)
%  changing names LAM_Init/LAM_Clean -> MPITB_Init/MPITB_Clean
%  which will be (hopefully) more meaningful to OMPI users.
%

% function LAM_Clean	% can't be, won't work as function
 
%  [M,MEX]=inmem;		% clear all MPI_* MEX files
%   M=MEX(strmatch('MPI_',MEX));
%  clear(M{:})                   % allow for MPI_Init again

  [LAM_Clean_info LAM_Clean_flgIni]=MPI_Initialized;
  [LAM_Clean_info LAM_Clean_flgFin]=MPI_Finalized;
if LAM_Clean_flgIni &&...
  ~LAM_Clean_flgFin
	MPI_Finalize;
end

   clear MPI_* LAM_Clean_*
