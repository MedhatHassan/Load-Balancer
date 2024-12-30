# GlobalClock
`GlobalClock` is a C++ class designed to simulate the passage of time in a controlled and customizable manner. It allows for **parallel execution** with the main application logic, making it useful for various simulation applications.

## Features
- Adjustable **simulation** time increment (**speed**).
- Tracks both **simulation** time and **real** elapsed time.
- Runs in a separate thread to provide **parallel execution**.
- Simple interface to get the current simulation time, reset the clock, and set the clock speed.

## Usage
### Creating an Instance
Instantiate the GlobalClock with an optional time increment parameter (default is 1.0).
```cpp
GlobalClock clock(100); // Custom time increment (speed)
```
***Adjust the speed** of the clock by this equation (simulation time (sec)= actual time (sec) * speed)*
### Getting Current Time
Get the current simulation time using the getCurrentTime() method.
```cpp
double simTime = clock.getCurrentTime();
```
### Resetting the Clock
Reset the simulation time to zero.
```cpp
clock.reset();
```
### Getting Real Elapsed Time
Get the real elapsed time since the simulation started.
```cpp
double realElapsedTime = clock.getRealElapsedTime();
```

## Example
```cpp
#include <iostream>
#include "GlobalClock.h"

int main() {
    GlobalClock clock(100); // Create an instance of GlobalClock with a custom speed

    // Simulate some activity
    std::cout << "Starting simulation..." << std::endl;

    // Run for a specific duration (e.g., simulate 10 seconds of real-time activity)
    double simulationDuration = 10.0;
    while (clock.getRealElapsedTime() < simulationDuration) {
        std::cout << "Current simulation time: " << clock.getCurrentTime() << " seconds" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep to prevent busy-waiting
    }

    std::cout << "Simulation ended." << std::endl;
    std::cout << "Final simulation time: " << clock.getCurrentTime() << " seconds" << std::endl;
    std::cout << "Real elapsed time: " << clock.getRealElapsedTime() << " seconds" << std::endl;

    return 0;
}
```
This example demonstrates how to: 
1. Create an instance of `GlobalClock`. 
2. Run a simulation for a specific duration. 
3. Print the current simulation time at regular intervals. 
4. Display the final simulation time and real elapsed time at the end.