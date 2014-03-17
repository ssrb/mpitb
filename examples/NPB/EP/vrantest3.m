function vrantest3
% Auxililary function to test different implementations of the batch random LCG
% SEE ALSO: vrantest2

%%%%%%%%%%%%%
% Constants %
%%%%%%%%%%%%%
    x0 = 271828183;		% prime number, random seed
     a = 5^13;			% LCG factor x_n+1 = a * x_n (mod 2^46)
  SOFT = detect;
  if all(SOFT=='MATLAB')	% number of pairs of random numbers to generate
	m=23;			% chosen so that runtime ~ 15s
  else,	m=15;
  end
    mm =  6;	nn = 2^mm;	% number of batches 2^6=64
    mk = m-mm;	nk = 2^mk;	
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
 tic,t1=x0; global X,	X=-1.D99*ones(2*nk,1);
%			X=       ones(2*nk,1);	% different initializations
%			X=      zeros(2*nk,1);	% very little impact, really
  t1=gvarari1(2*1,t1,a);  t1=gvarari2(2*1,t1,a);
  t1=gvarari3(2*1,t1,a);  t1=gvarari4(2*1,t1,a);
  for i=1:m, end		% loop a bit
 T(4)=toc;

 fprintf('================================================================\n');
 fprintf('floor/arith version from vrantest2 is timed, with some variants:\n');
 fprintf('1.-normal version (global)\n');
 fprintf('2.-factoring 2^-46 out of the loop\n');
 fprintf('3.-using while instead of for loop\n');
 fprintf('4.-both (factor, while) at once\n');


 fprintf('================================================================\n');
 			fprintf('normal (global):\t');
 tic, t1=x0; for i=1:nn, t1=gvarari1(2*nk,t1,a); end
 T(1)=toc; tprint(T(1)); chk(t1,rslt)


 			fprintf('factoring 2^-46:\t');
 tic, t1=x0; for i=1:nn, t1=gvarari2(2*nk,t1,a); end
 T(2)=toc; tprint(T(2)); chk(t1,rslt)


 			fprintf('while loop,not for:\t');
 tic, t1=x0; for i=1:nn, t1=gvarari3(2*nk,t1,a); end
 T(3)=toc; tprint(T(3)); chk(t1,rslt)


 			fprintf('both variants:  \t');
 tic, t1=x0; for i=1:nn, t1=gvarari4(2*nk,t1,a); end
 T(4)=toc; tprint(T(4)); chk(t1,rslt)


 fprintf('================================================================\n');
 			h={'floor/arith','factoring','while loop','both'};
 fprintf('seconds: ');	fprintf('\t%s'     ,h{:}); fprintf('\n\t');
			fprintf('\t%7.3f\t',T(:)); fprintf('\n');

% Measurements in a PentiumIII 1GHz w/128MB PC133
% ================================================================
% MATLAB: m=23    floor/arith     factoring       while loop      both
%                  5.699           7.038           5.500           6.823
% ================================================================
% Octave: m=15    floor/arith     factoring       while loop      both
%                 12.073          11.943          12.967          12.859
% ================================================================
% So we should use while under MATLAB ( 5.85-> 5.47) < 0.4s
%	 and factor 2^46 under Octave (11.97->11.89) < 0.1s
% Well, not so much difference, perhaps it's not worth the effort
% MATLAB is around 2^8 * 12/6  = 512x faster
% ================================================================
 fprintf('================================================================\n');
 fprintf('Perhaps the normal version is good enough for both MATLAB/Octave\n');
 fprintf('For comparing MATLAB/Octave, better use #randoms/time\n');
 fprintf('================================================================\n');
				n=2^(m+1);
 fprintf('rand/sec:');	fprintf('\t%s'  ,   h{:}); fprintf('\n\t');
			fprintf('\t%.3e',n./T(:)); fprintf('\n');
 fprintf('================================================================\n');
% ================================================================
% MATLAB:         floor/arith     factoring       while loop      both
%                 2.944e+06       2.384e+06       3.050e+06       2.459e+06
% ================================================================
% Octave:         floor/arith     factoring       while loop      both
%                 5.428e+03       5.487e+03       5.054e+03       5.096e+03
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


