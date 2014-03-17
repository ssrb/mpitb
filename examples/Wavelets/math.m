function trf=math(TRFM)
% MATH			Utility function to normalize 2D Wavelet Transform
%
%	transform = math (TRFM)
%
%  TRFM is a 2D cell of images in the format used in WaveSeq et al.
%  for instance, in WaveSeq each row is a wavelet decomposition level
%  and columns are: approx. detail horz/vert/diag, reconstruction
%
%	TRFM{nlevels,4} 4 columns a,dh,dv,dd		(WaveSeq)
%	TRFM{nlevels,1} 1 column  trf			(WaveLine/Strip)
%
%  transform is the usual representation of the wavelet transform
%  with same size as the original image, cut in 4 quadrants, with
%  upper left quadrant approx. and other quadrants details.
%  More levels of analysis -> approx is further cut in 4 quadrants as well
%
%  This routine equalizes the greylevel for the details (barely visible)
%  so to map the range [mean-stdev mean+stdev] into [0 1]
%  and then multiplies by the mean approx greylevel
%

[nlevels,cols]=size(TRFM);

    if cols==4
    app=TRFM{nlevels,1};			% select nth-level approx.
    trf=zeros(size(app));			% make a hole
  for LEVEL=nlevels:-1:1
    [dh,dv,dd]=deal(TRFM{LEVEL,2:4});		% nth-level details
    trf=[trf,dh ;...				% Usual organization
	  dv,dd];				% except nth-level approx. hole
  end

elseif cols==1
    app=TRFM{nlevels}(1:end/2,1:end/2);		% extract nth-level approx.
    tmp= zeros(size(app));			% hole
  for LEVEL=nlevels:-1:1
    trf=TRFM{LEVEL};				% nth-level approx./details
    trf(1:end/2,1:end/2)=tmp;			% replace approx. by decomp.
    tmp=trf;
  end

else
  error('unknown transform cell-array format')
end

M   = mean(trf(:));
st  = std (trf(:));				% normalize transform to 0..1
trf = (trf-M)/(2*st) + 0.5;			% so we can see anything
trf = trf*mean(app(:));				% weight according to approx
[x,y] = size(app); trf(1:x,1:y)=app;		% Paste approx
trf = uint8(trf);

