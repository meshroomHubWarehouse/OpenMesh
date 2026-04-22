/* ========================================================================= *
 *                                                                           *
 *                               OpenMesh                                    *
 *           Copyright (c) 2001-2025, RWTH-Aachen University                 *
 *           Department of Computer Graphics and Multimedia                  *
 *                          All rights reserved.                             *
 *                            www.openmesh.org                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * This file is part of OpenMesh.                                            *
 *---------------------------------------------------------------------------*
 *                                                                           *
 * Redistribution and use in source and binary forms, with or without        *
 * modification, are permitted provided that the following conditions        *
 * are met:                                                                  *
 *                                                                           *
 * 1. Redistributions of source code must retain the above copyright notice, *
 *    this list of conditions and the following disclaimer.                  *
 *                                                                           *
 * 2. Redistributions in binary form must reproduce the above copyright      *
 *    notice, this list of conditions and the following disclaimer in the    *
 *    documentation and/or other materials provided with the distribution.   *
 *                                                                           *
 * 3. Neither the name of the copyright holder nor the names of its          *
 *    contributors may be used to endorse or promote products derived from   *
 *    this software without specific prior written permission.               *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED *
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           *
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  *
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       *
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        *
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      *
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              *
 *                                                                           *
 * ========================================================================= */



#ifndef DOXY_IGNORE_THIS
// ----------------------------------------------------------------------------
#include <OpenMesh/Core/System/config.h>
#if defined(OM_CC_MIPS)
#  include <math.h>
#else
#  include <cmath>
#endif
// ---windows 32 --------------------------------------------------------------
#if defined(WIN32) && (defined(_MSC_VER) || defined(__INTEL_COMPILER) || defined (__MINGW32__) )
#  include <windows.h>
// ---posix time --------------------------------------------------------------
#elif defined(__GNUC__) && defined(__POSIX__) 
#  include <time.h>
// ---gettimeofday ------------------------------------------------------------
#elif (defined(__GNUC__) && !defined(__FreeBSD__) || (defined(__INTEL_COMPILER) && !defined(WIN32))) && !defined(__MINGW32__)

#  include <sys/time.h>
#  include <sys/resource.h>
#  include <unistd.h>
// ---standard implementation -------------------------------------------------
#else 
#  include <time.h>
#endif

#include "Timer.hh"
#include <sstream>
#include <iomanip>
// ----------------------------------------------------------------------------

// ------------------------------------------------------------- namespace ----

namespace OpenMesh {
namespace Utils {


// ----------------------------------------------------------------------------

using namespace std;

// -------------------------------------------------------------- TimerImpl ----
// just a base class for the implementation
class TimerImpl
{
protected:
public:
  TimerImpl()  { ; }
  virtual ~TimerImpl() { ; }

  virtual void   reset(void) = 0;
  virtual void   start(void) = 0;
  virtual void   stop(void)  = 0;
  virtual void   cont(void)  = 0;
  virtual double seconds(void) const = 0;
};

// compiler and os dependent implementation

// ------------------------------------------------------------- windows 32 ----
#if defined(WIN32) && (defined(_MSC_VER) || defined(__INTEL_COMPILER) || defined (__MINGW32__) )

class TimerImplWin32 : public TimerImpl
{
protected:
   LARGE_INTEGER freq_;
   LARGE_INTEGER count_;
   LARGE_INTEGER start_;

public:
   TimerImplWin32(void);
   ~TimerImplWin32(void) { ; }

   virtual void   reset(void) override;
   virtual void   start(void) override;
   virtual void   stop(void) override;
   virtual void   cont(void) override;
   virtual double seconds(void) const override;
};

TimerImplWin32::TimerImplWin32(void)
{
   if (QueryPerformanceFrequency(&freq_)==FALSE)
     throw std::runtime_error("Performance counter of of stock!");

   memset(&count_,0,sizeof(count_));
   memset(&start_,0,sizeof(count_));
}

void TimerImplWin32::reset(void)
{
   memset(&count_,0,sizeof(count_));
   memset(&start_,0,sizeof(count_));
}

void TimerImplWin32::start(void)
{
   reset();
   QueryPerformanceCounter(&start_);
}

void TimerImplWin32::stop(void)
{
   LARGE_INTEGER stop_;

   QueryPerformanceCounter(&stop_);
   count_.QuadPart += stop_.QuadPart - start_.QuadPart;
}

void TimerImplWin32::cont(void)
{
   QueryPerformanceCounter(&start_);
}

double TimerImplWin32::seconds(void) const
{
   return (double)count_.QuadPart/(double)freq_.QuadPart;
}

// ------------------------------------------------------------- posix time ----
#elif defined(__GNUC__) && defined(__POSIX__) 

template <clockid_t N>
class TimerImplPosix : public TimerImpl
{
public:
    TimerImplPosix() : id_(N), seconds_(0.0)
    { }

    ~TimerImplPosix()
    { }

    virtual void reset(void) override { seconds_ = 0.0; }

    virtual void start(void) override { seconds_ = 0.0; clock_gettime( id_, &start_ ); }
    virtual void stop(void) override
    {
        timespec stop;
        clock_gettime( id_, &stop );
        seconds_ += ( stop.tv_sec - start_.tv_sec );
        seconds_ += ( (double(stop.tv_nsec-start_.tv_nsec)*1e-9) );
    }

    virtual void cont(void) override { clock_gettime( id_, &start_ ); }

    virtual double seconds() override const { return seconds_; }

protected:
   clockid_t id_;
   double    seconds_;
   timespec  start_;
};

// ----------------------------------------------------------- gettimeofday ----
#elif (defined(__GNUC__) && !defined(__FreeBSD__) || (defined(__INTEL_COMPILER) && !defined(WIN32))) && !defined(__MINGW32__)

class TimerImplGToD: public TimerImpl
{
public:
    TimerImplGToD() : seconds_(0.0)
    { }

    ~TimerImplGToD()
    { }

    virtual void reset(void) override { seconds_ = 0.0; }
    virtual void start(void) override { seconds_ = 0.0; gettimeofday( &start_, &tz_ ); }

    virtual void stop(void) override
    {
      gettimeofday( &stop_, &tz_ );

      seconds_ +=  (double)(stop_.tv_sec - start_.tv_sec);
      seconds_ +=  (double)(stop_.tv_usec- start_.tv_usec)*1e-6;
    }

    virtual void cont(void) override  { gettimeofday( &start_, &tz_); }

    virtual double seconds() const override { return seconds_; }

private:
  
  struct timeval start_, stop_;
  struct timezone tz_;

  double seconds_;
};


#else // ---------------------------------------- standard implementation ----

static const unsigned long clockticks = CLOCKS_PER_SEC;

class TimerImplStd : public TimerImpl
{
public:
   TimerImplStd() : freq_(clockticks),count_(0),start_(0) { }
   ~TimerImplStd() { ; }

   virtual void   reset(void) override { count_ = 0; }
   virtual void   start(void) override { count_ = 0; start_ = clock(); }
   virtual void   stop(void) override;
   virtual void   cont(void) override  { start_ = clock(); }
   virtual double seconds(void) const override { return (double)count_/(double)freq_; }

protected:
   unsigned long freq_;
   unsigned long count_;
   unsigned long start_;
};

void TimerImplStd::stop(void)
{
   unsigned long stop_ = clock();
   count_ += stop_-start_;
}

#endif

// ----------------------------------------------------------------- Timer ----

Timer::Timer(void) :
    state_(Stopped)
{
#if defined(WIN32) && (defined(_MSC_VER) || defined(__INTEL_COMPILER) || defined(__MINGW32__)) 
  impl_       = new TimerImplWin32;
#elif defined(__GNUC__) && defined(__POSIX__)
// CLOCK_REALTIME
// CLOCK_MONOTONIC     - ?
// CLOCK_REALTIME_HR   - RTlinux
// CLOCK_MONOTONIC_HR  - ?
#  if defined(CLOCK_REALTIME_HR)
  impl_      = new TimerImplPosix<CLOCK_REALTIME_HR>;
#  else
  impl_      = new TimerImplPosix<CLOCK_REALTIME>;
#  endif
#elif (defined(__GNUC__) && !defined(__FreeBSD__) || (defined(__INTEL_COMPILER) && !defined(WIN32)) ) && !defined(__MINGW32__)
  impl_      = new TimerImplGToD;
#else
  impl_       = new TimerImplStd;
#endif

}

Timer::~Timer(void)
{
   delete impl_;
   state_ = Stopped;
}

void Timer::reset(void)
{
   state_ = Stopped;
   impl_->reset();
}

void Timer::start(void)
{
   state_ = Running;
   impl_->start();
}

void Timer::stop(void)
{
   impl_->stop();
   state_ = Stopped;
}

void Timer::cont(void)
{
   impl_->cont();
   state_ = Running;
}

double Timer::seconds(void) const
{
   return state_==Stopped ? impl_->seconds() : 0.0;
}

std::string Timer::as_string(Timer::Format format)
{
   if (state_ == Running)
       return "Running";
   return as_string(impl_->seconds(),format);
}

std::string Timer::as_string(double seconds, Format format)
{
    double abs_sec = std::abs(seconds);
    int integer_part = static_cast<int>(abs_sec);
    double fraction  = abs_sec - integer_part;
    int hour = integer_part / 3600;
    int min  = (integer_part % 3600) / 60;
    int sec  = integer_part % 60;

    std::ostringstream ss;
    if (seconds < 0) ss << "-";
    ss << std::setfill('0');

    switch(format) {
    case Timer::Seconds:
        ss << std::fixed << std::setprecision(3) << abs_sec << "s";
        break;
    case Timer::HSeconds:
        ss << std::fixed << std::setprecision(3) << abs_sec * 1e2 << "cs";
        break;
    case Timer::MSeconds:
        ss << std::fixed << std::setprecision(3) << abs_sec * 1e3 << "ms";
        break;
    case Timer::MicroSeconds:
        ss << std::fixed << std::setprecision(1) << abs_sec * 1e6 << "\xb5s";
        break;
    case Timer::NanoSeconds:
        ss << std::fixed << std::setprecision(1) << abs_sec * 1e9 << "ns";
        break;
    case Timer::Hours:
        ss << std::setw(2) << hour << "h:"
           << std::setw(2) << min << "m:"
           << std::setw(2) << sec << "s";
        break;
    case Timer::Minutes: {
        int frac = static_cast<int>(fraction * 100);
        ss << std::setw(2) << min << "m:"
           << std::setw(2) << sec << "."
           << std::setw(2) << frac << "s";
        break;
    }
    case Timer::Long: {
        long long frac = static_cast<long long>(fraction * 1e12);
        ss << std::setw(2) << hour << "h:"
           << std::setw(2) << min << "m:"
           << std::setw(2) << sec << "."
           << std::setw(12) << frac << "s";
        break;
    }
    case Timer::Automatic: {
        if (hour > 0 || min > 0) {
            if (hour > 0)
                ss << std::setw(2) << hour << "h:";
            ss << std::setw(2) << min << "m:"
               << std::setw(2) << sec << "s";
        } else if (integer_part > 0) {
            ss << std::fixed << std::setprecision(3) << abs_sec << "s";
        } else if      (fraction * 1e2 > 0.1) {
            ss << std::fixed << std::setprecision(3) << fraction * 1e2 << "cs";
        } else if (fraction * 1e3 > 0.1) {
            ss << std::fixed << std::setprecision(3) << fraction * 1e3 << "ms";
        } else if (fraction * 1e6 > 0.1) {
            ss << std::fixed << std::setprecision(1) << fraction * 1e6 << "\xb5s";
        } else if (fraction * 1e9 > 0.1) {
            ss << std::fixed << std::setprecision(1) << fraction * 1e9 << "ns";
        } else {
            ss << std::fixed << std::setprecision(1) << fraction * 1e12 << "ps";
        }
        break;
    }
    default:
        ss << std::fixed << std::setprecision(3) << abs_sec << "s";
        break;
    }

    return ss.str();
}


// ============================================================================
} // END_NS_UTILS
} // END_NS_OPENMESH
// ----------------------------------------------------------------------------
#endif // DOXY_IGNORE_THIS
// ============================================================================
// end of file Timer.cc
// ============================================================================
