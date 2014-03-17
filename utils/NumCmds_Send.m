## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------
function NumCmds_Send (nams,vals)
%NUMCMDS_SEND		NumCmds protocol - send named variables
%
%	NumCmds_Send (nams, vals)
%
%  nams (cell-of-strings)	desired names for the given values
%  vals (cell-of-values)	values for the declared names
%
%  Both can be row cells of the same length,
%	and then the same variables are sent to each slave Octave,
%  or vals could be a 2D cell with as many rows as slaves,
%	and then each row is sent to each slave.
%
%  SEE ALSO:	startup_MPITB, startup_ncmdsSlv, NumCmds_Init
%
%  Under NumCmds protocol, slave Octaves repeatedly receive packed buffers with
%  - cmd (str)	the desired command to be evaluated in this iteration
%  - vars	any argument useful for eval(cmd) (w/same name used in cmd)
%
%  This function packs & sends the specified items to NSLAVES (global var)
%  If no 'cmd' var is sent, the previous one is reused (send cmd='' to clear)
%  when cmd is evaluated, vars sent along are available, as well as previous
%  See startup_ncmdsSlv for more details
%

%%%%%%%%%%%%
% GLBCHECK %
%%%%%%%%%%%%
global NEWORLD NSLAVES TAG
if isempty(NEWORLD)
  error('NumCmds_Send: global NEWORLD required (call Octave_Spawn)');
end
if isempty(NSLAVES)
  error('NumCmds_Send: global NSLAVES required (call LAM_Init/Octave_Spawn)');
end
if isempty(TAG)
  error('NumCmds_Send: global TAG required (call NumCmds_Init)');
end

%%%%%%%%%%%%
% ARGCHECK %
%%%%%%%%%%%%
if nargin<2 || ~iscell(nams) || ~iscell(vals) ||...
	       columns(nams) ~= columns(vals) ||...
              (   rows(vals) ~= NSLAVES &&...
                  rows(vals) ~=       1 )
	error('NumCmds_Send: argcheck: see help')
end
	LNMS=length(nams);	ROWS=rows(vals);
for i=1:LNMS
	if ~ischar(nams{i})
	  error('NumCmds_Send: 1st arg must be cell-of-strings')
	end
end

	%% TODO: allow for common part/rank-specific part
	%% so that the first, common part can be packed for all ranks
	%% and the specific part attached for each rank


for j=1:ROWS			% ROWS == ( 1 | NSLAVES )
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % Build variables to Pack/Send %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    for i=1:LNMS
	eval([nams{i} '=vals{j,i};']);	% eval required to interpret varname
    end
    %%%%%%%%%%%
    % Pack'em %
    %%%%%%%%%%%
	SZ = 0;					% prepare to pack
    for i=1:LNMS				% compute total size
	eval(['[info sz]=MPI_Pack_size(' nams{i} ', NEWORLD);']);
	SZ+=sz;					% could change from row to row
    end
	msg=blanks(SZ); pos=0;			% exact size, 1B blanks
    for i=1:LNMS				% pack
	eval(['MPI_Pack(' nams{i} ',msg, pos, NEWORLD);']);
    end
	if pos~=SZ, error('NumCmds_Send: pack logic error'), end

    %%%%%%%%%%%%%%%
    % Send msg to % either slave j (ROWS==NSLAVES) or all (ROWS==1)
    %%%%%%%%%%%%%%%
 if ROWS==1
    for i=1:NSLAVES
	MPI_Send(msg,i,TAG,NEWORLD);		% send to all NSLAVES
    end
 else
	MPI_Send(msg,j,TAG,NEWORLD);		% 1 row/slave, j==1:ROWS
 end

end	% for j=1:ROWS


endfunction

