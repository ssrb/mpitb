function img=WHidwt2Mast(variant, ahav,ahdv,dhav,dhdv, h,g)
% WHIDWT2MAST		WaveHalf IDWT 2D, Master process (image halves)
%			variant #1: Pack/Send all vars using NumCmds_Send
%			variant #2: Send img directly, don't pack/NumCmds_Send
%
%	img = WH1idwt2Mast ( variant, ahav,ahdv,dhav,dhdv, h,g )
%
%  variant(int)	algorithm variant chosen (Pack&Send or Send separately)
%
%  ahav(matrix) 2-D signal horz. approx. vert. approx. area==1/4 of img
%  ahdv(matrix) 2-D signal horz. approx. vert. detail  (horz. details)
%  dhav(matrix) 2-D signal horz. detail  vert. approx. (vert. details)
%  dhdv(matrix) 2-D signal horz. detail  vert. detail  (diag. details)
%
%  h   (vector)	1-D approx. filter      ("low  pass")
%  g   (vector)	1-D detail  filter      ("high pass") same length lh==lg
%
%  img (matrix)	Reconstructed 2-D signal. Area = 4*ahav
%

time_stamp('WHidwt2Mast_ini');
%%%%%%%%%%%
% Lengths %
%%%%%%%%%%%
sz2=size(ahav); lih2=sz2(2); liv2=sz2(1);	% Length image horz/vert
sz =2*sz2;	lih =sz (2); liv =sz (1);	% size of original image...


disp('sending command'),	fflush(stdout);
time_stamp('cmdprep');
%%%%%%%%%%%%%%%
% Send coeffs % 2 groups of 2 ranks
%%%%%%%%%%%%%%%
	cmd=[	'WHidwt2Slav(img,f1,f2,prt,flg);',...
		'clear       img f1 f2 prt flg'];

	A=ahav;		C=dhav;
	B=ahdv;		D=dhdv;

    if variant==2
	cmd=[	'img=zeros  (img);',...		% img is actually imagesize
	'[inf,stat]=MPI_Recv(img,0,TAG,NEWORLD);',...
		 cmd];				% we'll later send img directly
	A=size(A);	C=size(C);
	B=size(B);	D=size(D);
    end

	nams={'cmd','img','f1','f2','prt','flg'};

global NSLAVES					% from LAM_Init
switch NSLAVES
case 4
  vals={ cmd , A,  h ,  h ,  2  ,  0 ;...
	 cmd , B,  h ,  g ,  1  ,  1 ;...
	 cmd , C,  g ,  h ,  4  ,  0 ;...
	 cmd , D,  g ,  g ,  3  ,  1 ;...
	};
case 3						% rank 0 (me) will work
  vals={ cmd , A,  h ,  h ,  2  ,  0 ;...
	 cmd , B,  h ,  g ,  1  ,  1 ;...
	 cmd , C,  g ,  h ,  0  ,  0 ;...
%	 cmd , D,  g ,  g ,  3  ,  1 ;...	% don't send my own workload
	};
end

	time_stamp('cmdsend');
	NumCmds_Send(nams,vals);		% send workload away

global TAG NEWORLD				% from NumCmds_Init/Octave_Spawn
    if variant==2				% Send now, w/o packing
	time_stamp('s1'); MPI_Send(ahav,1,TAG,NEWORLD);
	time_stamp('s2'); MPI_Send(ahdv,2,TAG,NEWORLD);
	time_stamp('s3'); MPI_Send(dhav,3,TAG,NEWORLD);
    if NSLAVES==4
	time_stamp('s4'); MPI_Send(dhdv,4,TAG,NEWORLD);
    end,end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ----             ---- <-+--    ----               --------
% | 1| ahav ->h -> | 1|   V      | 1| ah sup ->h -> |     1| a sup\
% ----        *2   |  |----      ----          *2   --------       \      r
% ----             ----|  |      ----               --------        \  --------
% | 2| ahdv ->g ->     | 2|      | 2| ah inf ->h -> |     2| a inf\  >-|      |
% ----        *2       ----      ----          *2   --------       \/  |______|
% ----             ---- <-+--    ----               --------       /\  |      |
% | 3| dhav ->h -> | 3|   V      | 3| dh sup ->g -> |     3| d sup/  >-|      |
% ----        *2   |  |----      ----          *2   --------        /  --------
% ----             ----|  |      ----               --------       /
% | 4| dhdv ->g ->     | 4|      | 4| dh inf ->g -> |     4| d inf/
% ----        *2       ----      ----          *2   --------
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%       Each CPU reverses last column filtering (vertical, 2* from idwt2flt)
%       lower/upper half is sent to lower/upper neighbour for addition
%               1 ahav->aha (lower half)\/(upper add) ah upper
%               2 ahdv->ahd (upper half)/\(lower add) ah lower
%               3 dhav->dha (lower half)\/(upper add) dh upper
%               4 dhdv->dhd (upper half)/\(lower add) dh lower
%       Then each CPU reverses 1st row filtering (horizontal) on its half-image
%       which is then sent to master for addition
%               1 ah->a upper---/master upper +\
%               2 ah->a lower-\/                \master full image
%               3 dh->d upper-/\                /
%               4 dh->d lower---\master lower +/
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


    time_stamp('work');
%%%%%%%%%%%%%%%%%%%%%
% Receive Reconstr. %				% Waiting for results
%%%%%%%%%%%%%%%%%%%%%
    img=zeros(liv ,lih);			% Pre-allocation
     ah=zeros(liv2,lih);
     dh=zeros(liv2,lih);			% lazy copy?

if NSLAVES==3					% will I work?
    disp('local computing'),	fflush(stdout);
    dh=WHidwt2Slav(dhdv,g,g, 3,1);		% If so, compute lower dh here
end						% while others are working

    disp('recv results'),	fflush(stdout);	% Receive

   time_stamp('r2'); MPI_Recv(ah,2,TAG,NEWORLD);% ah    lower from 2nd slave
if NSLAVES==4					%    dh lower locally computed
   time_stamp('r4'); MPI_Recv(dh,4,TAG,NEWORLD);%    dh lower from 4th slave
end
   time_stamp('sum'); img(liv2+1:liv ,:)=ah+dh;	% ah+dh lower


   time_stamp('r1'); MPI_Recv(ah,1,TAG,NEWORLD);% ah    upper from 1st slave
   time_stamp('r3'); MPI_Recv(dh,3,TAG,NEWORLD);%    dh upper from 3rd slave
   time_stamp('sum'); img(     1:liv2,:)=ah+dh;	% ah+dh upper

   time_stamp('WHidwt2Mast_fin');

