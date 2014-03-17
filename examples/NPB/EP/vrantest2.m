function vrantest2
% Auxililary function to test different implementations of the batch random LCG
% SEE ALSO: vrantest1

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
 tic,t1=x0; T(3,4)=0; global X, X=-1.D99*ones(2*nk,1);
  [t1,x]=vranari(2*1,t1,a); t1=glblari(2*1,t1,a); t1=gvarari(2*1,t1,a);
if all(SOFT=='MATLAB'),				  t1=gnarari(2*1,t1,a);
  [t1,x]=vranmod(2*1,t1,a); t1=glblmod(2*1,t1,a); t1=gvarmod(2*1,t1,a);
						  t1=gnarmod(2*1,t1,a);
end			%%% Octave can't do global X on function output arg.
if all(SOFT=='Octave')
  [t1,x]=vranbit(2*1,t1,a); t1=glblbit(2*1,t1,a); t1=gvarbit(2*1,t1,a);
end
  for i=1:m, end		% loop a bit
 T(3,4)=toc;

 fprintf('================================================================\n');
 fprintf('Two fastest versions from vrantest1 are timed, but this time with\n');
 fprintf('bigger loop (%d) _inside_ the routine, and several flavors:\n',2*nk);
 fprintf('1.-normal version (vrantest1, allocating and returning x)\n');
 fprintf('2.-using global access to batch storage X\n');
 fprintf('3/4.- both versions combined in a single function (w/ varargout)\n');
 if  all(SOFT=='MATLAB')	%%% Octave can't do global X on func.out.arg.
 fprintf('5/6.- both versions combined in a single function (w/ if nargout)\n');
 end


 fprintf('============= NORMAL (ALLOC/RETURN) VERSIONS ===================\n');
			fprintf('floor/arith alloc:\t');
 tic, t1=x0; for i=1:nn, [t1,x]=vranari(2*nk,t1,a); end
 T(1,1)=toc; tprint(T(1,1)); chk(t1,rslt)

 if all(SOFT=='MATLAB'),fprintf('floor/mod   alloc:\t');
 tic, t1=x0; for i=1:nn, [t1,x]=vranmod(2*nk,t1,a); end
 T(1,2)=toc; tprint(T(1,2)); chk(t1,rslt),	    end

 if all(SOFT=='Octave'),fprintf('bitshft/and alloc:\t');
 tic, t1=x0; for i=1:nn, [t1,x]=vranbit(2*nk,t1,a); end
 T(1,2)=toc; tprint(T(1,2)); chk(t1,rslt),	    end

 fprintf('============= GLOBAL ACCESS VERSIONS ===========================\n');
			fprintf('floor/arith glbal:\t');
 tic, t1=x0; for i=1:nn,  t1   =glblari(2*nk,t1,a); end
 T(1,3)=toc; tprint(T(1,3)); chk(t1,rslt)

 if all(SOFT=='MATLAB'),fprintf('floor/mod   glbal:\t');
 tic, t1=x0; for i=1:nn,  t1   =glblmod(2*nk,t1,a); end
 T(1,4)=toc; tprint(T(1,4)); chk(t1,rslt),	    end

 if all(SOFT=='Octave'),fprintf('bitshft/and glbal:\t');
 tic, t1=x0; for i=1:nn,  t1   =glblbit(2*nk,t1,a); end
 T(1,4)=toc; tprint(T(1,4)); chk(t1,rslt),	    end

 fprintf('============= ALLOC/GLOBAL (varargout) VERSIONS ================\n');
			fprintf('floor/arith alloc:\t');
 tic, t1=x0; for i=1:nn, [t1,x]=gvarari(2*nk,t1,a); end
 T(2,1)=toc; tprint(T(2,1)); chk(t1,rslt)

 if all(SOFT=='MATLAB'),fprintf('floor/mod   alloc:\t');
 tic, t1=x0; for i=1:nn, [t1,x]=gvarmod(2*nk,t1,a); end
 T(2,2)=toc; tprint(T(2,2)); chk(t1,rslt),	    end

 if all(SOFT=='Octave'),fprintf('bitshft/and alloc:\t');
 tic, t1=x0; for i=1:nn, [t1,x]=gvarbit(2*nk,t1,a); end
 T(2,2)=toc; tprint(T(2,2)); chk(t1,rslt),	    end

			fprintf('floor/arith glbal:\t');
 tic, t1=x0; for i=1:nn,  t1   =gvarari(2*nk,t1,a); end
 T(2,3)=toc; tprint(T(2,3)); chk(t1,rslt)

 if all(SOFT=='MATLAB'),fprintf('floor/mod   glbal:\t');
 tic, t1=x0; for i=1:nn,  t1   =gvarmod(2*nk,t1,a); end
 T(2,4)=toc; tprint(T(2,4)); chk(t1,rslt),	    end

 if all(SOFT=='Octave'),fprintf('bitshft/and glbal:\t');
 tic, t1=x0; for i=1:nn,  t1   =gvarbit(2*nk,t1,a); end
 T(2,4)=toc; tprint(T(2,4)); chk(t1,rslt),	    end


if all(SOFT=='MATLAB')
 fprintf('============= ALLOC/GLOBAL (if nargout) VERSIONS ===============\n');
			fprintf('floor/arith alloc:\t');
 tic, t1=x0; for i=1:nn, [t1,x]=gnarari(2*nk,t1,a); end
 T(3,1)=toc; tprint(T(3,1)); chk(t1,rslt)

			fprintf('floor/mod   alloc:\t');
 tic, t1=x0; for i=1:nn, [t1,x]=gnarmod(2*nk,t1,a); end
 T(3,2)=toc; tprint(T(3,2)); chk(t1,rslt)

			fprintf('floor/arith glbal:\t');
 tic, t1=x0; for i=1:nn,  t1   =gnarari(2*nk,t1,a); end
 T(3,3)=toc; tprint(T(3,3)); chk(t1,rslt)

			fprintf('floor/mod   glbal:\t');
 tic, t1=x0; for i=1:nn,  t1   =gnarmod(2*nk,t1,a); end
 T(3,4)=toc; tprint(T(3,4)); chk(t1,rslt)
end

 fprintf('================================================================\n');
 if all(SOFT=='MATLAB'),h={'floor/arith','floor/mod','glbal/arith','glbal/mod'};
			v={'separated'  ,'varargout','if nargout'};
 end
 if all(SOFT=='Octave'),h={'floor/arith','shift/and','glbal/arith','glbal/and'};
			v={'separated'  ,'varargout'};
 end

 fprintf('seconds: ');	fprintf('\t%s'     ,h{:}  ); fprintf('\n');
 for i=1:length(v)
 fprintf('%s',v{i});	fprintf('\t%7.3f\t',T(i,:)); fprintf('\n');
 end

% Measurements in a PentiumIII 1GHz w/128MB PC133
% ================================================================
% MATLAB: m=23    floor/arith     floor/mod       glbal/arith     glbal/mod
% separated         6.955          14.562           5.872          13.594
% varargout         6.813          14.849           5.706          13.746
% if nargout        6.830          14.521           5.719          13.407
% ================================================================
% Octave: m=15    floor/arith     shift/and       glbal/arith     glbal/and
% separated        12.100          14.690          12.045          14.592
% varargout        12.147          14.723          12.135          14.800
% ================================================================
% So global/varargout makes both versions available in one function
% and is almost as fast as global/separate, faster than normal/separate
% MATLAB is around 2^8 * 12/6  = 512x faster, improves a lot with global
% ================================================================
 fprintf('================================================================\n');
 fprintf('So global/varargout is good (almost best) for both MATLAB/Octave\n');
 fprintf('For comparing MATLAB/Octave, better use #randoms/time\n');
 fprintf('================================================================\n');
				n=2^(m+1);
 fprintf('rand/sec:');	fprintf('\t%s'  ,     h{:}); fprintf('\n');
 for i=1:length(v)
 fprintf('%s',v{i});	fprintf('\t%.3e',n./T(i,:)); fprintf('\n');
 end
 fprintf('================================================================\n');
% ================================================================
% MATLAB:         floor/arith     floor/mod       glbal/arith     glbal/mod
% separated       2.412e+06       1.152e+06       2.857e+06       1.234e+06
% varargout       2.463e+06       1.130e+06       2.940e+06       1.221e+06
% if nargout      2.456e+06       1.155e+06       2.934e+06       1.251e+06
% ================================================================
% Octave:         floor/arith     shift/and       glbal/arith     glbal/and
% separated       5.416e+03       4.461e+03       5.441e+03       4.491e+03
% varargout       5.395e+03       4.451e+03       5.400e+03       4.428e+03
% ================================================================

%fprintf('================================================================\n');
%fprintf('Also try global,separate,factoring 2^-46, while-not for, both:\n');
%fprintf('================================================================\n');

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


function [x,y]=vranbit(n,x,a)
  y=     zeros(n,1);		%		MATLAB	Octave
    a1 = bitshift(a, -23  );	% timing:	73.---	7.353
    a2 = bitand  (a,2^23-1);
  for i=1:n
    x1 = bitshift(x, -23  );
    x2 = bitand  (x,2^23-1);

    z  = bitand  ( a1*x2 + a2*x1, 2^23-1);
    x  = bitand  (z*2^23 + a2*x2, 2^46-1);

    y(i)=x * 2^-46;
  end
%end


function  x   =glblbit(n,x,a)
  global X			%		MATLAB	Octave
    a1 = bitshift(a, -23  );	% timing:	73.---	7.353
    a2 = bitand  (a,2^23-1);
  for i=1:n
    x1 = bitshift(x, -23  );
    x2 = bitand  (x,2^23-1);

    z  = bitand  ( a1*x2 + a2*x1, 2^23-1);
    x  = bitand  (z*2^23 + a2*x2, 2^46-1);

    X(i)=x * 2^-46;
  end
%end


function [x,varargout]=gvarbit(n,x,a)
if nargout<2, global X		%		MATLAB	Octave
else				% timing:	73.---	7.353
    X=-1.D99*ones(n,1);
end
    a1 = bitshift(a, -23  );
    a2 = bitand  (a,2^23-1);
  for i=1:n
    x1 = bitshift(x, -23  );
    x2 = bitand  (x,2^23-1);

    z  = bitand  ( a1*x2 + a2*x1, 2^23-1);
    x  = bitand  (z*2^23 + a2*x2, 2^46-1);

    X(i)=x * 2^-46;
  end
if nargout>1, varargout={X}; end
%end


function [x,y]=vranmod(n,x,a)
  y=     zeros(n,1);		%		MATLAB	Octave
    a1 = floor(a/2^ 23);	% timing:	1.848	41.---
    a2 = mod  (a,2^ 23);
  for i=1:n
    x1 = floor(x/2^ 23);
    x2 = mod  (x,2^ 23);

    z  = mod  (  a1*x2 + a2*x1, 2^23);
    x  = mod  (z*2^ 23 + a2*x2, 2^46);

    y(i)=x * 2^-46;
  end
%end


function  x   =glblmod(n,x,a)
  global X			%		MATLAB	Octave
    a1 = floor(a/2^ 23);	% timing:	1.848	41.---
    a2 = mod  (a,2^ 23);
  for i=1:n
    x1 = floor(x/2^ 23);
    x2 = mod  (x,2^ 23);

    z  = mod  (  a1*x2 + a2*x1, 2^23);
    x  = mod  (z*2^ 23 + a2*x2, 2^46);

    X(i)=x * 2^-46;
  end
%end


function [x,varargout]=gvarmod(n,x,a)
if nargout<2, global X		%		MATLAB	Octave
else				% timing:	1.848	41.---
    X=-1.D99*ones(n,1);
end
    a1 = floor(a/2^ 23);
    a2 = mod  (a,2^ 23);
  for i=1:n
    x1 = floor(x/2^ 23);
    x2 = mod  (x,2^ 23);

    z  = mod  (  a1*x2 + a2*x1, 2^23);
    x  = mod  (z*2^ 23 + a2*x2, 2^46);

    X(i)=x * 2^-46;
  end
if nargout>1, varargout={X}; end
%end


function [x,X]=gnarmod(n,x,a)
if nargout<2, global X		%		MATLAB	Octave can't global X
else				% timing:	1.848	41.---
    X=-1.D99*ones(n,1);
end
    a1 = floor(a/2^ 23);
    a2 = mod  (a,2^ 23);
  for i=1:n
    x1 = floor(x/2^ 23);
    x2 = mod  (x,2^ 23);

    z  = mod  (  a1*x2 + a2*x1, 2^23);
    x  = mod  (z*2^ 23 + a2*x2, 2^46);

    X(i)=x * 2^-46;
  end
%end


function [x,y]=vranari(n,x,a)
  y=     zeros(n,1);		%		MATLAB	Octave
    a1 = floor(a *2^-23);	% timing:	0.899	6.272
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

    y(i)=x * 2^-46;
  end
%end


function  x   =glblari(n,x,a)
  global X			%		MATLAB	Octave
    a1 = floor(a *2^-23);	% timing:	0.899	6.272
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
%end


function [x,varargout]=gvarari(n,x,a)
if nargout<2, global X		%		MATLAB	Octave
else				% timing:	0.899	6.272
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


function [x,X]=gnarari(n,x,a)
if nargout<2, global X		%		MATLAB	Octave can't global X
else				% timing:	0.899	6.272
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
%end


