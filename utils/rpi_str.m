## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------
function rpi = rpi_str(c)
%RPI_STR	Full LAM SSI RPI string given initial letter(s)
%
%	rpi = rpi_str (c)
%
%  c	initial char(s) of rpi name: t,l,u,s
%  rpi	full rpi name, one of: tcp, lamd, usysv, sysv
%	'' if c doesn't match to any supported rpi
%

%%%%%%%%%%%%
% ARGCHECK %
%%%%%%%%%%%%
flag = nargin~=1 || isempty(c) || ~ischar(c);
if flag, error('rpi_str: argchk: see help'), end
                                                                                
%%%%%%%%%%%%%%%%%
% RPI selection %
%%%%%%%%%%%%%%%%%

	c=lower(c(1));
	rpis={'tcp','lamd','usysv','sysv','none'};	% 'none' is sentinel
	for i=1:length(rpis)
	  if rpis{i}(1)==c, break, end
	end

	if i<length(rpis),	rpi=rpis{i};		% normal cases
	else			rpi='';			% no way, unknown rpi
	end

