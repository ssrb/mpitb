function res=WHidwt2Slav(img,f1,f2, prt,flg)
% WHIDWT2SLAV	WaveHalf IDWT 2D, Slave  process (image halves)
%		same code for variants #1 / #2
%
%	res = WHidwt2Slav ( img,f1,f2, prt,flg )
%
%  img (matrix)	2-D signal (wavelet coeffs)
%  f1  (vector)	1-D filter 1st stage (horz)
%  f2  (vector)	1-D filter 2nd stage (vert)
%
%  prt (int)	partenaire, MPI rank cooperating in the same half-image
%  flg (int)	0/1, flag indicating we are 2nd (lower) half
%
%  res (matrix)	2-D half-image (a/d upr/lwr), also returned via MPI to parent
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%      (img)						      (res)
%        |	   aha						|
% ----   V         ---- <-+--    ----               --------	V
% | 1| ahav ->h -> | 1|   V      | 1| ah upr ->h -> |     1| a upr\
% ----        *2   |  |----      ----          *2   --------       \      r
% ----             ----|  |      ----               --------        \  --------
% | 2| ahdv ->g ->     | 2|ahd   | 2| ah lwr ->h -> |     2| a lwr\  >-| orig |
% ----        *2   dha ----      ----          *2   --------       \/  |______|
% ----             ---- <-+--    ----               --------       /\  |      |
% | 3| dhav ->h -> | 3|   V      | 3| dh upr ->g -> |     3| d upr/  >-| img  |
% ----        *2   |  |----      ----          *2   --------        /  --------
% ----             ----|  |      ----               --------       /
% | 4| dhdv ->g ->     | 4|dhd   | 4| dh lwr ->g -> |     4| d lwr/
% ----        *2       ----      ----          *2   --------
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

global RANK
if isempty(RANK), time_stamp('WHidwt2Slav_ini'), end
%%%%%%%%%%%
% Lengths %
%%%%%%%%%%%
lih2=size(img,2); liv2=size(img,1);	% Length image horz/vert
lih =2*lih2;	  liv =2*liv2;		% img is 1/4 orig img


if isempty(RANK), time_stamp('WHidwt2Slav_cols'), end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Reverse 2nd stage, column filtering %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tmp=zeros(liv,lih2);			% Pre-allocation for full ah/dh
for col=1:lih2				% We have only [x]h[app/det]vertical
  if isempty(RANK), if mod(col,100)==1
    if  lih2-col>=99,	time_stamp('100c')
    else,		time_stamp([int2str(lih2-col+1) 'c'])
  end,end,end

  c  = img(:,col);			% We'll have to add det/app from prt
  cf2= idwtflt(c ,f2);			% xha/xhd depending on f2 ==h/g
  tmp(:,col) = cf2';			% upr/lwr depending on flg==0/1
end


%%%%%%%%%%%% discard one-half to partenaire & add partenaire's discarded half
% exchange % e.g.: rank 1 ahav->aha-\/- add - upper ah
%%%%%%%%%%%%       rank 2 ahdv->ahd-/\- add - lower ah
global TAG NEWORLD

if isempty(RANK), time_stamp('cp'), end
this=tmp(   flg *liv2+1:(flg+1)*liv2, : );		% this half for me
othr=tmp((1-flg)*liv2+1:(2-flg)*liv2, : );		% this for partenaire

if isempty(RANK), time_stamp('replace'), end
MPI_Sendrecv_replace(othr, prt,TAG, prt,TAG, NEWORLD);	% exchange

if isempty(RANK), time_stamp('sum'), end
tmp = this+othr;					% add / destroy tmp
					% tmp is now same size as ahav

if isempty(RANK), time_stamp('WHidwt2Slav_rows'), end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Reverse 1st stage, row filtering %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
res=zeros(liv2,lih);			% Pre-allocation a/d upr/lwr
for row=1:liv2				% res is half-size reconstructed img
  if isempty(RANK), if mod(row,100)==1
    if  liv2-row>=99,	time_stamp('100r')
    else,		time_stamp([int2str(liv2-row+1) 'r'])
  end,end,end

  r  = tmp(row,:);
  rf1= idwtflt(r',f1);			%   a/d   depending on f1 ==h/g
  res(row,:)= rf1 ;			% upr/lwr depending on flg==0/1
end

%%%%%%%%%%%%%%%%%%%%%%%
% send reconstruction %
%%%%%%%%%%%%%%%%%%%%%%%
[info rank]=MPI_Comm_rank(NEWORLD);
if rank					% when nslv==3, master must not run this
  MPI_Send(res,0,TAG,NEWORLD);		% so no need to timestamp, really
end					% not interested in slave timestamps

if isempty(RANK), time_stamp('WHidwt2Slav_fin'), end

