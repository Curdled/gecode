/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2009
 *
 *  Last modified:
 *     $Date$ by $Author$
 *     $Revision$
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

namespace Gecode { namespace Scheduling { namespace Unary {

  /*
   * Mandatory task
   */

  forceinline
  ManTask::ManTask(void) {}
  forceinline
  ManTask::ManTask(IntVar s, int p) : _s(s), _p(p) {}
  forceinline void
  ManTask::init(IntVar s, int p) {
    _s=s; _p=p;
  }

  forceinline int 
  ManTask::est(void) const {
    return _s.min();
  }
  forceinline int
  ManTask::ect(void) const {
    return _s.min()+_p;
  }
  forceinline int
  ManTask::lst(void) const {
    return _s.max();
  }
  forceinline int
  ManTask::lct(void) const {
    return _s.max()+_p;
  }
  forceinline IntVar
  ManTask::start(void) const {
    return _s;
  }
  forceinline int
  ManTask::p(void) const {
    return _p;
  }

  forceinline bool
  ManTask::assigned(void) const {
    return _s.assigned();
  }

  forceinline ModEvent 
  ManTask::est(Space& home, int n) {
    return _s.gq(home,n);
  }
  forceinline ModEvent
  ManTask::ect(Space& home, int n) {
    return _s.gq(home,n-_p);
  }
  forceinline ModEvent
  ManTask::lst(Space& home, int n) {
    return _s.lq(home,n);
  }
  forceinline ModEvent
  ManTask::lct(Space& home, int n) {
    return _s.lq(home,n-_p);
  }

  forceinline void
  ManTask::update(Space& home, bool share, ManTask& t) {
    _s.update(home,share,t._s); _p=t._p;
  }

  forceinline void
  ManTask::subscribe(Space& home, Propagator& p) {
    _s.subscribe(home, p, Int::PC_INT_BND);
  }
  forceinline void
  ManTask::cancel(Space& home, Propagator& p) {
    _s.cancel(home, p, Int::PC_INT_BND);
  }

  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ManTask& t) {
    std::basic_ostringstream<Char,Traits> s;
    s.copyfmt(os); s.width(0);
    s << t.est() << ':' << t.p() << ':' << t.lct();
    return os << s.str();
  }
    


  /*
   * Optional task
   */

  forceinline
  OptTask::OptTask(void) {}
  forceinline
  OptTask::OptTask(IntVar s, int p, BoolVar m) 
    : ManTask(s,p), _m(m) {}
  forceinline void
  OptTask::init(IntVar s, int p, BoolVar m) {
    ManTask::init(s,p); _m=m;
  }

  forceinline bool
  OptTask::mandatory(void) const {
    return _m.one();
  }
  forceinline bool
  OptTask::excluded(void) const {
    return _m.zero();
  }
  forceinline bool
  OptTask::optional(void) const {
    return _m.none();
  }

  forceinline bool
  OptTask::assigned(void) const {
    return ManTask::assigned() && _m.assigned();
  }

  forceinline ModEvent 
  OptTask::mandatory(Space& home) {
    return _m.one(home);
  }
  forceinline ModEvent 
  OptTask::excluded(Space& home) {
    return _m.zero(home);
  }

  forceinline void
  OptTask::update(Space& home, bool share, OptTask& t) {
    ManTask::update(home, share, t);
    _m.update(home,share,t._m);
  }

  forceinline void
  OptTask::subscribe(Space& home, Propagator& p) {
    ManTask::subscribe(home, p);
    _m.subscribe(home, p, Int::PC_BOOL_VAL);
  }
  forceinline void
  OptTask::cancel(Space& home, Propagator& p) {
    _m.cancel(home, p, Int::PC_BOOL_VAL);
    ManTask::cancel(home, p);
  }

  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const OptTask& t) {
    std::basic_ostringstream<Char,Traits> s;
    s.copyfmt(os); s.width(0);
    s << t.est() << ':' << t.p() << ':' << t.lct() << ':'
      << (t.mandatory() ? '1' : (t.optional() ? '?' : '0'));
    return os << s.str();
  }
    
}}}

// STATISTICS: scheduling-var