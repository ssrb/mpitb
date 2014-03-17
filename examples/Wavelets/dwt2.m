function [ahav, ahdv, dhav, dhdv]=dwt2(img, h,g, bord)
% DWT2		2-D Discrete Wavelet Transform (appr. & details horz/vert/diag)
%
%	[ahav, ahdv, dhav, dhdv] = dwt2 ( img, h,g [,bord] )
%
%  img (matrix)	2-D signal to analyze
%  h   (vector)	1-D scale   filter ( lowpass)
%  g   (vector)	1-D wavelet filter (highpass) same length lh==lg
%  bord   (int)	border policy
%		0 -> add zeros
%		1 -> add specular copy of signal (default)
%
%  ahav(matrix)	2-D signal horz. approx. vert. approx. area==1/4 of img
%  ahdv(matrix)	2-D signal horz. approx. vert. detail  (horz. details)
%  dhav(matrix)	2-D signal horz. detail  vert. approx. (vert. details)
%  dhdv(matrix)	2-D signal horz. detail  vert. detail  (diag. details)
%

time_stamp('dwt2_ini');
%%%%%%%%%%%
% Lengths %
%%%%%%%%%%%
lih =size(img,2); liv =size(img,1);		% Length image horz/vert
lih2=ceil(lih/2); liv2=ceil(liv/2);		% decimation: half/half+1
if nargin<4, bord=1; end


time_stamp('dwt2_rows');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ROWS filtering, along columns
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --------       ----
% |      |  --\  |  |
% |      |  --/  |  |
% --------       ----
%   img      h    ah	scale   ( lowpass) filtering -> horz.approx.
%            g    dh	wavelet (highpass) filtering -> horz.detail.
%                ----
%           --\  |  |
%           --/  |  |
%                ----
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ah=zeros(liv,lih2);			% Pre-allocation
dh=zeros(liv,lih2);
for row=1:liv				%%%%%%%%%%%%
   if mod(row,100)==1
     if  liv-row>=99,	time_stamp('100r')
     else,		time_stamp([int2str(liv-row+1) 'r'])
   end,end

   r      = img(row,:);			% img->ah,dh
  [ra rd] = dwt(r',h,g, bord);		%%%%%%%%%%%%
   ah(row,:) = ra;			% Pass always column 1-D vectors
   dh(row,:) = rd;			% original ra' & rd' in MATLAB
end


time_stamp('dwt2_cols');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% COLS filtering, along rows
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ----   h   ----       -> ah->h->ahav horz.approx.vert.approx.
% |  |  --\  |  | ----
% |  |  --/  ---- |  |  -> ah->g->ahdv horz.approx.vert.detail.
% ----   g        ----
%  ah        ahav ahdv
%%%%%%%%%%%%%%%%%%%%%%
% ----   h   ----       -> dh->h->dhav horz.detail.vert.approx.
% |  |  --\  |  | ----
% |  |  --/  ---- |  |  -> dh->g->dhdv horz.detail.vert.detail.
% ----   g        ----
%  dh        dhav dhdv
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ahav=zeros(liv2,lih2);			% Pre-allocation
ahdv=zeros(liv2,lih2);
dhav=zeros(liv2,lih2);			% Pre-allocation
dhdv=zeros(liv2,lih2);
for col=1:lih2
   if mod(col,50)==1
     if  lih2-col>=49,	time_stamp('100c')
     else,		time_stamp([int2str(2*(lih2-col+1)) 'c'])
   end,end

   c      = ah(:,col);			%%%%%%%%%%%%%%%
  [ca cd] = dwt(c ,h,g, bord);		% ah->ahav,ahdv
   ahav(:,col)= ca';			%%%%%%%%%%%%%%%
   ahdv(:,col)= cd';			% Pass always column 1-D vectors
					% original ca & cd in MATLAB, w/out '
   c      = dh(:,col);			%%%%%%%%%%%%%%%
  [ca cd] = dwt(c ,h,g, bord);		% dh->dhav,dhdv
   dhav(:,col)= ca';			%%%%%%%%%%%%%%%
   dhdv(:,col)= cd';
end

time_stamp('dwt2_fin');
