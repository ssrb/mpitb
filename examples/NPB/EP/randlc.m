function [r,x] = randlc(x,a)
%  RANDLC	NAS Parallel Benchmark randdp linear congruential generator
%		based on MATLAB/Octave version by Benjamin Buss & Dave Ennis
%		based in turn on C/FORTRAN version by David H. Bailey / NPB2.3
%
%  [r,x] = randlc (x, a)
%
%  x (flint)	46bit seed - current integer number in the LC sequence
%  a (flint)	sequence factor - NPB suggests a=5^13=1220703125, x_0=271828183
%		both a,x must be odd integers in the reange 1..2^46-1
%		the sequence is x_{k+1} = a x_k  (mod 2^46)
%		with the suggested values, the sequence period is 2^44
%		(x_0 is prime)
%
%  r (0...1)	next pseudorandom value x_{k+1}/2^46. x_{k+1} is also returned
%  x (flint)	the next integer number in the LC sequence is also returned
%		continuous sequence can be generated using same args over&over
%		user is expected to use same var for both x (in/out)
%
%  SEE ALSO:
%       VRANLC, EP, FT, MG

% -----------------------------------------------------
% ## Copyright (C) 2005-2007 Javier Fernández Baldomero, Mancia Anguita López
% ##
% ## This program is free software
% -----------------------------------------------------

%%%%%%%%%%%%%
%  HISTORY: %
%%%%%%%%%%%%%
%       A Pseudorandom Number Generator for the Parallel NAS Kernels
%       From "The NAS Parallel Benchmarks", Sect.2.3, pp.32--35
%       Technical Report RNR-94-007, March 1994.
%       http://www.nas.nasa.gov/Software/NPB/
%
% Adapted to MPITB by Javier Fernández-Baldomero (Universidad de Granada, 2005)
%			& Mancia Anguita López (Universidad de Granada)
% Different versions tested in script randtest.m

		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% From NPB2.3-MPI/common/randdp.f %
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%c---------------------------------------------------------------------
%      double precision function randlc (x, a)
%c---------------------------------------------------------------------
%c   This routine returns a uniform pseudorandom double precision number in the
%c   range (0, 1) by using the linear congruential generator
%c
%c   x_{k+1} = a x_k  (mod 2^46)
%c
%c   where 0 < x_k < 2^46 and 0 < a < 2^46.  This scheme generates 2^44 numbers
%c   before repeating.  The argument A is the same as 'a' in the above formula,
%c   and X is the same as x_0.  A and X must be odd double precision integers
%c   in the range (1, 2^46).  The returned value RANDLC is normalized to be
%c   between 0 and 1, i.e. RANDLC = 2^(-46) * x_1.  X is updated to contain
%c   the new seed x_1, so that subsequent calls to RANDLC using the same
%c   arguments will generate a continuous sequence.
%c
%c   This routine should produce the same results on any computer with at least
%c   48 mantissa bits in double precision floating point data.  On 64 bit
%c   systems, double precision should be disabled.
%c
%c   David H. Bailey     October 26, 1990
%c---------------------------------------------------------------------
%
%      implicit none
%
%      double precision r23,r46,t23,t46,a,x,t1,t2,t3,t4,a1,a2,x1,x2,z
%      parameter (r23 = 0.5d0 ** 23, r46 = r23 ** 2, t23 = 2.d0 ** 23,
%     >  t46 = t23 ** 2)

% Adapted to MPITB by Javier Fernández-Baldomero (Universidad de Granada, 2005)
%			& Mancia Anguita López (Universidad de Granada)
% Several choices exercised, see randtest.m for details
% best final choice for both MATLAB/Octave: use floor/arithmetics

%c---------------------------------------------------------------------
%c   Break A into two parts such that A = 2^23 * A1 + A2.
%c---------------------------------------------------------------------
%      t1 = r23 * a
%      a1 = int (t1)
%      a2 = a - t23 * a1

	a1 = floor(a *2^-23);	% floor has less overhead than fix
	a2 = a  -  a1*2^ 23 ;	% arith has less overhead than mod/rem

%c---------------------------------------------------------------------
%c   Break X into two parts such that X = 2^23 * X1 + X2,
%c---------------------------------------------------------------------
%      t1 = r23 * x
%      x1 = int (t1)
%      x2 = x - t23 * x1

	x1 = floor(x *2^-23);	% * is faster than /, so better use 2^-23
	x2 = x  -  x1*2^ 23 ;	% literals are faster than named constants
				% 2^23 only slightly slower than 8388608
				% (harder to compute, easier to interpret)

% Adapted to MPITB by Javier Fernández-Baldomero (Universidad de Granada, 2005)
%			& Mancia Anguita López (Universidad de Granada)
%					---------------------------------
%		A (46bits)	=	|	A1	|	A2	|
%					---------------------------------
%		X (46bits)	=	|	X1	|	X2	|
%					---------------------------------
%					|		A2X2 (46bits)	|
%			-------------------------------------------------
%			|	OVR	A1X2 (46bits)	|
%			---------------------------------
%			|	OVR	A2X1 (46bits)	|
%	-------------------------------------------------
%	|	OVR	A1X1 (46bits)	|
%	---------------------------------
%
%	Overflow for: whole A1X1 / MSPart A1X2+A2X1 =>
%			    A1X1 done, go for LSPart (23LSBs) of A1X2+A2X1

%c---------------------------------------------------------------------
%c   compute Z = A1 * X2 + A2 * X1  (mod 2^23),
%c---------------------------------------------------------------------
%      t1 = a1 * x2 + a2 * x1
%      t2 = int (r23 * t1)
%      z = t1 - t23 * t2

	t1 = a1*x2 + a2*x1;
	t2 = floor(t1*2^-23);
	z  = t1 -  t2*2^ 23 ;

%c---------------------------------------------------------------------
%c   and then X = 2^23 * Z + A2 * X2  (mod 2^46).
%c---------------------------------------------------------------------
%      t3 = t23 * z + a2 * x2
%      t4 = int (r46 * t3)
%      x = t3 - t46 * t4
%      randlc = r46 * x
%
%      return
%      end

	t1 = z*2^23 + a2*x2;	% reusing t1/t2 better than new t3/t4
	t2 = floor(t1*2^-46);
	x  = t1 -  t2*2^ 46 ;	% updated seed, also returned

	r  = x * 2^-46;		% random number in the range 0..1
%end

%%% the FORTRAN version used integer*8 (int64) for performance reasons
%%%		mod(x,2^46-1) operator comes almost free as iand
%%% under MATLAB/Octave, best turned out to be rand/arith

