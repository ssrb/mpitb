function [ahav,ahdv,dhav,dhdv]=WSdwt2TBag(vrnt, img, h,gd, inst)
% WSDWT2TBAG	WaveStrip  DWT 2D, TaskBag process (stripe-by-stripe)
%		variant #1: send stripelen later, copy stripes on separate vars
%		variant #2: send stripelen w/cmd
%
%	[ahav,ahdv,dhav,dhdv] = WSdwt2TBag ( vrnt, img, h,gd, inst )
%
%  vrnt   (int)	Choose 1 of the 2 variants of the algorithm
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
%				|  |  |			|ahdv|dhav|	
%				-------			-----------	
%

time_stamp('WSdwt2TBag_ini');
%%%%%%%%%%%
% Lengths %
%%%%%%%%%%%
sz =size(img ); lih =sz (2); liv =sz (1);	% Length image horz/vert
sz2=ceil(sz/2); lih2=sz2(2); liv2=sz2(1);	% decimation: half/half+1

  global NOCTAVES			% from Octave_Spawn
if inst
  CLKs=zeros(NOCTAVES,length(clock));	% Clock time for the last task sent
  TSTs=zeros(NOCTAVES,1);		% Time Spent in last task
end


%%%%%%%%%%%%%%%%%%%%%
% TaskBag Algorithm %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%		CMD msg |  lih,vstr       |=> to all CPUs  ---- ----
%			/ <--frst--> strp\                 |ah| |dh|
% --------	DATAmsg |      ....       |=> send/recv => |  | |  |
% | img  |  --\		\ <--last--> strp/                 ---- ----
% |      |  --/						    ---- ----
% --------	CMD msg |  liv,hstr       |=> to all CPUs  |a | |dv|
%			/ ^   .    ^     \                 ---- ----
%		DATAmsg | 1   . last strp |=> send/recv => |dh| |dd|
%			\ V   .    V     /                 ---- ----
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%	Two stages: ROWS & COLS. Each stage has:
%	1) one CMD msg (broadcast) which
%		puts len in slave's workspace (and h,g,inst, str later)
%	   on slaves side:
%		clear prev. & preallocates new STRIPE(len,str), RES(2*len2,str)
%		receive stripesize and stripe itself
%	2) nslaves DATA msgs containing
%		sends stripesize (nslave msgs, value hangs on slave, str/str+1)
%		stripe data (always in vertical orientation, even when ROWS)
%		filter it with both h and g (result length: 2*ceil(len/2))
%		send results back in RES(2*len2,str)...
%	3) nslave DATA receptions
%
% recvd RES are stored in tmp(liv x 2 lih2)	liv=nslaves*vstripe more or less
% Then tmp is striped in cols => dhav/ahdv are stored opposite as usual
% recv RES are stored in trf(2 liv2 x 2 lih2)	2lih2=nslaves*hstripe moreorless
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	% if not exact divisor length_image/nslaves, send more work to
	% first slaves?: don't make last slave finish yet later
	% last  slaves?: don't cause master contention when returning results
	% here we chose first slaves
					oflg=0;
disp('direct row analysis'),	fflush(stdout);
time_stamp('rows_ini');
%%%%%%%%
% ROWS %
%%%%%%%%
  [vstart,vstrips]=Stripe(liv,NOCTAVES,	oflg);

  if vrnt==1,	CMDmsg1(lih,        h,gd);	% Command-message
  else,		CMDmsg2(lih,vstrips,h,gd);
  end
		 global   TAG NEWORLD
  if vrnt==1, for i=1:NOCTAVES			% send stripSizes
	time_stamp(['strp#' int2str(i)]);
	if inst, CLKs(i,:)=clock; end		% time from cmd-send...
		 strp  =vstrips(i);
	MPI_Send(strp,  i,TAG,NEWORLD);	% let them allocate stripe
  end,end
  for i=1:NOCTAVES,			if vrnt==2	% send stripes
	time_stamp(['strp#' int2str(i)]);
	if inst, CLKs(i,:)=clock; end,	end	% time from cmd-send...
		 stripe=img(vstart(i):vstart(i+1)-1,:)';
	MPI_Send(stripe,i,TAG,NEWORLD);
  end


  time_stamp('alloc');
  tmp=zeros(liv,2*lih2);		% Pre-allocate ahdh result 1st epoch
  res=zeros(max(vstrips),2*lih2)';	% pre-allocate max size/transposed

  for i=1:NOCTAVES			% recv/same order as send? any order!
%	[info, stat]=MPI_Recv(res,MPI_ANY_SOURCE,TAG,NEWORLD);
%	src=stat.src;	% fortunately, msgs stored in FORTRAN col-major-order

	time_stamp(['r' int2str(i)]);
	src=i;
	MPI_Recv(res,src,TAG,NEWORLD);

	time_stamp('cp');
	tmp(   vstart(src):vstart (src+1)-1,:) =...
	res(: ,        1  :vstrips(src)      )';
	if inst, TSTs(src)=etime(clock,CLKs(src,:)); end
  end
time_stamp('rows_fin');


time_stamp('probe');
  [info flag stat] = MPI_Iprobe (MPI_ANY_SOURCE, MPI_ANY_TAG,  NEWORLD);
if info || flag				% there should be no msgs pending
    info,flag,stat,	error('pending messages between epochs')
end

time_stamp('.');
if inst	%% instrumentation info
	fprintf('ROW stripe decomposition\n');
	fprintf(    'CPU#\t');	fprintf('%d\t',[1:NOCTAVES]);
	fprintf('\nTspent\t');	fprintf('%7.5f\t', TSTs');
	fprintf('\n');		fflush(stdout);
end


disp('direct col analysis'),	fflush(stdout);
time_stamp('cols_ini');
%%%%%%%%
% COLS %
%%%%%%%%
  [hstart,hstrips]=Stripe(2*lih2,NOCTAVES,oflg);

  if vrnt==1,	CMDmsg1(liv,        h,gd);	% Command-message
  else,		CMDmsg2(liv,hstrips,h,gd);
  end

  if vrnt==1, for i=1:NOCTAVES			% send stripSizes
	time_stamp(['strp#' int2str(i)]);
	if inst, CLKs(i,:)=clock; end		% time from cmd-send...
		 strp  =hstrips(i);
	MPI_Send(strp,  i,TAG,NEWORLD);		% let them allocate stripe
  end,end
  for i=1:NOCTAVES,			if vrnt==2	% send stripes
	time_stamp(['strp#' int2str(i)]);
	if inst, CLKs(i,:)=clock; end,	end	% time from cmd-send...
		 stripe=tmp(:,hstart(i):hstart(i+1)-1,:);
	MPI_Send(stripe,i,TAG,NEWORLD);
  end


  time_stamp('alloc');
  trf=zeros(2*liv2,2*lih2);		% Pre-allocate transf result 2nd epoch
  res=zeros(2*liv2,max(hstrips));	% pre-allocate max size

  for i=1:NOCTAVES			% recv/same order as send? any order!
%	[info, stat]=MPI_Recv(res,MPI_ANY_SOURCE,TAG,NEWORLD);
%	src=stat.src;	% fortunately, msgs stored in FORTRAN col-major-order

	time_stamp(['r' int2str(i)]);
	src=i;
	MPI_Recv(res,src,TAG,NEWORLD);

	time_stamp('cp');
	trf(:,hstart(src):hstart(src+1)-1) =...
	res(:,        1  :hstrips(src)   ) ;
	if inst, TSTs(src)=etime(clock,CLKs(src,:)); end
  end
time_stamp('cols_fin');


time_stamp('probe');
  [info flag stat] = MPI_Iprobe (MPI_ANY_SOURCE, MPI_ANY_TAG,  NEWORLD);
if info || flag				% there should be no msgs pending
    info,flag,stat,	error('pending messages between epochs')
end

time_stamp('.');
if inst	%% instrumentation info
	fprintf('COL stripe decomposition\n');
	fprintf(    'CPU#\t'); fprintf('%d\t',[1:NOCTAVES]);
	fprintf('\nTspent\t'); fprintf('%7.5f\t', TSTs');
	fprintf('\n');	       fflush(stdout);
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


time_stamp('WSdwt2TBag_fin');

endfunction


%%%%%%%%%%%%%%%%%%%%
% Support routines %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%% strplens not used vrnt#1
function CMDmsg1(len, h,g) % varname change for slave workspace
%%%%%%%%%%%%%%%%%%%%%%%%%%%% lih/liv -> len, gd/gr -> g
  cmd=[	'clear stripe res,  len2=ceil(len/2);'	,...
	'str=0;	MPI_Recv(str   ,0,TAG,NEWORLD);',...	% str sent on-the-fly
	'stripe(len,str)=0; res(2*len2,str)=0;'	,...
	       'MPI_Recv(stripe,0,TAG,NEWORLD);',...
	'for istr=1:str,'			,...
	'  res(     1:  len2,istr)=dwtflt(stripe(:,istr),h)'';'	,...
	'  res(len2+1:2*len2,istr)=dwtflt(stripe(:,istr),g)'';'	,...
	'end,'					,...
	       'MPI_Send(res,0,TAG,NEWORLD);'	];
	
  nams={'cmd','len','h','g'};		% easier to send stripSize/stripe later
  vars={ cmd , len , h , g };		% as we do in cmd/Recv
  NumCmds_Send(nams,vars);


% TODO: avoid transposes / keep "col-major" advantage
% so we can recv res(str,len) over rcvbuf(str,len+1) and rcvbuf(:,len+1) empty
% / or was it "row-major"? / re-study what's up
% when recv'ing  res(len,str) over rcvbuf(len+1,str)... why does it work?!?


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function CMDmsg2(len,strplens,h,g) % varname change for slave workspace
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% lih/liv -> len, gd/gr -> g
  cmd=[ 'clear stripe res,  len2=ceil(len/2);'	,...
	'stripe(len,str)=0; res(2*len2,str)=0;'	,...	% str sent w/command
	       'MPI_Recv(stripe,0,TAG,NEWORLD);',...
	'for istr=1:str,'			,...
	'  res(     1:  len2,istr)=dwtflt(stripe(:,istr),h)'';'	,...
	'  res(len2+1:2*len2,istr)=dwtflt(stripe(:,istr),g)'';'	,...
	'end,'					,...
	       'MPI_Send(res,0,TAG,NEWORLD);'	];

  nams={'cmd','len','h','g','str'};	% send stripSize now / stripe later
  vars={ cmd , len , h , g , strplens(1)};

  global  NOCTAVES
  for i=2:NOCTAVES
	vars(i, : ) = vars( 1 ,:);
	vars(i,end) = strplens(i);	% each slave its stripSize
  end					% may differ +-1 row/col

  NumCmds_Send(nams,vars);

