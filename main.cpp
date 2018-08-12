#include <thread>
#include <chrono>
#include "raii_timer.hpp"

// Just take some time, for testing purposes
void fetch_data_mock(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main() {

  {
    // Measure the time spent on one activity 
    RAIITimer for_loop("for loop time");
    volatile int count = 1; 
    for( int i = 0; i < 100; ++i) {
      count += count;
    }
  }


  // Use the same timer to measure two separate events
  {
    RAIITimer data_fetching("data fetching");
    fetch_data_mock(100);
  }

  {
    // This timer will add to the previous "data fetching" timer 
    RAIITimer data_fetching("data fetching");
    fetch_data_mock(250);
  }

  {
    // Timing an empty scope measures 
    // how much time the creation of 
    // a raii_timer adds to the measures
    // (this is an error introduced by the 
    // measurement code)
    RAIITimer empty_scope("error margin");
  }

  std::cout << RAIITimer::getGlobalTimerData() << std::endl;
  return 0;
}