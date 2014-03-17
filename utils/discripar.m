%DISCRIPAR	Discriminate sequential/parallel time in instrumentation array
%
%	[seq, par] = discripar (instr, parname [, seqtime])
%
%  instr(struc)	instrumentation struct with fields
%	names	timestamp labels
%	times	corresponding timestamps
%  parname(str)	name of parallel[izable] routine
%  seqtime	assuming instr refers to an actual parallel program,
%		if the sequential time is given, the real speedup is printed
%
%  seq	(sequential) time spent in routines called something else
%  par	(parallel  ) time spent in routine parname
%
%  Instrumentation data should contain labels called <parname>_ini / _fin
%  Easiest way is enclosing routine <parname> with corresponding timestamps
%  gross estimation since entry/return time to/from <parname> is not accounted
%
function [seq,par]=discripar(instr,parname,seqtime)

  if nargin<2 || ~isstruct(instr) || ~isstr(parname) ||...
	~isfield(instr,'names')  || ~isfield(instr,'times')
	error('discripar: argchk: see help')
	seq=par=[];
	return
  end

  N=instr.names;    len =length(N);	% 2.1.50->57 changed row->col
  T=instr.times; if len~=length(T),error('annotation arrays mismatch'),end

     init_idx=1;		% time starts counting at T(1,:)
  seq=par=idx=0;		% initially no time spent in either part
  seqflg=true;			% initially code is sequential

     printf('=============== SECTIONS: =====================\n');

  while idx<len
try
     while ~rindex(N{++idx},parname), end	% skip until parname found
catch
     if idx~=len+1				% what? failed why?
	error('logic error indexing in instrumentation array')
     else					% oops, not found, back 1 row
	seqt=T(--idx)-T(init_idx);		% all remaining time sequential
	seq+=seqt;				% accumulate partial
     printf('#%5d - #%5d\tsequential  %f s\n',init_idx,idx,seqt);
	init_idx=idx;				% no more to account
	break					% finish, get out of while loop
     end
end_try_catch
     if ~strcmp(N{idx},[parname '_ini'])	% must be <parname>_ini
	error(['parallel annotation #' int2str(idx) ' starts w/out _ini '])
     end
     pini_idx=idx;
						% now skip until <parname>_fin
     while ~strcmp(N{++idx},[parname '_fin']), end
     pfin_idx=idx;

     seqt=T(pini_idx)-T(init_idx);		% sequential until _ini
     part=T(pfin_idx)-T(pini_idx);		% parallel from _ini to _fin
     seq+=seqt;					% accumulate partials
     par+=part;
     printf('#%5d - #%5d\tsequential  %f s\n',init_idx,pini_idx,seqt);
     printf('\t\t\t\t\t%f s  parallel\t#%5d - #%5d\n',part,pini_idx,pfin_idx);

	 init_idx=pfin_idx;			% keep counting from there

  endwhile

     printf('=============== TOTALS : ======================\n');
     printf('\t\tsequential  %f s\n',seq);
     printf('\t\t\t\t\t%f s  parallel\n',par);
     printf('\t\tTOTAL TIME\t%f s\n',seq+par);
     printf('MAX THEORETICAL SPEEDUP (T/s):\t%f\n',(seq+par)/seq);
if nargin>2 && isscalar(seqtime) && isreal (seqtime)
     printf('REAL            SPEEDUP (S/T):\t%f\n',seqtime/(seq+par));
end
     printf('===============================================\n');

endfunction
