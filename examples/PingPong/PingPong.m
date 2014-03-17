## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
function PingPong(N,mod)
% PingPong: Classic ping-pong test to exercise basic primitive performance
%
%	PingPong [ ( N [,mod] ) ]
%
%  N	[1E8]	maximum message size to transmit (bytes)
%  mod  ['s']	communication modality:  (s)end (r)educe
%
%  printed results array format is
%	1st col	size	array size 1-N bytes in exponential sweep
%	2nd col	time	transmission time--ping (one-way)
%
% Examples:
%
%  $ mpirun -c 2   octave -q --eval "PingPong(1E8,'s')"	# LAM: 2 ranks required
%
%  $ mpirun -c 2 [-H h1,h2]				# Open-MPI: -host[file]
%		 [-hostfile lam-bhost.def]
%		   octave -q --eval "PingPong(1E8,'s')"
%


%%%%%%%%%%
% ArgChk %
%%%%%%%%%%
if nargin<1,	N=1E8;  end
if nargin<2,  mod='s';  end
if nargin>2,	usage("PingPong(N,mod)"); end	% let all ranks complain
flag=0;						% code much simpler
flag=flag || ~isscalar(N) || ~isnumeric(N);
flag=flag  |   fix(N)~=N   |           N<1;
		   mod=lower(mod); mods='sr';
flag=flag  | isempty(findstr(mod,  mods));	% let them all error out
if flag,	usage("PingPong( <int> N>0, <char> mod=='s|r' )"); end

NTIMES=10;				% no clear reason to use other value

%%%%%%%%%%%%%%%%%
% Results array %
%%%%%%%%%%%%%%%%%
  [FCT,NIP] = log2(N);			% factor and nearest integer power
if FCT<0.75,  NIP--; end		% f.i. log2(8) returns [0.5 4]
results=zeros(NIP+1,2);			% Preallocation, size 2^0 also counts

%%%%%%%%%%%%
% PARALLEL % initialization
%%%%%%%%%%%%
		MPI_Init;			% should have lambooted first
   [info rnk] =	MPI_Comm_rank (MPI_COMM_WORLD);	% let it abort if it fails
   [info siz] =	MPI_Comm_size (MPI_COMM_WORLD);

    SLV = logical(rnk);			% handy shortcuts, master is rank 0
    MST = ~ SLV;			% slaves are all other

if siz~=2				% 2 ranks required
    if MST				% complain otherwise (at rank 0)
	if     siz==1			% cannot do it alone
	  MPI_Finalize;
	  disp('     2 Octave ranks required')
	  usage("mpirun -c 2 octave -q --eval PingPong")

	else % siz>2			% warn about extra ranks
	  disp('Only 2 Octave ranks required (extra ones will finish now)')
	end
    else % SLV & siz~=2 => rnk>1
	  printf('slave %d/%d quitting!!!', rnk,siz);
	  MPI_Finalize;
	  return
    end
else	 % siz==2
    if MST
 	printf("    Size(B)\t   Time(s)\tBWdth(MB/s)\n");
    end
end

%%%%%%%%%%%%
% PingPong %
%%%%%%%%%%%%
for pow=0:NIP				% Foreach power of two
    SZ=2^pow;				% compute size &

    switch mod

    case 's' %%%%%%%%%%%%%%%%%%%%%%%%%%%%
      TAG=7;				% message tag for _Send/_Recv
	 array=zeros(SZ,1,'int8');	% allocate array of bytes (col in fact)
      if SLV				% all zeros but a mark in slave
	 array(ceil(rand(1)*SZ))=int8(TAG);
      end				% easy to detect with find() or sum()
      MPI_Barrier(MPI_COMM_WORLD);	% synch for this measurement

      if MST
	T=clock;			% stopwatch only required at master
	for i=1:NTIMES
	  MPI_Recv(array,1,TAG,MPI_COMM_WORLD);		% first get slv mark
	  MPI_Send(array,1,TAG,MPI_COMM_WORLD);		% then circulate it
	end
	T=etime(clock,T);		% done
	if sum(array) ~= TAG
	   error('**** MPI_Send/Recv malfunction ****')
	end

      else % SLV
	for i=1:NTIMES
	  MPI_Send(array,0,TAG,MPI_COMM_WORLD);		% first send slv mark
	  MPI_Recv(array,0,TAG,MPI_COMM_WORLD);		% circulate on
	end
      end  % MST / SLV


    case 'r' %%%%%%%%%%%%%%%%%%%%%%%%%%%%
      SIZ =        SZ ;
      SOD = sizeof(SZ);			% sizeof(double)==8==2^3
      if SIZ<SOD, SIZ =SOD; end		% could reduce bytes with XOR, but
		  SIZ/=SOD;		% that would make code more complex
	array=zeros(SIZ,1,'double');	% and we wanted to reduce and
	arr_1=ones (SIZ,1,'double');	% add 1 each ping and each pong
	op=MPI_SUM;			% using the SUM operator
      MPI_Barrier(MPI_COMM_WORLD);	% synch for this measurement

      if MST
	T=clock;			% stopwatch only required at master
	for i=1:NTIMES			% dest array required at rank 1
	  MPI_Reduce(array,  [] ,op,1,MPI_COMM_WORLD);	% here we can use []
	  MPI_Reduce(arr_1,array,op,0,MPI_COMM_WORLD);	% not when reduce @ 0
	end				% whatever slv sent +1 => in array here
	T=etime(clock,T);		% done
	if any(array ~= 2*NTIMES)
	   error ('**** MPI_Reduce malfunction ****')
	end

      else % SLV
	for i=1:NTIMES			% whatever mst sent +1 => in array here
	  MPI_Reduce(arr_1,array,op,1,MPI_COMM_WORLD);
	  MPI_Reduce(array,  [] ,op,0,MPI_COMM_WORLD);
	end
      end  % MST / SLV

    end % switch mod

    if MST
	results(pow+1,1) = SZ;
	results(pow+1,2) = T/2/NTIMES;
 	printf("%10d\t%9.6f\t%10.6f\n",results(pow+1,:),
					SZ*2E-6*NTIMES/T);
    end

%     clear array, if mod=='r'		% not sure it helps to avoid
%     clear arr_1, end			% swapping on big sizes


end % for pow


	MPI_Finalize;


%%%%%%%%%%%
% RESULTS %
%%%%%%%%%%%
% if MST
% 	printf("    Size(B)\tTime(s)\tBandWidth(MB/s)\n");
% 	printf("%10d\t%9.6f\n",results');
% end
