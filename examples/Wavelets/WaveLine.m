function instr=WaveLine(variant, nslaves, nlevels, ntimes, withlena, inst)
% WAVELINE	Parallel wavelet multires. analysis - image lines, any #CPUs
%		compare to WaveHalf
%
%  instr = WaveLine [(variant[,nslaves[,nlevels[,ntimes[,withlena[,inst]]]]])]
%
%  variant	1..[4]	Choose 1 from up to 4 variants of the algorithm,
%			implemented in files WL[i]dwt2TBag.m
%		  1	Pack/Send row/col# to startup_wlet
%		  2	avoid Pack, sending row/col# to "Line cmd"
%		  3	avoid (data) rcvbuf realloc
%		 [4]	remove row/col check and most verbose errorcheck
%  nslaves	1..[n-1]#Octave slave processes, master never works
%			image is sent/recvd line by line (horz. & vert).
%			nslaves defaults to number of slave hosts in hostlist
%  nlevels	[1]...	desired number of decomposition levels
%			additional levels increase computation time
%  ntimes	[1]...	repeat the analysis this number of times
%			quick-n-dirty way to increase computation time
%  withlena	[0]/1,	show original, transform, reconstructed images
%			drawing of images is included in time measurements
%  inst		[0]..2	instrumentation level
%		[0]	just annotate to produce instrumentation data instr
%		 1	...and verify that line# recvd was sent to that CPU
%		 2	...and print line#-to-CPU# sent/recvd
%
%  See clas_instr for help on Instrumentation data format
%  ScaleLine saves instr to WaveLine_<variant>_<nslaves>.mat
%


  init_stamp(80);		% #annot for 1level, 1times, w/o lena, inst=0
				% 77 entries for all variants, 7slv
				% 1..7 slv:53, 57, 61, 65, 69, 73, 77
  time_stamp('WaveLine_argchk');


	%% Our choice here: global list that can be inherited from ScaleLine.m
	%% or defined here (if calling WaveLine directly)
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
if nargin<6,	inst   =0; end,	flag=flag ||~isnumeric(inst)	   ||...
				inst<0	  || inst>2		   ||...
					     fix(inst)~=inst;
if nargin<5, withlena  =0; end,	flag=flag ||~isnumeric(withlena);
if nargin<4, ntimes    =1; end,	flag=flag || fix(ntimes) ~=ntimes  || ntimes <1;
if nargin<3, nlevels   =1; end,	flag=flag || fix(nlevels)~=nlevels || nlevels<1;
if nargin<2, nslaves   =...
	  length(HOSTS)-1; end,	flag=flag || fix(nslaves)~=nslaves ||...
				 nslaves<1|| nslaves>=length(HOSTS);
if nargin<1, variant   =4; end,	flag=flag || fix(variant)~=variant ||...
				 variant<1|| variant>4;
if flag, error('WaveLine: argchk: see help'), end


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

  global NEWORLD		% NEWORLD defined by Octave_Spawn


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

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% column  1   for approximations
  % Pre-allocate transform cell-array % columns 2-4 for details ahdv,dhav,dhdv
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% column  5   for reconstructions
  TRFM=[];				% destroy previous TRFM if any
  TRFM{1,1}=dblena;			% - there are no details at LEVEL 0
  TRFM{nlevels+1,5}=[];			% pre-allocate (extend) TRFM

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % n-level Analysis/Reconstruction %
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% for LEVEL=1:nlevels,	[TRFM{LEVEL+1,1:4}] =...	% won't work oct-2.1.69
%  WLdwt2TBag(variant,	 TRFM{LEVEL  ,1  },	h,gd, inst);
% end

  for LEVEL=1:nlevels,	[T1, T2, T3, T4  ]=...
   WLdwt2TBag(variant,	 TRFM{LEVEL  ,1  },	h,gd, inst);
			[TRFM{LEVEL+1,1  },...
			 TRFM{LEVEL+1,2  },...
			 TRFM{LEVEL+1,3  },...
			 TRFM{LEVEL+1,4  }]=deal(T1,T2,T3,T4);
  end
					% LEVEL n reconstr == approx.
			 TRFM{LEVEL+1,  5} =...
			 TRFM{LEVEL+1,  1};

  for LEVEL=nlevels:-1:1,TRFM{LEVEL  ,  5} =...
   WLidwt2TBag(variant,	 TRFM{LEVEL+1,  5} ,...
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
% size(TRFM{LEVEL,1}) = 2 * size(TRFM{LEVEL+1,1}) (2* in each dimension)
% So TRFM cannot be an array, must be a cell-array, indexed by resolution level
%
% For each CPU, #row/col submitted is recorded in CPUs(cpu) so that
% the returned result can be easily placed on temp(row,:) or transform(:,col)
% #row/col can also be transmitted & received back for debugging purposes
% comparing returned #row/col with the one stored in CPUs(cpu).
%
% It is not required to record LEVEL/pass:
% Pass 2 cannot start until pass 1 finishes (we need full columns)
% there won't be mixed cols & rows submitted (mixed pass).
% Next LEVEL could start as soon as ahav is computed (halfway #cols)
% anyways, job must be done (remaining #cols for dhav, dhdv)
% and there is no point in having computers running on different LEVELs
% (it would involve threads in master loop???)
%
% Reconstruction always needs full image to proceed with previous LEVEL
%
% To facilitate for loops, we will store  Image in TRFM{      1,1}
% 				     transforms in TRFM{LEVEL+1,2:4}
%				reconstructions in TRFM{LEVEL+1,  5}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

