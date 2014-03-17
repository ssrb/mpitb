## Copyright (C) 2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
function Pi(N,mod)
% Pi:	Classic PI computation by numeric integration of arctan'(x) in [0..1]
%
%	Pi [ ( N [ ,mod ] ) ]
%
%  N	[1E7]	#subdivisions of the [0, 1] interval
%  mod	['s']	communication modality:  (s)end (r)educe
%
%  printed results struct contains
%	pi	estimated pi value
%	err	error
%	time	from argument xmit to pi computed
%
% Examples:
%
%  $ mpirun -c 5 octave -q --eval "Pi(2E7,'r')"	# LAM: parallel run, 5 ranks
%
%  $ mpirun -c 5 [-H h1,h2,h3,h4,h5]		# Open-MPI -host[file] syntaxes
%		 [-hostfile lam-bhost.def]
%		 octave -q --eval "Pi(2E7,'r')"
%
%  octave:1> Pi		% sequential invocation from Octave prompt
%  results =
%  {
%    pi =  3.1416
%    err = -6.4837e-14
%    time =  2.8742
%  }
%

%%%%%%%%%%
% ArgChk %
%%%%%%%%%%
if nargin<1,	N=1E7;	end
if nargin<2,  mod='s';	end
if nargin>2,	usage("Pi(N,mod)"); end		% let all ranks complain
flag=0;						% code much simpler
flag=flag || ~isscalar(N) || ~isnumeric(N);
flag=flag  |   fix(N)~=N   |           N<1;
		   mod=lower(mod); mods='sr';
flag=flag  | isempty(findstr(mod,  mods));	% let them all error out
if flag,	usage("Pi( <int> N>0, <char> mod=='s|r' )"); end

%%%%%%%%%%%%%%%%%%
% Results struct %
%%%%%%%%%%%%%%%%%%
results.pi   =0;
results.err  =0;
results.time =0;


%%%%%%%%%%%%
% PARALLEL % initialization, include MPI_Init time in measurement
%%%%%%%%%%%%
  T=clock; %
%%%%%%%%%%%%
		MPI_Init;			% should have lambooted first
   [info rnk] =	MPI_Comm_rank (MPI_COMM_WORLD);	% let it abort if it fails
   [info siz] =	MPI_Comm_size (MPI_COMM_WORLD);

    SLV = logical(rnk);			% handy shortcuts, master is rank 0
    MST = ~ SLV;			% slaves are all other

%%%%%%%%%%%%
% PARALLEL % computation (depends on rank/size)
%%%%%%%%%%%%			% vectorized code, equivalent to
  width=1/N; lsum=0;		% for i=rnk:siz:N-1
  i=rnk:siz:N-1;		%   x=(i+0.5)*width;
  x=(i+0.5)*width;		%   lsum=lsum+4/(1+x^2);
  lsum=sum(4./(1+x.^2));	% end
				% mem-bound, N=1E7 => size(i)=8E7/siz (80MB!!!)
%%%%%%%%%%%%
% PARALLEL % reduction and finish
%%%%%%%%%%%%
switch mod
  case 's',			TAG=7;	% Any tag would do
    if SLV				% All slaves send result back
	MPI_Send(lsum,             0,TAG,MPI_COMM_WORLD);
    else				% Here at master
	    Sum =lsum;			% save local result
      for slv=1:siz-1			% collect in any order
	MPI_Recv(lsum,MPI_ANY_SOURCE,TAG,MPI_COMM_WORLD);
	    Sum+=lsum;			% and accumulate
      end				% order: slv or MPI_ANY_SOURCE
    end
  case 'r',		Sum=0;		% reduction master = rank 0 @ WORLD
        MPI_Reduce(lsum,Sum, MPI_SUM,  0,MPI_COMM_WORLD);
end

		MPI_Finalize;
		MPITB_Clean;		% just in case we're at Octave prompt

if MST
    Sum      = Sum/N ; 			% better at end: don't loose resolution
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	% stopwatch measurement
results.time = etime(clock,T);  %	% but only at master after PI computed
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	% all them started T=clock;
results.err  = Sum-pi;
results.pi   = Sum % ;


%%%%%%%%%%%%%%%
% FINAL CHECK %
%%%%%%%%%%%%%%%
  if abs(results.err)>5E-10
    warning('pretty nice pi value! go fix it')
  end

end

