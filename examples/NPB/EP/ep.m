function ep(class)
%  Kernel EP:	an Embarrassingly Parallel benchmark
%		based on original NAS Technical Report RNR-94-007, March 1994
%		and on MATLAB/Octave version by Benjamin Buss&Dave Ennis 2004
%
%  ep [ (class) ]	for running 1 process from MATLAB/Octave environment
%
%  class ['S']	One of the 5 specified classes of the problem. 
%		Valid arguments are:
%	'S' = Class S problem -- 2^24 -- Sample to ensure code is error-free
%	'W' = Class W problem -- 2^25 -- Intended to run on workstations
%	'A' = Class A problem -- 2^28
%	'B' = Class B problem -- 2^30
%	'C' = Class C problem -- 2^32	(pairs of pseudorandoms generated)
%
%  REQUIRES:
%	randlc.m, vranlc.m
%
%  SEE ALSO:
%	EP, FT, IS, CG, MG, RANDLC, VRANLC
%
%  Examples:
%
%  octave:1> ep('S')			% from octave prompt
%
%  $ octave -q --eval "ep('S')"		# from command line
%
%  $ mpirun -c 5   octave -q --eval ep	# LAM: 5 parallel processes
%
%  $ mpirun -c 5 [-H h1,h2,h3,h4,h5]	# Open-MPI -host[file] syntaxes
%                [-hostfile lam-bhost.def]
%		   octave -q --eval ep
%
%  see remarks in source code for running a scalability test
%			   or for an explanation of output format
%

% -----------------------------------------------------
% ## Copyright (C) 2005-2007 Javier Fernández Baldomero, Mancia Anguita López
% ##
% ## This program is free software
% -----------------------------------------------------

%%%%%%%%%%%%%
%  HISTORY: %
%%%%%%%%%%%%%
%	Kernel EP: An Embarassingly Parallel benchmark.
%	From "The NAS Parallel Benchmarks", Sect.2.2.1, pp.14--16
%	Technical Report RNR-94-007, March 1994.
%	http://www.nas.nasa.gov/Software/NPB/
%
% First adapted to MATLAB w/MatlabMPI 
%	by Benjamin Buss (The Ohio State University, 2004)
%	&  Dave Ennis (Ohio Supercomputer Center)
%	http://www.ece.osu.edu/~bussb/research/
%
% Adapted to MPITB by Javier Fernández Baldomero (Universidad de Granada, 2005)
%			& Mancia Anguita López (Universidad de Granada)
%	nprocs not specified on argument list -- deduced from MPI_Comm_size

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% RUNNING PARALLEL PROCESSES %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%			for running 1 process from UNIX cmdline, use
%  matlab	 -r "ep('<class>');quit"	using MATLAB -run switch
%  octave -q --eval "ep('<class>')"		using Octave --eval switch
%
%			for running several processes from UNIX cmdline, use
%  mpirun -c <nprocs> matlab -nodisplay -nosplash -nojvm -r "ep('<class>');quit"
%  mpirun -c <nprocs> octave                      -q --eval "ep('<class>')"
% 
%			for running a scalability test 1..8PC's from bash, use
%  for i in 1 2 3 4 5 6 7 8; do mpirun -c $i ... ; done
%								or better
%  for t in 1 2 3; do for i in 1 2 3 4 5 6 7 8; do mpirun -c $i ... ; \
%  	done;done > ep<class>.<Oct|MLAB>.1-8.log &
%
%  See also the "ep8" bash script for an optimized way to run such a test

%%%%%%%%%%%%%%%%%
% OUTPUT FORMAT % Displayed to screen:
%%%%%%%%%%%%%%%%%
%    Starting header:	#random numbers to generate, #processes running
%    Benchmark result:	CPU time, #pairs, #Gaussian pairs (on unit circle)
%			sums (on X,Y), counts (on concentric square annuli)
%    Summary table:	Class, Size, Iterations, Time, #procs (run/compiled)
%(common/all benchmarks)Mop/s, Mop/s/process, OpType, Verif., version, runDate
%    if timers_enabled:	Total time, for Gaussian pairs, for Random numbers
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FORTRAN EXAMPLE %
%%%%%%%%%%%%%%%%%%%
%$ mpirun n1 ep.S.1
%
% NAS Parallel Benchmarks 2.3 -- EP Benchmark
%
% Number of random numbers generated:     33554432
% Number of active processes:                    1
%
%EP Benchmark Results:
%
%CPU Time =    4.7583
%N = 2^   24
%No. Gaussian Pairs =      13176389.
%Sums =    -3.247834652034750E+03   -6.958407078382243E+03
%Counts:
%  0       6140517.
%  1       5865300.
%  2       1100361.
%  3         68546.
%  4          1648.
%  5            17.
%  6             0.
%  7             0.
%  8             0.
%  9             0.
%
%
% EP Benchmark Completed.
% Class           =                        S
% Size            =                 33554432
% Iterations      =                        0
% Time in seconds =                     4.76
% Total processes =                        1
% Compiled procs  =                        1
% Mop/s total     =                     7.05
% Mop/s/process   =                     7.05
% Operation type  = Random numbers generated
% Verification    =               SUCCESSFUL
% Version         =                      2.3
% Compile date    =              01 Sep 2005
%
% Compile options:
%    MPIF77       = f77
%    FLINK        = f77
%    FMPI_LIB     = $(shell mpif77 -showme:link)
%    FMPI_INC     = $(shell mpif77 -showme:compile)
%    FFLAGS       = -O3 -Wno-globals
%    FLINKFLAGS   = (none)
%    RAND         = randdp
%
%
% Please send the results of this run to:
%
% NPB Development Team
% Internet: npb@nas.nasa.gov
%
% If email is not available, send this to:
%
% MS T27A-1
% NASA Ames Research Center
% Moffett Field, CA  94035-1000
%
% Fax: 415-604-3957
%
%
% Total time:       4.75828497
% Gaussian pairs:   2.51152638
% Random numbers:   2.24321454
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% MLB/OCT EXAMPLE %
%%%%%%%%%%%%%%%%%%%
%$ matlab
%>> ep('S')
% 
% NAS Parallel Benchmarks 2.3 -- EP Benchmark MPITB version
% 
% Number of random numbers generated:     33554432
% Number of active processes:                    1
% 
% Start   date    =     09-Sep-2005 12:56:18
% 
%EP Benchmarks Results:
% 
%CPU Time =    27.5278
%N = 2^   24
%No. Gaussian Pairs =      13176389.
%Sums =    -3.247834652034617e+03   -6.958407078382817e+03
%Counts:
%  0       6140517.
%  1       5865300.
%  2       1100361.
%  3         68546.
%  4          1648.
%  5            17.
%  6             0.
%  7             0.
%  8             0.
%  9             0.
% 
% 
% EP Benchmark Completed.
% Class           =                        S
% Size            =                 33554432
% Iterations      =                        0
% Time in seconds =                    27.53
% Total processes =                        1
% Compiled procs  =                        1
% Mop/s total     =                 1.218930
% Mop/s/process   =                 1.218930
% Operation type  = Random numbers generated
% Verification    =               SUCCESSFUL
% Version         =         NPB2.3 for MPITB
% Run     date    =     09-Sep-2005 12:56:46
% 
% Design  options:
%    ENVIRONMENT  = MATLAB 7.0.1.24704 (R14) Service Pack 1
%    MPI_LIBRARY  = LAM 7.1.1
%    TOOLBOX      = MPITB
%    RAND         = randdp (floor/arith)
% 
% 
%Total time:        27.52776980
%Gaussian pairs:    23.16399717
%Random numbers:     4.15743732
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%
% ENVIRON DETECT %
%%%%%%%%%%%%%%%%%%
	SOFT = detect;

		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%!-------------------------------------------------------------------------!
%!    This benchmark is part of the NAS Parallel Benchmark 2.3 suite.      !
%!    It is described in NAS Technical Report 95-020.                      !
%!-------------------------------------------------------------------------!
%c Author: P. O. Frederickson
%c         D. H. Bailey
%c         A. C. Woo
%c---------------------------------------------------------------------
%      program EMBAR
%c---------------------------------------------------------------------
%c   This is the MPI version of the APP Benchmark 1,
%c   the "embarassingly parallel" benchmark.
%c
%c   M = Log_2 of the number of complex pairs of uniform (0, 1) random numbers.
%
%      implicit none
%      include 'npbparams.h'		%%%	define class, m, npm
%	...
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/npbparams.h %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%	...
%        character class
%        parameter (class ='S')		%%%	   S/ W/ A/ B/ C
%        integer m, npm			%%%	m=24/25/28/30/32
%        parameter (m=24, npm=1)	%%%	npm=<nproc>=1,2,3,4,5...
%	...				%%%	npm unused for this benchmark

%%%%%%%%%%
% ARGCHK %
%%%%%%%%%%
if ~nargin, class='S'; end
class=upper(class);
switch	    class				% BUG, case S changed to 16/20
%   case 'A',  m=28;    case 'S',  m=24;	% for timing purposes (TUNABLE)
    case 'A',  m=28;    case 'S',  m=24;	% this is the normal size
    case 'B',  m=30;    case 'W',  m=25;
    case 'C',  m=32;    otherwise, m=24; class='S';
end

  npm = 1;
% npm (#processes) is meaningless for this app, fixed to 1
% no_nodes is later discovered w/MPI_Comm_size, after MPI_Init
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%c   MK is the Log_2 of the size of each batch of uniform random numbers.
%c   MK can be set for convenience on a given system,
%c   since it does not affect the results.
%
%      double precision Mops, epsilon, a, s, t1, t2, t3, t4, x, x1,
%     >                 x2, q, sx, sy, tm, an, tt, gc, dum(3),
%     >                 timer_read
%      integer          mk, mm, nn, nk, nq, np, ierr, node, no_nodes,
%     >                 i, ik, kk, l, k, nit, ierrcode, no_large_nodes,
%     >                 np_add, k_offset, j
%      logical          verified, timers_enabled
%      parameter       (timers_enabled = .false.)
%	...
%      parameter (mk = 16, mm = m - mk, nn = 2 ** mm,
%     >           nk = 2 ** mk, nq = 10, epsilon=1.d-8,
%     >           a = 1220703125.d0, s = 271828183.d0)
%	...
%      common/storage/ x(2*nk), q(0:nq-1), qq(10000)
%      data             dum /1.d0, 1.d0, 1.d0/
%
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/common/timers.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      double precision start(64), elapsed(64)
%      common /tt/ start, elapsed

% TUNABLE PARAMETERS:	default values	explanation
%			MATLAB/Octave
%	USE_VECTnLOOP	false /		use arrays-no loops f/gaussian
%				true	Octave has no JIT compiler
%	USE_REAL_MATH	true  /		use real variants log/sqrt f/gaussian
%				false	Octave has no reallog/realsqrt
%	timers_enabled	     any	time breakdown randdp/gaussian
%	USE_GLOBAL_X	true  /		reuse 1MB allocated f/64Kpairs
%				any	Octave does not benefit from global
%	USE_CLEAR	false /		clear vars f/vectorizedGaussian
%				?	only hurts MATLAB/VECTnLOOP version
%%% TUNABLE %%%
    USE_VECTnLOOP = true;	%%% use vectorization(array)language--no loops
    USE_REAL_MATH = true &&...	%%% faster reallog/realsqrt instead of log/sqrt
	(all(SOFT=='MATLAB'));	%%% recall Octave has no reallog / realsqrt

    timers_enabled = true;	%%% EXPLANATION: FORTRAN timers are global vars
    timer=zeros(64,1);		%%% timers can be started, stopped, restarted...
    %%% toggle them with the following sentence timer(i) = MPI_Wtime-timer(i);
    %%% clear  them with                        timer(i) = 0;

%   x = zeros(2*nk,1);		%%% random pairs array (warning: nk=64K x~1MB)
%   x = -1.D99 * ones(2*nk,1);	%%% later initialized to this value, nk undef
%%% TUNABLE %%%			%%% no need to define it, vranlc will create it
    USE_GLOBAL_X = true;	%%% unless we want it global (which is faster)
    USE_CLEAR    = false;	%%% migth swap/thrash with x1/x2/idx (line 550)
				%%% wouldn't depend on problem class, but on mk
			%%% after measurements, it turns out it doesn't mind
% ================================================================
% Measurements in a PentiumIII 1GHz w/128MB PC133
% ================================================================
% Octave 	  total   rand +gauss
% m=16	1**10	  28.46s( 27.96+ 0.42)
%mk=12	0**10	  47.59s( 27.89+19.61)	gaussian much slower w/out array lang
%	1**00	  28.59s( 28.08+ 0.42)	random   not benefited w/global
%	1**11	  28.58s( 28.03+ 0.47)	rnd/gaus not benefited w/clear
%
% MATLAB R13	  total   rand +gauss
% m=20	11*10	   4.68s(  0.74+ 3.92)
%	01*10	  28.20s(  0.74+27.44)	JIT not better than array language R13
%	10*10	   4.91s(  0.74+ 4.15)	gaussian slower w/out realmath
%	11*00	   4.82s(  0.87+ 3.92)	random   slower w/out global
%	11*11	   4.68s(  0.74+ 3.92)	not affected    w/clear
% ================================================================
% Measurements in an Athlon 2GHz w/2GB PC266 ???
% ================================================================
% Octave 	  total   rand +gauss
% m=16	1**10	  11.14s( 10.91+ 0.20)
%mk=12	0**10	  18.85s( 11.04+ 7.77)	gaussian much slower w/out array lang
%	1**00	  11.23s( 10.99+ 0.20)	random   not benefited w/global
%	1**11	  11.22s( 10.96+ 0.22)	rnd/gaus not benefited w/clear

% MATLAB R14	  total   rand +gauss
% m=24	01*10	   8.50s(  4.03+ 4.34)	JIT better than array language MLR14
%	11*10	  27.43s(  4.10+23.12)	gaussian slower w/out JIT
%	00*10	   9.99s(  4.09+ 5.78)	gaussian slower w/out realmath
%	01*00	   9.71s(  4.91+ 4.68)	random/g slower w/out global
%	01*11	   8.55s(  4.06+ 4.37)	not too affected w/clear
%	01*01	   9.70s(  4.91+ 4.66)		 slower w/out global
% ================================================================
% CONCLUSION: 11110 for all except MLR14 01110
% ================================================================

    Mops   =0    ;
    epsilon=1E-12;	%%% 1E-12 according to (bussb), 1.d-8 in FORTRAN above
    a = 1220703125.0;	%%% factor for linear congruential generator a=5^13;
    s =  271828183.0;	%%% random seed --- prime number --- period 2^44
   [t1,t2,t3,t4,tm]= deal(0);	%%% temporaries, timer read
   [an,tt,gc,sx,sy]= deal(0);	%%% x,x1,x2,q dims depend on integer values
    dum = [1,1,1];		%%% dummies

%%% mk=12;		% BUG, changed to 12 for timing purposes (TUNABLE above)
    mk=16;		%%% A number smaller than m to reduce the memory burden
%m=16
%mk=12
    mm=m-mk;		%%% We'll compute 2^m pairs of uniform random rumbers
    nn=2^mm;		%%% in nn=2^(m-mk) batches of nk=2^mk pairs
    nk=2^mk;		%%% recall m=24/25/28/30/32 for class=S/W/A/B/C

    %%% EXAMPLE: class S, m=24,	mk=16,			mm=8
    %%%				nk=2^16=64K pairs,	nn=2^8=256 batches
    %%% generated 2^24=16M pairs / 32M random numbers ( 2^(m+1) )

   [np,np_add]    =deal(0);	%%% number of batches per processor +-1
   [ierr,ierrcode]=deal(0);	%%% for MPI_Abort
   [node,no_nodes]=deal(0);	%%% rank, size
    no_large_nodes=     0 ;	%%% remaining batches after even division
   [i,j,k,l]      =deal(0);	%%% assorted loop indexes i=1:mk+1/nk/nq k=1:p
				%%% j unused (weird printf FORTRAN trick)
				%%% l index for q(l) square annuli
   [ik,kk]	  =deal(0);	%%% binary exponentiation algorithm
    nit           =     0 ;	%%% #iterations (0 for EP), just to print it

    nq=10;			%%% number of square annuli
    q = zeros(nq,1);		%%% now initialized, recall q(0:nq-1) 0-indexed
%   qq= zeros(10000,1);		%%% unused on ep.f, probably to cope with
				%%% q(l) overflow for l>9 (l up to 38 ?!? )

		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      call mpi_init(ierr)
%      call mpi_comm_rank(MPI_COMM_WORLD,node,ierr)
%      call mpi_comm_size(MPI_COMM_WORLD,no_nodes,ierr)
%
%      root = 0
%      if (node.eq.root)  then
%          write(*, 1000)
%	...
%          write (*,1001) size
%          write(*, 1003) no_nodes
%
% 1000 format(/,' NAS Parallel Benchmarks 2.3 -- EP Benchmark',/)
% 1001     format(' Number of random numbers generated: ', a14)
% 1003     format(' Number of active processes:         ', i12, /)
%
%      endif
%
%      verified = .false.
%

     ierr          =MPI_Init;
    [ierr    node ]=MPI_Comm_rank(MPI_COMM_WORLD);
    [ierr no_nodes]=MPI_Comm_size(MPI_COMM_WORLD);

    root=0;
    if node==root
	fprintf(['\n',...
	 ' NAS Parallel Benchmarks 2.3 -- EP Benchmark MPITB version',...
	 '\n\n']);
	fprintf(' Number of random numbers generated: %12.0f\n', 2^(m+1));
	fprintf(' Number of active processes:         %12d\n\n',no_nodes);
						if all(SOFT=='MATLAB')
	fprintf(' Start   date    = %24s\n',	datestr(now));	else
	fprintf(' Start   date    = %24s\n',	ctime (time));	end
%	fprintf(' Run     date    = %24s\n',	date);
    end

    verified=false;

		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%c   Compute the number of "batches" of rand.pairs generated per processor
%c   Adjust if the number of processors does not evenly divide the total number
%
%      np = nn / no_nodes
%      no_large_nodes = mod(nn, no_nodes)
%      if (node .lt. no_large_nodes) then
%         np_add = 1
%      else
%         np_add = 0
%      endif
%      np = np + np_add

    np             = floor(nn./no_nodes);
    no_large_nodes = mod  (nn, no_nodes);
    if node < no_large_nodes,	np_add = 1;
    else,			np_add = 0;
    end
    np = np + np_add;

    %%% EXAMPLE: class S, m=24,	mk=16,		mm=8
    %%%				nk=64K pairs,	nn=256 batches
    %%%				no_nodes=8,	np=256/8=32 batches/processor
    %%%						no_large_nodes=0
    %%%				no_nodes=7,	np=256/7=36 batches/processor
    %%%						no_large_nodes=4 batches left
    %%% np_add=1 for ranks 0,1,2,3 < 4

		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      if (np .eq. 0) then
%         write (6, 1) no_nodes, nn
% 1       format ('Too many nodes:',2i6)
%         call mpi_abort(MPI_COMM_WORLD,ierrcode,ierr)
%         stop
%      endif

    if np==0,	fprintf('Too many nodes: %6d %6d\n', no_nodes, nn);
		ierrcode=1;		%%% undefined in original ep.f
		ierr = MPI_Abort(MPI_COMM_WORLD,ierrcode);
    end

    %%% Unrealistic up to the point that the original ep.f forgot to
    %%% initialize ierrcode --- np = nn / no_nodes (batches/processor)
    %%% for class S you need at least 257 nodes to get np=0 batches/processor

		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      external         randlc, timer_read
%      double precision randlc, qq
%	...
%c   Call the random number generator functions and initialize
%c   the x-array to reduce the effects of paging on the timings.
%c   Also, call all mathematical functions that are used. Make
%c   sure these initializations cannot be eliminated as dead code.
%
%      call vranlc(0, dum(1), dum(2), dum(3))
%      dum(1) = randlc(dum(2), dum(3))
%      do 5    i = 1, 2*nk
%         x(i) = -1.d99
% 5    continue
%      Mops = log(sqrt(abs(max(1.d0,1.d0))))

			%%% N=0 randoms, x=dum(1)==a=dum(2)==dum(3)==1
			%%% return (empty) sequence in t3, update x in dum(1)
    [dum(1),   t3 ] = vranlc(0,dum(1),dum(2));	%%% t3=[], can't store on dum(3)
    [dum(1),dum(2)] = randlc  (dum(2),dum(3));
 if USE_GLOBAL_X		%%% don't destroy global with t1=vranlc()
     global X			%%% try to use from now on the syntax
     X =-1.D99 * ones(2*nk,1);	%%% [t1 y]=vranlc(   N,...) if N != 2nk
				%%%  t1   =vranlc(2*nk,...) to access global X
     if USE_CLEAR, clear X, end	%%% cleared from local workspace, not from glb
 end
%else				%%% no need to define x, vranlc will create it
%    x =-1.D99 * ones(2*nk,1);	%%% [t1 x]=vranlc(2*nk,...), no global X
%end
 if USE_REAL_MATH, Mops = reallog(realsqrt(abs(max(1,1))));
 else,		   Mops =     log(    sqrt(abs(max(1,1))));
 end

    %%% EXPLANATION: randlc was dblprec function -linear congruential generator
    %%% double precision function randlc(x, a) --- x_{k+1} = a x_k  (mod 2^46)
    %%% returns next x and alters previous x_k=x_{k+1} so we can go on calling
    %%%		     vranlc was a FORTRAN subroutine -no return value
    %%% SUBROUTINE VRANLC (N, X, A, Y) --- fills array Y with a sequence of N
    %%% array Y is accessed by reference and X is also updated

    %%% the x-array will hold the 2nk randoms (nk pairs) in a batch
    %%% we have two versions: global access to X, re-alloc/destroy x each time
    %%% see timer_start/stop(3) below

		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%c---------------------------------------------------------------------
%c      Synchronize before placing time stamp
%c---------------------------------------------------------------------
%      call mpi_barrier(MPI_COMM_WORLD, ierr)
%
%      call timer_clear(1)
%      call timer_clear(2)
%      call timer_clear(3)
%      call timer_start(1)
%
%      call vranlc(0, t1, a, x)	%%% can't make any sense of this

    ierr   = MPI_Barrier(MPI_COMM_WORLD);
    timer(1:3) = 0;				%%% already was 0
    timer(1)= MPI_Wtime-timer(1);		%%% toggle timer1 -> start

%    t1     = s;		%%% these FORTRAN guys love uninitialized vars
%   [t1,t3] = vranlc(0,t1,a);	%%% Hmpf... what was the previous call for ?!?
				%%% again t3=[], don't destroy global X

%%% vranlc already in-mem, these 2 lines do nothing ?!? can't understand it
%%% and timer1 _is_running_ !!! and x (t3) is distroyed (MATLAB/Octave) !!!
%%% double-checked: t1 declared@line 63 & used here@line 159 uninitialized
%%% tried on my computer: no time difference, measurements unaffected
%%% CLASS S: 8.66-8.70s w/lines, 8.65-8.71s w/out them
%%% difference falls inside measurement error

		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%c   Compute AN = A ^ (2 * NK) (mod 2^46).
%
%      t1 = a
%				%%% recall nk=2^mk = 2^16=64Kpairs batchsize
%      do 100 i = 1, mk + 1	%%% sequence:	x_0 = t1 = a
%         t2 = randlc(t1, t1)	%%%	i=1	x_1 = a^2 mod... t1 updated
% 100  continue			%%%     i=2	x_2 =(a^2 mod...)^2 mod...
%				%%%		    = a^4 mod...
%      an = t1			%%%	i=mk	x_mk= a^(2^ mk   ) ~= a^(2 mk)
%      tt = s			%%%	i=mk+1	xmk1= a^(2^(mk+1))  =...
%      gc = 0.d0		%%%		      a^(2^mk *2^1) = a^(nk*2)
%      sx = 0.d0
%      sy = 0.d0
%
%      do 110 i = 0, nq - 1
%         q(i) = 0.d0
% 110  continue

    t1 = a;
    for i = 1:mk+1		%%% (bussb) an = fix(5^13); for i=...
      [t2,t1] = randlc(t1,t1);	%%% 	... [t2,an] = vranlc(an,1,an);
    end				%%% an = a-next, mk=16, nk=64Kpairs

    [an, tt]	= deal(t1, s);	%%% AN = A^(2NK) = seq. factor to next batch
    [gc, sx, sy]= deal(0);	%%% TT = seed, Gaussian count & sums = 0

%   q = zeros(nq,1);	%%% already initialized, recall q(0:nq-1) 0-indexed
			%%% this is inside timer(1), good idea to alloc before

		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      if (np_add .eq. 1) then
%         k_offset = node * np -1
%      else
%         k_offset = no_large_nodes*(np+1) + (node-no_large_nodes)*np -1
%      endif

    %%% EXAMPLE: class S, m=24,	mk=16,		mm=8
    %%%				nk=64K pairs,	nn=256 batches
    %%%				no_nodes=8,	np=256/8=32 batches/processor
    %%%						no_large_nodes=0
    %%%				no_nodes=7,	np=256/7=36 batches/processor
    %%%						no_large_nodes=4 batches left
    %%% np_add=1 for ranks 0,1,2,3 < 4

    if np_add==1, k_offset =           node* np - 1;
    else,	  k_offset = no_large_nodes*(np+1) +...
		       (node-no_large_nodes)*np - 1;
    end

    %%% no_nodes=8, np=32 batches/proc, np_add=0 forall ranks node=0..7
    %%%		    k_offset=-1, 31, 63, 95, 127, 159, 191, 223     (next->255)
    %%% no_nodes=7, np=36 batches/proc, np_add=1 for    ranks node=0..3
    %%%		    np=37 for ranks 0..3 (4*(36+1)=148)
    %%%		    k_offset=-1, 36, 73, 110 // 148-1, 148+35, 148+71(nxt +107)
    %%%						147,   183,    219   (nxt->255)
    %%% CONCLUSION: this node is to compute np batches 0-indexed from
    %%%			k_offset+1 ... k_offset+np (0-indexed)

		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%c   Each instance of this loop may be performed independently.
%c   We compute the k offsets separately to take into account the fact that
%c   some nodes have more numbers to generate than others
%						
%      do 150 k = 1, np		%%% label 150 far way down; long, long loop
%         kk = k_offset + k	%%% kk = batch index (0-indexed)
%         t1 = s		%%% t1 = seed for batch#0
%         t2 = an		%%% an = factor for next batch
%
	%%% very long loop, broken into pieces, this is the 1st one
	%%% first we account which batch-indexes correspond to our np batches

    for k=1:np				%%% for kk=0:np-1, according to (bussb)
	 kk = k_offset+k;		%%% kk=0..31/32..63/.../224..255
	[t1, t2] = deal(s,an);		%%% kk=0..36/37..73/.../220..255

	%%% kk are our batches' indexes (0-indexed), say 32..63
	%%% we should continue the sequence from t1 = s * an^32
	%%% to make iterations independent, NPB recomputes t1 at each iteration
	%%% we could first compute t1 out of the loop and proceed sequentially
	%%% the binary exponentiation algorithm below is painful
	%%% kk is destroyed below - just generated for the binary exp alg

		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%c        Find starting seed t1 for this kk.
%
%         do 120 i = 1, 100	%%% binary algorithm for exponentiation
%            ik = kk / 2	%%% translating kk to binary by succesive div2
%            if (2 * ik .ne. kk) t3 = randlc(t1, t2)	%%% if LSB set, s*an^1
%            if (ik .eq. 0) goto 130			%%% no more bits?
%            t3 = randlc(t2, t2)		%%% more bits: an^2, ^4, ^8...
%            kk = ik
% 120     continue
%
%c        Compute uniform pseudorandom numbers.
% 130     continue

%%%	for i=1:100			%%% no clue, kk is at most 16bits long
%%%	    ik = floor(kk/2);		%%% should be while, not for
%%%	    if     mod(kk,2),	[t3,t1] = randlc(t1,t2); end
%%%	    if   ik==0,		break,		    end
%%%				[t3,t2] = randlc(t2,t2);
%%%	    kk = ik;
%%%	end

%%%	kkbits = bitget(kk,1:16);	%%% not so good idea, slow
%%%	kkpows = find(kkbits);

 	while kk~=0			%%% bits remaining: which powers?
 	    if     mod(kk,2),	[t3,t1] = randlc(t1,t2); end	% add this pwr
 	    kk = floor(kk/2);	[t3,t2] = randlc(t2,t2);	% next power
 	end				%%% last power-up not really required

%%% tried on my computer: no time difference, measurements/result unaffected
%%% CLASS S: 8.68-8.73s w/original, 8.67-8.90s w/new version
%%% difference falls inside measurement error

%%% recall m=24/25/28/30/32 for class=S/W/A/B/C
%%% EXAMPLE: class S, m=24,	mk=16,nk=2^16=64Kpairs, mm= 8,nn=2^8=256 batches
%%%	     class C, m=32,	mk=16,nk=2^16=64Kpairs, mm=16,nn=    64K batches
%%%				last kk values are 64K-np..64K-1, 16bit enough

		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         if (timers_enabled) call timer_start(3)
%         call vranlc(2 * nk, t1, a, x)
%         if (timers_enabled) call timer_stop(3)

			%%%%%%%%%%
			% RANDOM %
			%%%%%%%%%%

    if timers_enabled, timer(3) = MPI_Wtime-timer(3); end
      if USE_GLOBAL_X,	if USE_CLEAR, global X, end	%%% recover global X
			 t1     = vranlc(2*nk, t1, a);	%%% store on global X
      else,		[t1, X] = vranlc(2*nk, t1, a);	%%% alloc/destroy
      end						%%% upcase X also
    if timers_enabled, timer(3) = MPI_Wtime-timer(3); end


		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%c        Compute Gaussian deviates by acceptance-rejection method and
%c        tally counts in concentric square annuli.
%c        This loop is not vectorizable.
%
%         if (timers_enabled) call timer_start(2)
%
%         do 140 i = 1, nk
%            x1 = 2.d0 * x(2*i-1) - 1.d0	%%% upcase X serves both cases
%            x2 = 2.d0 * x(2*i) - 1.d0
%            t1 = x1 ** 2 + x2 ** 2
%            if (t1 .le. 1.d0) then
%               t2   = sqrt(-2.d0 * log(t1) / t1)
%               t3   = (x1 * t2)
%               t4   = (x2 * t2)		%%% l=square annulus for (x1,x2)
%               l    = max(abs(t3), abs(t4))	%%% l is not integer, but
%               q(l) = q(l) + 1.d0		%%% FORTRAN fixes on indexing
%               sx   = sx + t3			%%% recall q(0:nq-1) 0-indexed
%               sy   = sy + t4
%            endif
% 140     continue
%
%         if (timers_enabled) call timer_stop(2)
%
% 150  continue

			%%%%%%%%%%%%
			% GAUSSIAN %
			%%%%%%%%%%%%

	if timers_enabled, timer(2) = MPI_Wtime-timer(2); end

	%%% SCALAR (LOOP) VERSION
	if ~USE_VECTnLOOP		%%% surprisingly, better if not vector
					%%% for MLR14 (due to JIT compiler)
	  for i=1:nk			%%% scalar     version, nk=2^mk=64K
	    x1 = 2 .* X(2*i-1) - 1;	%%% odd x elements are abscissa
	    x2 = 2 .* X(2*i  ) - 1;	%%% even  elements are ordinates
	    t1 = x1.^2 + x2.^2;		%%% square radius (norm)
	    if t1<=1			%%% pairs inside unit circle

 		if  USE_REAL_MATH	%%% if inside for can hurt performance
 	t2=realsqrt(-2*reallog(t1)/t1);	else	%%% surprisingly this doesn't
 	t2=    sqrt(-2*    log(t1)/t1);	%%% w/realmath: 8.70s,  w/out: 10.14
 		end			%%% w/out ifs:  8.61s,         10.17
					%%% and never use mylog=@log... worst!!
		t3 = x1 * t2;		%%% reuse t1/t2 ?!?
		t4 = x2 * t2;		%%% square annuli foreach pair
		l  = floor(max(abs(t3),abs(t4)))+1;
%%%		if l>10			%%% this never happens !?!
%%%		   fprintf('\7l-index error batch#%d, rand#%d, l=%d\n',k,i,l);
%%%		end			%%% and if sentence hurts performance
		q(l)=q(l)+1;		%%% let it error out here in case l>10
		sx = sx + t3;
		sy = sy + t4;
	    end % if t1<=1
	  end % for i=1:nk

	%%% VECTORIZED (ARRAY LANG) VERSION
	else % if ~USE_VECTnLOOP

	%for i=1:nk			%%% vectorized version, nk=2^mk=64K
	    x1 = 2 .* X(1:2:2*nk) - 1;	%%% odd x elements are abscissa
	    x2 = 2 .* X(2:2:2*nk) - 1;	%%% even  elements are ordinates
	if  USE_CLEAR, clear X, end	%%% never clear global X, though
					%%% X~1MB/x1+x2~1MB/t1+(t2+idx)~1.5MB
	    t1 = x1.^2 + x2.^2;		%%% square radius (norm)
	    idx= find (t1 <= 1);	%%% pairs inside unit circle
	if  USE_REAL_MATH
	     t2 = realsqrt(-2.*reallog(t1(idx))./t1(idx) );
	else,t2 =     sqrt(-2.*    log(t1(idx))./t1(idx) );
	end
	if  USE_CLEAR, clear t1, end

	    t3 = x1(idx) .* t2;		%%% reuse t1/t2 ?!?
	    t4 = x2(idx) .* t2;
	if  USE_CLEAR, clear x1 x2 t2 idx, end

	    l =floor(max(abs(t3),abs(t4)))+1;	%%% square annuli foreach pair
	for i=1:10			%%% l>10 simply wouldn't be counted
	    q(i) = q(i) + length(find(l==i));
	end
	    sx = sx + sum(t3);
	    sy = sy + sum(t4);
	if  USE_CLEAR, clear t3 t4, end
	%end % for i=1:nk

	%%% NO MORE VERSIONS
	end % if ~USE_VECTnLOOP

	if timers_enabled, timer(2) = MPI_Wtime-timer(2); end

    end	% for k=1:np

	%%% EXPLANATION about q(l):
	%%% assume all pairs are on the x axis, ie, all x2 are 0
	%%% particularly we set x1=]0..1] (0 not included to compute log())
	%%% then all norms t1 = x1.^2, all inside the unit circle
	%%% we could plot t3 against x1 and we get something like this
	% 10|
	%  9|
	%  8|
	%  7|
	%  6|
	%  5|
	%  4\
	%  3 """***---___
	%  2             """"""******------_______
	%  1                                      ****----__
	%  0  0.1  0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9  1\
	%
	%%% some refs:	x1 = 1E-4 1E-5 1E-6 1E-7 1E-8 1E-9 1E-10 1E-11 1E-12
	%%%		t3 = 6.01 6.78 7.43 8.02 8.58 9.10 9.59  10.07 10.51
	%%% some refs:	x1 = 1E-15 1E-18 1E-24 1E-30 1E-40 1E-50 1E-100 1E-150
	%%%		t3 = 11.75 12.88 14.87 16.62 19.19 21.45 30.349 37.169
	%%% notice placement of the qq variable in the original FORTRAN code
	%%% to avoid overwriting variables past q(9). Apparently, to no avail
	%
	%      common/storage/ x(2*nk), q(0:nq-1), qq(10000)
	%      data             dum /1.d0, 1.d0, 1.d0/

    

		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      call mpi_allreduce(sx, x, 1, dp_type,
%     >                   MPI_SUM, MPI_COMM_WORLD, ierr)
%      sx = x(1)
%      call mpi_allreduce(sy, x, 1, dp_type,
%     >                   MPI_SUM, MPI_COMM_WORLD, ierr)
%      sy = x(1)
%      call mpi_allreduce(q, x, nq, dp_type,
%     >                   MPI_SUM, MPI_COMM_WORLD, ierr)
%
%      do i = 1, nq
%         q(i-1) = x(i)
%      enddo
%
%      do 160 i = 0, nq - 1
%        gc = gc + q(i)
% 160  continue

	%%% Allreduce really required ?!? not simply Reduce ?!?
	%%% Anyways, timing difference is soooo smaaaaallll...
	%%% different destination buffer required, Allreduce(sx,sx...) failed
	%%% X has been cleared, and length(q) ~= length(X)
	%%% can use a new x same size(q), gc=sum(x)
	%%% anyways, q=x(1:nq) required, since later check on q
 							   x=zeros(size(q));

    ierr = MPI_Allreduce(sx,x,MPI_SUM,MPI_COMM_WORLD);	sx=x(1);
    ierr = MPI_Allreduce(sy,x,MPI_SUM,MPI_COMM_WORLD);	sy=x(1);
    ierr = MPI_Allreduce(q ,x,MPI_SUM,MPI_COMM_WORLD);	q=x;     gc=sum(q);

%   ierr=MPI_Reduce(sx,x,MPI_SUM,root,MPI_COMM_WORLD);	sx=x(1);
%   ierr=MPI_Reduce(sy,x,MPI_SUM,root,MPI_COMM_WORLD);	sy=x(1);
%   ierr=MPI_Reduce(q ,x,MPI_SUM,root,MPI_COMM_WORLD);	q=x;     gc=sum(q);


		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      call timer_stop(1)
%      tm  = timer_read(1)
%
%      call mpi_allreduce(tm, x, 1, dp_type,
%     >                   MPI_MAX, MPI_COMM_WORLD, ierr)
%      tm = x(1)

    timer(1)= MPI_Wtime-timer(1);			tm = timer(1);
    ierr = MPI_Allreduce(tm,x,MPI_MAX,MPI_COMM_WORLD);	tm = x(1);
%   ierr=MPI_Reduce(tm,x,MPI_MAX,root,MPI_COMM_WORLD);	tm = x(1);


    if USE_GLOBAL_X, clear global X, end	% now timer(1) has stopped

		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      if (node.eq.root) then
%         nit=0
%         if (m.eq.24) then
%            if((abs((sx- (-3.247834652034740D3))/sx).le.epsilon).and.
%     >         (abs((sy- (-6.958407078382297D3))/sy).le.epsilon))
%     >         verified = .TRUE.
%         elseif (m.eq.25) then
%            if ((abs((sx- (-2.863319731645753D+03))/sx).le.epsilon).and.
%     >          (abs((sy- (-6.320053679109499D+03))/sy).le.epsilon))
%     >         verified = .TRUE.
%         elseif (m.eq.28) then
%            if ((abs((sx- (-4.295875165629892D3))/sx).le.epsilon).and.
%     >          (abs((sy- (-1.580732573678431D4))/sy).le.epsilon))
%     >         verified = .TRUE.
%         elseif (m.eq.30) then
%            if ((abs((sx- (4.033815542441498D4))/sx).le.epsilon).and.
%     >          (abs((sy- (-2.660669192809235D4))/sy).le.epsilon))
%     >         verified = .true.
%         elseif (m.eq.32) then
%            if ((abs((sx- (4.764367927995374D+04))/sx).le.epsilon).and.
%     >          (abs((sy- (-8.084072988043731D+04))/sy).le.epsilon))
%     >         verified = .true.
%         endif

    if node==root
	nit = 0;				%%% #Iterations, bogus
	switch m
	case 24, verified=...					%%% CLASS S
		(abs((sx- (-3.247834652034740D3))/sx) <= epsilon) &&...
		(abs((sy- (-6.958407078382297D3))/sy) <= epsilon) &&...
	all(q==[   6140517;   5865300;  1100361;   68546;  1648;  17; 0;0;0;0]);
	case 25, verified=...					%%% CLASS W
		(abs((sx- (-2.863319731645753D+03))/sx) <= epsilon) &&...
		(abs((sy- (-6.320053679109499D+03))/sy) <= epsilon) &&...
	all(q==[  12281576;  11729692;  2202726;  137368;  3371;  36; 0;0;0;0]);
	case 28, verified=...					%%% CLASS A
		(abs((sx- (-4.295875165629892D3))/sx) <= epsilon) &&...
		(abs((sy- (-1.580732573678431D4))/sy) <= epsilon) &&...
	all(q==[  98257395;  93827014; 17611549; 1110028; 26536; 245; 0;0;0;0]);
	case 30, verified=...					%%% CLASS B
		(abs((sx- ( 4.033815542441498D4))/sx) <= epsilon) &&...
		(abs((sy- (-2.660669192809235D4))/sy) <= epsilon) &&...
	all(q==[ 393058470; 375280898; 70460742; 4438852;105691; 948; 5;0;0;0]);
	case 32, verified=...					%%% CLASS C
		(abs((sx- ( 4.764367927995374D+04))/sx) <= epsilon) &&...
		(abs((sy- (-8.084072988043731D+04))/sy) <= epsilon) &&...
	all(q==[1572172634;1501108549;281805648;17761221;424017;3821;13;0;0;0]);
	end						%%% q data from buss
		%%%%%%%%%%%%%%%%%%%
		% From RNR-94-007 %
		%%%%%%%%%%%%%%%%%%%
% The two sums must agree with reference values to within one part in 10^12.
% Class A reference values: -4.295875165629892 10^3, -1.580732573678431 10^4
% Class B reference values:  4.033815542441498 10^4, -2.660669192809235 10^4
% Each of the q(1:10) counts must agree exactly with reference values table 2.1
% index l:         0         1        2       3      4   5 6 7 8 9
% Class A:  98257395  93827014 17611549 1110028  26536 245 0 0 0 0
% Class B: 393058470 375280898 70460742 4438852 105691 948 5 0 0 0
		%%%
		%%% original ep.f did not include q verification !?!

		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         Mops = 2.d0**(m+1)/tm/1000000.d0
%
%         write (6,11) tm, m, gc, sx, sy, (i, q(i), i = 0, nq - 1)
% 11      format ('EP Benchmark Results:'//'CPU Time =',f10.4/'N = 2^',
%     >           i5/'No. Gaussian Pairs =',f15.0/'Sums = ',1p,2d25.15/
%     >           'Counts:'/(i3,0p,f15.0))
%
%         call print_results('EP', class, m+1, 0, 0, nit, npm,
%     >                      no_nodes, tm, Mops,
%     >                      'Random numbers generated',
%     >                      verified, npbversion, compiletime, cs1,
%     >                      cs2, cs3, cs4, cs5, cs6, cs7)
%
%      endif

	Mops = 2^(m+1)/tm/1E6;

	fprintf('\nEP Benchmarks Results:\n\n');
	fprintf('CPU Time = %10.4f\n',		tm   );
	fprintf('N = 2^%5d\n',			 m   );
	fprintf('No. Gaussian Pairs =%14.0f.\n',gc   );
	fprintf('Sums = %25.15e%25.15e\n',	sx,sy);	%%% 1p?
	fprintf('Counts:\n');
	fprintf('%3d%14.0f.\n', [0:nq-1; q(1:nq)']);	%%% 0p?

		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/common/print_results.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         write (*, 2) name
% 2       format(//, ' ', A2, ' Benchmark Completed.')
%
%         write (*, 3) Class
% 3       format(' Class           = ', 12x, a12)
%	...
%            if (name(1:2) .eq. 'EP') then
%               write(size, '(f12.0)' ) 2.d0**n1
%               do j =13,1,-1
%                  if (size(j:j) .eq. '.') size(j:j) = ' '
%               end do
%               write (*,42) size
% 42            format(' Size            = ',12x, a14)
%            else

	fprintf('\n\n EP Benchmark Completed.\n');
	fprintf(' Class           = %24s  \n',  class);
	fprintf(' Size            = %24.0f\n',2^(m+1));

%         write (*, 5) niter
% 5       format(' Iterations      = ', 12x, i12)
%         write (*, 6) t
% 6       format(' Time in seconds = ',12x, f12.2)
%         write (*,7) nprocs_total
% 7       format(' Total processes = ', 12x, i12)
%         write (*,8) nprocs_compiled
% 8       format(' Compiled procs  = ', 12x, i12)
%         write (*,9) mops
% 9       format(' Mop/s total     = ',12x, f12.2)
%         write (*,10) mops/float( nprocs_total )
% 10      format(' Mop/s/process   = ', 12x, f12.2)
%         write(*, 11) optype
% 11      format(' Operation type  = ', a24)

%         call print_results('EP', class, m+1, 0, 0, nit, npm,
%     >                      no_nodes, tm, Mops,
%     >                      'Random numbers generated',
%     >                      verified, npbversion, compiletime, cs1,
%     >                      cs2, cs3, cs4, cs5, cs6, cs7)

%      subroutine print_results(name, class, n1, n2, n3, niter,
%     >               nprocs_compiled, nprocs_total,
%     >               t, mops, optype, verified, npbversion,
%     >               compiletime, cs1, cs2, cs3, cs4, cs5, cs6, cs7)

	fprintf(' Iterations      = %24d  \n',	nit);
	fprintf(' Time in seconds = %24.2f\n',	tm );
	fprintf(' Total processes = %24d  \n',	no_nodes);
	fprintf(' Compiled procs  = %24d  \n',	npm     );
	fprintf(' Mop/s total     = %24.6f\n',	Mops         );
	fprintf(' Mop/s/process   = %24.6f\n',	Mops/no_nodes);
	fprintf(' Operation type  = %24s  \n', 'Random numbers generated');

		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/common/print_results.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         if (verified) then
%            write(*,12) '  SUCCESSFUL'
%         else
%            write(*,12) 'UNSUCCESSFUL'
%         endif
% 12      format(' Verification    = ', 12x, a)
%         write(*,13) npbversion
% 13      format(' Version         = ', 12x, a12)
%         write(*,14) compiletime
% 14      format(' Compile date    = ', 12x, a12)
%         write (*,121) cs1
% 121     format(/, ' Compile options:', /,
%	...
%         write(*, 127) cs7
% 127     format('    RAND         = ', A)

	if verified, V='  '; else, V='UN'; fprintf('%c',7); end
	fprintf(' Verification    = %24s\n',	[V,'SUCCESSFUL']);
	fprintf(' Version         = %24s\n',	'NPB2.3 for MPITB');
						if all(SOFT=='MATLAB')
	fprintf(' Run     date    = %24s\n',	datestr(now));	else
	fprintf(' Run     date    = %24s\n',	ctime (time));	end
%	fprintf(' Run     date    = %24s\n',	date);
	fprintf('\n');
	fprintf(' Design  options:\n');
	fprintf('    ENVIRONMENT  = %s %s\n',	 SOFT,version);
	[s,t]=unix('lamboot -V'); t1=findstr('LAM',t);
				  t2=findstr( '-' ,t); t2=t2(1)-2;
	fprintf('    MPI_LIBRARY  = %s\n',	 t(t1:t2));
	fprintf('    TOOLBOX      = %s\n',	'MPITB');
	fprintf('    RAND         = %s\n',	'randdp (floor/arith)');

    end	%%% if node==root


		%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/EP/ep.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      if (timers_enabled .and. (node .eq. root)) then
%          print *, 'Total time:     ', timer_read(1)
%          print *, 'Gaussian pairs: ', timer_read(2)
%          print *, 'Random numbers: ', timer_read(3)
%      endif
%
%      call mpi_finalize(ierr)
%
%      end

    if timers_enabled && node==root
	fprintf('\n\n');
	fprintf('Total time:    %15.8f\n', timer(1));
	fprintf('Gaussian pairs:%15.8f\n', timer(2));
	fprintf('Random numbers:%15.8f\n', timer(3));
    end

    ierr = MPI_Finalize;
    MPITB_Clean;

%%%%%%%%%%%%%%%%%%%
% ENVIRON RESTORE %
%%%%%%%%%%%%%%%%%%%
    detect(0);

%end % function ep(class)
