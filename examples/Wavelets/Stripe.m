function [start,strips]=Stripe(len,nslaves,oflg)
% STRIPE	Compute stripe width

stripe=fix(len/nslaves);		% width of stripe for load balancing
strips=stripe*ones(1,nslaves);		% all stripes initially equal
rest  =mod(len,nslaves);		% remaining lines < nslaves

if rest					% correction required?
        corr=zeros(1,nslaves);		% distribute remaining lines among
if oflg,corr(nslaves-rest+1:nslaves)=1;	% last  slaves
else,   corr(1:rest)=1;			% first slaves
end
        strips=strips+corr;		% increment their stripe size
end
%       start=stripe*[0:nslaves-1]+1;	% starting row# for each slave
        start=cumsum(strips)+1;
        start=[1 start];
     if start(end)~=len+1, error('wrong vert.stripe computation'), end
%       start(end)=[];			% useful for "for" loop

