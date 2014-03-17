%LKUP_LABEL	Look up for a certain label in instrumentation data
%
%	lbls = lkup_label ('label',instr)
%
%  instr (cell)	instrumentation data (as returned by clas_instr/load_instr)
%  lbls  (cell)	copy of all cell rows with matching label, replacing the
%		1st column (label) with the index-path to the annotation
%		(ie, index sequence could be used with look_instr to reach lbl)
%
%	Instrumentation cell format:	lbls format:
%	{label, timespent, {};		{[path], timespent, {};
%	 ....., ........., {};		 ....., ........., {};
%	 r_call,globaltime,{more};	 [path], timespent, {}}
%	 ....., ........., {};
%	 label, timespent, {}}
%----------------------------------------------------------------
function lbls=lkup_label(label,instr)

  lbls={};
  STKins={}; cinstr=instr;		% stack of instr.data & current data
  STKidx=[];				% stack of indexes==path to annotation

  ROW=1; LEV=0;
  while ROW<=rows(cinstr)
    if strcmp(label,   cinstr{ROW,1})	% matching annotation
	lbls(end+1,:)= cinstr(ROW,:);	% copy to results
	lbls(end  ,1)=[STKidx,ROW];	% replace label by path
			% here we assume preferred rowvector STKidx
    end
    if ~isempty(cinstr{ROW,3})		% Hmpf, nested annotations
	LEV++;
	STKins{LEV}=cinstr;		% push current data to stack
	STKidx(LEV)=ROW;		% keep track of ongoing index
			% here we assume preferred rowvector STKidx
	cinstr=cinstr{ROW,3};		% change data - nest deep
		      ROW=0;		% start from beginning
    end
    ROW++;
    if ROW>rows(cinstr)			% data finished
      if LEV>0				% ¿nested deep?
	cinstr=STKins{LEV};		% recover old data
	ROW   =STKidx(LEV)+1;		% recover old index / advance
	       STKidx(LEV)=[];		% remove from index path
	LEV--;
      end
    end
  end

endfunction
