## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------
function LAM_Init(nslaves, hosts)
% LAM_Init	lamboot (if required) and MPI_Init (if required)
%		to match a given number of slave hosts.
%
%	LAM_Init [ ( nslaves [, hosts] ) ]
%
%  Both args are optional, and override defaults if provided. Default
%  global vars NSLAVES(int) and HOSTS(cell{NSLAVES+1}-of-strings)
%  can be defined prior to invocation, and they will be used by LAM_Init.
%  If they are not defined, hosts are taken from a builtin HOSTS list,
%  After OMPI_Init invocation, those 2 global vars will be defined.
%  SPAWN_INFO=MPI_INFO_NULL will also be defined f/compatibility with OMPI
%
%  If you plan to use this script regularly, you'll save a lot of typing
%  if you edit HOSTS at the beginning of OMPI_Init.m to match your cluster.
%  Replace the builtin HOSTS variable by your cluster node names.
%
%  This utility should not be called directly but rather as MPITB_Init
%  which detects whether this is LAM/MPI or other implementation (like OMPI)
%  Generic names such as MPITB_Init/MPITB_Clean will (hopefully) be more
%  readable to both LAM & OMPI users.
%

## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------

oldPSO=page_screen_output(0);

%%%%%%%%%%%%%%%%%%
% DEFAULT VALUES %
%%%%%%%%%%%%%%%%%%
global NSLAVES HOSTS SPAWN_INFO		% make sure they're sensible

if isempty(HOSTS) || ~iscell(HOSTS)	% master host must be last in list
	HOSTS={	'h8','h7','h6','h5','h4','h3','h2','h1'};
end
	%---------------------------------------
	% If your master host is biprocessor,
	% you could list it twice, last in HOSTS list.
	% You can also list other biprocessors twice in the list.
	% This script is not aware of the "cpu=2" nomenclature:
	% if you ask for nslaves==2 it will use the first 2 entries,
	% even if both of them say "cpu=2".
	% When you use MPI_Comm_spawn, CPUs will be [re]used [round-robin]
	% in the order specified in this list and confirmed by lamnodes.
	% Even if you list biprocessor CPUs in non-consecutive positions,
	% lamboot will "join them" in the first position (it's just 1 daemon)
	% ie, you can't avoid spawning a 2nd task on a host by listing it later
	%---------------------------------------

if isempty(NSLAVES) ||  ~ is_scalar (NSLAVES) || ~isreal(NSLAVES) ||...
	   NSLAVES<0|| fix(NSLAVES)~=NSLAVES  ||...
	   NSLAVES>length(HOSTS)-1
	   NSLAVES=length(HOSTS)-1;
end

if isempty(SPAWN_INFO) ||~ is_scalar(SPAWN_INFO)||~isreal(SPAWN_INFO)||...
	   SPAWN_INFO<0|| fix(SPAWN_INFO)~=SPAWN_INFO
	   SPAWN_INFO=MPI_INFO_NULL;
end

%%%%%%%%%%%%
% ARGCHECK %
%%%%%%%%%%%%
if nargin>1
	if ~iscell(hosts)		% hostlist cell
	    error([mfilename ': 2nd arg is not a cell'])
   else	for i=1:length(hosts)		% yet more,cell-of-strings
	  if ~ischar(hosts{i})
	    error([mfilename ': 2nd arg is not cell-of-strings'])
	  end
	end
	HOSTS=hosts;			% OK, pass hosts to global
	NSLAVES=length(HOSTS)-1;	% side-effect: default nslaves changed
   end
end
if nargin>0
	if ~is_scalar(nslaves) || ~isreal(nslaves) ||...
	fix(nslaves)~=nslaves  || nslaves>=length(HOSTS)
	    error([mfilename ': 1st arg is not a valid #slaves'])
   else	NSLAVES = nslaves;		% OK, nslaves is global new default
   end
end
					% from now on use just NSLAVES & HOSTS
					%	  forget about nslaves / hosts
%%%%%%%%%%%
% LAMHALT %
%%%%%%%%%%%
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% reasons to lamhalt:          %
		% - not enough nodes  (nslv+1) % NHL < NSLAVES+1
		% - localhost not      in list % weird - just lamboot (NHL=0)
		% - localhost not last in list % weird - just lamboot (NHL=0)
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[stat, LNO] = system('lamnodes');		% Lam Nodes Output
if ~stat					% already lambooted
	%----------------------------------------------------------
	 emptyflag=false;
   if    isempty(LNO)				% this shouldn't happen
	 emptyflag=true;			% it's Octave's fault I think
	 fprintf('pushing stubborn Octave "system" call (lamnodes): ');
   end
   while isempty(LNO) || stat,	fprintf('.');
	[stat, LNO] = system('lamnodes');
   end
   if    emptyflag, fprintf('\n'); end		% Ok, worked-around
	%----------------------------------------------------------

   LNO       =split(LNO,"\n");			% split lines in rows at \n
   LNO(end,:)=deblank(LNO(end,:));		% get rid of last empty \n

[stat, NHL] = system('lamnodes|wc -l');		% Number of Hosts in Lamnodes
	%----------------------------------------------------------
	 emptyflag=false;			% again,
   if    isempty(NHL)				% this shouldn't happen
	 emptyflag=true;			% it's Octave's fault I think
	 fprintf('pushing stubborn Octave "system" call (lamnodes|wc): ');
   end
   while isempty(NHL) || stat,	fprintf('.');
	[stat, NHL] = system('lamnodes|wc -l');
   end
   if    emptyflag, fprintf('\n'); end		% Ok, worked-around
	%----------------------------------------------------------

   NHL = str2num(NHL);
   if NHL~=rows(LNO) || ~ NHL>0			% Oh my, logic error
      NHL= 0;					% pretend there are no nodes
      disp([mfilename ': internal logic error: lamboot'])
   end						% to force lamboot w/o lamhalt
   if ~index(LNO(end,:),'this_node')		% master computer last in list
      disp([mfilename ': localhost not last in nodelist, hope that''s right'])
      beforeflag=0;
	for i=1:rows(LNO), if index(LNO(i,:),'this_node')
	    beforeflag=1; break;		% well, not 1st but it's there
	end, end				% we already warned the user
      if ~beforeflag				% Oh my, incredible, not there
        NHL= 0;					% pretend there are no nodes
        disp([mfilename ': local host not in LAM? lamboot'])
      end
   end						% to force lamboot w/o lamhalt

   if NHL > 0				% accurately account multiprocessors
      NCL = 0;					% number of CPUs in lamnodes
      for i=1:rows(LNO)				% add the 2nd ":"-separated
	NCL += str2num(split(LNO(i,:),":") (2,:));	% field, ie, #CPUs
      end
      if NCL<NHL				% Oh my, logic error
	NHL= 0;					% pretend there are no nodes
	disp([mfilename ': internal logic error: lamboot'])
      else, NHL=NCL;				% update count
%	  [info CPUS flag] = MPI_Attr_get(MPI_COMM_WORLD,LAM_UNIVERSE_NCPUS);
%	if info|~flag, error('LAM_Init: missing LAM_UNIVERSE_NCPUS attr?'), end
%	if  NCL~=CPUS, error('LAM_Init: wrong #CPUs'), end
      end					% can't get count from MPI, 
   end						% since might be not _Init'ed

   if NHL < NSLAVES+1				% we have to lamboot
						% but avoid getting caught
     [infI flgI]=MPI_Initialized;		% Init?
     [infF flgF]=MPI_Finalized;			% Finalize?
      if infI ||  infF
	 page_screen_output(oldPSO);
         error([mfilename ': error calling _Initialized/_Finalized?'])
      end
      if flgI && ~flgF				% avoid hangup due to
         MPI_Finalize;				% inminent lamhalt
         clear MPI_*				% force MPI_Init in Mast/Ping
         disp([mfilename ': MPI already used- clearing before lamboot'])
      end					% by pretending "not _Init"
      if NHL > 0                                % avoid lamhalt in weird cases
         disp([mfilename ': halting LAM'])
         [stat,dum]=system('lamhalt');		% won't get caught on this
%	%----------------------------------------------------------
 	pause(1.0);	% I don't understand why this is required in my cluster
%	%----------------------------------------------------------
      end
   end
end

%%%%%%%%%%%
% LAMBOOT %
%%%%%%%%%%%
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		% reasons to lamboot:          %
		% - not lambooted yet          % stat~=0
		% - lamhalted above (or weird) % NHL < NSLAVES+1 (0 _is_ <)
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if stat || NHL<NSLAVES+1
%	HNAMS=[HOSTS{1:NSLAVES}; HOSTS{end}]';	% not valid
%		HNAMS(NSLAVES+1,:)=...		% end not working oct-2.1.60
%				   HOSTS{prod(size(HOSTS))};
%						% end working again oct-2.1.69
		HNAMS='';
for i=1:NSLAVES,HNAMS=[HNAMS;HOSTS{ i }]; end
		HNAMS=[HNAMS;HOSTS{end}];	% master computer last in bhost
		HNAMS             =HNAMS';	% transpose for "for"
      fid=fopen('bhost','wt');
      for h=HNAMS, fprintf(fid,'%s\n',h'); end	% write slaves' hostnames
      fclose(fid);
      disp  ([mfilename ': booting LAM'])
      [stat,dum]=system('lamboot -s -v bhost');	% don't need bhost anymore
	%----------------------------------------------------------
      if    stat				% again, this shouldn't happen
	fprintf('pushing stubborn Octave "system" call (lamboot): ');
	  while stat
%	    pause(1.0); % Here?
	    fprintf('.');
	    [stat,dum] = system('lamboot -s -v bhost');
	  end
	fprintf('\n');
      end
	%----------------------------------------------------------
      system('rm -f bhost');		% won't wipe on exit/could lamhalt
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% MPI_Init if not already done % RPI check removed
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  [infI flgI] = MPI_Initialized;		% Init?
  [infF flgF] = MPI_Finalized;			% Finalize?
  if infI || infF
    page_screen_output(oldPSO);
    error([mfilename ': error calling _Initialized/_Finalized?'])
  end
 
% --------------------------
% rpi=getenv('LAM_MPI_SSI_rpi');		% parameter already saved: RPI
% rpiBAD = ~strcmp(rpi,RPI);			% RPI mismatch?
% --------------------------
% Deprecated: Let users fix their own environ variables before MPI_Init
% --------------------------
 
  if    flgI			% already _Initialized
     if flgF			% already _Finalized!!! cycle over
	disp([mfilename ': clearing MPITB & starting over'])
	clear MPI_* MPITB_* LAM_*
	MPI_Init;		% done
     else % flgF %
				% already _Init, nothing to do
     end  % flgF %

  else % flgI %			% not even _Init

  %	putenv('LAM_MPI_SSI_rpi',     'lamd')
  %	putenv('OMPI_MCA_btl_base_debug','2')
	MPI_Init;		% done

  end  % flgI %


%%%%%%%%%%%%%%%%%
% NSLAVES CHECK % LAM_Init(2) in a lambooted LAM of size 8 would be OK
%%%%%%%%%%%%%%%%%
  [info attr flag]=MPI_Attr_get(MPI_COMM_WORLD,MPI_UNIVERSE_SIZE);
  if info | ~flag
        error([mfilename ': attribute MPI_UNIVERSE_SIZE does not exist?'])
  end
  if attr<NSLAVES
        error([mfilename ': wrong MPI_UNIVERSE_SIZE value?'])
  end

    page_screen_output(oldPSO);

endfunction
