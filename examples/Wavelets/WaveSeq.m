function instr=WaveSeq(nlevels,ntimes,withlena)
% WAVESEQ	Sequential wavelet multires. analysis (Lena image)
%
%	instr = WaveSeq [ ( nlevels [,ntimes [,withlena]] ) ]
%
%  nlevels	[1]...	desired number of decomposition levels
%			additional levels increase computation time
%  ntimes 	[1]...	repeat the analysis this number of times
%			quick-n-dirty way to increase computation time
%  withlena	[0]/1,	show original, transform, reconstructed images
%			drawing of images is included in time measurements
%
%  See clas_instr for help on Instrumentation data format
%  ScaleHalf/Line/Strip save instr to WaveSeq.mat
%

% Requires:
% lena_funet.tif	Lena greylevel image
% h.mat         Approximation Wavelet Filter (1D)
% g.mat         Detail        Wavelet Filter (1D)
%
%  dwt2		2D Wavelet analysis
%  dwt		1D Wavelet analysis
% idwt2		2D Wavelet reconstruction
% idwt		1D Wavelet reconstruction
% math		Transform "equalization" (to make it visible)
%


init_stamp(40);			% 36 entries for 1 loop, 37 w/lena

time_stamp('WaveSeq_argchk');
%%%%%%%%%%
% ArgChk %
%%%%%%%%%%
				flag=0;
if nargin<3, withlena  =0; end,	flag=flag |~isnumeric(withlena);
if nargin<2, nlevels   =1; end,	flag=flag | fix(nlevels)~=nlevels | nlevels<1;
if nargin<1, ntimes    =1; end,	flag=flag | fix(ntimes )~=ntimes  | ntimes <1;
if flag, error('WaveSeq: argchk: see help'), end


time_stamp('ldimg');
%%%%%%%%%%%%%%%%%%%
% Image & Filters %
%%%%%%%%%%%%%%%%%%%
TRFM={};				% Cell to store all approx/details
lena=loadimage('lena.mat');		% Approximation at LEVEL "0"/no details
[TRFM(1,1:5)]={double(lena),[],[],[],[]};	% a,dh,dv,dd [,reconstr]
load h, load g				% Scale-Wavelet filters
gd=[  g(2:length(g));0];		% Detail filter for [d]ecomposition
gr=[0;g(1:length(g)-1)];		% Detail filter for [r]econstruction


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% NTIMES LOOP % n-level Analysis/Reconstruction %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for idx=1:ntimes
  time_stamp(['loop' int2str(idx)]);

% for LEVEL=1:nlevels,	[TRFM{LEVEL+1,1:4}]=...		% won't work oct-2.1.69
%		 dwt2(	 TRFM{LEVEL  ,1  },	h,gd);
% end
  disp('analisys'),		fflush(stdout);
  for LEVEL=1:nlevels,	[T1, T2, T3, T4  ]=...
		 dwt2(	 TRFM{LEVEL  ,1  },	h,gd);
			[TRFM{LEVEL+1,1  },...
			 TRFM{LEVEL+1,2  },...
			 TRFM{LEVEL+1,3  },...
			 TRFM{LEVEL+1,4  }]=deal(T1,T2,T3,T4);
  end
					% Level-n reconstr. == approx.
			 TRFM{LEVEL+1,  5} =...
			 TRFM{LEVEL+1,  1};

  disp('reconstruction'),	fflush(stdout);
  for LEVEL=nlevels:-1:1,TRFM{LEVEL  ,  5} =...
		idwt2(	 TRFM{LEVEL+1,  5} ,...
			 TRFM{LEVEL+1,  2} ,...
			 TRFM{LEVEL+1,  3} ,...
			 TRFM{LEVEL+1,  4} ,	h,gr);
  end

end


%%%%%%%%%%%%%%
% show image %
%%%%%%%%%%%%%%
if withlena
    time_stamp('graph');

    transform=math (TRFM(2:end,1:4));
    reconstr =uint8(TRFM{1    ,  5}); TS='truesize';

    figure; imshow(lena,	TS),	title('Original'),
    figure; imshow(transform,	TS),	title('Transform'),
    figure; imshow(reconstr,	TS),	title('Reconstruction'),
end


    last_stamp;
instr=clas_instr;


