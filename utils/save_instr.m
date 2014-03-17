%SAVE_INSTR	Save instrumentation data to file
%
%	save_instr (instr,fnam)
%
%  instr (cell)	instrumentation data	(as returned by clas_instr)
%  fnam   (str)	desired base filename	(usually program name)
%		.mat extension automatically added
%

function save_instr(instr,fnam)

   save([fnam '.mat'],'instr');

endfunction
