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
 
    TimerTotal();
    TimerTotal(TimerCounter elapsed_time_init);

    TimerTotal& operator+=(const TimerTotal& tt);      

    void stopTimer();
};

// Could be std::string_vew in the future,
// but since that requires the usage of 
// gcc7+ it'll be std::string for now.
using TimerID = std::string;

class GlobalTimerData : private std::unordered_map<TimerID, TimerTotal> {

  private:

    static GlobalTimerData gti;

    GlobalTimerData() = default;
    GlobalTimerData(const GlobalTimerData&) = delete;     
    GlobalTimerData& operator=(const GlobalTimerData&) = delete;

  public:
    
    class ScopeTimer;     

    // The exposure of GlobalTimerData to the "outside world"
    // is done through this getter.
    // The const qualifier guarantees GlobalTimerData 
    // is not modified by client code.
    static const GlobalTimerData& get();
    
    static void postTimer(const TimerID& timer_id, const TimerTotal& timer_total);

    friend std::ostream& operator<<(std::ostream& os, const GlobalTimerData& global_timer_info);
};

class GlobalTimerData::ScopeTimer {

  private:

    TimerID timer_id;
    TimerTotal timer_total;

  public:

    ScopeTimer(const TimerID& timer_id);

    // Time is posted when ScopeTimer falls out of scope. 
    ~ScopeTimer();
};
