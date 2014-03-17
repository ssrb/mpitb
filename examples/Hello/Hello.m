## Copyright (C) 2006-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

function Hello
% The classical greeting: "Hello (MPI_COMM_) world! I'm rank m/n"
% Demo suggested by Alain Garon, to have a really simple demo available
%
%  Examples:
%
%  $ mpirun -c 2 octave -q --eval Hello	# LAM: 2 copies from command-line
%
%  $ mpirun -c 2 [-H h1,h2]		# Open-MPI -host & -hostfile syntaxes
%		 [-hostfile lam-bhost.def]
%		 octave -q --eval Hello
%
%  octave:1> Hello			% only 1 rank possible from octave
%  Hello, MPI_COMM_world! I'm rank 0/1
%

## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------

   info       =  MPI_Init;
  [info rank] =  MPI_Comm_rank (MPI_COMM_WORLD);
  [info size] =  MPI_Comm_size (MPI_COMM_WORLD);
  [info name] =  MPI_Get_processor_name;

 fprintf("Hello, MPI_COMM_world! I'm rank %d/%d (%s)\n", rank, size, name);

   info       =  MPI_Finalize;

   MPITB_Clean		% clear all MPI_... DLDs from memory... why?
			% comment it & run twice Hello from octave prompt
