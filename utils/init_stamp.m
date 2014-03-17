%INIT_STAMP	Initializes timestamp array, labels cell, and index
%
%	init_stamp (nlabels)
%
%  nlabels(int)	number of labels for the whole run (if known beforehand)
%	   [0]	if exceeded, it is extended at runtime using
%		labels{end+1}=newlabel; wtimes(end+1)=wtime;
%
function init_stamp(nlabels)
 global	stamp_names     stamp_times	stamp_index
	stamp_names={}; stamp_times=[];	stamp_index=0;

if nargin>0 &&...
   isscalar(nlabels) &&   isreal(nlabels) &&...
	fix(nlabels)==nlabels && nlabels>0
	stamp_names  (nlabels)='';
	stamp_times  (nlabels)= 0;
end
