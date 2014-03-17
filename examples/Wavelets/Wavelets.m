% Wavelets Demo
%
% ------------------------------
% 1D Wavelet Transform Functions
% ------------------------------
% DWT		Discrete Wavelet Transform (aproximation and detail)
% IDWT		Inverse Discrete Wavelet Transform (reconstr from appr/detail)
%
% ------------------------------
% 2D Wavelet Transform Functions
% ------------------------------
% DWT2		2-D Discrete Wavelet Transform (appr. & details horz/vert/diag)
% IDWT2		2-D Inverse Discrete Wavelet Transform (from appr. & 3 details)
%
% -----------------------
% Utility Functions/Files
% -----------------------
% MATH		Utility function to normalize 2D Wavelet Transform
% G.MAT		Wavelet (highpass) Filter, impulse response
% H.MAT		Scale   ( lowpass) Filter, impulse response
% LENA_FUNET.TIF	Lena image (512x512 greylevel) from ftp.funet.fi
%
% ---------------------------------------------
% Simplest Sequential and Parallel applications
% ---------------------------------------------
% WAVESEQ	Sequential wavelet multires. analysis (Lena image)
% WAVEHALF	Parallel wavelet multires. analysis - image halves, 4 CPUs
%
% --------------------------
% WaveHalf support functions
% --------------------------
% WHDWT2MAST	WaveHalf 2D  DWT, Master process (image halves)
% WHDWT2SLAV	WaveHalf 2D  DWT, Slave  process (image halves)
% DWTFLT	Filter & downsample for Wavelet transform (approx. or detail)
%
% WHIDWT2MAST	WaveHalf IDWT 2D, Master process (image halves)
% WHIDWT2SLAV	WaveHalf IDWT 2D, Slave  process (image halves)
% IDWTFLT	Upsample & filter for Wavelet transform (f/approx. or detail)
%
% -------------------------------------
% More elaborated Parallel applications
% -------------------------------------
% WAVELINE	Parallel wavelet multires. analysis - image lines, any #CPUs
% WAVESTRIP	Parallel wavelet multires. analysis - image stripes, any #CPUs
%
% --------------------------
% WaveLine support functions
% --------------------------
% WLDWT2TBAG		WaveLine  DWT 2D, TaskBag process (line-by-line)
% WLIDWT2TBAG		WaveLine IDWT 2D, TaskBag process (line-by-line)
%
% ---------------------------
% WaveStrip support functions
% ---------------------------
% WSDWT2TBAG		WaveStrip  DWT 2D, TaskBag process (stripe-by-stripe)
% WSIDWT2TBAG		WaveStrip IDWT 2D, TaskBag process (stripe-by-stripe)
%
% STRIPE		Compute stripe width
%
% -----------------
% Scalability tests
% -----------------
% SCALEHALF	Scalability study for WaveHalf
% SCALELINE	Scalability study for WaveLine
% SCALESTRIP	Scalability study for WaveStrip
%
% WAVESEQ.MAT	Instrumentation files
% WAVEHALF_<variant>_<nslaves>.MAT
% WAVELINE_<variant>_<nslaves>.MAT
% WAVESTRIP_<variant>_<nslaves>.MAT
%
% -------------
% Startup Files
% -------------
% STARTUP_WLET		PATH/environ script for MPITB -- NumCmds Protocol
%
