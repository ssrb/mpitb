function ScaleHalf(sub)
% SCALEHALF	Speedup study for WaveHalf
%
%	ScaleHalf [ (sub) ]
%
%  sub ['data']	directory for data write/read. If files Wave(Seq/Half).mat...
%		exist:  plot drawings about WaveHalf gain relative to WaveSeq
%		don't: run Wave(Seq/Half) programs & save data before plotting
%

%%%%%%%%%%
% ArgChk %
%%%%%%%%%%
				flag=0;
if nargin<1, sub='data'; end,	flag=flag ||~ischar(sub) || ~exist(sub,'dir');
if flag, error('ScaleHalf: argchk: see help'), end

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
  WHinstr={};					
for vrnt=1:2, for slv=3:4
  fnam=[sub '/WaveHalf_' int2str(vrnt) '_' int2str(slv) '.mat'];
  if ~exist(fnam,'file')		% re-measure % WaveHalf %%%%%%%%
    fprintf('WaveHalf(%d,%d) started \t\t',vrnt,slv);	fflush(stdout);
					system('date');	fflush(stdout);
    instr=WaveHalf(vrnt,slv);
    save(fnam,'instr')
    fprintf('WaveHalf      finished\t\t');		fflush(stdout);
					system('date');	fflush(stdout);
  else					% load data %% WaveHalf %%%%%%%%
    load(fnam)
  end
  WHinstr{vrnt,slv-2}=instr;
end,end	% for vrnt/slv
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% WaveHalf instrumentation labels:
%   1WaveHalf_argchk        0.0006	  1 WH[i]dwt2Mast_ini     0.0002
%   2          ldimg        0.0123	  2        cmdprep        0.0199
%   3          lboot        0.1463	  3        cmdsend        0.0303
%   4          spawn        0.2969	  4             s1        0.0479
%   5          ncmds        0.0138	  5             s2        0.0170
%   6          loop1        0.0007	  6             s3        0.0172
%					  7           work        0.0007
%   7     WHdwt2Mast        0.9458   ->	  8  WH[i]dwt2Slav[s4]    0.7680   ->
%   8              -        0.0020	  9              -        0.0083
%					 10             r1        0.0154
%   9    WHidwt2Mast        0.9579   ->	 11  [r2/r4/sum]r2        0.0106
%  10              -        0.0011	 12  [r1/r3/sum]r3[r4]    0.0102
%					 13         TOTAL:        0.9458
%  11          clean        0.0278
%  12         TOTAL:        2.4053	 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%					  1 WH[i]dwt2Slav_ini     0.3928
%					  2        replace        0.0324
%					  3        compute        0.3428
%					  4         TOTAL:        0.7680
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Conclusions:
% Poor Speedup:	WaveSeq	WH(1,3)	(1,4)	(2,3)	(2,4)
%		5.21	4.21	4.59	2.43	2.35
%	Speed:	1	1.23	1.13	2.14	2.22
%	Using 3/4 computers, one would like Speedup 3/4
%
% Variant#1 is better with 3 Computers ?!?
% PLOT: 2x2 2D graph: top left: runtimes, top right: speedups
%
% Best Choice:
% r3/r4 prohibitive when 4 slaves (for both wariants)...
% so better let master work (3 slaves)
% - WaveHalf(2,4) if only interested in Speedup
% - WaveHalf(2,3) if also interested in Efficiency
% PLOT: bottom left: efficiencies
%
% cmdsend in Variant#1 prohibitive
% Speedup loss v#1 mainly due to lboot/spawn/cmdsend
% PLOT: bottom right: re-compute speedup ignoring these timespents
%
% processing time per element in row/col is not constant:
% larger rows/cols have less call overhead (e.g.: 1row 512 vs. 2rows 256)
% [i]dwt2 handles cols[rows] in pairs -> less loop overhead
% PLOT: 2x2 3D graph: mean time/row,col,icol,irow, for each program:
%		WaveSeq	WaveHalf_1_3	(n,4) don't have row/col annotations
%		<empty>	WaveHalf_2_3
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%
% Graphs % 2D figures vs. # Computers
%%%%%%%%%%
figure;
subplot(221)
title('Sequential & Parallel runtime'),	xlabel('# Computers'),ylabel('Time (s)')
WH1_34 = [  WHinstr{1,1}{end,2} ,...
	    WHinstr{1,2}{end,2}];
WH2_34 = [  WHinstr{2,1}{end,2} ,...
	    WHinstr{2,2}{end,2}];	__gnuplot_set__ key top left
WS     =    WSinstr     {end,2} ;	__gnuplot_set__ xtics 1
plot([3 4], WH1_34,	'-@+g;WaveHalf(1,3-4);')	,hold on
plot([3 4], WH2_34,	'-@*b;WaveHalf(2,3-4);')
plot(   1 , WS,		'-@xr;WaveSeq;')		,hold off
axis([1 4 0 ceil(WS)]),	grid on

subplot(222)
title('Speedups'),			xlabel('# Computers'), ylabel('')
	    WH1_34 = WS ./WH1_34;
	    WH2_34 = WS ./WH2_34;
plot([3 4], WH1_34,	'-@+g;WaveHalf(1,3-4);')	,hold on
plot([3 4], WH2_34,	'-@*b;WaveHalf(2,3-4);')	,hold off
axis([1 4 0 3])

subplot(223)
title('Efficiencies'),			xlabel('# Computers')
	    WH1_34=WH1_34./[3 4];
	    WH2_34=WH2_34./[3 4];
plot([3 4], WH1_34,	'-@+g;WaveHalf(1,3-4);')	,hold on
plot([3 4], WH2_34,	'-@*b;WaveHalf(2,3-4);')	,hold off
axis([1 4 0 1])

subplot(224),				xlabel('# Computers')
title('Speedups removing lboot/spawn/cmdsend')
lb1_34 =  [  lkup_label('lboot'  ,WHinstr{1,1}){  2} ,...
	     lkup_label('lboot'  ,WHinstr{1,2}){  2} ];
lb2_34 =  [  lkup_label('lboot'  ,WHinstr{2,1}){  2} ,...
	     lkup_label('lboot'  ,WHinstr{2,2}){  2} ];
sp1_34 =  [  lkup_label('spawn'  ,WHinstr{1,1}){  2} ,...
	     lkup_label('spawn'  ,WHinstr{1,2}){  2} ];
sp2_34 =  [  lkup_label('spawn'  ,WHinstr{2,1}){  2} ,...
	     lkup_label('spawn'  ,WHinstr{2,2}){  2} ];

cs1_34=[sum([lkup_label('cmdsend',WHinstr{1,1}){:,2}]),...
	sum([lkup_label('cmdsend',WHinstr{1,2}){:,2}])];
cs2_34=[sum([lkup_label('cmdsend',WHinstr{2,1}){:,2}]),...
	sum([lkup_label('cmdsend',WHinstr{2,2}){:,2}])];

WH1_34 = [  WHinstr{1,1}{end,2}  ,WHinstr{1,2}{end,2}];
WH2_34 = [  WHinstr{2,1}{end,2}  ,WHinstr{2,2}{end,2}];

	    WH1_34 = WH1_34 - lb1_34 - sp1_34 - cs1_34;
	    WH2_34 = WH2_34 - lb2_34 - sp2_34 - cs2_34;
	    WH1_34 = WS ./WH1_34;
	    WH2_34 = WS ./WH2_34;

plot([3 4], WH1_34,	'-@+g;WaveHalf(1,3-4);')	,hold on
plot([3 4], WH2_34,	'-@*b;WaveHalf(2,3-4);')	,hold off
axis([1 4 0 4])


%%%%%%%%%%
% Graphs % 2D figures vs. processing stage
%%%%%%%%%%
WS__100r=lkup_label('100r',WSinstr);  WH1_100r=lkup_label('100r',WHinstr{1,1});
WS__012r=lkup_label( '12r',WSinstr);  WH1_056r=lkup_label( '56r',WHinstr{1,1});
WS__100c=lkup_label('100c',WSinstr);  WH1_100c=lkup_label('100c',WHinstr{1,1});
WS__012c=lkup_label( '12c',WSinstr);  WH1_056c=lkup_label( '56c',WHinstr{1,1});

				      WH2_100r=lkup_label('100r',WHinstr{2,1});
				      WH2_056r=lkup_label( '56r',WHinstr{2,1});
				      WH2_100c=lkup_label('100c',WHinstr{2,1});
				      WH2_056c=lkup_label( '56c',WHinstr{2,1});

if any([rows(WS__100r),rows(WS__012r),rows(WS__100c),rows(WS__012c),...
	rows(WH1_100r),rows(WH1_056r),rows(WH1_100c),rows(WH1_056c),...
	rows(WH2_100r),rows(WH2_056r),rows(WH2_100c),rows(WH2_056c),...
	] ~= [10 2 10 2, 4 2 4 2, 4 2 4 2])
	error('unexpected instrumentation data')
end

WS__512rd = sum ( [WS__100r{      1:end/2,2} WS__012r{      1:end/2,2}] ) /512;
WS__512cd = sum ( [WS__100c{      1:end/2,2} WS__012c{      1:end/2,2}] ) /512;
WS__512ci = sum ( [WS__100c{end/2+1:end  ,2} WS__012c{end/2+1:end  ,2}] ) /512;
WS__512ri = sum ( [WS__100r{end/2+1:end  ,2} WS__012r{end/2+1:end  ,2}] ) /512;

WH1_256rd = sum ( [WH1_100r{      1:end/2,2} WH1_056r{      1:end/2,2}] ) /256;
WH1_256cd = sum ( [WH1_100c{      1:end/2,2} WH1_056c{      1:end/2,2}] ) /256;
WH1_256ci = sum ( [WH1_100c{end/2+1:end  ,2} WH1_056c{end/2+1:end  ,2}] ) /256;
WH1_256ri = sum ( [WH1_100r{end/2+1:end  ,2} WH1_056r{end/2+1:end  ,2}] ) /256;

WH2_256rd = sum ( [WH2_100r{      1:end/2,2} WH2_056r{      1:end/2,2}] ) /256;
WH2_256cd = sum ( [WH2_100c{      1:end/2,2} WH2_056c{      1:end/2,2}] ) /256;
WH2_256ci = sum ( [WH2_100c{end/2+1:end  ,2} WH2_056c{end/2+1:end  ,2}] ) /256;
WH2_256ri = sum ( [WH2_100r{end/2+1:end  ,2} WH2_056r{end/2+1:end  ,2}] ) /256;


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% PLOT: 2x2 3D graph: mean time/row,col,icol,irow, for each program:
%		WaveSeq	WaveHalf_1_3	(n,4) don't have row/col annotations
%		<empty>	WaveHalf_2_3
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure; clearplot, axis('auto')
%top_title('mean time per processing unit')
__gnuplot_set__ xtics ("rows" 1, "cols" 2, "inv.cols" 3, "inv.rows" 4)
__gnuplot_set__ nokey

subplot(221), clearplot,	title ('WaveSeq'),		grid on
xlabel('mean time per proc. unit (512)'), ylabel('Time (s)')
bar([WS__512rd WS__512cd WS__512ci WS__512ri])

subplot(222), clearplot,	title ('WaveHalf(1,3)'),	grid on
xlabel('mean time per proc. unit (256)'), ylabel('Time (s)')
bar([WH1_256rd WH1_256cd WH1_256ci WH1_256ri])

subplot(224), clearplot,	title ('WaveHalf(2,3)'),	grid on
xlabel('mean time per proc. unit (256)'), ylabel('Time (s)')
bar([WH2_256rd WH2_256cd WH2_256ci WH2_256ri])

__gnuplot_set__ xtics autofreq

endfunction

