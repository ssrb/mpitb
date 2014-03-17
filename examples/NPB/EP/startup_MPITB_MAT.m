% STARTUP_NAS		PATH/environ script for MPITB
%			copied from startup_MPITB
%			customized for the NAS Benchmarks

% _____________________________________________________________________
% | Copyright (C) 2000-05 Javier Fernández Baldomero <jfernand@ugr.es>|
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

% q = [p '/NPB'];
% if ~exist(q, 'dir'), clear p q, error('NAS Parallel Benchmarks not found'), end
% addpath(q);

%%%%%%%%%%%%%%%%
% MPITB envvar %
%%%%%%%%%%%%%%%%
q = getenv('MPITB_ROOT');
if isempty(q)
  putenv(['MPITB_ROOT=' p])
end
clear p q

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Greeting / Slave startup %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if isempty(getenv('LAMPARENT')) &&...	% Parent task - welcome message
   isempty(getenv('OMPI_MCA_universe'))
% disp('Set SSI rpi to tcp with the command:')
% disp('  putenv([''LAM_MPI_SSI_rpi=tcp'']), MPI_Init')
% disp('Help on MPI: help mpi')
% disp('Help on this demo: help <myappname>')

else					% spawned MATLAB process
	%% slave startup can also be controlled by using "matlab -r" switch
	%% there is a selection of startup protocols in the startups subdir

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	%% this line commented out/in for the TUTORIAL %%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% startup_mergeParent                   % this line, remove the %
% startup_mergeParent                   % yup, keep it commented out

	%% these lines used to invoke the different slave startup protocols
	%% see examples in the Spawn/PingPong/Pi/Wavelets subdirs

% startup_bcastSlv	% BCast protocol: 1 command is sent using MPI_Bcast
% startup_ncmdsSlv	% NumCommands protocol: several commands are sent

end  

