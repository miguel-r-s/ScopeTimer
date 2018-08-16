#include "scope_timer.hpp"

TimerTotal::TimerTotal() 
  : elapsed_time_init(Clock::now()) {}

TimerTotal::TimerTotal(TimerCounter elapsed_time_init)
  : elapsed_time_init(elapsed_time_init) {}


TimerTotal& TimerTotal::operator+=(const TimerTotal& tt) {
      
  elapsed_time_total += tt.elapsed_time_total;   
  return *this;
}

void TimerTotal::stopTimer() {
    elapsed_time_total = Clock::now() - elapsed_time_init;
}

std::ostream& operator<<(std::ostream& os, const TimerTotal& timer_info) {
  return os << timer_info.elapsed_time_total.count() << " (ns)";
}

const GlobalTimerData& GlobalTimerData::get() {
  return gti; 
} 

void GlobalTimerData::postTimer(const TimerID& timer_id, const TimerTotal& timer_total) {
  gti[timer_id] += timer_total;
}

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

GlobalTimerData::ScopeTimer::ScopeTimer(const TimerID& timer_id)
  : timer_id(timer_id),
    timer_total() {}

GlobalTimerData::ScopeTimer::~ScopeTimer() {

  timer_total.stopTimer();
  GlobalTimerData::postTimer(timer_id, timer_total);
}

// Initialize GlobalTimerData singleton
GlobalTimerData GlobalTimerData::gti {};





