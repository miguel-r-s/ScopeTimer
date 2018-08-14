#include <iostream>
#include <ctime>
#include <chrono>
#include <string>
#include <unordered_map>

#pragma once

using Clock          = std::chrono::high_resolution_clock;
using TimerPrecision = std::chrono::nanoseconds;
using TimerCounter   = Clock::time_point;

// Could be std::string_vew in the future,
// but since it forces the usage of gcc7+ it'll be std::string for now.
using TimerID        = std::string;

class TimerTotal {
  
  
  public:

    TimerTotal() 
      : elapsed_time_init(Clock::now()) {}

    TimerTotal(TimerCounter elapsed_time_init)
      : elapsed_time_init(elapsed_time_init),
        elapsed_time_total(0) {}

    TimerTotal& operator+=(const TimerTotal& tt) {
      
      elapsed_time_total += tt.elapsed_time_total;   
      return *this;
    } 

    inline void stopTimer() {
      elapsed_time_total = Clock::now() - elapsed_time_init;
    }

  private:
    
    TimerCounter    elapsed_time_init;
    TimerPrecision  elapsed_time_total;

    friend std::ostream& operator<<(std::ostream& os, const TimerTotal& timer_info);
};

std::ostream& operator<<(std::ostream& os, const TimerTotal& timer_info) {
    return os << timer_info.elapsed_time_total.count() << " (ns)";
}

class GlobalTimerData : public std::unordered_map<TimerID, TimerTotal> {

  public:
    
    static GlobalTimerData& getInstance() {
      static GlobalTimerData gti;
      return gti; 
    } 

    friend std::ostream& operator<<(std::ostream& os, const GlobalTimerData& global_timer_info);
  
  private:
    
    GlobalTimerData() = default;
    GlobalTimerData(const GlobalTimerData&) = delete;     
    GlobalTimerData& operator=(const GlobalTimerData&) = delete;

};

std::ostream& operator<<(std::ostream& os, const GlobalTimerData& global_timer_info) {

    os << "Timer Info" << std::endl;
    os << "----------" << std::endl;

    /* Iterate the elements in the unordered_map<>
     * and print each of the values.
     * Order might change between runs, and specially 
     * between different machines and implementations of 
     * std::unordered_map<> */
    for (auto& gti : global_timer_info) {
        os << gti.first << ": " << gti.second << '\n';
    }
    return os;
}

class ScopeTimer {

 public:

  ScopeTimer(const TimerID& timer_id)
    : timer_id(timer_id),
      timer_total() {}

  ~ScopeTimer() {
    
    GlobalTimerData& global_timer_info = GlobalTimerData::getInstance();   
    timer_total.stopTimer();
    global_timer_info[timer_id] += timer_total; 
  }

  // The exposure of GlobalTimerData to the "outside world"
  // is done through this getter.
  // The const qualifier guarantees GlobalTimerData 
  // is not modified by client code.
  static const GlobalTimerData& getGlobalTimerData() {
    return GlobalTimerData::getInstance();
  }

 private:

  TimerID timer_id;
  TimerTotal timer_total;
};
