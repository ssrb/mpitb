%LOAD_INSTR	Load instrumentation data from file
%
%	instr = load_instr(fnam);
%
%  fnam   (str)	base filename  used with save_instr
%  instr (cell)	instrumentation data (see clas_instr)
%
% Corresponding filename is <fnam>.mat
% There should be a variable called 'instr' in that file
%

function instr=load_instr(fnam)
  FN=[fnam '.mat'];
  load(FN);
  if ~exist('instr','var') || ~iscell(instr) || columns(instr)~=3
	error(['There was no instrumentation data on ' FN])
  end
endfunction
