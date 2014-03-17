function Zvalues= mLuc(limits,MaxIter,steps)
%Z=function mLuc(limits,MaxIter,steps)
%
%
% Description:
% Produces the Mandelbrot set as other user contributed
% "m" files, but this submission is VECTORIZED,
% achieving a speedup from 20 to 100 over the 
% traditional implementations.
%
% This is a very instructive example of how to vectorize
% nested loops.
%
% When being used in interactive mode you can zoom into a 
% region of interest and launch again the computation with
% a GUI push-button.
%
% Usage: 
%   mLuc(limits,MaxIter,steps)
%   mLuc(limits,MaxIter)
%   mLuc(limits)
%   Zvalues= mLuc(....)
%
%   
%   limits is a vector with the range for the real and imaginary
%          values to compute, if omitted or empty it uses the
%          default values: limits=[-2  1 -1.5 1.5]  
%   steps  is the number of steps to use in every dimension, the 
%          output will be a Matrix of size (steps,steps)
%          default: steps=200. Steps can also be a [2x1] vector with
%          the real and imaginary steps set separately.
%   MaxIter is the maximum number of iterations to perform for every
%          point to check if the system is stable or unstable
%          default: MaxIter=50
%
%   Zvalues is the output image, where each pixel has the number 
%          of iterations that were performed before the system 
%          became unstable for a particular initialization point,
%          if the system never became unstable that pixel will have
%          the value of MaxIter. 
%          If output is omitted then the interactive tool is launched
%          and log(Zvalues) is displayed in a new figure
%          Use the mouse box to select a box and zoom in, and use the
%          push button to recompute stepsXsteps new pixels within
%          that window
%
% If you want to see what happens when the code is not vectorized
% edit the first line the function to deactivate it.
%
%
% Programmed by: Lucio Andrade        Feb 2002
%
% if you have some code to vectorize email-me :)
% lucio@cetys.mx

vectorized=0;

if exist('limits')~=1
    lowerR=-2;   lowerI=-1.25;
    higherR=1;   higherI=1.25;
elseif numel(limits)==0
    lowerR=-2;   lowerI=-1.25;
    higherR=1;   higherI=1.25;
elseif  numel(limits)==2
    lowerR=min(real(limits));   
    lowerI=min(imag(limits));
    higherR=max(real(limits));  
    higherI=max(imag(limits)); 
elseif numel(limits)==4
    lowerR=limits(1);    lowerI=limits(3);
    higherR=limits(2);   higherI=limits(4); 
else error('Wrong input for ''limits'''); end
  
if exist('steps')~=1     
    stepsR=300; stepsI=400;  
elseif numel(steps) == 2 
    stepsR=steps(1); stepsI=steps(2);
elseif numel(steps) == 1 
    stepsR=steps(1); stepsI=steps(1);
else error('Wrong input for ''steps'''); end

if exist('MaxIter')~=1 || isempty(MaxIter) MaxIter=50; end

%compute other constants  
  Rwidth=higherR-lowerR;    
  Iwidth=higherI-lowerI;
  slR=Rwidth/(stepsR-1);
  slI=Iwidth/(stepsI-1);

% Initialize
 
[x,y]=meshgrid([0:stepsR-1]*slR+lowerR,[0:stepsI-1]*slI+lowerI);
  Zvalues=ones(size(x));
  initZ=zeros(size(x));
  c=(x+i*y);

tic

if vectorized
    
    z=initZ;
    h_z=1:(stepsR*stepsI);
    for counter=1:MaxIter      
      z(h_z)=z(h_z).^2+c(h_z);
      h_z= h_z(find(abs(z(h_z))<2));
      Zvalues(h_z)=Zvalues(h_z)+1;
    end

else   % loops & JIT slower by a factor ~2.5
   
  %  For each point on the plane,...
%   xv=0;
%   while  xv<stepsR 
%     xv=xv+1;
    for xv=1:stepsR
      for yv=1:stepsI
        IterNo=0;
%       z=initZ(yv,xv);
%       z=complex(0);	% ouch! worse!! 12.9->65.7 s
        z=0;
        cv=c(yv,xv);
        %  Iterate until modulus is exceeded
%       while norm(z)<2 & IterNo<MaxIter
%         z=z*z+c(yv,xv);
%       while abs(z)<2 && IterNo<MaxIter
%       while z*z'<4 && IterNo<MaxIter	% ouch! 2.76->56 s
%       while real(z)*real(z)+imag(z)*imag(z)<4 && IterNo<MaxIter
%%      while real(z)^2+imag(z)^2<4 & IterNo<MaxIter
%         z=z^2+cv;	.^		% ouch! 2.49->3.78->81 s
%%        z=z*z+cv;
%%        IterNo=IterNo+1;
%%      end

        zr=real(z); cr=real(cv);	% ouch! 2.49->0.17 !!! JIT!!!
        zi=imag(z); ci=imag(cv);
        while zr*zr + zi*zi < 4  & IterNo<MaxIter
          t = cr+ zr*zr - zi*zi;
          zi= ci+       2*zr*zi;
          zr= t ;
          IterNo=IterNo+1;
        end % while

        %  Place number of iterations in results matrix
        Zvalues(yv,xv)=IterNo;
      end
    end

end % vectorized or not

disp(['Elapsed time: ' num2str(toc)])

if nargout==0
  colormap jet(256);
  pcolor(x,y,log(double(Zvalues)));
  shading interp;
  h= uicontrol('Parent',gcf,'Interruptible','off',...
  'BusyAction','cancel','Units','points','BackgroundColor',[0.7 0.7 0.7], ...
  'Callback',['mLuc(axis,' num2str(MaxIter) ',[' num2str(stepsR) ' ' num2str(stepsI) ']);'], ...
  'Position',[0 0 40 20],'Style','pushbutton','String','redo',...
  'Tag','redoitbutton');
  zoom
  clear Zvalues
end
