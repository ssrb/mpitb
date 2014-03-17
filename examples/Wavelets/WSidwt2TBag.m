function rec=WSidwt2TBag(vrnt, ahav,ahdv,dhav,dhdv, h,gr, inst)
% WSIDWT2TBAG	WaveStrip IDWT 2D, TaskBag process (stripe-by-stripe)
%		variant #1: send stripelen later, copy stripes on separate vars
%		variant #2: send stripelen w/cmd
%
%	rec = WSidwt2TBag ( ahav,ahdv,dhav,dhdv, h,gr, inst )
%
%  vrnt   (int)	Choose 1 of the 2 variants of the algorithm
%  ahav(matrix) 2-D signal horz. approx. vert. approx. area==1/4 of img
%  ahdv(matrix) 2-D signal horz. approx. vert. detail  (horz. details)
%  dhav(matrix) 2-D signal horz. detail  vert. approx. (vert. details)
%  dhdv(matrix) 2-D signal horz. detail  vert. detail  (diag. details)
%
%  h   (vector) 1-D approx. filter      ("low  pass")
%  g   (vector) 1-D detail  filter      ("high pass") same length lh==lg
%  vflg   (int) verbose flag
%
%  rec (matrix) Reconstructed 2-D signal. Area = 4*ahav
%

%  Intermediate arrays:
%
%  trf (2*rows/2 x 2*cols/2) ---------------------------------------| transform
%  tmp (matrix)	(  rows   x 2*cols/2) -------------|		    V
%  rec (matrix)	(  rows   x   cols  ) ---          V		TRFM{LEVEL+1,1}
%			---------	|	-------		-----------
%			|       |	|	|  |  |		|ahav|dhav|	
%			|  rec  |   <----	|ah|dh|	 <----	|----|----|	
%			|       |		|  |  |		|ahdv|dhav|	
%			---------		-------		-----------	
%

time_stamp('WSidwt2TBag_ini');
%%%%%%%%%%%
% Lengths %
%%%%%%%%%%%
sz2=size(ahav);	lih2=sz2(2); liv2=sz2(1);	% Length transform horz/vert
sz =2*sz2;	lih =sz (2); liv =sz (1);	% size of original image
                                                                                
  trf = [ahav,dhav;ahdv,dhdv];		% handy reorganization
  global NOCTAVES			% from Octave_Spawn
if inst
  CLKs=zeros(NOCTAVES,length(clock));	% Clock time for the last task sent
  TSTs=zeros(NOCTAVES,1);		% Time Spent in last task
end


%%%%%%%%%%%%%%%%%%%%%
% TaskBag Algorithm %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%		CMD msg |  liv,hstr       |=> to all CPUs  ---- ----
%			/ ^   .    ^     \                 |ah| |dh|
% -------	DATAmsg | 1   . last strp |=> send/recv => |  | |  |
% |a |dv|		\ V   .    V     /                 ---- ----
% |--|--|  ==>						    ---------
% |dh|dd|	CMD msg |  lih,vstr       |=> to all CPUs  |       |
% -------		/ <--frst--> strp\                 |  rec  |
%		DATAmsg |    ....         |=> send/recv => |       |
%			\ <--last--> strp/                 ---------
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%	Two stages: COLS & ROWS. Each stage has:
%	1) one CMD msg (broadcast)which
%		puts len in slave's workspace (h,g are always there, str later)
%		sends str (nslave msgs, since value depends on slave, str/str+1)
%	   on slaves side:
%		clear prev. & preallocates new STRIPE(2*len2,str), RES(len,str)
%		receive stripe and...
%		(len2=ceil(len/2) can be deduced from len, not the opposite)
%	2) nslaves DATA msgs containing
%		stripe data (always in vertical orientation, even when ROWS)
%		filter it with both h and g (result length: 2*len2 -0/1=len)
%		send results back in RES(len)...
%	3) nslave DATA receptions
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	% if not exact divisor length_image/nslaves, send more work to
	% first slaves?: don't make last slave finish yet later
	% last  slaves?: don't cause master contention when returning results
	% here we chose first slaves
					oflg=0;
disp('inverse col analysis'),	fflush(stdout);
time_stamp('cols_ini');
%%%%%%%%
% COLS %
%%%%%%%%
  [hstart,hstrips]=Stripe(2*lih2,NOCTAVES,oflg);

  if vrnt==1,	CMDmsg1(liv,        h,gr);	% Command-message
  else,		CMDmsg2(liv,hstrips,h,gr);
  end
		 global   TAG NEWORLD
  if vrnt==1, for i=1:NOCTAVES			% send stripSizes
	time_stamp(['strp#' int2str(i)]);
	if inst, CLKs(i,:)=clock; end		% time from cmd-send...
		 strp  =hstrips(i);
	MPI_Send(strp,  i,TAG,NEWORLD);		% let them allocate stripe
  end,end
  for i=1:NOCTAVES,			if vrnt==2	% send stripes
	time_stamp(['strp#' int2str(i)]);
	if inst, CLKs(i,:)=clock; end,	end	% time from cmd-send...
		 stripe=trf(:,hstart(i):hstart(i+1)-1,:);
	MPI_Send(stripe,i,TAG,NEWORLD);
  end


  time_stamp('alloc');
  tmp=zeros(2*liv2,2*lih2);		% Pre-allocate ahdh result 1st epoch
  res=zeros(2*liv2,max(hstrips));	% pre-allocate max size

  for i=1:NOCTAVES			% recv/same order as send? any order!
%	[info stat]=MPI_Recv(res,MPI_ANY_SOURCE,TAG,NEWORLD);
%	src=stat.src;	% fortunately, msgs stored in FORTRAN col-major-order

	time_stamp(['r' int2str(i)]);
	src=i;
	MPI_Recv(res,src,TAG,NEWORLD);

	time_stamp('cp');
	tmp(:,hstart(src):hstart(src+1)-1) = ...
	res(:,        1  :hstrips(src)   ) ;
	if inst, TSTs(src)=etime(clock,CLKs(src,:)); end
  end
time_stamp('cols_fin');


time_stamp('probe');
  [info flag stat] = MPI_Iprobe (MPI_ANY_SOURCE, MPI_ANY_TAG,  NEWORLD);
if info || flag			% there should be no msgs pending
    info,flag,stat,	error('pending messages between epochs')
end

time_stamp('.');
if inst	%% instrumentation info
	fprintf('COL stripe reconstruction\n');
	fprintf(    'CPU#\t');	fprintf('%d\t',[1:NOCTAVES]);
	fprintf('\nTspent\t');	fprintf('%7.5f\t', TSTs');
	fprintf('\n');		fflush(stdout);
end


disp('inverse row analysis'),	fflush(stdout);
time_stamp('rows_ini');
%%%%%%%%
% ROWS %
%%%%%%%%
  [vstart,vstrips]=Stripe(liv,NOCTAVES,	oflg);

  if vrnt==1,	CMDmsg1(lih,        h,gr);	% Command-message
  else,		CMDmsg2(lih,vstrips,h,gr);
  end

  if vrnt==1, for i=1:NOCTAVES				% send stripSizes
	time_stamp(['strp#' int2str(i)]);
	if inst, CLKs(i,:)=clock; end		% time from cmd-send...
		 strp  =vstrips(i);
	MPI_Send(strp,  i,TAG,NEWORLD);		% let them allocate stripe
  end,end
  for i=1:NOCTAVES,			if vrnt==2	% send stripes
	time_stamp(['strp#' int2str(i)]); 
	if inst, CLKs(i,:)=clock; end,	end	% time from cmd-send...
		 stripe=tmp(vstart(i):vstart(i+1)-1,:)';
	MPI_Send(stripe,i,TAG,NEWORLD);
  end


  time_stamp('alloc');
  rec=zeros(liv,lih);			% Pre-allocate transf result 2nd epoch
  res=zeros(max(vstrips),2*lih2)';	% pre-allocate max size/transposed

  for i=1:NOCTAVES			% recv/same order as send? any order!
%	[info stat]=MPI_Recv(res,MPI_ANY_SOURCE,TAG,NEWORLD);
%	src=stat.src;	% fortunately, msgs stored in FORTRAN col-major-order

	time_stamp(['r' int2str(i)]);
	src=i;
	MPI_Recv(res,src,TAG,NEWORLD);

	time_stamp('cp');
	rec(  vstart(src):vstart(src+1)-1,:) =...
	res(:,        1  :vstrips(src)     )';
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
	fprintf('ROW stripe reconstruction\n');
	fprintf(    'CPU#\t');	fprintf('%d\t',[1:NOCTAVES]);
	fprintf('\nTspent\t');	fprintf('%7.5f\t', TSTs');
	fprintf('\n');		fflush(stdout);
end


time_stamp('WSidwt2TBag_fin');

endfunction


%%%%%%%%%%%%%%%%%%%%
% Support routines %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%
function CMDmsg1(len,h,g) % varname change for slave workspace
%%%%%%%%%%%%%%%%%%%%%%%%%%% lih/liv -> len, gd/gr -> g
  cmd=[ 'clear stripe res,  len2=ceil(len/2);'	,...
	'str=0;	MPI_Recv(str   ,0,TAG,NEWORLD);',...
	'res(len,str)=0; stripe(2*len2,str)=0;'	,...
	       'MPI_Recv(stripe,0,TAG,NEWORLD);',...
	'for istr=1:str,'			,...
	'  res(:,istr)=idwtflt(stripe(     1:  len2,istr),h)''+'	,...
		      'idwtflt(stripe(len2+1:2*len2,istr),g)'';'	,...
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
	'res(len,str)=0; stripe(2*len2,str)=0;'	,...
	       'MPI_Recv(stripe,0,TAG,NEWORLD);',...
	'for istr=1:str,'			,...
	'  res(:,istr)=idwtflt(stripe(     1:  len2,istr),h)''+'	,...
		      'idwtflt(stripe(len2+1:2*len2,istr),g)'';'	,...
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

