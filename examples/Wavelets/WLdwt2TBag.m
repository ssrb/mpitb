function [ahav,ahdv,dhav,dhdv]=WLdwt2TBag(vrnt, img, h,gd, inst)
% WLDWT2TBAG	WaveLine  DWT 2D, TaskBag process (line-by-line)
%		variant #1: Pack/Send row/col# to startup_wlet
%		variant #2: avoid Pack, sending row/col# to "Line cmd"
%		variant #3: avoid (data) rcvbuf realloc
%		variant #4: remove row/col check and most verbose errorcheck
%
%	[ahav,ahdv,dhav,dhdv] = WLdwt2TBag ( vrnt, img, h,gd, inst )
%
%  vrnt   (int)	Choose 1 from up to 4 variants of the algorithm
%  img (matrix) 2-D signal to analyze. 1 Approx. and 3 Details are returned
%  h   (vector) 1-D approx. filter      ("low  pass")
%  g   (vector) 1-D detail  filter      ("high pass") same length lh==lg
%  inst   (int)	instrumentation level
%
%  ahav(matrix) 2-D signal horz. approx. vert. approx. area==1/4 of img
%  ahdv(matrix) 2-D signal horz. approx. vert. detail  (horz. details)
%  dhav(matrix) 2-D signal horz. detail  vert. approx. (vert. details)
%  dhdv(matrix) 2-D signal horz. detail  vert. detail  (diag. details)
%

%  Intermediate arrays:
%
%  trf (matrix)	(2*rows/2 x 2*cols/2) -----------------------| transform    
%  tmp (matrix)	(  rows   x 2*cols/2) ---- intermediate      V
%				-------	 |		-----------
%				|  |  |	 |		|ahav|dhav|	
%				|ah|dh|	<-		|----|----|	
%				|  |  |			|ahdv|dhdv|	
%				-------			-----------	

time_stamp('WLdwt2TBag_ini');
%%%%%%%%%%%
% Lengths %
%%%%%%%%%%%
sz =size(img ); lih =sz (2); liv =sz (1);	% Length image horz/vert
sz2=ceil(sz/2); lih2=sz2(2); liv2=sz2(1);	% decimation: half/half+1

  global NOCTAVES			% from Octave_Spawn
if inst>1, global CPUs, end		% if verbose, LineRecv needs CPUs 
  CPUs=zeros(NOCTAVES,1);		% mark all CPUs free

if inst,   global CNTs CLKs STSs	% Use-Counts, Clocks, SumsTimeSpent
  CNTs=zeros(NOCTAVES,1);		% #Times the i-th CPU has been used
  CLKs=zeros(NOCTAVES,length(clock));	% Clock time for the last task sent
  STSs=zeros(NOCTAVES,1);		% Sums-Time-Spent
end


%%%%%%%%%%%%%%%%%%%%%
% TaskBag Algorithm %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%		SIZE cmd |  lih,h,gd,inst  |=> to all CPUs  ---- ----
%			 / <--frst--> row#\                 |ah| |dh|
% --------	LINE cmd |      ....       |=> send/recv => |  | |  |
% | img  |  --\		 \ <--last--> row#/                 ---- ----
% |      |  --/						    ---- ----
% --------	SIZE cmd |  liv,h,gd,inst  |=> to all CPUs  |a | |dh|
%			 / ^   .    ^     \                 ---- ----
%		LINE cmd | 1   . last col# |=> send/recv => |dv| |dd|
%			 \ V   .    V     /                 ---- ----
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%	Two taskbag epochs: ROWS & COLS. Each epoch has:
%	1) one "Size" CMD which
%		puts len in slave's workspace (and h,g,inst)
%		clear previous & preallocates new LINE(len), RES(2*len2))
%		changes "Size cmd" to "Line cmd" (version #1)
%		(inst was also sent to control whether row/col# is recvd/sent)
%	2) "unpredictably" many LINE CMDs which
%		are started by simply sending row/col# (no new command is sent)
%		(row/col# is recv by startup_wlet, not by "Line cmd")
%		(whole row/col is recvd in LINE(len) by "Line cmd", not startup
%		filter it with both h and g (result length: 2*ceil(len/2))
%		send back results in RES(2*len2)  (and row/col# for errorcheck)
%
% recvd RES are stored in tmp(liv x 2 lih2)	  (row/col# can be checked)
% Then tmp is taskbagged in cols => dhav/ahdv are stored opposite as usual
% recv RES are stored in trf(2 liv2 x 2 lih2)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


if vrnt>2
	global DATA			% avoid reiterative allocations
end


disp('direct row analysis'),	fflush(stdout);
time_stamp('rows_ini');
%%%%%%%%
% ROWS %
%%%%%%%%
  SizeCMD(vrnt, lih,h,gd, inst)		% "SIZE cmd", inst not used in varnt#4
if vrnt>2
   DATA=zeros(2*lih2,1);		% avoid reallocations
end

  tmp  =zeros(liv,2*lih2);		% Pre-allocate ahdh result 1st epoch
  for row=1:liv
    if mod(row,100)==1
	if  liv-row>=99,time_stamp('100r')
	else,		time_stamp([int2str(liv-row+1) 'r'])
    end,end
    cpu=find(CPUs==0);			% look for free CPUs
    if ~isempty(cpu), cpu=cpu(1);	% take first of them
    else				% if none, recv results from some CPU
			if vrnt<3
    [cpu,data]=LineRcv(vrnt,inst,'ROW');% should be marked as free now
      tmp(CPUs(cpu),:) = data';		% after storing result
			else
     cpu      =LineRcv(vrnt,inst,'ROW');% DATA globally accessed from LineRcv
      tmp(CPUs(cpu),:) = DATA';		% store result, mark free? no!
			end
    end
    CPUs(cpu)=row;			% mark CPU busy, send img row
    LineCMD(vrnt, img(row,:),cpu,inst,row,'ROW');
  end

  while any(CPUs)			% Wait for pending ROW (pass 1) CPUs
     time_stamp('.');
			if vrnt<3
    [cpu,data]=LineRcv(vrnt,inst,'ROW');
      tmp(CPUs(cpu),:) = data';		% after storing result
			else
     cpu      =LineRcv(vrnt,inst,'ROW');% DATA globally accessed from LineRcv
      tmp(CPUs(cpu),:) = DATA';		% after storing result
			end
	  CPUs(cpu)=0;	if vrnt>1	% marked as free now
  QuitCMD(vrnt,cpu);	end		% let it scape from "Line cmd"
  end
time_stamp('rows_fin');


time_stamp('probe');					global NEWORLD
  [info flag stat] = MPI_Iprobe (MPI_ANY_SOURCE, MPI_ANY_TAG,  NEWORLD);
if info || flag				% there should be no msgs pending
    info,flag,stat,	error('pending messages between epochs')
end

	%% should clear STSs / CNTs between epochs ?!?


disp('direct col analysis'),	fflush(stdout);
time_stamp('cols_ini');
%%%%%%%%
% COLS %
%%%%%%%%
  SizeCMD(vrnt, liv,h,gd, inst);	% "SIZE cmd", inst not used in varnt#4
if vrnt>2
   DATA=zeros(2*liv2,1);		% avoid reallocations
end

  trf  =zeros(2*liv2,2*lih2);		% Pre-allocate transf result 2nd epoch
  for col=1:2*lih2
    if mod(col,100)==1
	if  2*lih2-col>=99,time_stamp('100c')
	else,		   time_stamp([int2str(2*lih2-col+1) 'c'])
    end,end
    cpu=find(CPUs==0);			% look for free CPUs
    if ~isempty(cpu), cpu=cpu(1);	% take first of them
    else				% if none, recv results from some CPU
			if vrnt<3
    [cpu,data]=LineRcv(vrnt,inst,'COL');% should be marked as free now
      trf(:,CPUs(cpu)) = data ;		% after storing result
			else
     cpu      =LineRcv(vrnt,inst,'COL');% DATA globally accessed from LineRcv
      trf(:,CPUs(cpu)) = DATA ;		% store result, mark free? no!
			end
    end
    CPUs(cpu)=col;			% mark CPU busy, send ahdh col
    LineCMD(vrnt, tmp(:,col),cpu,inst,col,'COL');
  end

  while any(CPUs)			%% Wait for pending COL (pass 2) CPUs
     time_stamp('.');
			if vrnt<3
    [cpu,data]=LineRcv(vrnt,inst,'COL');
      trf(:,CPUs(cpu)) = data ;		% after storing result
			else
     cpu      =LineRcv(vrnt,inst,'COL');% DATA globally accessed from LineRcv
      trf(:,CPUs(cpu)) = DATA ;		% after storing result
			end
	    CPUs(cpu)=0;if vrnt>1	% marked as free now
    QuitCMD(vrnt,cpu);	end		% let it scape from "Line cmd"
  end
time_stamp('cols_fin');


time_stamp('probe');
  [info flag stat] = MPI_Iprobe (MPI_ANY_SOURCE, MPI_ANY_TAG,  NEWORLD);
if info || flag				% there should be no msgs pending
    info,flag,stat,	error('pending messages between epochs')
end


time_stamp('reorg');
%%%%%%%%%%%%%%%%%%
% Reorganization %
%%%%%%%%%%%%%%%%%%
						%	-----------
	ahav = trf(     1:liv2,     1:lih2);	%	|ahav|dhav|	
	ahdv = trf(liv2+1:liv ,     1:lih2);	%	|----|----|	
	dhav = trf(     1:liv2,lih2+1:lih );	%	|ahdv|dhdv|	
	dhdv = trf(liv2+1:liv ,lih2+1:lih );	%	-----------	


%%%%%%%%%%%%%%%%%
% Clear globals %
%%%%%%%%%%%%%%%%%
if inst
time_stamp('clear');
	MTSs=STSs./CNTs;		% Mean-Time-Spent f/ 1task on i-th CPU
	fprintf(  'CPU#\t');	fprintf('%d\t',[1:NOCTAVES]);
	fprintf('\nUsed\t');	fprintf('%d\t',	CNTs');
	fprintf('\nMean\t');	fprintf('%7.5f\t',	MTSs');
	fprintf('\n');		fflush(stdout);
	clear global CPUs CNTs CLKs STSs
end,	clear global DATA		% if variant>2


time_stamp('WLdwt2TBag_fin');

endfunction


%%%%%%%%%%%%%%%%%%%%
% Support routines %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function SizeCMD(vrnt, len,h,g, inst) % varname change for slave workspace
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% lih/liv -> len, gd/gr -> g
  cmd=[ 'clear line res, len2=ceil(len/2);'		,...
	'line(len,1)=0;  res(2*len2,1)=0;'		];

				% "SIZE cmd" changes cmd itself to "LINE cmd"
if vrnt==1			% row/col# (rcn) sent on each "Line cmd"
  cmd=[  cmd,...
 'cmd= ''MPI_Recv(line,0,TAG,NEWORLD);'			,...
	'res(     1:  len2)=dwtflt(line,h);'		,...
	'res(len2+1:2*len2)=dwtflt(line,g);'		,...
	'MPI_Send(res,0,TAG,NEWORLD);'			,...
	'if inst>1, MPI_Send(rcn,0,TAG,NEWORLD); end'';'];
end

if vrnt<4
	nams={'cmd','len','h','g','inst'};
	vars={ cmd , len , h , g , inst };
else				% vrnt#4 doesn't use inst
	nams={'cmd','len','h','g'};
	vars={ cmd , len , h , g };
end
	NumCmds_Send(nams,vars);

if   vrnt>1
  if vrnt<4						% variants#2,3
   cmd=['rcn=0; MPI_Recv(rcn,0,TAG,NEWORLD); while rcn,',...
	       'MPI_Recv(line,0,TAG,NEWORLD);'		,...
	  'res(     1:  len2)=dwtflt(line,h);'		,...
	  'res(len2+1:2*len2)=dwtflt(line,g);'		,...
		   'MPI_Send(res,0,TAG,NEWORLD);'	,...
	'if inst>1, MPI_Send(rcn,0,TAG,NEWORLD); end,'	,...
		   'MPI_Recv(rcn,0,TAG,NEWORLD);'	,...
	'end;'						];
  else							% variant#4
   cmd=['[i,stat]=MPI_Recv(line,0,MPI_ANY_TAG,NEWORLD);',...
	'while stat.tag==TAG,'				,...
	  'res(     1:  len2)=dwtflt(line,h);'		,...
	  'res(len2+1:2*len2)=dwtflt(line,g);'		,...
		 'MPI_Send(res,0,TAG,NEWORLD);'		,...
	'[i,stat]=MPI_Recv(line,0,MPI_ANY_TAG,NEWORLD);',...
	'end;'						];
  end

  NumCmds_Send({'cmd'},{cmd});
end
endfunction


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function LineCMD(vrnt, line,cpu,inst,rcn,ROWCOL) %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if inst>1
	fprintf(['rank %02d <-\t' ROWCOL '#%04d\n'], cpu,rcn);	fflush(stdout);
end

if inst
global CNTs CLKs			% Use-Counts, Clocks, MeanTimeSpent
	CNTs(cpu  )=CNTs(cpu)+1;	% Increment use-count
	CLKs(cpu,:)=clock;		% time-stamp
end

global TAG NEWORLD			% from NumCmds_Init/Octave_Spawn

if vrnt==1
	if~inst,rcn=[];end		
[i s] = MPI_Pack_size(rcn,     NEWORLD);% this msg recvd by startup_wlet
	rcnbf=blanks(s); 		% variant#2: recvd by "Line cmd"
	MPI_Pack(rcn,rcnbf,  0,NEWORLD);
	MPI_Send(rcnbf,cpu,TAG,NEWORLD);
	MPI_Send(line ,cpu,TAG,NEWORLD);% this msg recvd by "Line cmd"
else
    if vrnt<4
	MPI_Send(rcn ,cpu,TAG,NEWORLD);	% both msgs recvd by "Line cmd"
    end					% send rcn=0 to quit
	MPI_Send(line,cpu,TAG,NEWORLD);	% var#4: send w/different TAG to quit
end
endfunction


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function QuitCMD(vrnt, cpu) %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
global TAG NEWORLD			% from NumCmds_Init/Octave_Spawn
	Z=0;
if vrnt<4
	MPI_Send( Z,cpu,TAG,  NEWORLD);	% send rcn=0 to quit
else
	MPI_Send( Z,cpu,TAG+1,NEWORLD);	% send w/different TAG to quit
end
endfunction


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [cpu,line]=LineRcv(vrnt, inst,ROWCOL) % line not returned by
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% variants#3,4
global TAG NEWORLD			% from NumCmds_Init/Octave_Spawn
if vrnt>2, global DATA, end

if vrnt<3
[info stat] = MPI_Probe(      MPI_ANY_SOURCE, TAG, NEWORLD);
else
[info stat] = MPI_Recv (DATA, MPI_ANY_SOURCE, TAG, NEWORLD);
end

[info elms] = MPI_Get_elements(stat, []);

if vrnt<3,		line=zeros(elms,1);
	      MPI_Recv (line, stat.src, TAG, NEWORLD);
end
			  cpu=stat.src;

if vrnt>2,	if elms~=length(DATA)
		error([ROWCOL 'of size %04d recvd from rank %02d, ',...
				' expected size %04d\n'],...
				  elms, cpu, length(DATA))
end,		end

if inst>1,		rcn =0;				global CPUs
	if vrnt==4,	rcn =CPUs(cpu);
	else, MPI_Recv (rcn , stat.src, TAG, NEWORLD);
	end
	      fprintf(['\t\t' ROWCOL '#%04d <- rank %02d\t'], rcn,cpu);
							fflush(stdout);
	      if	rcn~=CPUs(cpu)
		 error([ROWCOL '#%04d recvd from rank %02d, ',...
				' to whom ' ROWCOL '#%04d was assigned\n'],...
				 rcn,                  cpu,CPUs(cpu))
end,	      end

if inst
global CNTs CLKs STSs			% Use-Counts, Clocks, SumsTimeSpent
	tspnt=etime(clock,CLKs(cpu,:));
	STSs(cpu)=STSs(cpu)+tspnt;
end
if inst>1
	times=CNTs(cpu);
	newm =STSs(cpu)/times;
	fprintf('used %d spent %6.4f mean %6.4f\n', times,tspnt,newm);
							fflush(stdout);
end


endfunction

