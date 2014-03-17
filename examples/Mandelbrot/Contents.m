LangC files:
  Makefile		Make file to build mandelbrot.c -> mandelbrot
  mandelbrotbw.c	Sequential mandelbrot code from http://www.cs.uncc.edu/
  mandelbrotcolor.c	Modified version to explore XSetFore & XDrawPoint
  mandelbrot.c		Modified version to explore some color variants

FileExchange files
  mandelLucio.m		Original version of Lucio's MSetV
  mLuc.m		Modified version of Lucio's MSetV
			used to verify that JIT wins over vectorized code
			and vectorized is best in Octave
tests files
  mandelnof.m		Normal 2D-loop version
  mandelfun.m		1D-loop version on rows with function call
			function carries an inner 1D-loop on cols
  mandelcol.m		-
  mandelrow.m		-

MPITB files:
  lam-bhost.def				Default hostfile used by lamboot
  startup_Mandel_MAT.m / startup	MATLAB startup, "path-only" (f/mpirun)
  startup_Mandel_OCT.m / .octaverc	Octave startup, "path-only" (f/mpirun)

Mandelbrot files
  mandelStup.m		setup -- default arg values, argchk, etc (used by all)

  mandelbrot.m		Sequential mandelbrot code for Octave/MATLAB (2D-loop)
			loosely based on Lucio's code from TMW FileExchange
  jet.m			approx. colormap to reproduce Lucio's set appearance
  mandelfunc.m		Sequential version with function calls (1D-loop)

  mandelStrd.m		Parallel version, static task assignment (strided)
  mandelStrp.m		Parallel version, static task assignment (stripes)
  mandelTBag.m		Parallel version, taskbag (by lines), n0 casts & works
  mandelTBg2.m		Parallel version, taskbag (by lines), n0 oversubscribed

Scripts / Execution results
  timingBrot/Func	scripts to time the sequential versions
  timingStrd/Strp	scripts to time the parallel   versions
  timingTBag/TBg2	scripts to time the taskbag    versions
  timing.ods/.pdf	OpenOffice SpreadSheet/PDF showing my results
  traces		my XMPI traces (using xmpi-2.2, thanks Brian!)

