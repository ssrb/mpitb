function instr=WaveStrip(variant,nslaves, nlevels, ntimes, withlena, inst)
% WAVESTRIP	Parallel wavelet multires. analysis - image stripes, any #CPUs
%		compare to WaveHalf/WaveLine
%
%  instr = WaveStrip [(variant[,nslaves[,nlevels[,ntimes[,withlena[,inst]]]]])]
%
%  variant	1..[2]	Choose among the 2 variants of the algorithm,
%			implemented in files WS[i]dwt2TBag.m
%		  1	send stripelen later, copy stripes on separate vars
%		 [2]	send stripelen w/cmd
%  nslaves	1..[n-1]#Octave slave processes, master never works
%			image is divided in nslaves stripes horz. & vert.
%			nslaves defaults to number of slave hosts in hostlist
%  nlevels	[1]...	desired number of decomposition levels
%			additional levels increase computation time
%  ntimes	[1]...	repeat analysis this number of times
%			quick-n-dirty way to increase computation time
%  withlena	[0]/1,	show original, transform, reconstructed images
%			drawing of images is included in time measurements
%  inst		[0]..1	instrumentation level
%		[0]	just annotate to produce instrumentation data instr
%		 1	...and time each CPU task (stripe)
%
%  See clas_instr for help on Instrumentation data format
%  ScaleStrip saves instr to WaveStrip_<variant>_<nslaves>.mat
%


  init_stamp(120);		% #annot for 1level, 1times, w/o lena, inst=0
				% 1-7slv: 45,57,69,81,93,105,117 entries
  time_stamp('WaveStrip_argchk');


	%% Our choice here: global list that can be inherited from ScaleStrip.m
        %% or defined here (if calling WaveStrip directly)
	%% modify at your will, but make sure your master node is last in list
	%% if you don't specify nslaves, all them will be used

  global HOSTS, if isempty(HOSTS)
  HOSTS={ 'hypercat08','hypercat07','hypercat06','hypercat05',...
	  'hypercat04','hypercat03','hypercat02','hypercat01'};
  end

  global DEBUG; DEBUG=true;		% let xterms show during development
  global DISPLAY_REQUIRED;		% set to true if your setup requires it
         DISPLAY_REQUIRED=true;		% see Octave_Spawn.m code

  DEBUG=false;

%%%%%%%%%%
% ArgChk %
%%%%%%%%%%
				flag=0;
if nargin<6,	inst   =0; end,	flag=flag ||~isnumeric(inst)	||...
				inst<0    || inst>1             ||...
                                             fix(inst)~=inst;
if nargin<5, withlena  =0; end,	flag=flag ||~isnumeric(withlena);
if nargin<4, ntimes    =1; end,	flag=flag || fix(ntimes )~=ntimes  || ntimes <1;
if nargin<3, nlevels   =1; end,	flag=flag || fix(nlevels)~=nlevels || nlevels<1;
if nargin<2, nslaves   =...
	  length(HOSTS)-1; end, flag=flag || fix(nslaves)~=nslaves ||...
				 nslaves<1|| nslaves>=length(HOSTS);
if nargin<1, variant   =2; end, flag=flag || fix(variant)~=variant ||...
				 variant<1|| variant>2;
if flag, error('WaveStrip: argchk: see help'), end


  time_stamp('ldimg');
%%%%%%%%%%%%%%%%%%%
% Image & Filters %
%%%%%%%%%%%%%%%%%%%
  lena=loadimage('lena.mat');		% Approximation at LEVEL "0"
  dblena=double(lena);
  load h, load g			% Scale-Wavelet filters
  gd=[  g(2:length(g));0];		% Detail filter for [d]ecomposition
  gr=[0;g(1:length(g)-1)];		% Detail filter for [r]econstruction


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  MPITB: Start LAM / Spawn Octaves % NumCmds Protocol setup
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  time_stamp('lboot');
  LAM_Init(nslaves,'t')		% LAM_Init(#slaves, desired-rpi, host-list)

  time_stamp('spawn');
  fprintf('about to spawn %d slave Octaves...',nslaves); fflush(stdout);
  Octave_Spawn;			% Octave_Spawn(#slaves) defaults to NSLAVES
  fprintf('done\n'); fflush(stdout);

  global NEWORLD		% NEWORLD/NOCTAVES defined by Octave_Spawn


  time_stamp('ncmds');
  qflg = ~DEBUG;
  NumCmds_Init(0,qflg);	% Defaults to (ncmd=0, quit=false, tag=777, glbl DEBUG)

	%% Command-controlled loop
	%% lengthy explanation at End-Of-File


%%%%%%%%%%%%%%%
% NTIMES LOOP % In theory we would analyze a different image each time
%%%%%%%%%%%%%%%
for idx=1:ntimes
  time_stamp(['loop' int2str(idx)]);

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % Pre-allocate transform cell-array %
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  TRFM=[];				% destroy previous TRFM if any
  TRFM{1,1}=dblena;			% - there are no details at LEVEL 0
  TRFM{nlevels+1,5}=[];			% pre-allocate (extend) TRFM

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % n-level Analysis/Reconstruction %
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% for LEVEL=1:nlevels,	[TRFM{LEVEL+1,1:4}] =...	% won't work oct-2.1.69
%  WSdwt2TBag(variant,	 TRFM{LEVEL  ,1  },	h,gd, inst);
% end

  for LEVEL=1:nlevels,	[T1, T2, T3, T4  ]=...
   WSdwt2TBag(variant,	 TRFM{LEVEL  ,1  },	h,gd, inst);
			[TRFM{LEVEL+1,1  },...
			 TRFM{LEVEL+1,2  },...
			 TRFM{LEVEL+1,3  },...
			 TRFM{LEVEL+1,4  }]=deal(T1,T2,T3,T4);
  end
					% LEVEL n reconstr == approx.
			 TRFM{LEVEL+1,  5} =...
			 TRFM{LEVEL+1,  1};

  for LEVEL=nlevels:-1:1,TRFM{LEVEL  ,  5} =...
   WSidwt2TBag(variant,	 TRFM{LEVEL+1,  5} ,...
			 TRFM{LEVEL+1,  2} ,...
			 TRFM{LEVEL+1,  3} ,...
			 TRFM{LEVEL+1,  4} ,	h,gr, inst);
  end
 
end

    time_stamp('clean');

    NumCmds_Send({'cmd'},{'break'});	% send break cmd to orderly quit slaves
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


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Transform at a given resolution is kept in an array:
% [ahav,dhav;	This is not like the usual layout ->	[ahav,ahdv;
%  ahdv,dhdv]	But is much more comfortable for us	 dhav,dhdv]
%		so we can return approx & detail concatenated in a row/col
%
% Each CPU has both filters h=scale/g=wavelet (low/highpass)
% In a first pass (horz.), image is filtered in rows with both h/g
% results are decimated & concatenated <org.row> -> <approx><detail>
% Then the second pass (vert) works on columns
%
%       LEVEL 0        LEVEL 1 pass 1 (horz)                LEVEL 1 pass 2 (v)
%
%     #1 <- h ->       #1 <h/2><h/2>        #1       #N
%          ...               ...             ^   .   ^
%     #N <- h ->       #N <h/2><h/2>         V   .   V
% From TRFM{LEVEL,1}                                       TRFM{LEVEL+1,1}
%    |---------|        |----|----|         |----|----|      |----|----|
%    |         |	|    |    |         |    |    |      |ahav|dhav|
%    |   img   |   ->   | ah | dh |         | ah | dh |   -> |----|----|
%    |         |        |    |    |         |    |    |      |ahdv|dhdv|
%    |---------|        |----|----|         |----|----|      |----|----|
%
% Transforms in each resolution are 1/4 size of previous resolution
% size(TRFM{LEVEL,2}) = 2 * size(TRFM{LEVEL+1,2}) (2* in each dimension)
% So TRFM cannot be an array, must be a cell-array, indexed by resolution level
% We can remove TRFM{:,1} later if we wanted.
%
% In the horizontal pass, horizontal stripes are sent/processed/recvd
% image is divided in as many stripes as computers
% if not even division, user can choose which stripes are 1 row taller:
% either first or last <mod(liv,N)> stripes
% Stripes are submitted in an ordered for loop, and received in the same order
% perhaps we should allow for disordered receives
%
% Pass 2 cannot start until pass 1 finishes (we need full columns)
% Next LEVEL could start as soon as ahav is computed (halfway #cols)
% anyways, job must be done (remaining #cols for dhav, dhdv)
% and there is no point in having computers running on different LEVELs
% (it would involve threads in master loop???)
%
% Reconstruction always needs full image to proceed with previous LEVEL
%
% To facilitate for loops, we will store  Image in TRFM{      1,1}
% 				     transforms in TRFM{LEVEL+1,1}
%				reconstructions in TRFM{LEVEL+1,2}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

