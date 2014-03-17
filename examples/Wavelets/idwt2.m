function img=idwt2(ahav, ahdv, dhav, dhdv, h,g, bord)
% IDWT2		2-D Inverse Discrete Wavelet Transform (from appr. & 3 details)
%
%	img = idwt2 ( ahav, ahdv, dhav, dhdv, h,g [,bord] )
%
%  ahav(matrix) 2-D approx.
%  ahdv(matrix) 2-D horz.detail
%  dhav(matrix) 2-D vert.detail
%  dhdv(matrix) 2-D diag.detail.
%
%  h   (vector) 1-D scale   filter ( lowpass)
%  g   (vector) 1-D wavelet filter (highpass) same length. lh = lg
%  bord   (int)	border policy
%		0 -> add zeros
%		1 -> add specular copy of signal (default)
%
%  img (matriz) 2-D reconstruction. Area=4*approx.
%

time_stamp('idwt2_ini');
%%%%%%%%%%%
% Lengths %
%%%%%%%%%%%
l2=size(ahav);
if any(l2~=size(ahdv))||...
   any(l2~=size(dhav))||...
   any(l2~=size(dhdv)), error('differently sized coeffs'), end
lih2=l2(2);  liv2=l2(1);			% Length image horz/vert half
lih =lih2*2; liv =liv2*2;			% Length image horz/vert
if nargin<7, bord=1; end


time_stamp('idwt2_cols');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% COLS de-filtering, along rows
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ----   h   ----       -> ah->h->ahav, reverse now
% |  |  /--  |  | ----
% |  |  \--  ---- |  |  -> ah->g->ahdv,
% ----   g        ----
%  ah        ahav ahdv
%%%%%%%%%%%%%%%%%%%%%%
% ----   h   ----       -> dh->h->dhav,
% |  |  /--  |  | ----
% |  |  \--  ---- |  |  -> dh->g->dhdv, reverse now
% ----   g        ----
%  dh        dhav dhdv
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ah=zeros(liv,lih2);			% Pre-allocation
dh=zeros(liv,lih2);			% Pre-allocation
for col=1:lih2
   if mod(col,50)==1
     if  lih2-col>=49,	time_stamp('100c')
     else,		time_stamp([int2str(2*(lih2-col+1)) 'c'])
   end,end

   ca=ahav(:,col); cd=ahdv(:,col);	%%%%%%%%%%%%%%%
   c =idwt(ca ,cd ,h,g, bord);		% ahav,ahdv->ah
   ah(:,col)= c';			%%%%%%%%%%%%%%%

   ca=dhav(:,col); cd=dhdv(:,col);	%%%%%%%%%%%%%%%
   c =idwt(ca ,cd ,h,g, bord);		% dhav,dhdv->dh
   dh(:,col)= c';			%%%%%%%%%%%%%%%
end


time_stamp('idwt2_rows');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ROWS de-filtering, along columns
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --------       ----
% |      |  /--  |  |
% |      |  \--  |  |
% --------       ----
%   img      h    ah    img->h->ah, reverse now
%            g    dh    img->g->dh, reverse now
%                ----
%           /--  |  |
%           \--  |  |
%                ----
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
img=zeros(liv,lih);			% Pre-allocation
for row=1:liv
   if mod(row,100)==1
     if  liv-row>=99,	time_stamp('100r')
     else,		time_stamp([int2str(liv-row+1) 'r'])
   end,end

   ra=ah(row,:); rd=dh(row,:);		%%%%%%%%%%%%
   r =idwt(ra',rd',h,g, bord);		% ah,dh->img
   img(row,:)= r ;			%%%%%%%%%%%%
end					% Pass always column 1-D vectors

time_stamp('idwt2_fin');
