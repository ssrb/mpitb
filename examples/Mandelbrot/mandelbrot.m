function mandelbrot(coords,pixels,max_it)
%MANDELBROT	Mandelbrot set program, normal sequential version (2D-loop)
%
%  mandelbrot [ (coords [ ,pixels [ ,max_it ]] ) ]
%
%  coords: [-1.78-1.42i, 1.42+1.78i]	two corners of space to compute, or
%	   [-1.78,1.42 ,-1.42,1.78 ]	four values, 2 on real/2 on imag axes
%  pixels:	[ 600]			default resolution in abscissa
%		[1200]		(MATLAB default much larger for same runtime)
%  max_it:	[  64]			default#iterations divergence decision
%		[1024]		(MATLAB default much larger for same runtime)
%  FIGURES	[undef]			if defined global & true, draw the set
%
%  Examples:
%	octave:1> global FIGURES, FIGURES=true;
%	octave:2> mandelbrot([-2-2i, 2+2i])	% from octave prompt
%
%	$ octave -q --eval mandelbrot		# from command line
%	$ octave -q --eval \
%	"global FIGURES, FIGURES=true; mandelbrot([-2-2i, 2+2i])"
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

%%%%%%%%%%%%%%%%%%%%%%%%
% POSSIBLE IMPROVEMENT %
%%%%%%%%%%%%%%%%%%%%%%%%
% double operation not really required for mandel image, it's just iter counts
% but Lucio and others like to use imagesc(log(mandel)) to improve visibility
% and Matlab won't let us increment an uint16 variable (perhaps assign...)
% Octave does indeed allow increment, but subjacent datatype is really real,
% so there may be no computation time improvement -- study it
% Even though, perhaps communication time improvement pays for the costs of
% translate double->uint16->xmit->recv->uint16->double -- study that too

	tic				% start stopwatch

	mandelStup			% setup, which defines

%	[soft, isOctave, isMatlab] = 'Octave'/'MATLAB', true/false;
%	[r_min,r_max, i_min,i_max] = ...	image limits in complex plane
%	[x_siz,y_siz, scl_r,scl_i] = ...	#pixels and pixel size
%	[max_it, MOD, SQR_MOD] = 64/1024, 2, 4;	stop criteria

	FLAWED=true;			% enable original flawed version
	tic				% start stopwatch, alternate point

if isOctave				% Calculate and draw points
%-------------------------------	% a-la-vectorized
 [x,y]=meshgrid(r_min + scl_r*[0.5:1:x_siz],...
		i_min + scl_i*[0.5:1:y_siz]);
       c=     (x + i*y);		% complex plane itself
       z=           c  ;		% z-seq for each point in complex plane
% mandel=zeros(size(c));		% image to display (iteration counts)
   mset=1:prod(size(c));		% idxs candidate (non-divergent) points

  if any(size(c)~=[y_siz,x_siz])	% just to be safe
	error('internal maths error (meshgrid)')
  end

if FLAWED				% this is slower, more assignments
  z     =zeros(size(c));		% flaw in the original version
  mandel=zeros(size(c));		% image to display (iteration counts)
  for counter=1:max_it			% initially z=c, there may already be...
	mset(abs(z(mset))>=MOD)=[];	% diverging points, trim off
	    mandel(mset)=counter;	% inc.color on remaining candidates
	z(mset)= z(mset).^2+c(mset);	% iterate candidates: z = z^2 + c;
%	fprintf('%3d/%2d iters\r',counter,max_it);
  end
else					% this is faster, less assignments
  mandel= ones(size(c))*max_it;		% assume all are final candidates
  for counter=0:max_it-1		% and color failing ones as they fail
	toff= abs(z(mset))>=MOD;	% these fail now (even when counter=0)
	mandel(mset(toff))=counter;	% so color them now, before
	       mset(toff)=[];		% diverging points, trim off
	z(mset) = z(mset).^2+c(mset);	% iterate candidates: z = z^2 + c;
%	fprintf('%3d/%2d iters\r',counter,max_it);
  end
end

else	% isMatlab
%-------------------------------	% a-la-JIT
  mandel=zeros(y_siz,x_siz);	% image preallocation (same dims as w/meshgrid)

  for x=1:x_siz				% do all cols (col-major)
    for y=1:y_siz			% do all rows (vertical sweep)
	zr=0; cr=r_min + scl_r*(x-0.5);	% each point in complex plane
	zi=0; ci=i_min + scl_i*(y-0.5);	% compute real/imag parts
	k =0;				% iteration/color counter
 
      while zr*zr+zi*zi<SQR_MOD & k<max_it
 	t = cr+ zr*zr - zi*zi;		% this is fastest in MATLAB
 	zi= ci+       2*zr*zi;		% (faster than abs(),z*z', etc)
 	zr= t ;				% due to JIT compiler
 	k = k +   1;
      end % while

	mandel(y,x) = k;
    end	% for y=0..y_siz-1
	% fprintf('%3d/%2d lines\r',x,x_siz);
  end	% for x=0..x_siz-1

end	% if isOctave
%-------------------------------

  fprintf('elapsed time = %.4f s\n',toc);	% display-time not computed


global FIGURES, if FIGURES		% set global FIGURES=true to see it
	tic				% huge time for large npixels (swap)
%-------------------------------	% log more complex than mandel itself
   SCALECLR=ceil(max_it/10);		% 2 ways to better display border:
if SCALECLR				% use rotating color instead of log
    mandel=mod(mandel*SCALECLR, max_it);% cool: center black when B/W
else
  if isOctave
    mandel=    log(mandel+1);		%     +1 to avoid log(0)
  else					%    log to get more detailed border
    mandel=reallog(mandel+1);		% reallog for better performance
  end
end		% FIGURES
%-------------------------------
if isOctave				% colormap now opens a window 2.9.10
    colormap(jet);			% default colormap in MATLAB
    mandel= flipud(mandel);		% flipud to get y axis upward
end

  imagesc([r_min r_max],...
	  [i_min i_max],...		% axis stuff won't hurt Octave
 	   mandel);			% imagesc to better scale colormap
	axis('image');			% axis stuff won't hurt Octave

if isMatlab				% uicontrol code from Lucio
	set(gca,'YDir','normal');	% trick to avoid flipud(mandel)
	uicontrol('Parent',gcf,...
		  'BusyAction','cancel', 'Interruptible','off',...
		  'String','redo',	 'Tag','redoitbutton',...
		  'Units','points',	 'BackgroundColor',[0.7 0.7 0.7], ...
		  'Position',[0 0 40 20],'Style','pushbutton',...
	 'Callback',...
	['mandelbrot(axis,' num2str(pixels) ',' num2str(max_it) ');']);
	zoom
	set(gca,'XGrid','on')
	set(gca,'YGrid','on')
else
	sleep(1)	% pause(1)	% for Oct-2.9.10 to let fig survive
end
%-------------------------------

  fprintf('display time = %.3f s\n',toc);
end	% if false/true


if isOctave
	page_screen_output(oldPSO);
end


% end % function			% this generates warning in MATLAB
