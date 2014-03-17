function mandelStrp(coords,pixels,max_it)
%MANDELSTRP	Mandelbrot set, parallel version (static task assignmnt/stripes)
%
%  mandelStrp [ (coords [ ,pixels [ ,max_it ]] ) ]	1 sequential process
%
%  Examples:
%
%  mpirun -c 1 -x DISPLAY octave -q --eval \
%	    "global FIGURES, FIGURES=true; mandelStrp([-2-2i, 2+2i])"
%
%  mpirun -c 5            octave -q --eval mandelStrp
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
% static task assignment algorithm at p.89
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
%  matlab -r  mandelStrp			using MATLAB -run switch
%  octave <<< mandelStrp			using bash here-document/string
%  octave -q --eval mandelStrp			using Octave --eval switch
%
%			for running several processes from UNIX cmdline, use
%  mpirun -c <nprocs> matlab -nodisplay -nosplash -nojvm -r mandelStrp
%  mpirun -c <nprocs> octave                      -q --eval mandelStrp
%
%			for running a scalability test 1..8PC's from bash, use
%  for i in 1 2 3 4 5 6 7 8; do mpirun -c $i ... ; done
%                                                               or better
%  for t in 1 2 3; do for i in 1 2 3 4 5 6 7 8; do \		or also
%  for i in 1 2 3 4 5 6 7 8; do for t in 1 2 3; do
%	echo ___ $i Processes ___ time $t ____
%	mpirun -c $i ...
%	sleep 1					to avoid tbox cache errors
%	done;done >     mandelStrp.<Oct|MLAB>.1-8x3.log &	or also
%	done;done | tee mandelStrp.<Oct|MLAB>.1-8x3.log
%-------------------------------

	tic				% start stopwatch

	global y_siz r_min i_min scl_r scl_i MAX_IT MOD SQR_MOD	% before setup
	mandelStup			% setup, which defines

%	[soft, isOctave, isMatlab] = 'Octave'/'MATLAB', true/false;
%	[r_min,r_max, i_min,i_max] = ...	image limits in complex plane
%	[x_siz,y_siz, scl_r,scl_i] = ...	#pixels and pixel size
%	[max_it, MOD, SQR_MOD] = 64/1024, 2, 4;	stop criteria
                                                                                
	MAX_IT=max_it;	% error: can't make function parameter `max_it' global

	global FLAWED			% see CalcStrip
	FLAWED=true;			% enable original flawed version
	INSTR =false;			% enable instrumentation details
					% or better, use XMPI-2.2.3b8
%%%%%%%%%%%%%%%
% MPI startup %
%%%%%%%%%%%%%%%
      MAST =       0 ;				% Master rank
      RTAG =       8 ;				% Result tag
[ANYS,ANYT]=deal(MPI_ANY_SOURCE,MPI_ANY_TAG);	% save typing
      WORLD=MPI_COMM_WORLD;			% Default communicator
 info      =MPI_Init;
[info rank]=MPI_Comm_rank(WORLD);
[info nprc]=MPI_Comm_size(WORLD);

	tic,		if INSTR	% start stopwatch, alternate point
	tics  =zeros(1,4);   end	% timestamp details / compute-xmit-wkld

  %%%%%%%%%%%%%%%%%%%%%%%%%%%		% vertical stripes (x_siz/nprc lines)
  % static task assignment: %		% remaining r lines on first r ranks
  %%%%%%%%%%%%%%%%%%%%%%%%%%%		% for example: 11 lines/5 ranks
      lins =   fix(x_siz/nprc);		% lins=2 lin/rank
      rlns =   rem(x_siz,nprc);		% rlns=1 remaining line
 nlns=lins +	   (rank<rlns);		% nlns=3 just for rank 0
 strl=lins*rank+min(rank,rlns);		% starting-line=lins*rank = 0/2/4/6/8
					%		+min(...) = 0/3/5/7/9
					% add 1 to index into matrices
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % WRT indexing: we like 1st dim (row) corresponding to ordinate (y)	%
  %	and 2nd dim (column) as abscissa (x), so we say matrix(y,x)	%
  % That's the way MATLAB manages image(matrix), help says that for	%
  % C(M,N) the outer extent of the image occupies [0.5 N+0.5 0.5 M+0.5] %
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % MATLAB/Octave use Fortran col-major (ie, they store column after column)
  % we would like to incorporate results of varying size:		%
  %	normal stripes of size [y_siz X lins  ] (lins=x_siz/nproc)	%
  %	extend stripes of size [y_siz X lins+1]				%
  % That's why we cut stripes in vertical (or we could transpose image)	%
  % so we don't need complex Matlab indexing when			%
  % sending a short (lins) stripe to a large (lins+1) recv buffer	%
  % (it WOULD be a problem sending [MxN] data to a [M+1 x N] buffer)	%
  % FIX IDEA: use nlins in the 2nd dimension (whole cols, lins or l+1)	%
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  %%%%%%%%%%%%%%%%%%%%%%
  % Work on local task %
  %%%%%%%%%%%%%%%%%%%%%%
if rank==MAST
	mandel=zeros(y_siz,x_siz);	% image preallocation / only at master
	mandel(:,1:nlns)=CalcStrip(strl,nlns,isOctave);	% strl=0 at MAST=0

	if INSTR, tics(1)=toc; end			% computing time
%	fprintf('Rank %d -- Computing time %.3f s\n',rank,toc);
%	fprintf('%3d lines %3d-%3d/%3d <- n%d\t after %.3f s\n',...
%		nlns,strl+[1,nlns],x_siz,rank,		toc);
else
	strip		=CalcStrip(strl,nlns,isOctave);	% compute strip
	if INSTR, tics(1)=toc; end			% computing time
%	fprintf('Rank %d -- Computing time %.3f s\n',rank,toc);
	info = MPI_Send(strip,MAST,RTAG,WORLD);		% and send result back
	if INSTR, tics(2)=toc; end			% xmit time - slave
%	fprintf('Rank %d -- Data sent   \t   \t after %.3f s\n',rank,toc);
end	% if rank==MAST			% no problem if slave nlns < max nlns


  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % Incorporate slave results %
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if rank==MAST
  if nprc>1				% only if required f/recv
	 strip=zeros(y_siz, nlns);	% strip preallocation / only at master
  end
  if INSTR
	tocs=zeros(nprc,6);		% gather tics (performance) data
	tocs(     1,1)=toc;		% time  to data recvd
	tocs(     1,2)=  1;		% order to data recvd
  end
  for slv=1:nprc-1			% get slave results -- any order
       [info stat]=MPI_Recv(strip,ANYS,RTAG,WORLD);	% recvd in col-major
	task=stat.src;				% perhaps missing last col(line)
     if INSTR
	tocs(task+1,1)=toc;			% time
	tocs(task+1,2)=slv+1;			% order
     end
 	 nlns=lins+	   (task<rlns);		% nlns/strl for this task
	 strl=lins*task+min(task,rlns);		% maybe slv_nlns = mast_nlns-1
	mandel(:,strl+[1:nlns])=strip(:,1:nlns);	% if so, here we cut it

%	fprintf('%3d lines %3d-%3d/%3d <- n%d\t after %.3f s\n',...
%		nlns,strl+[1,nlns],x_siz,task,		toc);
  end	% for slv=1:nprc-1
     if INSTR
	tics(2)=toc;					% xmit time - master
     end


  %%%%%%%%%%%%%
  % StopWatch % & Display
  %%%%%%%%%%%%%

  fprintf('elapsed time = %.3f s\n',toc);	% display time not computed

global FIGURES, if FIGURES		% set global FIGURES=true to see it
  SCALECLR=ceil(MAX_IT/10);		% remove the -nodisplay matlab switch
  mandel=   mod(mandel*SCALECLR,MAX_IT);% to see this image
  mandel=flipud(mandel);		% flipud to get y axis upward
	imagesc(mandel);		% just for verifying correct operation
% if isMatlab, pause(5), end		% time to see drawing, deleted w/quit
		pause(1)		% for Oct-2.9.10 to let fig survive
end		% FIGURES

  %%%%%%%%%%%%%%%%%%%%%%
  % Detailed StopWatch %
  %%%%%%%%%%%%%%%%%%%%%%
  if INSTR
	nlns=lins +	   (rank<rlns);		% rank==0 here
	tics(3)=sum(sum(mandel(:,1:nlns)));	% workload for MASTER
	tics(4)= sizeof(mandel          ) ;	% xmitload for MASTER
	tocs(    1,3:end)=tics;			% MASTER data
    for slv=1:nprc-1			% get slave data -- normal order
       [info stat]=MPI_Recv(tics,slv ,RTAG+1,WORLD);
	tocs(slv+1,3:end)=tics;
    end	% for slv=1:nprc-1

  tocs(:,4)=tocs(:,4) -tocs(:,3);	% xmit times / not wall but incremental
  tocs(:,7)=tocs(:,5)./tocs(:,3);	% iter/s = wkload / comput time
  tocs(:,8)=tocs(:,6)./tocs(:,4)/2^20;	% MB/s   = size  / xmit time
  tocs(:,9)=tocs(:,5)./tocs(:,6);	% iter/px= size  / wkload
  tocs(1,9)=...
	sum(tocs(:,5))/tocs(1,6);	% iter/px, mean

  fprintf(['rank\twalltm\trecv\tcomput\ttxmit\t',...
				'wrkload\tsize\titer/s\tMB/s\tits/pix\n']);
  fprintf(['%4d\t%6.4f\t%4d\t'  '%.4f\t%.4f\t',...
				     '%7d\t%7d\t%.0f\t%5.2f\t%6.3f\n'],...
	[[0:nprc-1]' tocs  ]');
  fprintf(['\t\t\t\t\t\t\tEfficiency\t%6.3f\n'],tocs(1,9)/max(tocs(:,9)));
  end	% if INSTR

else	% if rank==MAST
  if INSTR
	tics(3)=sum(sum(strip));			% workload - slave
	tics(4)= sizeof(strip) ;			% xmitload - slave
	info      =MPI_Send(tics,MAST,RTAG+1,WORLD);	% send timings/wkload
  end
end	% if rank==MAST


  %%%%%%%%
  % Quit % MPI and MATLAB/Octave
  %%%%%%%%
 info      =MPI_Finalize;
	    MPITB_Clean;

if isOctave
	page_screen_output(oldPSO);
end


% end % function			% this generates warning in MATLAB

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function strip=CalcStrip(stcol,ncols,isOctave)
global FLAWED
global y_siz r_min i_min scl_r scl_i MAX_IT MOD SQR_MOD
					% recall Fortran col-major order
%	strip=zeros(y_siz,ncols);	% advantage: nlns or nlns+1, don't care
					% we can send less data, last col void

if isOctave				% Calculate points
%-------------------------------	% a-la-vectorized
       [x,y]=meshgrid(r_min + scl_r*([0.5:1:ncols]+stcol),...
		      i_min + scl_i* [0.5:1:y_siz]);
	c   =      (x + i*y);		% complex plane itself / stripe version
	z   =            c  ;		% z-seq for each point in complex plane
	mset=1:prod(size(c));		% idxs candidate (non-divergent) points

  if any(size(c)~=[y_siz,ncols])	% just to be safe
	error('internal maths error (meshgrid)')
  end

if FLAWED
	z   = zeros(size(c));		% flaw in the original version
	strip=zeros(size(c));		% advantage: nlns or nlns+1, don't care
  for counter=1:MAX_IT
	mset(abs(z(mset))>=MOD)=[];	% trim off diverging points
	z(mset)=z(mset).^2+c(mset);	% iterate candidates: z = z^2 + c;
	strip(mset)=counter;		% color on remaining candidates
%	fprintf('%3d/%2d iters\r',counter,max_it);
  end
else
	strip=ones(size(c))*MAX_IT;	% assume all are final candidates
  for counter=0:MAX_IT-1		% and color failing ones as they fail
	toff= abs(z(mset))>=MOD;	% these fail now (even when counter=0)
	 strip(mset(toff))=counter;	% so color them now, before
	       mset(toff)=[];		% diverging points, trim off
	z(mset) = z(mset).^2+c(mset);	% iterate candidates: z = z^2 + c;
%	fprintf('%3d/%2d iters\r',counter,max_it);
  end
end

else	% isMatlab
%-------------------------------	% a-la-JIT
	strip=zeros(y_siz,ncols);	% advantage: nlns or nlns+1, don't care
  for x=1:ncols					% do all cols (col-major)
	     %cr=r_min + scl_r*(x-0.5+stcol);	% no need to factor out this
    for y=1:y_siz				% do all rows (vertical sweep)
	zr=0; cr=r_min + scl_r*(x-0.5+stcol);	% JIT eats it easily
	zi=0; ci=i_min + scl_i*(y-0.5);		% compute real/imag parts
	k =0;					% iteration/color counter

      while zr*zr+zi*zi<SQR_MOD & k<MAX_IT
	t = cr+ zr*zr - zi*zi;		% this is fastest in MATLAB
	zi= ci+       2*zr*zi;		% (faster than abs(),z*z', etc)
	zr= t ;				% due to JIT compiler
	k = k +   1;
      end % while

	strip(y,x) = k;
    end	% for y=0..y_siz-1
	% fprintf('%3d/%2d lines\r',x,x_siz);
  end	% for x=0..ncols-1

end	% if isOctave
%-------------------------------

% end % function			% this generates warning in MATLAB
