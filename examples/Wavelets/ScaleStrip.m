function ScaleStrip(nslv,sub)
% SCALESTRIP	Speedup study for WaveStrip
%
%	ScaleStrip [ ( nslv [,sub] ) ]
%
%  nslv	(int)	max. number of slaves for scalability sweep
%	 [6]	data files are generated for 1..2 variants 1..nslv slaves
%  sub ['data']	directory for data write/read. If files Wave(Seq/Strip).mat...
%		exist:  plot drawings about WaveStrip gain relative to WaveSeq
%		don't: run Wave(Seq/Strip) programs & save data before plotting
%

%%%%%%%%%%
% ArgChk %
%%%%%%%%%%
				flag=0;
if nargin<2, sub='data'; end,	flag=flag ||~ischar(sub) || ~exist(sub,'dir');
if nargin<1, nslv=   6 ; end,	flag=flag ||~isnumeric(nslv) ||...
					    fix(nslv)~=nslv  || nslv<1;
if flag, error('ScaleStrip: argchk: see help'), end

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
  WPinstr={};					
for vrnt=1:2, for slv=1:nslv
  fnam=[sub '/WaveStrip_' int2str(vrnt) '_' int2str(slv) '.mat'];
  if ~exist(fnam,'file')		% re-measure % WaveStrip %%%%%%%%
    fprintf('WaveStrip(%d,%d) started \t\t',vrnt,slv);	fflush(stdout);
					system('date');	fflush(stdout);
    instr=WaveStrip(vrnt,slv);
    save(fnam,'instr')
    fprintf('WaveStrip      finished\t\t');		fflush(stdout);
					system('date');	fflush(stdout);
  else					% load data %% WaveStrip %%%%%%%%
    load(fnam)
  end
  WPinstr{vrnt,slv}=instr;
end,end	% for vrnt/slv
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% WaveStrip instrumentation labels:
%  1WaveStrip_argchk       0.0005	  1 WS[i]dwt2TBag_ini     0.0005
%  2          ldimg        0.0118	  2           rows[cols]  0.7210   ->
%  3          lboot        2.4794	  3              -        0.0001
%  4          spawn        1.1169	  4          probe        0.0189
%  5          ncmds        0.0184	  5              .        0.0006
%  6          loop1        0.0007	  6           cols[rows]  0.6771   ->
%  7     WSdwt2TBag        1.4463   ->	  7              -        0.0001
%  8              -        0.0024	  8          probe        0.0087
%  9    WSidwt2TBag        1.3134   ->	  9              .        0.0001
% 10              -        0.0011	 10          reorg        0.0192
% 11          clean        0.0559	 11         TOTAL:        1.4463
% 12         TOTAL:        6.4467	
%
%  1       rows_ini        0.0571	  1       cols_ini        0.0270
%  2         strp#1        0.0048	  2         strp#1        0.0027
%		...					...
%  7         strp#6        0.2548	  7         strp#6        0.3118
%  8          alloc        0.0201	  8          alloc        0.0118
%  9             r1        0.0164	  9             r1        0.0135
% 10             cp        0.0107	 10             cp        0.0073
%		...					...
% 19             r6        0.0157	 19             r6        0.0081
% 20             cp        0.0116	 20             cp        0.0080
% 21         TOTAL:        0.7210	 21         TOTAL:        0.6771
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Conclusions:
% Acceptable Speedup:	WaveSeq	WS(1,1)	(1,2)	(2,1)	(2,2)
%			5.21	7.20	4.35	7.16	4.46
%
% Good Scalability:
%		1	2	3	4	5	6	7
%	WS1	7.20	4.35	3.50	3.07	2.85	2.69	2.60
%	WS2	7.16	4.46	3.56	3.14	2.89	2.78	2.63
%
% Variant#1 is better with 3 Computers ?!?
% PLOT: 2x2 2D graph: top left: runtimes, top right: speedups
%
% Best Choice:
% master completely busy from 3-4slaves and on
%	mean slave processing time equals master turnaround time
%	double_check with instrumentation level 1
%	best efficiency
% - WaveStrip(4,7) if only interested in Speedup
% - WaveStrip(4,3) if also interested in Efficiency
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
%		WaveSeq	WaveStrip(1,1-nslv)
%		<empty>	WaveStrip(2,1-nslv)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%
% Graphs % 2D figures vs. # Computers
%%%%%%%%%%
%		WP1 = [WPinstr{1,1:nslv}{end,2}];	% Hmf, can't do that!!!
		WP=zeros(2,nslv);
for vrnt=1:2, for slv=1:nslv
		WP(vrnt,slv)=WPinstr{vrnt,slv}{end,2};
end,end,	WS          =WSinstr          {end,2} ;


figure;
subplot(221)
title('Sequential & Parallel runtime'),	xlabel('# Computers'),ylabel('Time (s)')
%					__gnuplot_set__ key top left
					__gnuplot_set__ xtics 1
plot(WP(1,:),	'-@+b;WStrip(1);')	,hold on
plot(WP(2,:),	'-@*c;WStrip(2);')
plot(1,WS,	'-@xr;WaveSeq;')	,hold off
axis([1 nslv 0 10*ceil(max(max(WP))/10)]),	grid on


subplot(222)
title('Speedups'),			xlabel('# Computers'), ylabel('')
		WP = WS ./WP;
plot(WP(1,:),	'-@+b;WStrip(1);')	,hold on
plot(WP(2,:),	'-@*c;WStrip(2);')	,hold off
axis('auto')


subplot(223)
title('Efficiencies')
		WP=WP./repmat([1:nslv],2,1);
plot(WP(1,:),	'-@+b;WStrip(1);')	,hold on
plot(WP(2,:),	'-@*c;WStrip(2);')	,hold off
axis([1 nslv 0 1])


%		WP1 = [WPinstr{1,1:nslv}{end,2}];	% Hmf, can't do that!!!
%		WP=zeros(2,nslv);
for vrnt=1:2, for slv=1:nslv
		WP(vrnt,slv)=WPinstr{vrnt,slv}{end,2};
end,end,	WM=max(WP(:,1));

end,end,	


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% cmdsend in Variant#1 prohibitive
% Speedup loss v#1 mainly due to lboot/spawn
% PLOT: bottom right: re-compute speedup ignoring these timespents
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		lboot=spawn=zeros(2,nslv);
for vrnt=1:2, for slv=1:nslv
		lboot(vrnt,slv)=lkup_label('lboot'  ,WPinstr{vrnt,slv}){2};
		spawn(vrnt,slv)=lkup_label('spawn'  ,WPinstr{vrnt,slv}){2};
end,end

subplot(224)
title('Speedups removing lboot/spawn')
		WP = WP - lboot - spawn;
		WP = WS ./WP;
plot(WP(1,:),	'-@+b;WStrip(1);')	,hold on
plot(WP(2,:),	'-@*c;WStrip(2);')	,hold off
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


for vrnt=1:2, for slv=1:nslv
WP__rows=lkup_label('rows',WPinstr{vrnt,slv});	flag=flag||rows(WP__rows)~=2;
WP__cols=lkup_label('cols',WPinstr{vrnt,slv});	flag=flag||rows(WP__cols)~=2;
					if	flag
					error('unexpected instrumentation data')
					end
WP512rcdi{vrnt,slv}=...
	[WP__rows{1,2} WP__cols{1,2} WP__cols{2,2} WP__rows{2,2}] / 512;
end,end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% PLOT: 2x2 3D graph: mean time/row,col,icol,irow, for each program:
%		WaveSeq	WaveStrip(1,1-nslv)	(2,1-nslv)
%		<empty>	WaveStrip(3,1-nslv)	(4,1-nslv)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure; clearplot, axis('auto')
%top_title('mean time per processing unit')
__gnuplot_set__ xtics ("rows" 1, "cols" 2, "icol" 3, "irow" 4)
__gnuplot_set__ nokey

subplot(221), clearplot,	title ('WaveSeq'),		grid on
xlabel('mean time per proc. unit (512)'), ylabel('Time (s)')
bar(WS512rcdi)

subplot(222), clearplot,	title ('WaveStrip(1,n)'),	grid on
xlabel('mean time per proc. unit (512)'), ylabel('Time (s)'),	for slv=1:nslv
bar(WP512rcdi{1,slv}),		hold('on'),			end, hold('off')

subplot(224), clearplot,	title ('WaveStrip(2,n)'),	grid on
xlabel('mean time per proc. unit (512)'), ylabel('Time (s)'),	for slv=1:nslv
bar(WP512rcdi{1,slv}),		hold('on'),			end, hold('off')

__gnuplot_set__ xtics autofreq

endfunction

