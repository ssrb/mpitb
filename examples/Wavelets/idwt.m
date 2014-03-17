function x=idwt(a,d,h,g, bord)
% IDWT		Inverse Discrete Wavelet Transform (reconstr from appr/detail)
%
%	x = idwt ( a,d, h,g [,bord] )
%
%  a (vector)	1-D approx.
%  d (vector)	1-D detail. Same length la = ld
%
%  h (vector)	1-D scale   filter ( lowpass)
%  g (vector)	1-D wavelet filter (highpass) same length. lh = lg
%  bord   (int)	border policy
%		0 -> add zeros
%		1 -> add specular copy of signal (default)
%
%  x (vector)   1-D reconstruction. Double length (lx = 2la = 2ld)
%


%%%%%%%%%%						    lf2   1    lf2
% ArgChk % lf2 half-filter. lf odd => lf2 is tail length (<tail>center<tail>)
%%%%%%%%%%
la=length(a); ld=length(d);
lh=length(h); lg=length(g);
if nargin<3, bord=1; end

if la~=ld, error('differently sized approx/detail'),	end
if lh~=lg, error('differently sized filters'),		end

lh2=floor(lh/2); lg2=floor(lg/2);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Upsample, 2 reflections of length lh2, Convolution & Centering
% ly == 2lx + 2lf2				(filter f = h/g)
% lz == ly + lf - 1 == 2lx + 2lf2 + lf - 1	(signal x = a/d)
% trim down to 2lx (centering)			(signal z = reconstr a/d)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%
% Approx.
%%%%%%%%%
y=zeros(2*la,1);		% Pre-alloc
y(1:2:2*la)=a;			% Upsample. a(1),0,a(2),0...a(la),0

if bord,y=[y(lh2+1:-1:2); y ; y(2*la-1:-1:2*la-lh2)];
else,	y=[zeros(lh2, 1); y ; zeros(lh2,1)];
end;

za=conv(y,h);			% Filter
za=za(lh:2*la+lh-1);		% Trim out tails


%%%%%%%%%
% Detail.
%%%%%%%%%
y=zeros(2*ld,1);		% Pre-alloc
y(1:2:2*ld)=d;			% Upsample. d(1),0,d(2),0...d(ld),0
if bord,y=[y(lg2+1:-1:2); y ; y(2*ld-1:-1:2*ld-lg2)];
else,	y=[zeros(lg2, 1); y ; zeros(lg2,1)];
end;

zd=conv(y,g);			% Filter
zd=zd(lg:2*ld+lg-1);		% Trim out tails

%%%%%%%%%%%
% Result			% 2x to recover from R^2 normalization
%%%%%%%%%%%
x=2*(za+zd);


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

