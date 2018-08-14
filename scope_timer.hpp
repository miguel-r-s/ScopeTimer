#include <iostream>
#include <ctime>
#include <chrono>
#include <string>
#include <unordered_map>

#pragma once

class TimerTotal {
  
  private:
    
    using Clock          = std::chrono::high_resolution_clock;
    using TimerPrecision = std::chrono::nanoseconds;
    using TimerCounter   = Clock::time_point;
    
    TimerCounter    elapsed_time_init;
    TimerPrecision  elapsed_time_total;

    friend std::ostream& operator<<(std::ostream& os, const TimerTotal& timer_info);
  
  public:
 
    TimerTotal() 
      : elapsed_time_init(Clock::now()) {}

    TimerTotal(TimerCounter elapsed_time_init)
      : elapsed_time_init(elapsed_time_init) {}

    TimerTotal& operator+=(const TimerTotal& tt) {
      
      elapsed_time_total += tt.elapsed_time_total;   
      return *this;
    } 

    inline void stopTimer() {
      elapsed_time_total = Clock::now() - elapsed_time_init;
    }
};

std::ostream& operator<<(std::ostream& os, const TimerTotal& timer_info) {
    return os << timer_info.elapsed_time_total.count() << " (ns)";
}

// Could be std::string_vew in the future,
// but since that requires the usage of 
// gcc7+ it'll be std::string for now.
using TimerID        = std::string;

class GlobalTimerData : public std::unordered_map<TimerID, TimerTotal> {

  private:

    static GlobalTimerData gti;

    GlobalTimerData() = default;
    GlobalTimerData(const GlobalTimerData&) = delete;     
    GlobalTimerData& operator=(const GlobalTimerData&) = delete;

  public:
    
    class ScopeTimer;     

    static GlobalTimerData& getInstance() {
      return gti; 
    } 

    friend std::ostream& operator<<(std::ostream& os, const GlobalTimerData& global_timer_info);
};

GlobalTimerData GlobalTimerData::gti {};

std::ostream& operator<<(std::ostream& os, const GlobalTimerData& global_timer_info) {

    os << "Timer Info" << std::endl;
    os << "----------" << std::endl;

    // Iterate the elements in the unordered_map<>
    // and print each of the values.
    // Order might change between runs, and specially 
    // between different machines and implementations of 
    // std::unordered_map<> 
    for (auto& gti : global_timer_info) {
        os << gti.first << ": " << gti.second << '\n';
    }
    return os;
}

class GlobalTimerData::ScopeTimer {
  private:

    TimerID timer_id;
    TimerTotal timer_total;

  public:

    ScopeTimer(const TimerID& timer_id)
      : timer_id(timer_id),
        timer_total() {}


    // Time is registered when ScopeTimer
    // falls out of scope. 
    ~ScopeTimer() {

      timer_total.stopTimer();
 
      GlobalTimerData& global_timer_info = GlobalTimerData::getInstance();   
      global_timer_info[timer_id] += timer_total; 
    }

    // The exposure of GlobalTimerData to the "outside world"
    // is done through this getter.
    // The const qualifier guarantees GlobalTimerData 
    // is not modified by client code.
    static const GlobalTimerData& getGlobalTimerData() {
      return GlobalTimerData::getInstance();
    }
};
