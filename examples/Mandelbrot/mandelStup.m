%MANDELSTUP	Mandelbrot set program -- setup
%
%	Setup code to manage inputs (coords,pixels,max_it) etc
%

% -----------------------------------------------------
% ## Copyright (C) 2006-2007 Javier Fernández Baldomero, Mancia Anguita López
% ##
% ## This program is free software
% -----------------------------------------------------

%-------------------------------
% Inspired in "Parallel Programming" by Barry Wilkinson & Michael Allen
% C code from http://www.cs.uncc.edu/par_prog
%	 from: http://www.cs.uncc.edu/~abw/ITCS3145F05/index.html
% MATLAB code by Lucio Andrade from http://www.mathworks.com/matlabcentral/...
%			fileexchange/loadFile.do?objectId=1329&objectType=file
%-------------------------------

%-------------------------------	% Octave/Matlab issues
if exist('OCTAVE_VERSION'),	soft='Octave'; isOctave=true; isMatlab=false;
else				soft='MATLAB'; isMatlab=true; isOctave=false;
end

if isOctave
oldPSO=	page_screen_output( );
	page_screen_output(0);
end
%-------------------------------	% Code copied from Lucio
    if ~exist('coords','var')		% default coordinates
%	r_min=-2.00;	r_max= 1.00;	% abscissa from -2    to 1
%	i_min=-1.25;	i_max= 1.25;	% ordinate from -1.25 to 1.25
	r_min=-1.78;	r_max= 1.42;	% abscissa from -1.78 to 1.42
	i_min=-1.42;	i_max= 1.78;	% ordinate from -1.42 to 1.78
elseif  numel(coords)==2		% coordinates with 2 complex corners
	r_min=min(real(coords)); r_max=max(real(coords));
	i_min=min(imag(coords)); i_max=max(imag(coords));
elseif  numel(coords)==4		% coordinates with X and Y axes
	r_min=coords(1);r_max=coords(2);% first abscissa min/max
	i_min=coords(3);i_max=coords(4);% then  ordinate min/max
else	error('coords = [-min-min*i,max+max*i] or [rmin rmax, imin imax]')
end
%-------------------------------
    if ~exist('pixels','var')		% default resolution in abscissa
     if isOctave
%	pixels= 300;			% good for Octave / time ok
	pixels= 600;			% good for Octave / time ok
     else
%	pixels=1200;			% good for MATLAB / no swap
	pixels=1200;			% good for MATLAB / no swap
     end
elseif  isnumeric(pixels) & fix(pixels)==pixels & pixels>1
	% good
else	error('pixels = integer no. pixels in abscissa')
end
	x_siz = pixels;			% x size in pixels (wide)
%	y_siz = pixels;			% y size in pixels (tall)
	y_siz = fix(x_siz * (i_max-i_min) /...
			    (r_max-r_min));
	scl_r = (r_max-r_min)/x_siz;	% x=0..x_siz-1 (scale real) pixel size
	scl_i = (i_max-i_min)/y_siz;	% y=0..y_siz-1 (scale imag)
%-------------------------------
    if ~exist('max_it','var')		% max mandelbrot iterations per pixel
     if isOctave
	max_it=   64;			% good for Octave / time ok
     else
	max_it= 1024;			% (4x)^2 image size, 16x iters
     end
elseif  isnumeric(max_it) & fix(max_it)==max_it & max_it>0
	% good
else	error('max_it = integer no. iterations for mandelbrot sequence')
end
%-------------------------------
	    MOD=    2;
	SQR_MOD=MOD^2;	% maximum square modulus -- stop mandelbrot iterations
	% should be larger than any point on [r/i_min..r/i_max]
	% or a fictitious circle will appear as if it was a Mandelbrot-curve

