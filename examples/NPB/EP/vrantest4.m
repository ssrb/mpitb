function vrantest4
% Auxililary function to test different implementations of the batch random LCG
% SEE ALSO: vrantest3

%%%%%%%%%%%%%
% Constants %
%%%%%%%%%%%%%
    x0 = 271828183;		% prime number, random seed
     a = 5^13;			% LCG factor x_n+1 = a * x_n (mod 2^46)
  SOFT = detect;
  if all(SOFT=='MATLAB')	% number of pairs of random numbers to generate
	m=22; range=10;		% chosen so that no swapping in my 128MB (MLAB)
				% MATLAB leaves around 40MB free > 32MB = 4Mdbl
  else,	m=16; range= 5;		% or we don't die waiting in my 1GHz (Octave)
  end				% runtime ~ 5s(ML)/22s(Octave) for each routine
%   mm =  6;	nn = 2^mm;	% number of batches 2^6=64
%   mk = m-mm;	nk = 2^mk;	
%   mk = 16;	nk = 2^mk;	% batch size 2^16 = 64Kpairs
%   mm = m-mk;	nn = 2^mm;

  rslt = a; for i=1:m+1, [r,rslt]=randlc(rslt,rslt); end	% binary exp
			 [r,rslt]=randlc(  x0,rslt);

% fprintf('m = %d, rslt = %.0f\n',m,rslt); return
% rslt = 28520395293911;        % Linear Congruential Generator iteration 2^15
% rslt = 61967321777367;        % Linear Congruential Generator iteration 2^23

%%%%%%%%%%%%%%%
% Load in-mem %
%%%%%%%%%%%%%%%
 tic,t1=x0; global X, 	X=-1.D99*ones(2*1, 1);
  t1=gvarari1(2*1,t1,a);  t1=gvarari2(2*1,t1,a);
  t1=gvarari3(2*1,t1,a);  t1=gvarari4(2*1,t1,a);
  for i=1:m, end		% loop a bit
 T(range,4)=toc;

 fprintf('================================================================\n');
 fprintf('Same versions(vrantest3) are timed, along a 2^%d size sweep\n',range);
 fprintf('Total # of random numbers always the same, but computed in\n');
 fprintf('(2^%d...2) batches of size (2^%d...2^%d)\n', range, m-range,m-1);

%	m=23; range=10;
% else,	m=15; range=10;
	
for ir=1:range			% size sweep
%   mm =  6;	nn = 2^mm;	% number of batches
%   mk = m-mm;	nk = 2^mk;	% batch size

    mm = ir;	nn = 2^mm;	% batches: from 2^1 (2) to 2^10 (1024)
    mk = m-mm;	nk = 2^mk;	% sizes:   from 2^22 to 2^13 (MATLAB)
				%	   from 2^14 to 2^ 5 (Octave)
    clear global X, global X
    X=-1.D99*ones(2*nk,1);	% destroy old global / define new one
%   X=       ones(2*nk,1);	% different initializations
%   X=      zeros(2*nk,1);	% very little impact, really


 fprintf('=== 2^%d batches x size 2^%d == 2^%d total size ==============\n',...
		mm,		mk,	m);
 			fprintf('normal (global):\t');
 tic, t1=x0; for i=1:nn, t1=gvarari1(2*nk,t1,a); end
 T(ir,1)=toc; tprint(T(ir,1)); chk(t1,rslt)


 			fprintf('factoring 2^-46:\t');
 tic, t1=x0; for i=1:nn, t1=gvarari2(2*nk,t1,a); end
 T(ir,2)=toc; tprint(T(ir,2)); chk(t1,rslt)


 			fprintf('while loop,not for:\t');
 tic, t1=x0; for i=1:nn, t1=gvarari3(2*nk,t1,a); end
 T(ir,3)=toc; tprint(T(ir,3)); chk(t1,rslt)


 			fprintf('both variants:  \t');
 tic, t1=x0; for i=1:nn, t1=gvarari4(2*nk,t1,a); end
 T(ir,4)=toc; tprint(T(ir,4)); chk(t1,rslt)

end % for ir=1:range

 fprintf('================================================================\n');
 			h={'floor/arith','factoring','while loop','both'};
 fprintf('seconds: ');	fprintf('\t%s'       ,h{:}  ); fprintf('\n');
 for i=1:range,		fprintf('2^(%2d+%2d)',i, m-i);
			fprintf('\t%7.3f\t'  ,T(i,:)); fprintf('\n');
 end

% Measurements in a PentiumIII 1GHz w/128MB PC133
% ================================================================
% MATLAB:         floor/arith     factoring       while loop      both
% 2^( 1+21)         2.832           3.514           2.744           3.418
% 2^( 2+20)         2.842           3.496           2.728           3.409
% 2^( 3+19)         2.845           3.495           2.735           3.399
% 2^( 4+18)         2.840           3.508           2.733           3.409
% 2^( 5+17)         2.853           3.511           2.753           3.412
% 2^( 6+16)         2.881           3.544           2.772           3.443
% 2^( 7+15)         2.933           3.540           2.831           3.442
% 2^( 8+14)         2.958           3.260           2.850           3.149
% 2^( 9+13)         2.856           2.892           2.752           2.790
% 2^(10+12)         2.827           2.870           2.720           2.765
% ================================================================
% Octave: m=15    floor/arith     factoring       while loop      both
% 2^( 1+15)        22.410          22.231          24.407          24.167
% 2^( 2+14)        22.440          22.239          24.436          24.154
% 2^( 3+13)        22.411          22.231          24.430          24.251
% 2^( 4+12)        22.468          22.319          24.477          24.224
% 2^( 5+11)        22.534          22.327          24.583          24.333
% ================================================================
% Runtime seems not affected by array size, at least for arrays >= 1Kdouble
% MATLAB penalti for factoring seems to grove with array size (2.87->3.51)
%	and while loop is consistently better for all sizes
% Octave factoring is also consistently better for all sizes
% Well, not so much difference, perhaps it's not worth the effort
% MATLAB is around 2^6 * 22.5/3  = 512x faster
% ================================================================
 fprintf('================================================================\n');
 fprintf('Perhaps the normal version is good enough for both MATLAB/Octave\n');
 fprintf('For comparing MATLAB/Octave, better use #randoms/time\n');
 fprintf('================================================================\n');
				n=2^(m+1);
 fprintf('rand/sec:');	fprintf('\t%s'       ,    h{:}); fprintf('\n');
 for i=1:range,		fprintf('2^(%2d+%2d)',  i, m-i);
			fprintf('\t%.3e'     ,n./T(i,:)); fprintf('\n');
 end
 fprintf('================================================================\n');
% ================================================================
% MATLAB:         floor/arith     factoring       while loop      both
% 2^( 1+21)       2.963e+06       2.387e+06       3.057e+06       2.454e+06
% 2^( 2+20)       2.952e+06       2.399e+06       3.075e+06       2.461e+06
% 2^( 3+19)       2.949e+06       2.400e+06       3.067e+06       2.468e+06
% 2^( 4+18)       2.954e+06       2.391e+06       3.070e+06       2.461e+06
% 2^( 5+17)       2.940e+06       2.389e+06       3.047e+06       2.459e+06
% 2^( 6+16)       2.912e+06       2.367e+06       3.026e+06       2.437e+06
% 2^( 7+15)       2.860e+06       2.370e+06       2.963e+06       2.437e+06
% 2^( 8+14)       2.836e+06       2.573e+06       2.943e+06       2.664e+06
% 2^( 9+13)       2.937e+06       2.901e+06       3.048e+06       3.007e+06
% 2^(10+12)       2.968e+06       2.923e+06       3.084e+06       3.034e+06
% ================================================================
% Octave:         floor/arith     factoring       while loop      both
% 2^( 1+15)       5.849e+03       5.896e+03       5.370e+03       5.424e+03
% 2^( 2+14)       5.841e+03       5.894e+03       5.364e+03       5.427e+03
% 2^( 3+13)       5.849e+03       5.896e+03       5.365e+03       5.405e+03
% 2^( 4+12)       5.834e+03       5.873e+03       5.355e+03       5.411e+03
% 2^( 5+11)       5.817e+03       5.871e+03       5.332e+03       5.387e+03
% ================================================================

 clear global X
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


function [x,varargout]=gvarari1(n,x,a)
if nargout<2, global X
else
    X=-1.D99*ones(n,1);
end
    a1 = floor(a *2^-23);
    a2 = a  -  a1*2^ 23 ;
  for i=1:n
    x1 = floor(x *2^-23);
    x2 = x  -  x1*2^ 23 ;

    t1 = a1*x2 + a2*x1;
    t2 = floor(t1*2^-23);
    z  = t1 -  t2*2^ 23 ;

    t1 = z*2^23 + a2*x2;
    t2 = floor(t1*2^-46);
    x  = t1 -  t2*2^ 46 ;

    X(i)=x * 2^-46;
  end
if nargout>1, varargout={X}; end
%end


function [x,varargout]=gvarari2(n,x,a)
if nargout<2, global X
else
    X=-1.D99*ones(n,1);
end
    a1 = floor(a *2^-23);
    a2 = a  -  a1*2^ 23 ;
  for i=1:n
    x1 = floor(x *2^-23);
    x2 = x  -  x1*2^ 23 ;

    t1 = a1*x2 + a2*x1;
    t2 = floor(t1*2^-23);
    z  = t1 -  t2*2^ 23 ;

    t1 = z*2^23 + a2*x2;
    t2 = floor(t1*2^-46);
    x  = t1 -  t2*2^ 46 ;

    X(i)=x;
  end
    X   =X * 2^-46;
if nargout>1, varargout={X}; end
%end


function [x,varargout]=gvarari3(n,x,a)
if nargout<2, global X		%		MATLAB	Octave
else				% timing:	0.899	6.272
    X=-1.D99*ones(n,1);
end
    a1 = floor(a *2^-23);
    a2 = a  -  a1*2^ 23 ;
  i=0; while i<n, i=i+1;
    x1 = floor(x *2^-23);
    x2 = x  -  x1*2^ 23 ;

    t1 = a1*x2 + a2*x1;
    t2 = floor(t1*2^-23);
    z  = t1 -  t2*2^ 23 ;

    t1 = z*2^23 + a2*x2;
    t2 = floor(t1*2^-46);
    x  = t1 -  t2*2^ 46 ;

    X(i)=x * 2^-46;
  end
if nargout>1, varargout={X}; end
%end


function [x,varargout]=gvarari4(n,x,a)
if nargout<2, global X
else
    X=-1.D99*ones(n,1);
end
    a1 = floor(a *2^-23);
    a2 = a  -  a1*2^ 23 ;
  i=0; while i<n, i=i+1;
    x1 = floor(x *2^-23);
    x2 = x  -  x1*2^ 23 ;

    t1 = a1*x2 + a2*x1;
    t2 = floor(t1*2^-23);
    z  = t1 -  t2*2^ 23 ;

    t1 = z*2^23 + a2*x2;
    t2 = floor(t1*2^-46);
    x  = t1 -  t2*2^ 46 ;

    X(i)=x;
  end
    X   =X * 2^-46;
if nargout>1, varargout={X}; end
%end


