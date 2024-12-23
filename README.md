# Load-Balancer

## GlobalClock

`GlobalClock` is a C++ class designed to simulate the passage of time in a controlled and customizable manner. It allows for **parallel execution** with the main application logic, making it useful for various simulation scenarios.

### Features
- Adjustable **simulation** time increment (**speed**).
- Tracks both **simulation** time and **real** elapsed time.
- Runs in a separate thread to provide **parallel execution**.
- Simple interface to get the current simulation time, reset the clock, and set the clock speed.

### Usage
#### Creating an Instance
Instantiate the GlobalClock with an optional time increment parameter (default is 1.0).
```cpp
GlobalClock clock(100); // Custom time increment (speed)
```
***Adjust the speed** of the clock by this equation (simulation time (sec)= actual time (sec) * speed)*
#### Getting Current Time
Get the current simulation time using the getCurrentTime() method.
```cpp
double simTime = clock.getCurrentTime();
```
#### Resetting the Clock
Reset the simulation time to zero.
```cpp
clock.reset();
```
#### Getting Real Elapsed Time
Get the real elapsed time since the simulation started.
```cpp
double realElapsedTime = clock.getRealElapsedTime();
```
