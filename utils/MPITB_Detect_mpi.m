## Copyright (C) 2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

function mpi=MPITB_Detect_mpi
% MPITB_Detect_mpi	Guess which MPI DLD is being used: LAM/MPI or Open MPI
%
%  mpi = MPITB_detect_mpi
%
%  mpi	(str)	Returned MPI identification string: 'LAM/MPI' or 'Open MPI'
%
%  Examples:
%
%  switch MPITB_Detect_mpi		mpi=MPITB_Detect_mpi;
%    case "LAM/MPI",	disp('LAM');	 if strcmp(mpi,"Open MPI"), disp('OMPI')
%    case "Open MPI",	disp('OMPI');	elseif strcmp(mpi,"LAM/MPI"),disp('LAM')
%    otherwise,	error('unknown');	else, error('unknown');
%  endswitch				end
%
%  SEE ALSO: MPITB_Init, MPITB_Clean
%

pti = getenv('MPITB_HOME');		% Path-To-Init
if isempty(pti), error("MPITB_HOME environ var unset ?!?\n"), end
pti = [pti '/DLD/MPI_Init.oct'];

[st ldd]=system(['ls ' pti]);		% do check whether it exists indeed
if st || isempty(ldd) || length(strfind(ldd,"\n"))~=1
		error("cannot ls a single MPI_Init.oct file ?!?\n")
end
					% get which libmpi.so it depends on
[st ldd]=system(['ldd ' pti ' | grep libmpi.so |  cut -d\  -f3 ']);
if st || isempty(ldd) || length(strfind(ldd,"\n"))~=1
		error("cannot ldd a single libmpi.so dependency ?!?\n")
end
		ldd(end)=[];		% remove trailing LF

	% -----------------------------	% identify libmpi.so
[st msg]=system(['strings ' ldd ' | grep "^LAM/MPI" ']);
if ~st	% -----------------------------	%  LAM/MPI evidency
    [st ldd]=system(['ldd ' pti ' | grep liblam.so ']);
    if st || isempty(ldd) || length(strfind(ldd,"\n"))~=1
		error(["cannot ldd liblam.so dependency ?!?\n",...
		"your libmpi.so is LAM/MPI but ",...
		"your MPI_Init.oct DLD is not linked to liblam.so\n"])
    end
    [st ldd]=system(['ldd ' pti ' | grep libopen- ']);
    if ~st
		error(["ldd found unexpected libopen-* dependencies ?!?\n",...
		"your libmpi.so is LAM/MPI but ",...
		"your MPI_Init.oct DLD is linked to libopen-(*).so\n"])
    end
	mpi='LAM/MPI'; return
end
	% -----------------------------	% identify libmpi.so
[st msg]=system(['strings ' ldd ' | grep "^Open MPI" ']);
if ~st	% -----------------------------	%  Open MPI evidency
    [st ldd]=system(['ldd ' pti ' | grep libopen- ']);
    if st || isempty(ldd) || length(strfind(ldd,"\n"))~=2
		error(["cannot ldd libopen-rte/-pal.so dependencies ?!?\n",...
		"your libmpi.so is OpenMPI but ",...
		"your MPI_Init.oct DLD is not linked to libopen-rte/pal.so\n"])
    end
    [st ldd]=system(['ldd ' pti ' | grep liblam.so ']);
    if ~st
		error(["ldd found unexpected liblam.so dependencies ?!?\n",...
		"your libmpi.so is OpenMPI but ",...
		"your MPI_Init.oct DLD is linked to liblam.so\n"])
    end
	mpi='Open MPI'; return
end

	mpi=''; return			% No MPI found.

