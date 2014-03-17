function vrantest1
% Auxililary function to test different implementations of the batch random LCG
% SEE ALSO: vranlc, randlc, randtest

%%%%%%%%%%%%%
% Constants %
%%%%%%%%%%%%%
    x0 = 271828183;		% prime number, random seed
     a = 5^13;			% LCG factor x_n+1 = a * x_n (mod 2^46)
  SOFT = detect;
  if all(SOFT=='MATLAB')	% number of pairs of random numbers to generate
	m=20;			% chosen so that we don't die waiting
  else,	m=14;			% worst case ~ 40s/75s
  end
    mm =  6;	nn = 2^mm;	% number of batches 2^6=64
    mk = m-mm;	nk = 2^mk;	
%   mk = 16;	nk = 2^mk;	% batch size 2^16 = 64Kpairs
%   mm = m-mk;	nn = 2^mm;

  rslt = a; for i=1:m+1, [r,rslt]=randlc(rslt,rslt); end	% binary exp
			 [r,rslt]=randlc(  x0,rslt);

% fprintf('m = %d, rslt = %.0f\n',m,rslt); return
% rslt = 38045862446295;        % Linear Congruential Generator iteration 2^14
% rslt = 51726618182871;        % Linear Congruential Generator iteration 2^20

%%%%%%%%%%%%%%%
% Load in-mem %
%%%%%%%%%%%%%%%
 tic,t1=x0; [T0,T1,T2,T3,T4,T5,T6,T7,T8]=deal(0);
  [t1,x]=vranbit(2*1,t1,a);	if all(SOFT=='Octave')
  [t1,x]=vranoct(2*1,t1,a);	end
  [t1,x]=vranmod(2*1,t1,a);
  [t1,x]=vranari(2*1,t1,a);
  [t1,x]=vranie3(2*1,t1,a);
  for i=1:m, end		% loop a bit
 T=toc;

 if  all(SOFT=='Octave'), nv=5; else nv=4; end
 fprintf('================================================================\n');
 fprintf('Same (%d) versions from randtest are timed, but this time:\n',nv);
 fprintf(' big loop (%d) _inside_ the routine, not _calling_ it\n',2*nk);
 fprintf(' small loop (%d) _outside_ the routine, calling it\n',nn);

 fprintf('======================= 3rd (ML) / 2nd (Oct) ===================\n');
 fprintf('23bit bitshift/bitand:\t');
 tic, t1=x0; for i=1:nn, [t1,x]=vranbit(2*nk,t1,a); end
 T4=toc; tprint(T4); chk(t1,rslt)

 if all(SOFT=='Octave')
 fprintf('======================= not working (saturation, 32<46 ) =======\n');
 fprintf('uint32 bitshift Octave:\t');
 tic, t1=x0; for i=1:nn, [t1,x]=vranoct(2*nk,t1,a); end
 T8=toc; tprint(T8); chk(t1,rslt)
 end

 fprintf('======================= 2ND!(ML) / 3rd (Oct) ===================\n');
 fprintf('23bit floor/mod:\t');
 tic, t1=x0; for i=1:nn, [t1,x]=vranmod(2*nk,t1,a); end
 T5=toc; tprint(T5); chk(t1,rslt)

 fprintf('======================= 1ST!(ML) / 1st (Oct) ===================\n');
 fprintf('23bit floor/arith:\t');
 tic, t1=x0; for i=1:nn, [t1,x]=vranari(2*nk,t1,a); end
 T6=toc; tprint(T6); chk(t1,rslt)

 fprintf('======================= not working (roundoff, 52<46x2) ========\n');
 fprintf('52bit IEEE-754dbl:\t');
 tic, t1=x0; for i=1:nn, [t1,x]=vranie3(2*nk,t1,a); end
 T7=toc; tprint(T7); chk(t1,rslt)

 fprintf('================================================================\n');
	labels={'bitshift/and','floor/mod','floor/arith','IEEE-754'};
	watchs={            T4,         T5,           T6,       T7 };
 fprintf('seconds: ');	fprintf(     '%s\t',labels{:}); fprintf('\n');
			fprintf('\t%7.3f\t',watchs{:}); fprintf('\n');
 if all(SOFT=='Octave')
			fprintf('\t%s'  ,'uint32/and'); fprintf('\n');
			fprintf('\t%7.3f',         T8); fprintf('\n');
 end
% Measurements in a PentiumIII 1GHz w/128MB PC133
% ================================================================
% MATLAB:  bitshift/and   floor/mod       floor/arith     IEEE-754
% 2^(6+14) 73.499           1.859           0.898           0.717
% ================================================================
% Octave:  bitshift/and   floor/mod       floor/arith     IEEE-754
% 2^(6+ 8)  7.401          40.018           6.166          14.064
%         uint32/and
%           8.370
%================================================================
%	like randtest but MATLAB's floor/arith now faster than mod
% again MATLAB's slowest is 74s/2^20 iters (2^20 = 1M ~ 1E6)
% while Octave's fastest is  6s/2^14 iters => around 2^6/12 = 5x faster
% 			       floor/arith => around 2^6/ 9 = 7x faster
% ================================================================
 fprintf('================================================================\n');
 fprintf('Relative speed order (in parentheses the ones not working)\n');
 fprintf('    like randtest but MATLAB''s floor/arith now faster than mod\n');
 fprintf('MATLAB: (IEEE-754)  floor/ARITH! floor/MOD!         ... bitshift\n');
 fprintf('Octave: floor/arith bitshift (uint32/and)(IEEE-754) ... floor/mod\n');
 fprintf('So floor/arith correct & fastest for both MATLAB/Octave\n');
 fprintf('For comparing MATLAB/Octave, better use #randoms/time\n');
 fprintf('================================================================\n');
			n=2^(m+1);
	watchs={         n/T4,       n/T5,         n/T6,     n/T7 };
 fprintf('rand/sec:');	fprintf(  '%s\t',labels{:});  fprintf('\n');
			fprintf('\t%.3e',watchs{:});  fprintf('\n');
 if all(SOFT=='Octave')
			fprintf('\t%s','uint32/and'); fprintf('\n');
			fprintf('\t%.3e',     n/T8 ); fprintf('\n');
 end
 fprintf('================================================================\n');
% ================================================================
% MATLAB:  bitshift/and   floor/mod       floor/arith     IEEE-754
%         2.853e+04       1.128e+06       2.334e+06       2.925e+06
% ================================================================
% Octave:  bitshift/and   floor/mod       floor/arith     IEEE-754
%         4.427e+03       8.188e+02       5.315e+03       2.330e+03
%         uint32/and
%         3.915e+03
% ================================================================

%fprintf('================================================================\n');
%fprintf('Consider MATLAB''s floor/mod (2x slower) & Octave''s bitshift\n');
%fprintf('                   for a more thorough study (vrantest2)\n');
%fprintf('Also try global,separate,factoring 2^-46, while-not for, both:\n');
%fprintf('================================================================\n');


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


function [x,y]=vranie3(n,x,a)
% t46=        2^46;
   y   = zeros(n,1);		%		MATLAB	Octave
  for i=1:n			% timing:	0.722	14.634
%  x   = mod(x*a, t46);		% w/div:	0.734	14.872
%  y(i)=     x/   t46 ;		% w/div & t46:	0.727	15.006

   x   = mod(x*a,2^46);
   y(i)=     x* 2^-46;
  end
%end


function [x,y]=vranbit(n,x,a)
% t23m1=         2^23-1;
  y=     zeros(n,1);		%		MATLAB	Octave
    a1 = bitshift(a, -23  );	% timing:	73.380	7.353
    a2 = bitand  (a,2^23-1);	% w/t23m1:	73.420	7.459
  for i=1:n
    x1 = bitshift(x, -23  );
    x2 = bitand  (x,2^23-1);

    z  = bitand  ( a1*x2 + a2*x1, 2^23-1);
    x  = bitand  (z*2^23 + a2*x2, 2^46-1);

    y(i)=x * 2^-46;
  end
%end


function [x,y]=vranoct(n,x,a)
  t23m1=         2^23-1;
  y=     zeros(n,1);		%			Octave
    au = uint32  (a );		% timing:		8.352
    a1 = bitshift(au, -23  );	% w/t23m1:		8.431
    a2 = bitand  (au, t23m1);
  for i=1:n
    xu = uint32  (x );
    x1 = bitshift(xu, -23  );
    x2 = bitand  (xu, t23m1);

    z  = bitand  ( a1*x2 + a2*x1,  t23m1);
    x  = bitand  (z*2^23 + a2*x2, 2^46-1);

    y(i)=x * 2^-46;
  end
%end


function [x,y]=vranmod(n,x,a)
% t23=         2^ 23;
  y=     zeros(n,1);		%		MATLAB	Octave
%   a1 = floor(a*2^-23);	% timing:	1.848	41.250
%   a1 = fix  (a*2^-23);	% w/fix:	1.862	41.498
    a1 = floor(a/2^ 23);	% w/div:	1.858	41.954
    a2 = mod  (a,2^ 23);
  for i=1:n
%   x1 = floor(x*2^-23);	% w/div & t23:	1.835	41.774
%   x1 = fix  (x*2^-23);	% ...& 8388608:	1.857	41.004
    x1 = floor(x/2^ 23);
    x2 = mod  (x,2^ 23);

    z  = mod  (  a1*x2 + a2*x1, 2^23);
    x  = mod  (z*2^ 23 + a2*x2, 2^46);

    y(i)=x * 2^-46;
  end
%end


function [x,y]=vranari(n,x,a)
  y=     zeros(n,1);		%		MATLAB	Octave
%   a1 = fix  (a *2^-23);	% timing:	0.899	6.272
    a1 = floor(a *2^-23);	% w/fix:	0.926	6.276
    a2 = a  -  a1*2^ 23 ;
%   a2 = mod  (a ,2^ 23);
  for i=1:n
%   x1 = fix  (x *2^-23);
    x1 = floor(x *2^-23);
    x2 = x  -  x1*2^ 23 ;
%   x2 = mod  (x ,2^ 23);

    t1 = a1*x2 + a2*x1;		% w/mod for z:	1.147	18.436
%   t2 = fix  (t1*2^-23);	% w/mod for z/x:1.445	29.372
    t2 = floor(t1*2^-23);	% for z/x/a2/x2:1.851	41.341
    z  = t1 -  t2*2^ 23 ;
%   z  = mod  (  a1*x2 + a2*x1, 2^23);

%   t3 = z*2^23 + a2*x2;	% w/t3-t4:	5.232	6.318
    t1 = z*2^23 + a2*x2;	% reusing t1/2:	0.873	6.260
%   t2 = fix  (t1*2^-46);
%   t4 = floor(t3*2^-46);
    t2 = floor(t1*2^-46);
%   x  = t3 -  t4*2^ 46 ;
    x  = t1 -  t2*2^ 46 ;
%   x  = mod  (z*2^ 23 + a2*x2, 2^46);

    y(i)=x * 2^-46;
  end
%end


