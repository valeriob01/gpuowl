#pragma once

#include "clwrap.h"
#include "timeutil.h"
#include "common.h"

#include <string>
#include <vector>
#include <memory>

struct TimeInfo {
  double total = 0;
  u32 n = 0;

  void add(double deltaTime, u32 deltaN = 1) { total += deltaTime; n += deltaN; }
  void reset() { total = 0; n = 0; }
};

class Kernel {
  Holder<cl_kernel> kernel;
  cl_queue queue;
  int workGroups;
  string name;
  bool doTime;
  int groupSize;
  TimeInfo stats;

  template<typename T> void setArgs(int pos, const T &arg) { ::setArg(kernel.get(), pos, arg); }
  
  template<typename T, typename... Args> void setArgs(int pos, const T &arg, const Args &...tail) {
    setArgs(pos, arg);
    setArgs(pos + 1, tail...);
  }

public:
  Kernel(cl_program program, cl_queue q, cl_device_id device, int workGroups, const std::string &name, bool doTime) :
    kernel(makeKernel(program, name.c_str())),
    queue(q),
    workGroups(workGroups),
    name(name),
    doTime(doTime),
    groupSize(getWorkGroupSize(kernel.get(), device, name.c_str()))
  {
    // assert((workSize % groupSize == 0) || (log("%s\n", name.c_str()), false));
  }

  template<typename... Args> void setFixedArgs(int pos, const Args &...tail) { setArgs(pos, tail...); }
  
  template<typename... Args> void operator()(const Args &...args) {
    setArgs(0, args...);
    run(workGroups);
  }

  void run(u32 nWorkGroups) {
    if (doTime) {
      finish(queue);
      Timer timer;
      ::run(queue, kernel.get(), groupSize, nWorkGroups * groupSize, name);
      finish(queue);
      stats.add(timer.deltaMicros());
    } else {
      ::run(queue, kernel.get(), groupSize, nWorkGroups * groupSize, name);
    }
  }
  
  string getName() { return name; }

  TimeInfo resetStats() { auto ret = stats; stats.reset(); return ret; }
};
