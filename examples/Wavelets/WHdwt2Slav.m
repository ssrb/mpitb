function res=WHdwt2Slav(img12,f1,f2, prt,flg)
% WHDWT2SLAV	WaveHalf 2D  DWT, Slave  process (image halves)
%		same code for variants #1 / #2
%
%	res = WHdwt2Slav ( img12,f1,f2, prt,flg )
%
% img12(matrix)	2-D signal upper/lower half of image (img1/2 in graph below)
%  f1  (vector)	1-D filter 1st stage (horz)
%  f2  (vector)	1-D filter 2nd stage (vert)
%
%  prt (int)	partenaire, MPI rank handling the other half-image
%  flg (int)	0/1, flag indicating we are 2nd (lower) half
%
%  res (matrix)	2-D signal approx/detail, also returned via MPI to parent
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%		 (img12)					      (res)
%                   |              ----           ----           ----
%                   V       --->h->| 1| ah upr\  /| 1|     ->h-> | 1| ahav
%                          /       ----        \/ |  |----       ----
%                --------  |       ----        /\ ----|  |       ----
% --------       | img1 | -|  />h->| 2| ah lwr/  \    | 2| ->g-> | 2| ahdv
% | org  |  --\  --------   \/     ----               ----       ----
% | img  |  --/  --------   /\     ----           ----           ----
% --------       | img2 | -|  \>g->| 3| dh upr\  /| 3|     ->h-> | 3| dhav
%                --------  |       ----        \/ |  |----       ----
%                          \       ----        /\ ----|  |       ----
%                           --->g->| 4| dh lwr/  \    | 4| ->g-> | 4| dhdv
%                                  ----               ----       ----
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

global RANK
if isempty(RANK), time_stamp('WHdwt2Slav_ini'), end
%%%%%%%%%%%
% Lengths %
%%%%%%%%%%%
lih =size(img12,2); liv2=size(img12,1);	% Length image horz/vert
lih2=ceil(lih/2  ); liv =2*liv2;	% img12 is upper/lower half


if isempty(RANK), time_stamp('WHdwt2Slav_rows'), end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 1st stage, row filtering %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tmp=zeros(liv,lih2);			% Pre-allocation for full ah/dh
for row=1:liv2				% We have only upper/lower half (flg?)
  if isempty(RANK), if mod(row,100)==1
    if  liv2-row>=99,	time_stamp('100r')
    else,		time_stamp([int2str(liv2-row+1) 'r'])
  end,end,end

  r  = img12(row,:);			% We'll get only upr/lwr (flg?) ah/dh
  rf1= dwtflt(r',f1);			%  ah/dh  depending on f1 ==h/g
  tmp(flg*liv2+row,:) = rf1 ;		% upr/lwr depending on flg==0/1
end


%%%%%%%%%%%%
% exchange %
%%%%%%%%%%%%
global TAG NEWORLD			% from NumCmds_Init/MATLAB_Spawn

if isempty(RANK), time_stamp('cp'), end
msg=tmp(flg*liv2+1:(flg+1)*liv2, : );	% exchange our half with partenaire's

if isempty(RANK), time_stamp('replace'), end
MPI_Sendrecv_replace(msg, prt,TAG, prt,TAG, NEWORLD);

if isempty(RANK), time_stamp('cp'), end
tmp((1-flg)*liv2+1:(2-flg)*liv2, : )=msg;		% paste to ah/dh


if isempty(RANK), time_stamp('WHdwt2Slav_cols'), end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 2nd stage, column filtering %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
res=zeros(liv2,lih2);			% Pre-allocation ahav/.../dhdv (res)
for col=1:lih2				% ah/dh is half-horz.
  if isempty(RANK), if mod(col,100)==1
    if  lih2-col>=99,	time_stamp('100c')
    else,		time_stamp([int2str(lih2-col+1) 'c'])
  end,end,end

  c  = tmp(:,col);
  cf2= dwtflt(c ,f2);			% [x]hav/dv depending on f2 ==h/g
  res(:,col)= cf2';			%    whole tmp column==xh (ah/dh)
end


%%%%%%%%%%%%%%%%%%
% send transform %
%%%%%%%%%%%%%%%%%%
[info rank]=MPI_Comm_rank(NEWORLD);
if rank					% when nslv==3, master must not run this
  MPI_Send(res,0,TAG,NEWORLD);		% so no need to timestamp, really
end					% not interested in slave timestamps

if isempty(RANK), time_stamp('WHdwt2Slav_fin'), end

