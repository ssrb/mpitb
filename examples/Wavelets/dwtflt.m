function z=dwtflt(x,f, bord)
% DWTFLT	Filter & downsample for Wavelet transform (approx. or detail)
%
%	z = dwtflt ( x, f [,bord] )
%
%  x (vector)	1-D signal to convolve (column)
%  f (vector)	wavelet filter impulse response (h "lowpass"/g "highpass")
%  bord (int)	border policy
%		0 -> add zeros
%		1 -> add specular copy of signal (default)
%
%  z (vector)	resulting 1-D signal (approx/detail depending on f=h/g)
%

%%%%%%%%%%						    lf2   1    lf2
% ArgChk % lf2 half-filter. lf odd => lf2 is tail length (<tail>center<tail>)
%%%%%%%%%%
lx=length(x); lf=length(f); lf2=floor(lf/2);
if nargin<3, bord=1; end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% left-right reflections of size lf2, Convolution, Centering & Downsample
% ly == lx + 2lf2
% lz == ly + lf - 1 == lx + 2lf2 + lf - 1
% trim down to lx (center), and then to lx/2 (downsample)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if bord,y=[x(lf2+1:-1:2); x ; x(lx-1:-1:lx-lf2)];
else,	y=[zeros(lf2, 1); x ; zeros(lf2,1)];
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% reflection: lf odd =>
% when filter center is over x(1), filter tail is over x(2)...x(lf2+1)
% Add lf2 specular samples (2..lf2+1), not repeating x(1)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
z=conv(y,f);		% Filter
z=z(lf:lx+lf-1);	% Trim out tails
z=z(1:2:lx);		% Downsample. Final length ceil(lx/2) perhaps round-up

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Correspondence between signal indexes:
% original:	               x(1)...x(     lx)
% w/border:        y(1)... y(lf2+1)...y( lf2+lx)...y(2lf2+lx)
% conv:	z(1)...z(lf2+1)...z(2lf2+1)...z(2lf2+lx)...z(3lf2+lx)...z(4lf2+lx)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% If lf odd:		...z(lf)   ...z(lx+lf-1)...
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% If lf even:		...z(lf+1) ...z(lx+lf)  ...
%  should we trim between   (lf+1  ...  lx+lf)  ???
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

