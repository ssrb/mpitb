function soft=detect(finish)
% DETECT	Detect whether running under MATLAB or Octave
%
%  env = detect		To detect either environment/adjust screen paging etc
%	 detect(0)	To return screen paging to its previous state
%
%  env (string)	environment running this function ('MATLAB'/'Octave')

ver_flags = [exist('OCTAVE_VERSION'), exist('version'),...	%%% Octave
		    exist('license'), exist('ver')   ];		%%% MATLAB
    if all(ver_flags==[102, 2, 0, 0]),	soft='Octave';
elseif all(ver_flags==[  0, 5, 5, 2]),	soft='MATLAB';
else
%   disp('Not sure if this is Octave or MATLAB')
    if     ver_flags(1),		soft='Octave';		%%% rely on
    else,				soft='MATLAB';		%%% OCT_VERSION
end,end

if all(soft=='Octave')
   if ~nargin,	global	OldPSO
			OldPSO= page_screen_output;
				page_screen_output(false );
   else,	global	OldPSO,	page_screen_output(OldPSO);
   end
end

%end

%%% Other possibilities to base detection on: octave_config_info

