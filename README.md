# RAIITimer
A lighweight timer in C++ using RAII for simplicity. 

RAIITimer introduces global state which stores results for the timers as RAIITimer objects fall out of scope. This means a simple declaration of a timer within a certain scope is enough to measure the time it takes. 

# Examples: 

Measuring how long something takes:

```cpp
{
  RAIITimer something("something");
  do_something();
}
```
Measuring how much time is spent on a certain kind of activity.
In this case, we'll assume `fetch_data()` is called multiple times during the execution of some program and we're interested in measuring how much time is spent in this activity in total.

```cpp
// Use the same timer to measure two separate events
{
  RAIITimer data_fetching("data fetching");
  fetch_data(); 
} 

(do something else here...)

{ 
  // This timer will add to the previous "data fetching" timer 
  RAIITimer data_fetching("data fetching");
  fetch_data();
} 
```

The two timers with the same name will be added up. This is useful when measuring how much time is spent in a certain type of activity that is spread accross multiple places in the code.

