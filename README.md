# RAIITimer
A lighweight timer in C++ using RAII for simplicity. 

RAIITimer introduces global state which stores results for the timers as RAIITimer objects fall out of scope. This means a simple declaration of a timer within a certain scope is enough to measure the time it takes. 

# Examples: 

Measuring how long something takes:

```cpp
{
  RAIITimer for_loop("for loop time");
  do_something();
}
```
Measuring how much time is spent on a certain kind of activity:

```cpp
// Use the same timer to measure two separate events
{
  RAIITimer data_fetching("data fetching");
  fetch_data_mock(100); 
} 

(do something else here...)

{ 
  // This timer will add to the previous "data fetching" timer 
  RAIITimer data_fetching("data fetching");
  fetch_data_mock(250);
} 
```

The two timers with the same name will be added up. This is useful when measuring how much time is spent ins a certain type of activity that is spread accross multiple places in the code.

