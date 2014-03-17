function mandelnof
%MANDELNOF	Mandelbrot set program, nested loop version (2D-loop)
%
%	Compare run time with mandelFun

	tic				% start stopwatch

%-------------------------------	% Octave/Matlab issues
if exist('OCTAVE_VERSION'),	soft='OCTAVE'; isOctave=true; isMatlab=false;
else				soft='MATLAB'; isMatlab=true; isOctave=false;
end

if isOctave
oldPSO = page_screen_output;
	 page_screen_output=0;
end
%-------------------------------

  R_MIN =-2.00;	% abscissa from -2.00 to 1.00
  R_MAX = 1.00;
  I_MIN =-1.25;	% ordinate from -1.25 to 1.25
  I_MAX = 1.25;

if isOctave,	X_SIZ =  120;		% x size in pixels (wide)
else,		X_SIZ =  135;		% similar runtimes: 7.82(Oct)/7.44(ML)
end					% y size in pixels (tall)
  Y_SIZ = fix (X_SIZ * (I_MAX-I_MIN) /...
		       (R_MAX-R_MIN));
  scl_r = (R_MAX-R_MIN)/X_SIZ;		% x=0..X_SIZ-1 (scale real) pixel size
  scl_i = (I_MAX-I_MIN)/Y_SIZ;		% y=0..Y_SIZ-1 (scale imag)

		MAX_IT = 50;		% Divergence criteria
		SQR_MOD=  4;

	mandel=zeros(Y_SIZ,X_SIZ);	% image preallocation

%-------------------------------	% Calculate and draw points

	tic				% start stopwatch, alternate point

for y=1:Y_SIZ				% 1D-loop

	ci=(I_MIN + (y-0.5)*scl_i);	% factor out improves: 7.93->7.84

    for x=1:X_SIZ			% 2D-loop

	z=0+0i;
	c = R_MIN + (x-0.5)*scl_r + i*ci;
	k=0;
      while z*z'<SQR_MOD && k<MAX_IT
	z = c + z^2;
	k = k +   1;
      end % while

	mandel(y,x) = k;
    end % for x=0..X_SIZ-1
	% fprintf('%3d/%2d lines\r',y,Y_SIZ);
end	% for y=0..Y_SIZ-1

fprintf('elapsed time = %.4f s\n',toc);	% display time not computed

image(mandel);

if isOctave
	 page_screen_output=oldPSO;
end

% end % function			% this generates warning in MATLAB
