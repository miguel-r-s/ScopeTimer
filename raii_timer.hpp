#include <iostream>
#include <ctime>
#include <chrono>
#include <unordered_map>

using namespace std::chrono;

using Clock_t          = high_resolution_clock;
using TimerID_t        = const char*;
using TimerPrecision_t = nanoseconds;

class TimerInfo {

  using TimerCounter_t = Clock_t::time_point;
 
  public:

    TimerInfo()
      : elapsed_time_init(Clock_t::now()) {}

  protected:

    TimerCounter_t elapsed_time_init;
};

class TimerTotal : public TimerInfo {
  
  public:

    TimerTotal() 
      : TimerTotal(TimerPrecision_t(0)) {}

    TimerTotal(TimerPrecision_t  elapsed_time_total)
      : elapsed_time_total(elapsed_time_total) {}

    TimerTotal& operator+=(const TimerTotal& tt) {
      
      elapsed_time_total += tt.elapsed_time_total;   
      return *this;
    } 

    inline void stopTimer() {
      elapsed_time_total = Clock_t::now() - elapsed_time_init;
    }
    
    inline TimerPrecision_t getElapsedTime() const {
      return elapsed_time_total;
    }

  private:
    
    TimerPrecision_t  elapsed_time_total;

    friend std::ostream& operator<<(std::ostream& os, const TimerTotal& timer_info);
};

std::ostream& operator<<(std::ostream& os, const TimerTotal& timer_info) {
    return os << timer_info.elapsed_time_total.count() << " (ns)";
}

class GlobalTimerData : public std::unordered_map<TimerID_t, TimerTotal> {

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
     * and print each of the values */
    for (auto& gti : global_timer_info) {
        os << gti.first << ": " << gti.second << '\n';
    }
    return os;
}

class RAIITimer {

 public:

  RAIITimer(const TimerID_t& timer_id)
    : timer_id(timer_id),
      timer_total() {}

  ~RAIITimer() {
    
    GlobalTimerData& global_timer_info = GlobalTimerData::getInstance();   
    timer_total.stopTimer();
    global_timer_info[timer_id] += timer_total.getElapsedTime(); 
  }

  // The exposure of GlobalTimerData to the "outside world"
  // is done through this getter.
  // The const qualifier guarantees GlobalTimerData 
  // is not modified by client code.
  static const GlobalTimerData& getGlobalTimerData() {
    return GlobalTimerData::getInstance();
  }

 private:

  TimerID_t timer_id;
  TimerTotal timer_total;
};
