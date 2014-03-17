/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define NAME unsetenv
/*
 * ----------------------------------------------------
 * Delete environment variable
 * unsetenv('var')
 * ----------------------------------------------------
 */
#include <oct.h>

 DEFUN_DLD ( unsetenv, args, nargout,
"UNSETENV      Delete environment variable.\n\
\n\
    UNSETENV('NAME') removes from the underlying operating system's\n\
    environment list the specified variable.\n\
\n\
  SEE ALSO: putenv, getenv\n\
\n\
")
{
    int  nargin = args.length();
/*
 * unsetenv('var')
 */
    if ((nargin != 1)
	|| (! args(0).is_string())){
	error("unsetenv: arg must be string");
	return octave_value ();			// error returns nothing
    }
    char *str = (char*) args(0).string_value().c_str();
    
    unsetenv(str);

    return octave_value();
}
