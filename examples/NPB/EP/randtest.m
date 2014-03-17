function randtest
% Auxililary function to test different implementations of the random LCG
% SEE ALSO: randlc, vranlc, vrantest

% -----------------------------------------------------
% ## Copyright (C) 2005-2007 Javier Fernández Baldomero, Mancia Anguita López
% ##
% ## This program is free software
% -----------------------------------------------------

%%%%%%%%%%%%%
% Constants %
%%%%%%%%%%%%%
    x0 = 271828183;		% prime number, random seed
     a = 5^13;			% LCG factor x_n+1 = a * x_n (mod 2^46)
  SOFT = detect;
  if all(SOFT=='MATLAB')	% iterations for time measurements
	m=5E5;			% chosen so that each case lasts some 15-20s
  else,	m=5E3;			% and we can do the maths in our heads
  end
  rslt=x0; mm=m; s=a;		%%% binary exponentiation algorithm
  while mm~=0			%%% bits remaining: which powers?
    if     mod(mm,2),	[r,rslt] = randlc(rslt,s); end	% add this pwr
    mm = floor(mm/2);   [r,s   ] = randlc(   s,s);	% next power
  end				%%% last power-up not really required

% fprintf('m = %d, rslt = %.0f\n',m,rslt); return
% rslt = 25826191335223;	% Linear Congruential Generator iteration 3E3
% rslt = 66542889707383;	% Linear Congruential Generator iteration 5E3
% rslt = 50833261544983;	% Linear Congruential Generator iteration 1E4
% rslt =  5457720079191;	% Linear Congruential Generator iteration 5E5
% rslt = 35523939983831;	% Linear Congruential Generator iteration 1E6

%%%%%%%%%%%%%%%
% Load in-mem %
%%%%%%%%%%%%%%%
 tic,x =x0; [T0,T1,T2,T3,T4,T5,T6,T7,T8]=deal(0);
	routemp(x,a);
  [r,x]=routasg(x,a); 
  [r,x]=randbit(x,a);	if all(SOFT=='Octave')
  [r,x]=randoct(x,a); 	end
  [r,x]=randmod(x,a); 
  [r,x]=randari(x,a); 
  [r,x]=randie3(x,a);
  for i=1:m, end	% loop a bit
 T=toc;

 fprintf('================================================================\n');
 fprintf('First measurements comparing Octave/MATLAB, not for rand purposes\n');
 fprintf('Timing for assignment-routine-call loop is lower limit for rand\n');
 fprintf('number of iterations = %d\n',m);
 fprintf('================================================================\n');
 fprintf('timing an empty-------------------loop\n');
 tic, x=x0; for i=1:m,                     end, T0=toc;
 fprintf('timing an assignment--------------loop\n');
 tic, x=x0; for i=1:m,  r=x;               end, T1=toc;
 fprintf('timing an empty------routine-call loop\n');
 tic, x=x0; for i=1:m,       routemp(x,a); end, T2=toc;
 fprintf('timing an assignment-routine-call loop\n');
 tic, x=x0; for i=1:m, [r,x]=routasg(x,a); end, T3=toc;


 if  all(SOFT=='Octave'), nv=5; else nv=4; end
 fprintf('================================================================\n');
 fprintf('%d versions of LCG code are timed below, the ones that work\n',nv);
 fprintf('handle 46bit quantities as two 23bit halves. 52bit won''t work\n');

 fprintf('======================= 3rd (ML) / 2nd (Oct) ===================\n');
 fprintf('23bit bitshift/bitand:\t');
 tic, x=x0; for i=1:m, [r,x]=randbit(x,a); end, T4=toc; tprint(T4); chk(x,rslt)

 if all(SOFT=='Octave')
 fprintf('======================= not working (saturation, 32<46 ) =======\n');
 fprintf('uint32 bitshift Octave:\t');
 tic, x=x0; for i=1:m, [r,x]=randoct(x,a); end, T8=toc; tprint(T8); chk(x,rslt)
 end

 fprintf('======================= 1st (ML) / 3rd (Oct) ===================\n');
 fprintf('23bit floor/mod:\t');
 tic, x=x0; for i=1:m, [r,x]=randmod(x,a); end, T5=toc; tprint(T5); chk(x,rslt)

 fprintf('======================= 2nd (ML) / 1st (Oct) ===================\n');
 fprintf('23bit floor/arith:\t');
 tic, x=x0; for i=1:m, [r,x]=randari(x,a); end, T6=toc; tprint(T6); chk(x,rslt)

 fprintf('======================= not working (roundoff, 52<46x2) ========\n');
 fprintf('52bit IEEE-754dbl:\t');
 tic, x=x0; for i=1:m, [r,x]=randie3(x,a); end, T7=toc; tprint(T7); chk(x,rslt)

 fprintf('================================================================\n');
	label1={'Empty loop','w/Assign','w/Call-empty','w/Call-assign'};
	watch1={	  T0,	     T1,	    T2,		   T3 };
 fprintf('seconds: ');	fprintf(     '%s\t',label1{:}); fprintf('\n');
			fprintf('\t%7.3f\t',watch1{:}); fprintf('\n');
	label2={'bitshift/and','floor/mod','floor/arith','IEEE-754'};
	watch2={	    T4,		T5,	      T6,	T7 };
			fprintf('\t%s'     ,label2{:}); fprintf('\n');
			fprintf('\t%7.3f\t',watch2{:}); fprintf('\n');
 if all(SOFT=='Octave')
			fprintf('\t%s'  ,'uint32/and'); fprintf('\n');
			fprintf('\t%7.3f',         T8); fprintf('\n');
 end
% Measurements in a PentiumIII 1GHz w/128MB PC133
% ================================================================
% MATLAB:  Empty loop     w/Assign        w/Call-empty    w/Call-assign
% 5E5 iter 0.011           0.014          11.459          16.291
%         bitshift/and    floor/mod       floor/arith     IEEE-754
%         50.544          20.614          20.941          17.264
% ================================================================
% Octave:  Empty loop     w/Assign        w/Call-empty    w/Call-assign
% 5E3 iter 0.004           0.043          21.146          21.550
%         bitshift/and    floor/mod       floor/arith     IEEE-754
%         23.427          30.933          22.787          24.691
%         uint32/and
%         24.081
% ================================================================
% 	MATLAB's slowest is 52s/5E5 iters
% while Octave's fastest is 23s/5E3 iters => around 10^2/2 = 50x faster
% 			      floor/arith => around 10^2   =100x faster
% ================================================================
 fprintf('================================================================\n');
 fprintf('Relative speed order (in parentheses the ones not working)\n');
 fprintf('MATLAB: (IEEE-754)  floor/mod floor/arith           ... bitshift\n');
 fprintf('Octave: floor/arith bitshift (uint32/and)(IEEE-754) ... floor/mod\n');
 fprintf('So floor/arith correct & fast as IEEE-754 for both MATLAB/Octave\n');
 fprintf('For comparing MATLAB/Octave, better use #randoms/time\n');
 fprintf('================================================================\n');
	watch1={	m/T0,	   m/T1,	  m/T2,	    m/T3 };
 fprintf('iter/sec:');	fprintf(  '%s\t',label1{:}); fprintf('\n');
			fprintf('\t%.3e',watch1{:}); fprintf('\n');
	watch2={	m/T4,	   m/T5,	  m/T6,     m/T7 };
			fprintf('\t%s'  ,label2{:}); fprintf('\n');
			fprintf('\t%.3e',watch2{:}); fprintf('\n');
 if all(SOFT=='Octave')
			fprintf('\t%s','uint32/and'); fprintf('\n');
			fprintf('\t%.3e',     T8/m ); fprintf('\n');
 end
 fprintf('================================================================\n');
%================================================================
% MATLAB: Empty loop     w/Assign        w/Call-empty    w/Call-assign
%        4.493e+07       3.535e+07       4.363e+04       3.069e+04
%        bitshift/and    floor/mod       floor/arith     IEEE-754
%        9.892e+03       2.426e+04       2.388e+04       2.896e+04
%================================================================
% Octave: Empty loop     w/Assign        w/Call-empty    w/Call-assign
%        1.295e+06       1.168e+05       2.364e+02       2.320e+02
%        bitshift/and    floor/mod       floor/arith     IEEE-754
%        2.134e+02       1.616e+02       2.194e+02       2.025e+02
%        uint32/and
%        4.816e-03
%================================================================

 detect(0);
%end




function tprint(T)
  fprintf('Elapsed time is %f seconds.',T);
%end

function chk(x,rslt)
  if x==rslt,	fprintf('\t=== PASS ===\n');
  else,		fprintf('\t=== %cFAIL ===\n',7);
		fprintf('Expected %20.3f\n',rslt);
		fprintf('Obtained %20.3f\n',x);
  end
%end


function       routemp(x,a)	% Empty      routine
%end


function [r,x]=routasg(x,a)	% Assignment routine
r=a;
%end


function [r,x]=randie3(x,a)	% THE WRONG VERSION: IEEE-754 arith (52 bits)
% t46=        2^46;		% each shift/and => conversion dbl->int->dbl ??
% x = mod(x*a, t46);
% r =     x  / t46 ;		%		MATLAB	Octave
  x = mod(x*a,2^46);		% timing:	17.359 	24.687
  r =     x* 2^-46 ;		% with division:17.312	24.741
%end				% div & t46:	18.547	24.830


function [r,x]=randbit(x,a)	% BITSHIFT/BITAND VERSION: (2*23 bits)
% t23m1=         2^23-1;
  a1 = bitshift(a, -23);	% in fact a=5^13 always, should not recompute
  a2 = bitand  (a,2^23-1);	% vran does exactly that, factor a out-loop
				%		MATLAB	Octave
  x1 = bitshift(x, -23);	% timing:	50.651	23.873
  x2 = bitand  (x,2^23-1);	% w/t23m1:	51.559	23.874

  z  = bitand  ( a1*x2 + a2*x1, 2^23-1);	% in fact, 52bits > 46bits
  x  = bitand  (z*2^23 + a2*x2, 2^46-1);

  r  = x * 2^-46;
%end


function [r,x]=randoct(x,a)	% BITSHIFT/BITAND Octave VERSION:
  au = uint32  (a );
  a1 = bitshift(au, -23);	% Octave has uint32 product
  a2 = bitand  (au,2^23-1);	% but unfortunately saturates on max/min

  xu = uint32  (x );		% and there is neither uint64 product nor sum
  x1 = bitshift(xu, -23);	% so there is no way z&x can be computed right
  x2 = bitand  (xu,2^23-1);	% z can/will saturate and z*2^23 is 46bit wide

  z  = bitand  ( a1*x2 + a2*x1, 2^23-1);	% this would require roll-over
  x  = bitand  (z*2^23 + a2*x2, 2^46-1);	% for "mod" operator for free

  r  = x * 2^-46;		% no way, no way, really
%end


function [r,x]=randmod(x,a)	% FLOOR/MOD VERSION:
% t23=         2^ 23;		%		MATLAB	Octave
  a1 = floor(a/2^ 23);		% timing:	20.635	31.437
% a1 = floor(a*2^-23);		% w/*:		20.744	31.434
  a2 = mod  (a,2^ 23);

  x1 = floor(x/2^ 23);		% w/t23:	21.216	31.386
% x1 = floor(x*2^-23);		% w/8388608:	20.732	31.526
  x2 = mod  (x,2^ 23);

  z  = mod  (  a1*x2 + a2*x1, 2^23);
  x  = mod  (z*2^ 23 + a2*x2, 2^46);
                                                                                
  r  = x * 2^-46;
%end


function [r,x]=randari(x,a)	% FIX/ARITH VERSION:
% a1 = fix  (a *2^-23);		%		MATLAB	Octave
  a1 = floor(a *2^-23);		% w/fix:	21.291	23.001
  a2 = a  -  a1*2^ 23 ;		% timing/floor:	21.091	23.032
% a2 = mod  (a, 2^ 23);

% x1 = fix  (x *2^-23);
  x1 = floor(x *2^-23);
  x2 = x  -  x1*2^ 23 ;
% x2 = mod  (x, 2^ 23);

  t1 = a1*x2 + a2*x1;		% w/mod for z:	21.384	25.881
% t2 = fix  (t1*2^-23);		% w/mod for z/x:20.544	27.550 (randmod)
  t2 = floor(t1*2^-23);		% for z/x/a2/x2:20.706	31.349
  z  = t1 -  t2*2^ 23 ;	
% z  = mod  (  a1*x2 + a2*x1, 2^23);

% t3 = z*2^23 + a2*x2;		% w/t3-t4:	22.131	23.056
  t1 = z*2^23 + a2*x2;		% reusing t1/t2:21.059	23.046 (randari)
% t2 = fix  (t1*2^-46);
% t4 = floor(t3*2^-46);
  t2 = floor(t1*2^-46);
% x  = t3 -  t4*2^ 46 ;
  x  = t1 -  t2*2^ 46 ;
% x  = mod  (z*2^ 23 + a2*x2, 2^46);

  r  = x * 2^-46;
%end

