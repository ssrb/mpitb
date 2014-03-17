function mandelfunc(coords,pixels,max_it)
%MANDELFUNC	Mandelbrot set program, funct-call sequential version (1D-loop)
%
% segregates the cal_pixel routine for a whole line out of main loop code
% compare timing to original version without function calls (2D-loop)
%	to measure how nested routines hurt Octave/MATLAB performance
%	very little, but opposite signs: Octave improves, MATLAB degrades
%		Octave: 8.78 -> 8.08
%		MATLAB: 7.44 -> 7.49
%
%  Example:
%	$ octave -q --eval \
%	"global FIGURES, FIGURES=true; mandelfunc([-2-2i, 2+2i])"
%

% -----------------------------------------------------
% ## Copyright (C) 2006-2007 Javier Fernández Baldomero, Mancia Anguita López
% ##
% ## This program is free software
% -----------------------------------------------------

%-------------------------------
% Inspired in "Parallel Programming" 2nd Ed. by Barry Wilkinson & Michael Allen
% pp.86--88 sequential code, p.89 parallel, pp.90--93 load-balanced (by lines)
% taskbag/workpool algorithm by lines at p.91
%
% C code from http://www.cs.uncc.edu/par_prog
%	 from: http://www.cs.uncc.edu/~abw/ITCS3145F05/index.html
% MATLAB code by Lucio Andrade from http://www.mathworks.com/matlabcentral/...
%			fileexchange/loadFile.do?objectId=1329&objectType=file
%-------------------------------

	tic				% start stopwatch

	global y_siz r_min i_min scl_r scl_i MAX_IT MOD SQR_MOD	% before setup
	mandelStup			% setup, which defines

%	[soft, isOctave, isMatlab] = 'Octave'/'MATLAB', true/false;
%	[r_min,r_max, i_min,i_max] = ...	image limits in complex plane
%	[x_siz,y_siz, scl_r,scl_i] = ...	#pixels and pixel size
%	[max_it, MOD, SQR_MOD] = 64/1024, 2, 4;	stop criteria

%	tic				% start stopwatch, alternate point
	MAX_IT=max_it;	% error: can't make function parameter `max_it' global

%-------------------------------
  mandel=zeros(y_siz,x_siz);		% image preallocation

  for x=1:x_siz				% do all cols (col-major) (1D-loop)
	mandel(:,x) = CalcLine(x,isOctave);
	% fprintf('%3d/%2d lines\r',x,x_siz);
  end	% for x=0..x_siz-1
%-------------------------------

  fprintf('elapsed time = %.4f s\n',toc);	% display time not computed


global FIGURES, if FIGURES		% set global FIGURES=true to see it
  SCALECLR=ceil(MAX_IT/10);		% remove the -nodisplay matlab switch
  mandel=   mod(mandel*SCALECLR,MAX_IT);% to see this image
  mandel=flipud(mandel);		% flipud to get y axis upward
        imagesc(mandel);		% just for verifying correct operation
% if isMatlab, pause(5), end		% time to see drawing, deleted w/quit
		pause(1)		% for Oct-2.9.10 to let fig survive
end		% FIGURES


if isOctave
	page_screen_output(oldPSO);
end


% end % function			% this generates warning in MATLAB

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function line=CalcLine(x,isOctave)
global y_siz r_min i_min scl_r scl_i MAX_IT MOD SQR_MOD

%	line=zeros(y_siz,1);
	cr=r_min + scl_r*(x-0.5);

if isOctave				% Calculate points
%-------------------------------	% a-la-vectorized
	c   =cr + i*...			% complex plane itself / line version
	  (i_min + scl_i*[0.5:1:y_siz]');
	z   =            c  ;		% z-seq for each point in complex plane
	mset=1:prod(size(c));		% idxs candidate (non-divergent) points

  if any(size(c)~=[y_siz,1])		% just to be safe
	error('internal maths error (meshgrid)')
  end

if false
	line=zeros(size(c));
  for counter=1:MAX_IT
	mset(abs(z(mset))>=MOD)=[];	% trim off diverging points
	z(mset)=z(mset).^2+c(mset);	% iterate candidates: z = z^2 + c;
	line(mset)=counter;		% color on remaining candidates
  end
else
	line=ones(size(c))*MAX_IT;	% assume all are final candidates
  for counter=0:MAX_IT-1		% and color failing ones as they fail
	toff= abs(z(mset))>=MOD;	% these fail now (even when counter=0)
	  line(mset(toff))=counter;	% so color them now, before
	       mset(toff)=[];		% diverging points, trim off
	z(mset) = z(mset).^2+c(mset);	% iterate candidates: z = z^2 + c;
  end
end

else    % isMatlab
%-------------------------------	% a-la-JIT
	line=zeros(y_siz,1);
  for y=1:y_siz				% do all columns
	zr=0;%cr=r_min + scl_r*(x-0.5);	% each point in complex plane
	zi=0; ci=i_min + scl_i*(y-0.5);	% compute real/imag parts
	k =0;				% iteration/color counter

    while zr*zr+zi*zi<SQR_MOD & k<MAX_IT
	t = cr+ zr*zr - zi*zi;		% this is fastest in MATLAB
	zi= ci+       2*zr*zi;		% (faster than abs(),z*z', etc)
	zr= t ;				% due to JIT compiler
	k = k +   1;
    end % while

	line(y)=k;
  end % for y=0..y_siz-1

end	% if isOctave
%-------------------------------

% end % function			% this generates warning in MATLAB
