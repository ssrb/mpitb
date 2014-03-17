%CLAS_INSTR	Classify in levels timestamp labels and timespent
%
%	instr = clas_instr
%
%  instr (cell)	instrumentation data classified in a cell array with format:
%	{label, timespent, {};
%	 ....., ........., {};
%	 r_call,globaltime,{more};
%	 ....., ........., {};
%	 label, timespent, {}}
%
%  global time_stamp variables are accessed and cleared after this command
%  first timestamp is subtracted from next ones, so walltimes become timespents
%  subroutine calls should'v been instrumented with "_ini" and "_fin" labels
%  timespent from "<subr>_ini" to "<subr>_fin" is summarized in a single entry
%

function instr=clas_instr

oldPSO=page_screen_output(0);

	printf('\nclassifying instrumentation data... wait...\n');

global	stamp_names   stamp_times     stamp_index
      N=stamp_names;T=stamp_times;len=stamp_index;
	N(len+1:end)=[];	% make sure there are no
	T(len+1:end)=[];	% empty entries in names/times

	T(end+1) = T(1);	% for computing global timespent when i=end
				% works both for T(1)==0 or not
	  INSTR={ {} };		% classified instrumentation data
	  LEV  =   1  ;		% annotation level

clear global stamp_names stamp_times stamp_index

%----------------------------------------------------------------
% instrumentation data will be classified in a cell array with format:
%	{label, timespent, {};
%	 ....., ........., {};
%	 r_call,globaltime,{more};
%	 ....., ........., {};
%	 label, timespent, {}}
% Eventually, some of the annotations will correspond to a subroutine call
% - the timespent will be the global time spent in the routine
%	expected labels are _ini for routine start, _fin for routine end.
%	global time is thus (_fin)-(_ini). Elapsed time from _fin to next label
%	is accounted for in the caller, under a fictitious '-' label.
% - {more} is a breakdown (into corresponding labels) of the routine itself
%          with the same format as the global breakdown.
% - if the routine further calls other routines, recursive breakdown.
%----------------------------------------------------------------
% in this code, INSTR{1} is the global breakdown where data is annotated
% when a _ini label is found, instrumentation data is annotated in INSTR{2}
% when a _fin label is found, INSTR{2} is linked as {more} to INSTR{1}(end,3)
% INSTR{1}(end,1) global routine name is extracted from label by removing _ini
% INSTR{1}(end,2) global time is the sum of the breakdown until _fin
% several checks are done so labels _ini/_fin match and times sum OK.
% INSTR{3}, {4}, etc are used as required by the nesting level reached
%----------------------------------------------------------------
% a fictitious label '-' is added after a _fin label, in caller's level
%----------------------------------------------------------------

i=0; while i<len
	if ~rem(i++,100)
	  printf('\t%2.0f%% done\r',100*i/len);
	end						% annotate even...
	INSTR{LEV}(end+1,:)={N{i},T(i+1)-T(i),{}};	% _ini and _fin labels

    tokens=split(N{i},"_");
    ini_found = strcmp (deblank(tokens(end,:)), 'ini');	% _ini found
    fin_found = strcmp (deblank(tokens(end,:)), 'fin');	% _fin found
       prefix = N{i}(1:rindex(N{i},'_')-1);		% cut either
%----------------
    if ini_found				% pass _ini annotation
	INSTR{LEV+1}     =...			% to next level LEV+1
	INSTR{LEV  }(end,:);			% and save routine name here
	INSTR{LEV++}(end,:)={prefix,  i,{}};	% with current-i for CHECKS
    end						% from now on annotate there
%----------------
    if fin_found				% change _fin annotation
	INSTR{LEV  }{end  ,1}='-';		% to fictitious '-' label
	INSTR{LEV-1}(end+1,:)=...		% and move it to caller level
	INSTR{LEV  }(end  ,:);			% in the called level, replace
	INSTR{LEV  }(end  ,:)={'TOTAL:',0,{}};	% by a 'total' label
	if ~strcmp(				% CHECK: label _ini match
	INSTR{LEV-1}{end-1,1},prefix)
		printf('%s != %s\n',INSTR{LEV-1}{end-1,1},prefix);
		error('clas_instr logic error - _ini/_fin mismatch');
	end
						% CHECK: etime == sum(etimes) ?
 S=sum([INSTR{LEV  }{   : ,2}]);		% should match etime _fin-_ini
  old_i=INSTR{LEV-1}{end-1,2};			% recall saved then-current i
	if S~=T(i)-T(old_i)
		error('clas_instr accounting error - spentime mismatch');
	end
	INSTR{LEV--}(end  ,2)=S;		% place sum in TOTAL label
	INSTR{LEV  }(end-1,:)={prefix,S,...	% link callee breakdown in
	INSTR{LEV+1}};				% caller level before '-' label
    end
%----------------
end
	  printf('\n');

	t=INSTR{1}{end,2};		% change sign of last (reversed) etime
	  INSTR{1}(end,:)={'TOTAL:',-t,{}};
    instr=INSTR{1};			% recall we added T(end+1) = T(1);


       page_screen_output(oldPSO);

endfunction
