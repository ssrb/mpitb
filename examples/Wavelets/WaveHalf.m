function instr=WaveHalf(variant, nslaves, nlevels, ntimes, withlena)
% WAVEHALF	Parallel wavelet multires. analysis - image halves, 4 CPUs
%		4 computers (0,1,2,3) in 2 groups of 2 computers (0,1),(2,3)
%
%	instr = WaveHalf [ ( variant,[nslaves[,nlevels[,ntimes[,withlena]]]] ) ]
%
%  variant	1..[2]	Choose among the 2 variants of the algorithm,
%			implemented in files WH[i]dwt2[Mast/Slav].m
%		  1	Pack/Send all vars using NumCmds_Send
%		 [2]	Send img directly, don't pack/NumCmds_Send
%  nslaves	[3]/4,	#Octave slave processes, 3 => master also works
%			image is divided in two horz. halves
%  nlevels	[1]...	desired number of decomposition levels
%			additional levels increase computation time
%  ntimes	[1],	repeat the analysis this number of times
%			quick-n-dirty way to increase computation time
%  withlena	[0]/1,	show original, transform, reconstructed images
%			drawing of images is included in time measurements
%
%  See clas_instr for help on Instrumentation data format
%  ScaleHalf saves instr to WaveHalf_<variant>_<nslaves>.mat
%

% Requires:
% lena_funet.tif	Lena graylevel image, from ftp.funet.fi
% h.mat         Approximation Wavelet Filter (1D)
% g.mat         Detail        Wavelet Filter (1D)
%
% WH[i]dwt2Mast	WaveHalf 2D [i]DWT, Master process
% WH[i]dwt2Slav	WaveHalf 2D [i]DWT, Slave  process
%     dwtflt	1-Filter dwt
%    idwtflt	1-Filter inverse dwt
%
% math		Transform "equalization" (to make it visible)
%


  init_stamp(60);		% all these #annot for 1level, 1times, w/o lena
	% 52/28 entries for variant#1, 3/4slv
	% 58/36 entries for variant#2, 3/4slv
  time_stamp('WaveHalf_argchk');

	%% Our choice here: local list directly passed to LAM_Init
	%% modify at your will, but make sure your master node is last in list
	%% and at least you list other 3/4 slave nodes first

  hosts={'hypercat05','hypercat04','hypercat03','hypercat02','hypercat01'};

  global DEBUG; DEBUG=true;		% let xterms show during development
  global DISPLAY_REQUIRED;		% set to true if your setup requires it
         DISPLAY_REQUIRED=true;		% see Octave_Spawn.m code

  DEBUG=false;

%%%%%%%%%%
% ArgChk %
%%%%%%%%%%
				flag=0;
if nargin<5, withlena  =0; end,	flag=flag ||~isnumeric(withlena);
if nargin<4, ntimes    =1; end,	flag=flag || fix(ntimes )~=ntimes  || ntimes <1;
if nargin<3, nlevels   =1; end, flag=flag || fix(nlevels)~=nlevels || nlevels<1;
if nargin<2, nslaves   =3; end,	flag=flag || fix(nslaves)~=nslaves ||...
							 nslaves<3 || nslaves>4;
if nargin<1, variant   =2; end, flag=flag || fix(variant)~=variant ||...
							 variant<1 || variant>2;

if flag, error('WaveHalf: argchk: see help'), end


  time_stamp('ldimg');
%%%%%%%%%%%%%%%%%%%
% Image & Filters %
%%%%%%%%%%%%%%%%%%%
  lena=loadimage('lena.mat');		% Approximation at LEVEL "0"
  dblena=double(lena);
  load h, load g			% Scale-Wavelet filters
  gd=[  g(2:length(g));0];		% Detail filter for [d]ecomposition
  gr=[0;g(1:length(g)-1)];		% Detail filter for [r]econstruction


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% MPITB: Start LAM / Spawn Octaves % NumCmds Protocol setup
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  time_stamp('lboot');
  LAM_Init(nslaves,'t',hosts)	% LAM_Init(#slaves, desired-rpi, host-list)

  time_stamp('spawn');
  fprintf('about to spawn %d slave Octaves...',nslaves); fflush(stdout);
  Octave_Spawn;			% Octave_Spawn(#slaves) defaults to NSLAVES
  fprintf('done\n'); fflush(stdout);	% taken from LAM_Init(nslaves)

  global NEWORLD		% NEWORLD defined by Octave_Spawn


  time_stamp('ncmds');
  ncmd = 2*nlevels*ntimes;		% n-level-depth analysis/reconstruction
  qflg = ~DEBUG;			% quit slave Octaves if no debugging
  NumCmds_Init(ncmd,qflg);		% default tag 777
					% default debug == global DEBUG
	%% NUMCMDS-controlled loop
	%% lengthy explanation at End-Of-File


%%%%%%%%%%%%%%%
% NTIMES LOOP % In theory we would analyze a different image each time
%%%%%%%%%%%%%%%
for idx=1:ntimes
  time_stamp(['loop' int2str(idx)]);

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% column  1   for approximations
  % Pre-allocate transforms cell % columns 2-4 for details ahdv,dhav,dhdv
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% column  5   for reconstructions
  TRFM=[];				% destroy previous TRFM if any
  TRFM{1,1}=dblena;			% - there are no details at LEVEL 0
  TRFM{nlevels+1,5}=[];			% pre-allocate (extend) TRFM

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % n-level Analysis/Reconstruction %
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% for LEVEL=1:nlevels,	[TRFM{LEVEL+1,1:4}]=...		% won't work oct-2.1.69
%  WHdwt2Mast(variant,	 TRFM{LEVEL  ,1  },	h,gd);
% end

  for LEVEL=1:nlevels,	[T1, T2, T3, T4  ]=...
   WHdwt2Mast(variant,	 TRFM{LEVEL  ,1  },	h,gd);
			[TRFM{LEVEL+1,1  },...
			 TRFM{LEVEL+1,2  },...
			 TRFM{LEVEL+1,3  },...
			 TRFM{LEVEL+1,4  }]=deal(T1,T2,T3,T4);
  end
					% Level-n reconstr. == approx.
			 TRFM{LEVEL+1,  5} =...
			 TRFM{LEVEL+1,  1};

  for LEVEL=nlevels:-1:1,TRFM{LEVEL  ,  5} =...
   WHidwt2Mast(variant,	 TRFM{LEVEL+1,  5} ,...
			 TRFM{LEVEL+1,  2} ,...
			 TRFM{LEVEL+1,  3} ,...
			 TRFM{LEVEL+1,  4} ,	h,gr);
  end

end

    time_stamp('clean');

    MPI_Finalize;
    LAM_Clean;

    clear global DEBUG DISPLAY_REQUIRED NEWORLD
    clear global HOSTS DISPLAY NSLAVES NOCTAVES RPI TAG


%%%%%%%%%%%%%%
% show image %
%%%%%%%%%%%%%%
if withlena
    time_stamp('graph');

    transform=math (TRFM(2:end,1:4));
    reconstr =uint8(TRFM{1    ,  5}); TS='truesize';

    figure; imshow(lena,	TS),	title('Original')
    figure; imshow(transform,	TS),	title('Transform')
    figure; imshow(reconstr,	TS),	title('Reconstruction')
end

    last_stamp;
instr=clas_instr;


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

