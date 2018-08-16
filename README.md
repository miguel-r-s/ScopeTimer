# ScopeTimer
A lighweight timer in C++ using RAII for simplicity. 

ScopeTimer introduces global state which stores results for the timers as ScopeTimer objects fall out of scope. This means a simple declaration of a timer within a certain scope is enough to measure the time it takes. 

# Examples: 

Measuring how long something takes:

```cpp
{
  ScopeTimer something("something");
  do_something();
}
```
Measuring how much time is spent on a certain kind of activity.
In this case, we'll assume `fetch_data()` is called multiple times during the execution of some program and we're interested in measuring how much time is spent in this activity in total.

```cpp
// Use the same timer to measure two separate events
{
  ScopeTimer data_fetching("data fetching");
  fetch_data(); 
} 

(do something else here...)

{ 
  // This timer will add to the previous "data fetching" timer 
  ScopeTimer data_fetching("data fetching");
  fetch_data();
} 
```

The two timers with the same name will be added up. This is useful when measuring how much time is spent in a certain type of activity that is spread accross multiple places in the code.

To access the data:

```cpp
 std::cout << GlobalTimerData::get() << std::endl;
```

Note that `GlobalTimerData` only stores data regarding timers that have already fallen out of scope, so the following program will print nothing:

```cpp
int main() {

  ScopeTimer st("scope timer");
  std::cout << GlobalTimerData::get() << std::endl;
  
  return 0;
}
```

While the following:

```cpp
int main() {

  {
    ScopeTimer st("scope timer 1");
  }
  std::cout << GlobalTimerData::get() << std::endl;
  
  return 0;
}
```
Yields:

```
Timer Info
----------
scope timer 1: 261 (ns)
```


   
