function mandelTBag(coords,pixels,max_it)
%MANDELTBAG	Mandelbrot set program, parallel version (taskbag, by lines)
%
%  mandelTBag [ (coords [ ,pixels [ ,max_it ]] ) ]	1 sequential process
%
%  Examples:
%
%  mpirun -c 1 -x DISPLAY octave -q --eval \
%           "global FIGURES, FIGURES=true; mandelTBag([-2-2i, 2+2i])"
%
%  mpirun -c 5            octave -q --eval mandelTBag
%
%  see the code for ideas on running a scalability test or drawing figures
%

% -----------------------------------------------------
% ## Copyright (C) 2006-2007 Javier Fernández Baldomero, Mancia Anguita López
% ##
% ## This program is free software
% -----------------------------------------------------

%-------------------------------
% Inspired in "Parallel Programming" 2nd Ed. by Barry Wilkinson & Michael Allen
% pp.86--88 sequential code, p.89 parallel, pp.90--93 load-balanced (by lines)
% taskbag/workpool algorithm by lines at p.91
%
% C code from http://www.cs.uncc.edu/par_prog
%	 from: http://www.cs.uncc.edu/~abw/ITCS3145F05/index.html
% MATLAB code by Lucio Andrade from http://www.mathworks.com/matlabcentral/...
%			fileexchange/loadFile.do?objectId=1329&objectType=file
%-------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% RUNNING PARALLEL PROCESSES %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%			for running 1 process from UNIX cmdline, use
%  matlab -r  mandelTBag			using MATLAB -run switch
%  octave <<< mandelTBag			using bash here-document/string
%  octave -q --eval mandelTBag			using Octave --eval switch
%
%			for running several processes from UNIX cmdline, use
%  mpirun -c <nprocs> matlab -nodisplay -nosplash -nojvm -r mandelTBag
%  mpirun -c <nprocs> octave                      -q --eval mandelTBag
%
%			for running a scalability test 1..8PC's from bash, use
%  for i in 1 2 3 4 5 6 7 8; do mpirun -c $i ... ; done
%                                                               or better
%  for t in 1 2 3; do for i in 1 2 3 4 5 6 7 8; do \		or also
%  for i in 1 2 3 4 5 6 7 8; do for t in 1 2 3; do
%	echo ___ $i Processes ___ time $t ____
%	mpirun -c $i ...
%	sleep 1					to avoid tbox cache errors
%       done;done >     mandelTBag.<Oct|MLAB>.1-8x3.log &	or also
%       done;done | tee mandelTBag.<Oct|MLAB>.1-8x3.log
%-------------------------------

	tic				% start stopwatch

	global y_siz r_min i_min scl_r scl_i MAX_IT MOD SQR_MOD	% before setup
	mandelStup			% setup, which defines

%	[soft, isOctave, isMatlab] = 'Octave'/'MATLAB', true/false;
%	[r_min,r_max, i_min,i_max] = ...	image limits in complex plane
%	[x_siz,y_siz, scl_r,scl_i] = ...	#pixels and pixel size
%	[max_it, MOD, SQR_MOD] = 64/1024, 2, 4;	stop criteria

	MAX_IT=max_it;	% error: can't make function parameter `max_it' global

%%%%%%%%%%%%%%%
% MPI startup %
%%%%%%%%%%%%%%%
	   MAST =         0 ;			% Master rank
[DTAG,RTAG,TTAG]=deal(7,8,9);			% Data, Result, Termination tags
[ANYS,ANYT]=deal(MPI_ANY_SOURCE,MPI_ANY_TAG);	% save typing
      WORLD=MPI_COMM_WORLD;			% Default communicator
 info      =MPI_Init;
[info rank]=MPI_Comm_rank(WORLD);
[info nprc]=MPI_Comm_size(WORLD);

	tic				% start stopwatch, alternate point

		%%%%%%%%%%
if rank==MAST %%% MASTER %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		%%%%%%%%%%
	mandel=zeros(y_siz, x_siz);	% image preallocation
	  line=zeros(y_siz,     1);	% line  preallocation (col)
%	TaskCast=zeros(  1,nprc-1);	% task  sent to each slave
%					% works even for 1 master/no slaves
%	TaskCount=zeros( 1,nprc-1);	%#tasks sent to each slave
	lcl=0;				% local #tasks for MAST
%	if isOctave,	rotsym='/-\\|';	% rotating symbols for local tasks
%	else,		rotsym='/-\|';	% Octave needs escaped \\
%	end				% complaints when parsing Matlab code
			rotsym='/-\\|';	% workaround
	if isMatlab,	rotsym(3)=[]; end

  %%%%%%%%%%%%%%%%%%%%%
  % Initial task cast % each slave proc is sent its first task
  %%%%%%%%%%%%%%%%%%%%%
  for slv=1:nprc-1			% send line index to slaves
    info=MPI_Send(slv,slv,DTAG,WORLD);	% col==slv for this first task
%   TaskCast (slv)=slv;			% assumes nprc<<x_siz
%   TaskCount(slv)=  1;			% this can be done at once later
  end	% for slv=1:nprc-1
    TaskCast =     1:nprc-1 ;		% redefined here, commented above
    TaskCount=ones(1,nprc-1);
    flght=  nprc-1;			% #tasks in-flight
    col  =  nprc-1;			% last task (line) sent

  %%%%%%%%%%%%%%%% when slave returns result, send it another task
  % TaskBag loop % variant: if no slaves pending, work locally on next task
  %%%%%%%%%%%%%%%% this way avoids efficiency down to 0.5 when using 1 slave
  while flght>0 || col<x_siz	% works even for 1 master/no slaves
				% when no more lines and all sent->recv, draw
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if  flght>0			% if no slaves, no tasks in-flight, skip this
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
     [info flag stat]=MPI_Iprobe(ANYS,RTAG,WORLD);
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% slaves have precedence
     while flag	% while flag -- pending recv	% don't exit this loop
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% while results awaiting
     [info stat]=MPI_Recv(line,ANYS,RTAG,WORLD);% get them on line buff
      slv= stat.src;	 slvcol =TaskCast(slv);	% which slave? which line sent?
		mandel(:,slvcol)=line;		% store in corresp.mandel line
%     fprintf('%3d/%3d lines<-n%d\r',...	% this task done
%		slvcol,x_siz,slv);
      if col<x_siz				% taskbag not empty
	 col  =  col+1;		% update next task, in-flight count unchanged
	 info =  MPI_Send(col, slv, DTAG,WORLD);% and send next task
	 TaskCast (slv) = col;
	 TaskCount(slv) = TaskCount(slv)+1;
      else
	 flght=flght-1;		% taskbag empty, one in-flight task less 
	 info =  MPI_Send(col, slv, TTAG,WORLD);% and send termination msg
      end	% if col<x_siz
     [info flag stat]=MPI_Iprobe(ANYS,RTAG,WORLD);
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% slaves have precedence
     end	% while flag -- pending recv	% don't exit this loop
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% while results awaiting
    end	% if flght>0

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if col<x_siz		% 2 reasons to enter here: no slv/slow slaves
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	col = col+1; lcl=lcl+1;			% process next line locally
	mandel(:,col)=CalcLine(col,isOctave);	% hopefully there will be
%	fprintf('%*s\r',20,rotsym(rem(lcl,4)+1));
%	fprintf('%3d/%3d local(n0)\r',col,x_siz);% results awaiting next time
    end	%  col<x_siz

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  end	% while flght>0	|| col<x_siz		% TaskBag Loop

  fprintf('elapsed time = %.3f s\n',toc);	% display time not computed
  fprintf('Task tallies:\n');
  fprintf('n%02d%5d (local)\n',MAST,lcl);	if nprc>1
  fprintf('n%02d%5d\n',[1:nprc-1;TaskCount]);	end
  fprintf('sum%5d (x_siz %d)\n',lcl+sum(TaskCount),x_siz);

global FIGURES, if FIGURES		% set global FIGURES=true to see it
  SCALECLR=ceil(MAX_IT/10);		% remove the -nodisplay matlab switch
  mandel=   mod(mandel*SCALECLR,MAX_IT);% to see this image
  mandel=flipud(mandel);		% flipud to get y axis upward
        imagesc(mandel);		% just for verifying correct operation
% if isMatlab, pause(5), end		% time to see drawing, deleted w/quit
		pause(1)		% for Oct-2.9.10 to let fig survive
end		% FIGURES

		%%%%%%%%%%
else %%%%%%%%%%%% SLAVE  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5%%%%%%%%%%%
		%%%%%%%%%%
			col = 0;
  [info stat]=MPI_Recv( col,MAST,ANYT,WORLD);	% receive msg from master
  while stat.tag==DTAG				% still data tag - proceed

		 line=CalcLine(col,isOctave);	% compute mandelbrot line
   info      =MPI_Send(line,MAST,RTAG,WORLD);	% and send result back
  [info stat]=MPI_Recv( col,MAST,ANYT,WORLD);	% Data/Term msg from master

  end	% while stat.tag==DTAG

		%%%%%%%%%%%%%%%%
end %%%%%%%%%%%%% MASTER/SLAVE %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		%%%%%%%%%%%%%%%%

 info      =MPI_Finalize;
	    MPITB_Clean;
if isOctave
	page_screen_output(oldPSO);
end


% end % function			% this generates warning in MATLAB

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function line=CalcLine(x,isOctave)
global y_siz r_min i_min scl_r scl_i MAX_IT MOD SQR_MOD

%	line=zeros(y_siz,1);
	cr=r_min + scl_r*(x-0.5);

if isOctave				% Calculate points
%-------------------------------	% a-la-vectorized
	c   =cr + i*...			% complex plane itself / line version
	  (i_min + scl_i*[0.5:1:y_siz]');
	z   =            c  ;		% z-seq for each point in complex plane
	mset=1:prod(size(c));		% idxs candidate (non-divergent) points

  if any(size(c)~=[y_siz,1])		% just to be safe
	error('internal maths error (meshgrid)')
  end

if false
	line=zeros(size(c));
  for counter=1:MAX_IT
	mset(abs(z(mset))>=MOD)=[];	% trim off diverging points
	z(mset)=z(mset).^2+c(mset);	% iterate candidates: z = z^2 + c;
	line(mset)=counter;		% color on remaining candidates
%       fprintf('%3d/%2d iters\r',counter,max_it);
  end
else
	line=ones(size(c))*MAX_IT;
  for counter=0:MAX_IT-1                % and color failing ones as they fail
        toff= abs(z(mset))>=MOD;        % these fail now (even when counter=0)
          line(mset(toff))=counter;     % so color them now, before
               mset(toff)=[];           % diverging points, trim off
        z(mset) = z(mset).^2+c(mset);   % iterate candidates: z = z^2 + c;
%       fprintf('%3d/%2d iters\r',counter,max_it);
  end
end

else    % isMatlab
%-------------------------------	% a-la-JIT
	line=zeros(y_siz,1);
  for y=1:y_siz				% do all columns
	zr=0;%cr=r_min + scl_r*(x-0.5);	% each point in complex plane
	zi=0; ci=i_min + scl_i*(y-0.5);	% compute real/imag parts
	k =0;				% iteration/color counter

    while zr*zr+zi*zi<SQR_MOD & k<MAX_IT
	t = cr+ zr*zr - zi*zi;		% this is fastest in MATLAB
	zi= ci+       2*zr*zi;		% (faster than abs(),z*z', etc)
	zr= t ;				% due to JIT compiler
	k = k +   1;
    end % while

	line(y)=k;
  end % for y=0..y_siz-1

end	% if isOctave
%-------------------------------

% end % function			% this generates warning in MATLAB
