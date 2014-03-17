function [a, d]=dwt(x,h,g, bord)
% DWT		Discrete Wavelet Transform (aproximation and detail)
%
%	[a d] = dwt ( x, h,g [,bord] )
%
%  x (vector)	1-D signal to analyze
%  h (vector)	1-D scale   filter ( lowpass)
%  g (vector)	1-D wavelet filter (highpass) same length lh==lg
%  bord (int)	border policy
%		0 -> add zeros
%		1 -> add specular copy of signal (default)
%
%  a (vector)	1-D approx. Length half of x
%  d (vector)	1-D detail. Same length (la = ld = ceil(lx/2))
%


%%%%%%%%%%						    lf2   1    lf2
% ArgChk % lf2 half-filter. lf odd => lf2 is tail length (<tail>center<tail>)
%%%%%%%%%%
lx=length(x); lh=length(h); lg=length(g);
if nargin<3, bord=1; end

if lh~=lg, error('differently sized filters'), end

lh2=floor(lh/2); lg2=floor(lg/2);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% left-right reflections of size lf2, Convolution, Centering & Downsample
% ly == lx + 2lf2				(filter f = h/g)
% lz == ly + lf - 1 == lx + 2lf2 + lf - 1	(signal z = a/d)
% trim down to lx (center), and then to lx/2 (downsample)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%
% Approx.
%%%%%%%%%
if bord,y=[x(lh2+1:-1:2); x ; x(lx-1:-1:lx-lh2)];
else,	y=[zeros(lh2, 1); x ; zeros(lh2,1)];
end;

a=conv(y,h);		% Filter
a=a(lh:lx+lh-1);	% Trim out tails
a=a(1:2:lx);		% Downsample. Final length ceil(lx/2) perhaps round-up


%%%%%%%%%
% Detail.
%%%%%%%%%
if bord,y=[x(lg2+1:-1:2); x ; x(lx-1:-1:lx-lg2)];
else,	y=[zeros(lg2, 1); x ; zeros(lg2,1)];
end

d=conv(y,g);		% Filter
d=d(lg:lx+lg-1);	% Trim out tails
d=d(1:2:lx);		% Downsample. Final length ceil(lx/2) perhaps round-up


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% reflections: lf odd =>
% when filter center is over x(1), filter tail is over x(2)...x(lf2+1)
% Add lf2 specular samples (2..lf2+1), not repeating x(1)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Correspondence between signal indexes:
% original:                    x(1)...x(     lx)
% w/border:        y(1)... y(lf2+1)...y( lf2+lx)...y(2lf2+lx)
% conv: z(1)...z(lf2+1)...z(2lf2+1)...z(2lf2+lx)...z(3lf2+lx)...z(4lf2+lx)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% If lf odd:            ...z(lf)   ...z(lx+lf-1)...
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% If lf even:           ...z(lf+1) ...z(lx+lf)  ...
%  should we trim between   (lf+1  ...  lx+lf)  ???
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

