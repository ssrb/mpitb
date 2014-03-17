function z=idwtflt(x,f, bord)
% IDWTFLT	Upsample & filter for Wavelet transform (f/approx. or detail)
%
%       z = idwtflt ( x, f [,bord] )
%
%  x (vector)   1-D signal to convolve (column)
%  f (vector)   wavelet filter impulse response (h "lowpass"/g "highpass")
%  bord (int)   border policy
%               0 -> add zeros
%               1 -> add specular copy of signal (default)
%
%  z (vector)   resulting 1-D signal
%

%%%%%%%%%%						    lf2   1    lf2
% ArgChk % lf2 half-filter. lf odd => lf2 is tail length (<tail>center<tail>)
%%%%%%%%%%
lx=length(x); lf=length(f); lf2=floor(lf/2);
if nargin<3, bord=1; end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Upsample, left-right reflections of size lf2, Convolution & Centering
% ly == 2lx + 2lf2
% lz == ly + lf - 1 == 2lx + 2lf2 + lf - 1
% trim down to 2lx (centering)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
y=zeros(2*lx,1);		% Pre-allocation
y(1:2:2*lx)=x;			% Upsample. x(1),0,x(2),0...x(lx),0

if bord,y=[y(lf2+1:-1:2); y ; y(2*lx-1:-1:2*lx-lf2)];
else,	y=[zeros(lf2, 1); y ; zeros(lf2,1)];
end;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% reflection: lf odd=>
%  when filter center is over y(1), filter tail is over y(2)...y(lf2+1)
% Add lf2 specular samples (2..lf2+1), not repeating y(1)==x(1)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
z=conv(y,f);			% Filter/recover from R^2 normalization
z=2*z(lf:2*lx+lf-1);		% Trim out tails. Final length 2lx
				% 2x to recover from R^2 normalization
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Correspondence between signal indexes:
% original:                    x(1)...x(lx)
% Upsample:	 	  y/x(1),0,...x(lx),y(2lx)==0
% w/border:        y(1)...y(lf2+1) ...      y(2lx+ lf2)...y(2lx+2lf2)
% Conv: z(1)...z(lf2+1)...z(2lf2+1)...      z(2lx+2lf2)...z(2lx+3lf2)...+4lf2)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% If lf odd:           ...z(lf)    ...      z(2lx+lf-1)...
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% If lf even:          ...z(lf+1)  ...      z(2lx+lf)  ...
%  should we trim between  (lf+1   ...        2lx+lf)  ???
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

