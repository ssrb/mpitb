function [x,varargout] = vranlc(n,x,a)
%  VRANLC	NAS Parallel Benchmark randdp batch LinearCongruentialGenerator
%		based on MATLAB/Octave version by Benjamin Buss & Dave Ennis
%
%   x     = vranlc (n, x, a)		% global var X(1:n) overwritten
%  [x, y] = vranlc (n, x, a)		% new  array y(1:n) created & returned
%
%  n (int)	number of pseudorandom numbers to generate
%		n numbers in the range (0..1) are returned in y (or stored in X)
%  x (flint)	seed - current integer number in the LC sequence
%  a (flint)	sequence factor - NPB suggests a=5^13=1220703125, x_0=271828183
%		both a,x must be odd integers (in fact flints) 1..2^46-1
%		the sequence is x_{k+1} = a x_k  (mod 2^46)
%		with the suggested values, the sequence period is 2^44
%		(x_0 is prime)
%
%  x (flint)	last integer number in the LC sequence x_n is also returned
%		continuous sequence can be generated using same args over&over
%		user is expected to use same var for both x (in/out)
%  y / global X	array that stores the n pseudorandom numbers in the range (0..1)
%
%  SEE ALSO:
%	RANDLC, EP, FT, MG

% -----------------------------------------------------
% ## Copyright (C) 2005-2007 Javier Fernández Baldomero, Mancia Anguita López
% ##
% ## This program is free software
% -----------------------------------------------------

%%%%%%%%%%%%%
%  HISTORY: %
%%%%%%%%%%%%%
%	A Pseudorandom Number Generator for the Parallel NAS Kernels
%	From "The NAS Parallel Benchmarks", Sect.2.3, pp.32--35
%	Technical Report RNR-94-007, March 1994.
%	http://www.nas.nasa.gov/Software/NPB/
%
% Adapted to MPITB by Javier Fernández-Baldomero (Universidad de Granada, 2005)
%			& Mancia Anguita López (Universidad de Granada)
% Different versions tested in scripts vrantest[1-4].m

		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/common/randdp.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%c---------------------------------------------------------------------
%      subroutine vranlc (n, x, a, y)
%c---------------------------------------------------------------------
%c   This routine generates N uniform pseudorandom double precision numbers in
%c   the range (0, 1) by using the linear congruential generator
%c
%c   x_{k+1} = a x_k  (mod 2^46)
%c
%c   where 0 < x_k < 2^46 and 0 < a < 2^46.  This scheme generates 2^44 numbers
%c   before repeating.  The argument A is the same as 'a' in the above formula,
%c   and X is the same as x_0.  A and X must be odd double precision integers
%c   in the range (1, 2^46).  The N results are placed in Y and are normalized
%c   to be between 0 and 1.  X is updated to contain the new seed, so that
%c   subsequent calls to VRANLC using the same arguments will generate a
%c   continuous sequence.  If N is zero, only initialization is performed, and
%c   the variables X, A and Y are ignored.
%c
%c   This routine is the standard version designed for scalar or RISC systems.
%c   However, it should produce the same results on any single processor
%c   computer with at least 48 mantissa bits in double precision floating point
%c   data.  On 64 bit systems, double precision should be disabled.
%c---------------------------------------------------------------------
%      implicit none
%
%      integer i,n
%      double precision y,r23,r46,t23,t46,a,x,t1,t2,t3,t4,a1,a2,x1,x2,z
%      dimension y(*)
%      parameter (r23 = 0.5d0 ** 23, r46 = r23 ** 2, t23 = 2.d0 ** 23,
%     >  t46 = t23 ** 2)

% Adapted to MPITB by Javier Fernández-Baldomero (Universidad de Granada, 2005)
%			& Mancia Anguita López (Universidad de Granada)
% Several choices exercised, see vrantest?.m for details
% best final choice: use rand/arith w/global Y array
% MATLAB would have benefited from while-loop (instead of for-loop)
% Octave would have benefited from factoring out 2^-46 (instead of elm-by-elm)

if nargout<2			% MATLAB X is FORTRAN y(*)
  global X			% X should have room for N randoms
else
  X=-1.D99*ones(n,1);		%		room for N randoms
% X=       ones(n,1);		%		with different initializations
% X=      zeros(n,1);		%		very little impact, really
end

%c---------------------------------------------------------------------
%c   Break A into two parts such that A = 2^23 * A1 + A2.
%c---------------------------------------------------------------------
%      t1 = r23 * a
%      a1 = int (t1)
%      a2 = a - t23 * a1

    a1 = floor(a *2^-23);	% avoid overhead repeated calling randlc
    a2 = a  -  a1*2^ 23 ;	% factor out A=A1:A2 decomposition

%c---------------------------------------------------------------------
%c   Generate N results.   This loop is not vectorizable.
%c---------------------------------------------------------------------
%      do i = 1, n

  for i=1:n
%c---------------------------------------------------------------------
%c   Break X into two parts such that X = 2^23 * X1 + X2,
%c---------------------------------------------------------------------
%        t1 = r23 * x
%        x1 = int (t1)
%        x2 = x - t23 * x1

    x1 = floor(x *2^-23);
    x2 = x  -  x1*2^ 23 ;
                                                                                
%c---------------------------------------------------------------------
%c   compute Z = A1 * X2 + A2 * X1  (mod 2^23),
%c---------------------------------------------------------------------
%        t1 = a1 * x2 + a2 * x1
%        t2 = int (r23 * t1)
%        z = t1 - t23 * t2

    t1 = a1*x2 + a2*x1;
    t2 = floor(t1*2^-23);
    z  = t1 -  t2*2^ 23 ;
                                                                                
%c---------------------------------------------------------------------
%c   and then X = 2^23 * Z + A2 * X2  (mod 2^46).
%c---------------------------------------------------------------------
%        t3 = t23 * z + a2 * x2
%        t4 = int (r46 * t3)
%        x = t3 - t46 * t4
%        y(i) = r46 * x
%      enddo

    t1 = z*2^23 + a2*x2;
    t2 = floor(t1*2^-46);
    x  = t1 -  t2*2^ 46 ;
                                                                                
    X(i)=x * 2^-46;
  end

%      return
%      end

%%% MATLAB language was able to simply declare	function [x,X] = vranlc(n,x,a)
%%% with global X and then this sentence was not required.
%%% Octave grammar won't let us make global X on a function return value
%%% so we better declare		function [x,varargout] = vranlc(n,x,a)
%%% and then we need this sentence.

  if nargout>1
   varargout={X};
  end

%end

