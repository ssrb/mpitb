% startup_wlet		PATH/environ script for MPITB
%			copied from startup_MPITB
%			customized for the Wavelets demo

% _____________________________________________________________________
% | Copyright (C) 2004-05 Javier Fernández Baldomero <jfernand@ugr.es>|
% | Depto. de Arquitectura y Tecnología de Computadores               |
% | ETSI Informática, Universidad de Granada                          |
% | c/ Periodista Daniel Saucedo Aranda s/n                           |
% | 18071-GRANADA SPAIN                                               |
% |___________________________________________________________________|
%
% This file is part of MPITB for Octave
%
% MPITB is free software
% GNU GPL License included for reference in the file gpl.txt


L = LOADPATH;
p = getenv('MPITB_HOME');
if isempty(p)
  p=[getenv('HOME') '/octave/mpitb'];
end
q = [p '/mpi'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (help files)'), end
L=path(L,q);
q = [p '/DLD'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (DLD files)'), end
L=path(L,q);
q = [p '/startups'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (startups)'), end
L=path(L,q);
q = [p '/utils'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (utilities)'), end
L=path(L,q);

q = [p '/Wavelets'];
if ~exist(q, 'dir'), clear p q, error('Wavelets demo not found'), end
L=path(L,q);

q = getenv('MPITB_HOME');
if isempty(q)
  putenv('MPITB_HOME', p)
end
clear p q L

if isempty(getenv('LAMPARENT'))		% Parent task - welcome message
  disp("    Minimal test:")
  disp(["  \tMPI_Init, [a b]=MPI_Initialized, ",...
	"MPI_Finalize, [a b]=MPI_Finalized"	])
  disp("    Examples of environment variables to affect MPI behaviour")
  disp("\tLAM/MPI SSI interface: putenv('LAM_MPI_SSI_rpi', 'lamd'), MPI_Init")
  disp('    Help on MPI: help mpi')
  disp('Help(this demo): help Wavelets')

else					% spawned Octave process

  startup_ncmdsSlv	% NumCommands protocol: several commands are sent

end  

