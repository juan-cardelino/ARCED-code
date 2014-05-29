A review of classic edge detectors
==================================
Version 1.0 - June 21, 2013
Version 1.1 - May  13, 2014
Haldo Sponton <haldos@fing.edu.uy>
Juan Cardelino <juanc@fing.edu.uy>


Introduction
------------

This is an implementation of five classic edge detectors: Roberts, Prewitt,
Sobel, Marr-Hildreth and Haralick. This code is part of the following
publication:

  "A review of classic edge detectors"
  by Haldo Sponton and Juan Cardelino, Image Processing On Line, 2014.


Files
-----

README.txt                - This file.
COPYING                   - GNU GENERAL PUBLIC LICENSE Version 3.
Makefile                  - Compilation instructions for 'make'.
classic_edge_detectors.c  - edge detector code, C99, peer reviewed file.
classic_edge_detectors.h  - edge detector header, C99, peer reviewed file.
edges.c                   - command line interface for the edge detectors.
io_png.c                  - interface to the libPNG for image input and output.
io_png.h                  - interface to the libPNG for image input and output.
lena.png                  - test image
molino.png                - test image
oranges.png               - test image
expected_results          - directory, contains the expected results for the
                            five algorithms for the image molino.png

The files "classic_edge_detectors.c" and "classic_edge_detectors.h" were
subject to peer review as part of the acceptance process of the IPOL article.


Compiling
---------

This code is programmed in C programming language following the C99 standard.

The main module, "classic_edge_detectors.c" and "classic_edge_detectors.h",
does not depend on external software (except for the standard C library).

A command line interface is provided, allowing to call each of the edge
detectors and selecting the particular parameters. This interface use the
libPNG library for image input and output in PNG format.

In the distribution is included a Makefile file with instructions to build the
command lines program interface 'edges', as well a to run on the test images
with the parameters used in the IPOL document.

On a Unix environment, the compiling instruction is just

  make

from the directory where the source codes and the Makefile are located.

To verify a correct compilation you can apply the edge detector to the test
images and compare the result to the provided ones. This is done with

  make test


Running edges Command
---------------------

The simplest edges command execution is just

  edges

or

  ./edges

if the command is not in the path. That should print an error message and quit.
To obtain instructions, type

  edges -H

A simple execution is

  edges -r 0.1 molino.png

that will apply Roberts' algorithm to the image "molino.png" with a threshold
parameter 0.1. The result is stored in an file on the current directory. The
name of the output file is fixed for each algorithm, in this case
"out_roberts.png".

More than one algorithm can be applied to the same image with the same
command. For example:

  edges -r 0.1 -l 3 29 0.13 molino.png

will apply Roberts' algorithm with threshold parameter 0.1, and the
Marr-Hildreth (LoG) with parameters sigma=3.00, N=29, TZC=0.13; the results are
stored in the files "out_roberts.png" and "out_mhl.png".

Note: it executes only one algorithm each time, i.e. it can't run two simultaneously.


Tested platforms
----------------

* Ubuntu Linux 14.04 x86_64
   * gcc version 4.8.2 (Ubuntu 4.8.2-19ubuntu1)
* OSX Mavericks 10.9.2
   * clang: Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)

Documentation
-------------
There are some doxygen comments added to the code, and a doxygen.cfg file is provided to generate the documentation.

Changelog
---------

* 2014.04.30:
   * Improved documentation to reflect the new implementation executables.
   * Added the padding method as a input argument.
   * Improved the README, explaining simultaneous execution.
	* Verified Haralick's conditions.
	* Fixed kernel normalization.
	* Added doxygen config file.

Copyright and License
---------------------

Copyright (c) 2011-2013, Haldo Sponton <haldos@fing.edu.uy>
Copyright (c) 2011-2013, Juan Cardelino <juanc@fing.edu.uy>

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.


Thanks
------

Comments with suggestions, errors, bugs or strange results are welcome. Please contact Juan Cardelino <juanc@fing.edu.uy>
