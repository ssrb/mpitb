function [ahav,ahdv,dhav,dhdv] = WHdwt2Mast(variant, img, h,g)
% WHDWT2MAST		WaveHalf  DWT 2D, Master process (image halves)
%			variant #1: Pack/Send all vars using NumCmds_Send
%			variant #2: Send img directly, don't pack/NumCmds_Send
%
%	[ahav, ahdv, dhav, dhdv] = WHdwt2Mast ( variant, img, h,g )
%
%  variant(int)	algorithm variant chosen (Pack&Send or Send separately)
%
%  img (matrix)	2-D signal to analyze. 1 Approx. and 3 Details are returned
%  h   (vector)	1-D approx. filter	("low  pass")
%  g   (vector)	1-D detail  filter	("high pass") same length lh==lg
%
%  ahav(matrix)	2-D signal horz. approx. vert. approx. area==1/4 of img
%  ahdv(matrix)	2-D signal horz. approx. vert. detail  (horz. details)
%  dhav(matrix)	2-D signal horz. detail  vert. approx. (vert. details)
%  dhdv(matrix)	2-D signal horz. detail  vert. detail  (diag. details)
%

time_stamp('WHdwt2Mast_ini');
%%%%%%%%%%%
% Lengths %
%%%%%%%%%%%
lih =size(img,2); liv =size(img,1);		% Length image horz/vert
lih2=ceil(lih/2); liv2=ceil(liv/2);		% decimation: half/half+1


disp('sending command'),	fflush(stdout);
time_stamp('cmdprep');
%%%%%%%%%%%%%%%%%%%%
% Send half images % 2 groups of 2 ranks
%%%%%%%%%%%%%%%%%%%%
	cmd=[	'WHdwt2Slav (img12,f1,f2,prt,flg);',...
		'clear       img12 f1 f2 prt flg'];

	img1=img(     1:liv2,:); A=img1;	% Upper half to 1st. ranks
	img2=img(liv2+1:liv ,:); B=img2;	% Lower half to 2nd. ranks

    if variant==2
	cmd=[   'img12=zeros(img12);',...	% img is actually imagesize
	'[inf,stat]=MPI_Recv(img12,0,TAG,NEWORLD);',...
		 cmd];				% we'll later send img directly
				A=size(A);
				B=size(B);
    end

	nams={'cmd','img12','f1','f2','prt','flg'};

global NSLAVES					% from LAM_Init
switch NSLAVES
case 4
  vals={ cmd , A  ,  h ,  h ,  2  ,  0 ;...
	 cmd , B  ,  h ,  g ,  1  ,  1 ;...
	 cmd , A  ,  g ,  h ,  4  ,  0 ;...
	 cmd , B  ,  g ,  g ,  3  ,  1 ;...
	};
case 3						% rank 0 (me) will work
  vals={ cmd , A  ,  h ,  h ,  2  ,  0 ;...
	 cmd , B  ,  h ,  g ,  1  ,  1 ;...
	 cmd , A  ,  g ,  h ,  0  ,  0 ;...
%	 cmd , B  ,  g ,  g ,  3  ,  1 ;...	% don't send my own workload
	};
end

	time_stamp('cmdsend');
	NumCmds_Send(nams,vals);		% send workload away

global TAG NEWORLD				% from NumCmds_Init/Octave_Spawn
    if variant==2				% Send now, w/o packing
	time_stamp('s1'); MPI_Send(img1,1,TAG,NEWORLD);
	time_stamp('s2'); MPI_Send(img2,2,TAG,NEWORLD);
	time_stamp('s3'); MPI_Send(img1,3,TAG,NEWORLD);
    if NSLAVES==4
	time_stamp('s4'); MPI_Send(img2,4,TAG,NEWORLD);
    end,end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                  ----           ----           ----
%                           --->h->| 1| ah sup\  /| 1|     ->h-> | 1| ahav
%                          /       ----        \/ |  |----       ----
%                --------  |       ----        /\ ----|  |       ----
% --------       | img1 | -|  />h->| 2| ah inf/  \    | 2| ->g-> | 2| ahdv
% | img  |  --\  --------   \/     ----               ----       ----
% |      |  --/  --------   /\     ----           ----           ----
% --------       | img2 | -|  \>g->| 3| dh sup\  /| 3|     ->h-> | 3| dhav
%                --------  |       ----        \/ |  |----       ----
%                          \       ----        /\ ----|  |       ----
%                           --->g->| 4| dh inf/  \    | 4| ->g-> | 4| dhdv
%                                  ----               ----       ----
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%       First stage: filter by rows - along columns, horizontal
%       1st group (1,2) computes approx. (ah) - 2nd group (3,4) detail (dh)
%         1st CPU in each group filters upper image half - 2nd CPU lower half
%         exchange of results - each CPU has a full ah/dh
%               1 ah upper\/ 1 full ah - 3 dh upper\/ 3 full dh
%               2 ah lower/\ 2 full ah - 4 dh lower/\ 4 full dh
%
%       Second stage: filter by columns - along rows, vertical
%         1st CPU in each group filters intermediate approx. [x]hav
%         2nd CPU in each group filters intermediate detail  [x]hdv
%                       1 ah->ahav      -       3 dh->dhav
%                       2 ah->ahdv      -       4 dh->dhdv
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


    time_stamp('work');
%%%%%%%%%%%%%%%%%%%%%
% Receive Transform %				% Waiting for results
%%%%%%%%%%%%%%%%%%%%%
if NSLAVES==3					% will I work?
    disp('local computing'),	fflush(stdout);
    dhdv=WHdwt2Slav(img2,g,g, 3,1);		% If so, compute dhdv here
end						% while others are working

    ahav=zeros(liv2,lih2);			% Preallocate recv buffers
    ahdv=ahav; ahdv(1)=0;			% Avoid lazy copy
    dhav=ahav; dhav(1)=0;
if NSLAVES==4
    dhdv=ahav; dhdv(1)=0;
end

    disp('recv results'),	fflush(stdout);	% Receive

   time_stamp('r1'); MPI_Recv(ahav,1,TAG,NEWORLD);
   time_stamp('r2'); MPI_Recv(ahdv,2,TAG,NEWORLD);
   time_stamp('r3'); MPI_Recv(dhav,3,TAG,NEWORLD);
if NSLAVES==4
   time_stamp('r4'); MPI_Recv(dhdv,4,TAG,NEWORLD);
end

%   trf=[ahav,ahdv;dhav,dhdv];			% usual transform organization

   time_stamp('WHdwt2Mast_fin');

