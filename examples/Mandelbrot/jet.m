## Copyright (C) 2006-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

## Not to be used from that other software environment which does not use #,
## is not free, and has its own implementation of this colormap

function cmap = jet(n)
%JET    Default MATLAB colormap
%  jet(n) returns an nx3 matrix to be used as argument to colormap().
%  Should range from dark blue, thru blue, cyan, green, yellow and red,
%  to end with dark red. jet() returns a colormap of the same length as
%  the current colormap.
%
%  Example:
%	colormap(jet)
%

if nargin<1, n=rows(colormap); end

%  dark blue, thru blue, cyan, green, yellow and red.	5 color segments
%  R      0 -------- 0 ---- 0 ---- 0 ^^^^ 1 ==== 1
%  G      0 -------- 0 ^^^^ 1 ==== 1 ==== 1 vvvv 0
%  B      0 ^^^^^^^^ 1 ==== 1 vvvv 0 ---- 0 ---- 0
%  idxs:1n/n       1n/8   3n/8   5n/8   7n/8   8n/8
%  segment:   0_1      1_3    3_5    5_7    7_8		5 color segments
%  n=16:  1   ...    2 .... 6 ....10 ....14 ....16
%
if n<12, error("jet not available for less than a dozen"), end

cmap = zeros(n,3);				% preallocate for efficiency
red=1;			green=2;	blue=3;	% column indexes to colormap
eigth_1=round(  n/8);	eigth_3=round(3*n/8);	% row hits 1n/8, 3n/8...
eigth_5=round(5*n/8);	eigth_7=round(7*n/8);

%egm_01=        1:eigth_1; segm_13=eigth_1+1:eigth_3;	% color segments
%egm_35=eigth_3+1:eigth_5; segm_57=eigth_5+1:eigth_7;	% don't repeat color
%egm_78=eigth_7+1:      n;				% entries in cmap

segm_01=      1:eigth_1;	segm_13=eigth_1:eigth_3;% color segments
segm_35=eigth_3:eigth_5;	segm_57=eigth_5:eigth_7;
segm_78=eigth_7:      n;

%  segment:   0_1      1_3    3_5    5_7    7_8		5 color segments
%  R      0 -------- 0 ---- 0 ---- 0 ^^^^ 1 ==== 1
cmap(segm_57,		red)=linspace(0,1,length(segm_57));
cmap(segm_78,		red)=1;

%  segment:   0_1      1_3    3_5    5_7    7_8		5 color segments
%  G      0 -------- 0 ^^^^ 1 ==== 1 ==== 1 vvvv 0
cmap( segm_13,		green)=linspace(0,1,length(segm_13));
cmap([segm_35 segm_57],	green)=1;
cmap( segm_78,		green)=linspace(1,0,length(segm_78));

%  segment:   0_1      1_3    3_5    5_7    7_8		5 color segments
%  B      0 ^^^^^^^^ 1 ==== 1 vvvv 0 ---- 0 ---- 0
cmap(segm_01,		blue)=linspace(0,1,length(segm_01));
cmap(segm_13,		blue)=1;
cmap(segm_35,		blue)=linspace(1,0,length(segm_35));

