function mandelrow(nprc,coords,pixels,max_it)
%MANDELROW	Mandelbrot set program, nested function versions
%
% segregates the cal_pixel routine for a whole line out of main loop code
% compare timing to original version without function calls (2D-loop)
%	to measure how nested routines hurt Octave/MATLAB performance
%	very little:	Octave: 8.78 -> 8.08	-- improves!!!
%			MATLAB: 7.44 -> 7.49

	tic				% start stopwatch

        global x_siz r_min i_min scl_r scl_i MAX_IT MOD SQR_MOD

%*****************************************************************************
%-------------------------------	% Octave/Matlab issues
if exist('OCTAVE_VERSION'),	soft='OCTAVE'; isOctave=true; isMatlab=false;
else				soft='MATLAB'; isMatlab=true; isOctave=false;
end

if isOctave
oldPSO = page_screen_output;
	 page_screen_output=0;
end
%-------------------------------        % Code copied from Lucio
    if ~exist('coords','var')           % default coordinates
%       r_min=-2.00;    r_max= 1.00;    % abscissa from -2    to 1
%       i_min=-1.25;    i_max= 1.25;    % ordinate from -1.25 to 1.25
        r_min=-1.78;    r_max= 1.42;    % abscissa from -1.78 to 1.42
        i_min=-1.42;    i_max= 1.78;    % ordinate from -1.42 to 1.78
elseif  numel(coords)==2                % coordinates with 2 complex corners
        r_min=min(real(coords)); r_max=max(real(coords));
        i_min=min(imag(coords)); i_max=max(imag(coords));
elseif  numel(coords)==4                % coordinates with X and Y axes
        r_min=coords(1);r_max=coords(2);% first abscissa min/max
        i_min=coords(3);i_max=coords(4);% then  ordinate min/max
else    error('coords = [-min-min*i,max+max*i] or [rmin rmax, imin imax]')
end
%-------------------------------
    if ~exist('pixels','var')           % default resolution in abscissa
     if isOctave
%       pixels= 300;                    % good for Octave / time ok
        pixels= 600;                    % good for Octave / time ok
     else
%       pixels=1200;                    % good for MATLAB / no swap
        pixels=1200;                    % good for MATLAB / no swap
     end
elseif  isnumeric(pixels) & fix(pixels)==pixels & pixels>1
        % good
else    error('pixels = integer no. pixels in abscissa')
end
        x_siz = pixels;                 % x size in pixels (wide)
        y_siz = pixels;                 % y size in pixels (tall)
%       y_siz = fix(x_siz * (i_max-i_min) /...
%                           (r_max-r_min));
        scl_r = (r_max-r_min)/x_siz;    % x=0..x_siz-1 (scale real) pixel size
        scl_i = (i_max-i_min)/y_siz;    % y=0..y_siz-1 (scale imag)
%-------------------------------
    if ~exist('max_it','var')           % max mandelbrot iterations per pixel
     if isOctave
        max_it=   64;                   % good for Octave / time ok
     else
        max_it= 1024;                   % (4x)^2 image size, 16x iters
     end
elseif  isnumeric(max_it) & fix(max_it)==max_it & max_it>0
        % good
else    error('max_it = integer no. iterations for mandelbrot sequence')
end
%-------------------------------
            MOD=    2;
        SQR_MOD=MOD^2;  % maximum square modulus -- stop mandelbrot iterations
        MAX_IT=max_it;  % error: can't make function parameter `max_it' global
%-------------------------------
%*****************************************************************************

    if ~exist('nprc','var')		% "processes"
	nprc=1;
    end

	tic				% start stopwatch, alternate point

  %%%%%%%%%%%%%%%%%%%%%%%%%%%           % horzntal stripes (y_siz/nprc lines)
  % static task assignment: %           % remaining r lines on first r ranks
  %%%%%%%%%%%%%%%%%%%%%%%%%%%           % for example: 11 lines/5 ranks
      lins =   fix(y_siz/nprc);         % lins=2 lin/rank
      rlns =   rem(y_siz,nprc);         % rlns=1 remaining line

  %%%%%%%%%%%%%%%%%%%%%%
  % Work on local task %
  %%%%%%%%%%%%%%%%%%%%%%
% if rank==MAST
		    rank=0    ;		% this is task 0
 nlns=lins +       (rank<rlns);         % nlns=3 just for rank 0
 strl=lins*rank+min(rank,rlns);         % starting-line=lins*rank = 0/2/4/6/8

	mandel=zeros(y_siz,x_siz);	% image preallocation
        mandel(1:nlns,:)=CalcStrip(strl,nlns,isOctave); % strl=0 at MAST=0
% else
  for rank=1:nprc-1
 nlns=lins+        (rank<rlns);         % nlns/strl for this task
 strl=lins*rank+min(rank,rlns);         % maybe slv_nlns = mast_nlns-1

	mandel(strl+[1:nlns],:)=...
        		 CalcStrip(strl,nlns,isOctave); % compute strip
  end
% end

fprintf('elapsed time = %.4f s\n',toc);	% display time not computed

if false
image(mandel);
end

if isOctave
	 page_screen_output=oldPSO;
end

  quit

% end % function			% this generates warning in MATLAB

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function strip=CalcStrip(strow,nrows,isOctave)
global x_siz r_min i_min scl_r scl_i MAX_IT MOD SQR_MOD
                                        % recall Fortran col-major order
%       strip=zeros(nrows,x_siz);       % advantage: nlns or nlns+1, don't care
                                        % we can send less data, last col void
if isOctave                             % Calculate points
%-------------------------------        % a-la-vectorized
       [x,y]=meshgrid(r_min + scl_r*([0.5:1:x_siz]),...
                      i_min + scl_i*([0.5:1:nrows]+strow));
        c   =      (x + i*y);           % complex plane itself / stripe version
%	z   =            c  ;           % z-seq for each point in complex plane
            z=zeros(size(c));		% flaw in the original version
        strip=zeros(size(c));		% image to display (iteration counts)
        mset=1:prod(size(c));		% idxs candidate (non-divergent) points
                                                                                
  if any(size(c)~=[nrows,x_siz])        % just to be safe
        error('internal maths error (meshgrid)')
  end
                                                                                
if true					% this is slower, more assignments
%	strip=ones(size(c));		% image to display (iteration counts)
  for counter=1:MAX_IT			% initially z=c, there may already be..
        mset(abs(z(mset))>=MOD)=[];	% diverging points, trim off
	     strip(mset)=counter;	% inc.color on remaining candidates 
        z(mset)= z(mset).^2+c(mset);	% iterate candidates: z = z^2 + c;
  end
else
	strip=ones(size(c))*MAX_IT;	% advantage: nlns or nlns+1, don't care
  for counter=0:MAX_IT-1
        toff= abs(z(mset))>=MOD;        % these fail now (even when counter=0)
        strip( mset(toff))=counter;     % so color them now, before
               mset(toff)=[];           % diverging points, trim off
        z(mset) = z(mset).^2+c(mset);   % iterate candidates: z = z^2 + c;
  end
end     % if false
                                                                                
else    % isMatlab
%-------------------------------        % a-la-JIT
	strip=zeros(nrows,x_siz);	% advantage: nlns or nlns+1, don't care
  for y=1:nrows                                 % do all cols (col-major)
             %ci=i_min + scl_i*(y-0.5+strow);   % no need to factor out this
    for x=1:x_siz                               % do all rows (vertical sweep)
        zr=0; cr=r_min + scl_r*(x-0.5);         % JIT eats it easily
        zi=0; ci=i_min + scl_i*(y-0.5+strow);   % compute real/imag parts
        k =0;                                   % iteration/color counter
                                                                                
      while zr*zr+zi*zi<SQR_MOD & k<MAX_IT
        t = cr+ zr*zr - zi*zi;          % this is fastest in MATLAB
        zi= ci+       2*zr*zi;          % (faster than abs(),z*z', etc)
        zr= t ;                         % due to JIT compiler
        k = k +   1;
      end % while

        strip(y,x) = k;
    end % for y=0..y_siz-1
        % fprintf('%3d/%2d lines\r',x,x_siz);
  end   % for x=0..ncols-1
                                                                                
end     % if isOctave
%-------------------------------
                                                                                
% end % function                        % this generates warning in MATLAB

