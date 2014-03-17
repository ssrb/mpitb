%LOOK_INSTR	Browse previously classified (clas_instr) instrumentation data
%
%	look_instr (instr)
%
%  instr (cell)	instrumentation data (as returned by clas_instr/load_instr)
%
%  An interactive dialog is started, where user is shown numbered labels
%  at certain level, the corresponding timespent and global timespent.
%  By typing numbers, a label position can be expanded to list the
%  timestamps comprised under that label (at a lower level), etc.
%
function look_instr(instr)

oldPSO=page_screen_output(0);

	INSTR{1}=instr;		% we'll promote instr{index,3} to INSTR{LEVEL+1}
				% each time we expand a subroutine call
%----------------------------------------------------------------
% instrumentation data hasbeen classified in a cell array with format:
%	{label, timespent, {};
%	 ....., ........., {};
%	 r_call,globaltime,{more};
%	 ....., ........., {};
%	 label, timespent, {}}
%----------------------------------------------------------------
%----------------------------------------------------------------
% classified data will be printed one level at a time
% keyboard controls:
% - digit: expand label numbered 'digit' (press quick to fit more digits in)
% - u    : up, return to caller
% - q    : quit
%----------------------------------------------------------------

  FIGURES=true;
  NPLOTS=4;					% plots per figure
  LEV=1; REPLOT=0;
  calltree=INSTR{LEV}{1,1};			% take program name
  calltree=calltree(1:rindex(calltree,'_')-1);	% cut trailing _start

  if FIGURES					% 1st figure setup
    figure(ceil(LEV/NPLOTS));
    gset('multiplot')
    gset('nokey')
    gset('noytics')
    PLTSIZ=1/NPLOTS;
    gset(['size 1.0,' num2str(PLTSIZ)])		% size constant for all plots
  end

  while LEV
    if FIGURES					% position & title change
	PLTPOS=1-PLTSIZ*(rem(LEV-1,NPLOTS)+1);
	gset(['origin 0,' num2str(PLTPOS)])
	clearplot
	gset('title',['"instrumentation data for ' calltree(LEV,:) '"'])
	x=zeros(1,rows(INSTR{LEV}));		% room for abscissa data
	THR=0; SHK=0;			% shake labels if nearer than threshold
	for i = 1:rows(INSTR{LEV}), THR+=INSTR{LEV}{i,2}; end
	THR/=150;			% estimated letter size in a label
    end					% labels will shake center/up/down/cntr

    if ~REPLOT					% special cases: just plot
	printf('dumping instrumentation data for ');
	calltree				% normal cases: text output
	printf('-----------------------------------------\n');
    end

    for i =     1:rows(INSTR{LEV})
	if ~REPLOT				% special cases: just plot
	  if  ~isempty(INSTR{LEV}{i,3}), expnflg='->'; else expnflg=''; end
	  printf('%3d%15s\t%8.4f\t%s\n',...	% normal cases: text output
		    i, INSTR{LEV}{i,1},
		       INSTR{LEV}{i,2},  expnflg);
	end
	if FIGURES &&...			% labels for bargraph
		i<rows(INSTR{LEV})
		x(i+1)=INSTR{LEV}{i,2}; midpos=num2str(sum(x(1:i))+x(i+1)/2);
		    nm=INSTR{LEV}{i,1}; nm=nm(rindex(nm,'_')+1:end);
		if x(i+1) >...			% room for label greater than
		    THR*(length(nm)-2)		% estimated label size?
		  SHK=0;			% yes, stop shaking
		else SHK=SHK+1; end		% no, start shaking labels
		%erpos=num2str( ( rem(SHK,3)+(SHK==0) ) / 2 );
		verpos=num2str(   rem(SHK+1,3)          / 2 );
		gset('label',['"' nm '"'],['at ' midpos ',' verpos ' center'])
	end
    endfor

    if FIGURES					% abscissas for bargraph
	x=cumsum(x);
	Y=repmat([1 0 1],size(x));
	X=zeros(size(Y));
	for i=1:3, X(i:3:end)=x; end
	plot(X,Y);				% plot them
	gset('nolabel')				% clear labels & title for next
	gset('title')
    endif

    if REPLOT, REPLOT--; LEV++;		% skip / special cases loop over
    else				% normal cases: kbd input

      printf('-----------------------------------------\n');
      printf('Enter digit / quick sequence of them (<0.5s) / [u]p / [q]uit : ');
      k = kbhit(); printf(k);

		OLDLEV=	LEV;
      if k=='q',	LEV=0; close all
  elseif k=='u',	LEV--; if ~LEV, LEV=1;	% close(figno) broken
			else calltree(end,:)=''; end
  elseif isdigit(k), str='';

	while ~isempty(k)			% possible sequence of digits
	  str=[str,k];
	  sleep(0.5)
	  k = kbhit(1); 
	  if ~isdigit(k), k=''; end
	endwhile
	k=str2num(str);				% desired entry chosen
	if k > rows (INSTR{LEV}),k=1; end	% avoid indexing error
        if ~isempty (INSTR{LEV}{k,3})		% not all entries are calls
	INSTR{LEV+1}=INSTR{LEV}{k,3};
			   LEV++;
		name=INSTR{LEV}{1,1};		% take routine name
		name=name(1:rindex(name,'_')-1);% cut trailing _ini
		calltree=[calltree;name];
        endif % ~isempty

      endif % k=='q' elseif isdigit(k)

      printf('\n-----------------------------------------\n');

      if FIGURES && LEV && ceil(OLDLEV/NPLOTS)~=ceil(LEV/NPLOTS)
	gset('nomultiplot')			% it's a pity that
	figure(ceil(LEV/NPLOTS));		% changing from figure
	gset( 'multiplot')			% requires resetting multiplot
	gset('nokey')				% so previous plots get lost
	gset('noytics')				% on the very next clearplot
	gset(['size 1.0,' num2str(PLTSIZ)])	% workaround: replot all
	if LEV==OLDLEV-1			% coming back, replot
	   LEV =OLDLEV-NPLOTS;			% from 1st plot in figure
	   REPLOT=NPLOTS-1;			% special cases: all but last
	end
      endif % FIGURES

    endif % REPLOT

  endwhile

page_screen_output(oldPSO);

endfunction

function gset(varargin)
	__gnuplot_set__(varargin{:})
endfunction
