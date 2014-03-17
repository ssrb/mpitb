% STARTUP_MPITB		PATH/environ script for MPITB
%			(add to/replacement for) startup.m
%			customize the <myappname>-marked sections -> Mandelbrot

% _____________________________________________________________________
% | Copyright (C) 2000-07 Javier Fernández Baldomero <jfernand@ugr.es>|
% |                       Mancia Anguita López                        |
% | Depto. de Arquitectura y Tecnología de Computadores               |
% | ETSI Informática, Universidad de Granada                          |
% | c/ Periodista Daniel Saucedo Aranda s/n                           |
% | 18071-GRANADA SPAIN                                               |
% |___________________________________________________________________|
%
% This file is part of MPITB for MATLAB
%
% You can copy and use MPITB in the same terms of LAM/MPI
% (see http://www.lam-mpi.org/community/license.php)
% replacing names where sensible (MPITB instead of LAM, etc)
% LAM/MPI License included for reference in the file LAM_license.txt

%%%%%%%%
% PATH %
%%%%%%%%
p = getenv('MPITB_ROOT');
if isempty(p)
  p=[getenv('HOME') '/matlab/mpitb'];
     putenv(['MPITB_ROOT=' p])
end
q = [p '/mpi'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (help files)'), end
addpath(q);
q = [p '/mpi/MEX'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (MEX files)'), end
addpath(q);		% so that MEX files with same name as M files come 1st
q = [p '/startups'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (startups)'), end
addpath(q);
q = [p '/utils'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (utilities)'), end
addpath(q);

  q = [p '/Mandelbrot'];
  if ~exist(q, 'dir'), clear p q, error('Mandelbrot demo not found'), end
  addpath(q);

clear p q

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Greeting / Slave startup %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if isempty(getenv('LAMPARENT')) &&...	% Parent task - welcome message
   isempty(getenv('OMPI_MCA_universe'))
  disp("    Minimal test:")
  disp(["  \tMPI_Init, [a b]=MPI_Initialized, ",...
	"MPI_Finalize, [a b]=MPI_Finalized"	])
  disp("    Examples of environment variables to affect MPI behaviour")
  disp("\tLAM/MPI SSI iface: putenv('LAM_MPI_SSI_rpi',     'lamd'), MPI_Init")
  disp("\tOpenMPI MCA iface: putenv('OMPI_MCA_btl_base_debug','2'), MPI_Init")
  disp('    Help on MPI: help mpi')
  disp('Help(this demo): help Mandelbrot')
  disp('Help(this demo): help mandelbrot mandelStrp mandelStrd mandelTBag')

else					% spawned MATLAB process
	%% slave startup can also be controlled by using "matlab -r" switch
	%% there is a selection of startup protocols in the startups subdir

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	%% this line commented out/in for the TUTORIAL %%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% startup_mergeParent			% this line, remove the %
% startup_mergeParent			% yup, keep it commented out

	%% these lines used to invoke the different slave startup protocols
	%% see examples in the Spawn/PingPong/Pi/Wavelets subdirs

% startup_bcastSlv	% BCast protocol: 1 command is sent using MPI_Bcast
% startup_ncmdsSlv	% NumCommands protocol: several commands are sent

end

