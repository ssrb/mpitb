## Copyright (C) 2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------
function MPITB_Init(nslaves, hosts, otherinfo)
% MPITB_Init	Prepare Octave session for a future Octave_Spawn
%		under LAM/MPI: call to LAM_Init, which lamboot's and MPI_Init's
%		under OpenMPI: call to OMPI_Init, which creates INFO/MPI_Init's
%
%  MPITB_Init(nslaves, hosts, otherinfo)
%
%  nslaves   (int)	desired number of slave nodes to use
%  hosts (cellstr)	list of hostnames (at least nslaves+1)
%  otherinfo (str)	for LAM_Init, desired rpi to lamboot ('tcp'/'lamd')
%			this is deprecated, obsoleted by the SSI/MCA interface
%			don't use this 3rd argument, it will be removed soon
%
%  SEE ALSO: LAM_Init, OMPI_Init, MPITB_Clean, MPITB_detect_mpi
%

switch MPITB_Detect_mpi
    case "LAM/MPI",	LAM_Init
    case "Open MPI",	OMPI_Init
    otherwise,		error("can't Init: unknown MPI / no MPI detected");
endswitch        

