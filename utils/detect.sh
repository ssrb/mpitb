#!/bin/bash

# --------------------------------------------------------------
# Open-MPI has mpirun -V switch. How convenient!
# -----------
# $ mpirun -V
# mpirun (Open MPI) 1.2.3
# $ mpirun -h
# mpirun (Open MPI) 1.2.3
# ...
# $ ompi_info -v ompi full
#                 Open MPI: 1.2.3
# ...
# --------------------------------------------------------------
# LAM/MPI has no mpirun -V switch. Pity!
# -----------
# $ laminfo -version lam full
#              LAM/MPI: 7.1.3
# $ lamboot -V
# LAM 7.1.3/MPI 2 C++/ROMIO - Indiana University
# ...
# $ mpirun -h
# ...
# Description:    Start an MPI application in LAM/MPI.
# ...
# --------------------------------------------------------------

	 LOMPI=`mpirun -V 2>&1 | grep MPI | head -1`

  if [[ $LOMPI == *LAM/MPI. ]]; then
	 LAM=1
elif [[ $LOMPI == *Open\ MPI* ]]; then
	  OMPI=1
else
	 UNKNW=1
	echo Unrecognized mpirun
  fi  # $LOMPI == regexp

unset    LOMPI

# -------------------------------
# Usage: In other scripts you can
# -------------------------------
# source detect.sh
# if [ $LAM ]; then
# 	echo LAM/MPI
# fi # $LAM
# if [ $OMPI ]; then
# 	echo Open-MPI
# fi # $OMPI
# -------------------------------
