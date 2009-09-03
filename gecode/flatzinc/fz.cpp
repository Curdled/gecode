/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2007
 *
 *  Last modified:
 *     $Date: 2006-12-11 03:27:31 +1100 (Mon, 11 Dec 2006) $ by $Author: schulte $
 *     $Revision: 4024 $
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <iostream>
#include <fstream>
#include "flatzinc.hh"

using namespace std;

int main(int argc, char** argv) {
  
  Gecode::Support::Timer t_total;
  t_total.start();
  FlatZincOptions opt("Gecode/FlatZinc");
  opt.parse(argc, argv);
  
  if (argc!=2) {
    cerr << "Usage: " << argv[0] << " [options] <file>" << endl;
    cerr << "       " << argv[0] << " -help for more information" << endl;
    exit(EXIT_FAILURE);
  }
  
  const char* filename = argv[1];
  opt.name(filename);
  
  FZPrinter p;
  FlatZincGecode* fg = NULL;
  if (!strcmp(filename, "-")) {
    fg = FlatZinc::solve(cin, p);
  } else {
    fg = FlatZinc::solve(filename, p);
  }
  
  if (fg) {
    fg->run(std::cout, p, opt, t_total);
  } else {
    exit(EXIT_FAILURE);    
  }
  delete fg;
  
  return 0;
}