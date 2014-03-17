function ScaleLine(nslv,sub)
% SCALELINE	Speedup study for WaveLine
%
%	ScaleLine [ ( nslv [,sub] ) ]
%
%  nslv	(int)	max. number of slaves for scalability sweep
%	 [6]	data files are generated for 1..4 variants 1..nslv slaves
%  sub ['data']	directory for data write/read. If files Wave(Seq/Line).mat...
%		exist:  plot drawings about WaveLine gain relative to WaveSeq
%		don't: run Wave(Seq/Line) programs & save data before plotting
%

%%%%%%%%%%
% ArgChk %
%%%%%%%%%%
				flag=0;
if nargin<2, sub='data'; end,	flag=flag ||~ischar(sub) || ~exist(sub,'dir');
if nargin<1, nslv=   6 ; end,	flag=flag ||~isnumeric(nslv) ||...
					    fix(nslv)~=nslv  || nslv<1;
if flag, error('ScaleLine: argchk: see help'), end

%%%%%%%%%%%%%%%%%
% Generate Data % or simply load it
%%%%%%%%%%%%%%%%%
  fnam=[sub '/WaveSeq.mat'];
  if ~exist(fnam,'file')		% re-measure % WaveSeq %%%%%%%%%
    fprintf('WaveSeq started \t\t');	fflush(stdout);
			system('date');	fflush(stdout);
    instr=WaveSeq;
    save(fnam,'instr')
    fprintf('WaveSeq finished\t\t');	fflush(stdout);
			system('date');	fflush(stdout);
  else					% load data %% WaveSeq %%%%%%%%%
    load(fnam)
  end
  WSinstr=instr;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% WaveSeq instrumentation labels:
%   1 WaveSeq_argchk        0.0003		 1    [i]dwt2_ini        0.0002
%   2          ldimg        0.0154		 2   [i]dwt2_rows[cols]  0.0080
%   3          loop1        0.0006		 3           100r[c]     0.3027
%							...
%   4           dwt2        2.8624        ->	 8            12r[c]     0.0357
%   5              -        0.0019		 9   [i]dwt2_cols[rows]  0.0082
%						10           100c[r]     0.2590
%   6          idwt2        3.0968        ->		...
%   7              -        0.0038		15            12c[r]     0.0306
%						16         TOTAL:        2.8624
%   8         TOTAL:        5.9811		
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  WLinstr={};					
for vrnt=1:4, for slv=1:nslv
  fnam=[sub '/WaveLine_' int2str(vrnt) '_' int2str(slv) '.mat'];
  if ~exist(fnam,'file')		% re-measure % WaveLine %%%%%%%%
    fprintf('WaveLine(%d,%d) started \t\t',vrnt,slv);	fflush(stdout);
					system('date');	fflush(stdout);
    instr=WaveLine(vrnt,slv);
    save(fnam,'instr')
    fprintf('WaveLine      finished\t\t');		fflush(stdout);
					system('date');	fflush(stdout);
  else					% load data %% WaveLine %%%%%%%%
    load(fnam)
  end
  WLinstr{vrnt,slv}=instr;
end,end	% for vrnt/slv
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% WaveLine instrumentation labels:
%  1WaveLine_argchk        0.0007	  1 WL[i]dwt2TBag_ini     0.0005
%  2          ldimg        0.0121	  2           rows        7.5800   ->
%  3          lboot        0.1266	  3              -        0.0001
%  4          spawn        0.4289	  4          probe        0.0154
%  5          ncmds        0.0162	  5           cols        7.4530   ->
%  6          loop1        0.0294	  6              -        0.0002
%  7     WLdwt2TBag       15.0747  ->	  7          probe        0.0063
%  8              -        0.0295	  8          reorg        0.0193
%  9    WLidwt2TBag       15.0172  ->	  9         TOTAL:       15.0747
% 10              -        0.0010
% 11          clean        0.0340
% 12         TOTAL:       30.7703
%
%  1       rows_ini        0.0364	  1       cols_ini        0.0313
%  2           100r        1.5451	  2           100c        1.4607
%  3           100r        1.4464	  3           100c        1.4448
%  4           100r        1.4515	  4           100c        1.4450
%  5           100r        1.4466	  5           100c        1.4438
%  6           100r        1.4701	  6           100c        1.4471
%  7            12r        0.1764	  7            12c        0.1731
%  8              .        0.0074	  8              .        0.0072
%  9         TOTAL:        7.5800	  9         TOTAL:        7.4530
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Conclusions:
% Very Poor Speedup:	WaveSeq	WL(1,1)	(2,1)	(3,1)	(4,1)
%			5.21	4.21	4.59	2.43	2.35
%
% Acceptable Scalability:
%		1	2	3	4	5	6	7
%	WL1
%	WL2
%	WL3
%	WL4
%
% Variant#1 is better with 3 Computers ?!?
% PLOT: 2x2 2D graph: top left: runtimes, top right: speedups
%
% Best Choice:
% master completely busy from 3-4slaves and on
%	mean slave processing time equals master turnaround time
%	double_check with instrumentation level 1
%	best efficiency
% - WaveLine(4,7) if only interested in Speedup
% - WaveLine(4,3) if also interested in Efficiency
% PLOT: bottom left: efficiencies
%
% cmdsend in Variant#1 prohibitive
% Speedup loss v#1 mainly due to lboot/spawn/cmdsend
% PLOT: bottom right: re-compute speedup ignoring these timespents
%
% processing time per element in row/col is not constant:
% larger rows/cols have less call overhead (e.g.: 1row 512 vs. 2rows 256)
% [i]dwt2 handles cols[rows] in pairs -> less loop overhead
% PLOT: 2x3 3D graph: mean time/row,col,icol,irow, for each program:
%		WaveSeq	WaveLine(1,1-nslv)	(2,1-nslv)
%		<empty>	WaveLine(3,1-nslv)	(4,1-nslv)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%
% Graphs % 2D figures vs. # Computers
%%%%%%%%%%
%		WL1 = [WLinstr{1,1:nslv}{end,2}];	% Hmf, can't do that!!!

		WL=zeros(4,nslv);
for vrnt=1:4, for slv=1:nslv
		WL(vrnt,slv)=WLinstr{vrnt,slv}{end,2};
end,end,	WS          =WSinstr          {end,2} ;


figure;
subplot(221)
title('Sequential & Parallel runtime'),	xlabel('# Computers'),ylabel('Time (s)')
%					__gnuplot_set__ key top left
					__gnuplot_set__ xtics 1
plot(WL(1,:),	'-@+b;WLine(1);')	,hold on
plot(WL(2,:),	'-@*c;WLine(2);')
plot(WL(3,:),	'-@+g;WLine(3);')
plot(WL(4,:),	'-@*m;WLine(4);')
plot(1,WS,	'-@xr;WaveSeq;')	,hold off
axis([1 nslv 0 10*ceil(max(max(WL))/10)]),	grid on


subplot(222)
title('Speedups'),			xlabel('# Computers'), ylabel('')
		WL = WS ./WL;
plot(WL(1,:),	'-@+b;WLine(1);')	,hold on
plot(WL(2,:),	'-@*c;WLine(2);')
plot(WL(3,:),	'-@+g;WLine(3);')
plot(WL(4,:),	'-@*m;WLine(4);')	,hold off
axis('auto')


subplot(223)
title('Efficiencies')
		WL=WL./repmat([1:nslv],4,1);
plot(WL(1,:),	'-@+b;WLine(1);')	,hold on
plot(WL(2,:),	'-@*c;WLine(2);')
plot(WL(3,:),	'-@+g;WLine(3);')
plot(WL(4,:),	'-@*m;WLine(4);')	,hold off


%		WL1 = [WLinstr{1,1:nslv}{end,2}];	% Hmf, can't do that!!!
%		WL=zeros(4,nslv);
for vrnt=1:4, for slv=1:nslv
		WL(vrnt,slv)=WLinstr{vrnt,slv}{end,2};
end,end,	WP=max(WL(:,1));


subplot(224)
title('Scalability (gain wrt Par(1) instead of Seq)')
		WL = WP ./WL;
plot(WL(1,:),	'-@+b;WLine(1);')	,hold on
plot(WL(2,:),	'-@*c;WLine(2);')
plot(WL(3,:),	'-@+g;WLine(3);')
plot(WL(4,:),	'-@*m;WLine(4);')	,hold off
axis([1 nslv 0 nslv])


%%%%%%%%%%
% Graphs % 2D figures vs. processing stage
%%%%%%%%%%
						flag=0;
WS__100r=lkup_label('100r',WSinstr);		flag=flag||rows(WS__100r)~=10;
WS__012r=lkup_label( '12r',WSinstr);		flag=flag||rows(WS__012r)~= 2;
WS__100c=lkup_label('100c',WSinstr);		flag=flag||rows(WS__100c)~=10;
WS__012c=lkup_label( '12c',WSinstr);		flag=flag||rows(WS__012r)~= 2;
					if	flag
					error('unexpected instrumentation data')
					end
% don't know what's going on here... I surrender (oct-2.1.69)
%WS512rcdi=[sum ( [WS__100r{      1:end/2,2},WS__012r{      1:end/2,2}] ),...
%	    sum ( [WS__100c{      1:end/2,2},WS__012c{      1:end/2,2}] ),...
%	    sum ( [WS__100c{end/2+1:end  ,2},WS__012c{end/2+1:end  ,2}] ),...
%	    sum ( [WS__100r{end/2+1:end  ,2},WS__012r{end/2+1:end  ,2}] )]/512;

[a b c d]=...
    deal(   sum ( [WS__100r{      1:end/2,2},WS__012r{      1:end/2,2}] ),...
	    sum ( [WS__100c{      1:end/2,2},WS__012c{      1:end/2,2}] ),...
	    sum ( [WS__100c{end/2+1:end  ,2},WS__012c{end/2+1:end  ,2}] ),...
	    sum ( [WS__100r{end/2+1:end  ,2},WS__012r{end/2+1:end  ,2}] ));
WS512rcdi=[a b c d]/512;


for vrnt=1:4, for slv=1:nslv
WL__100r=lkup_label('100r',WLinstr{vrnt,slv});	flag=flag||rows(WL__100r)~=10;
WL__012r=lkup_label( '12r',WLinstr{vrnt,slv});	flag=flag||rows(WL__012r)~= 2;
WL__100c=lkup_label('100c',WLinstr{vrnt,slv});	flag=flag||rows(WL__100c)~=10;
WL__012c=lkup_label( '12c',WLinstr{vrnt,slv});	flag=flag||rows(WL__012c)~= 2;
					if	flag
					error('unexpected instrumentation data')
					end
% don't know what's going on here... I surrender (oct-2.1.69)
[a b c d]=...
    deal(   sum ( [WL__100r{      1:end/2,2} WL__012r{      1:end/2,2}] ),...
	    sum ( [WL__100c{      1:end/2,2} WL__012c{      1:end/2,2}] ),...
	    sum ( [WL__100c{end/2+1:end  ,2} WL__012c{end/2+1:end  ,2}] ),...
	    sum ( [WL__100r{end/2+1:end  ,2} WL__012r{end/2+1:end  ,2}] ));
WL512rcdi{vrnt,slv}=[a b c d]/512;
end,end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% PLOT: 2x3 3D graph: mean time/row,col,icol,irow, for each program:
%		WaveSeq	WaveLine(1,1-nslv)	(2,1-nslv)
%		<empty>	WaveLine(3,1-nslv)	(4,1-nslv)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure; clearplot, axis('auto')
%top_title('mean time per processing unit')
__gnuplot_set__ xtics ("rows" 1, "cols" 2, "icol" 3, "irow" 4)
__gnuplot_set__ nokey

subplot(231), clearplot,	title ('WaveSeq'),		grid on
xlabel('mean time per proc. unit (512)'), ylabel('Time (s)')
bar(WS512rcdi)

subplot(232), clearplot,	title ('WaveLine(1,n)'),	grid on
xlabel('mean time per proc. unit (512)'), ylabel('Time (s)'),	for slv=1:nslv
bar(WL512rcdi{1,slv}),		hold('on'),			end, hold('off')

subplot(233), clearplot,	title ('WaveLine(2,n)'),	grid on
xlabel('mean time per proc. unit (512)'), ylabel('Time (s)'),	for slv=1:nslv
bar(WL512rcdi{2,slv}),		hold('on'),			end, hold('off')

subplot(235), clearplot,	title ('WaveLine(3,n)'),	grid on
xlabel('mean time per proc. unit (512)'), ylabel('Time (s)'),	for slv=1:nslv
bar(WL512rcdi{3,slv}),		hold('on'),			end, hold('off')

subplot(236), clearplot,	title ('WaveLine(4,n)'),	grid on
xlabel('mean time per proc. unit (512)'), ylabel('Time (s)'),	for slv=1:nslv
bar(WL512rcdi{4,slv}),		hold('on'),			end, hold('off')

__gnuplot_set__ xtics autofreq

endfunction

