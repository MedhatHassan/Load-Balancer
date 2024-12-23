#ifndef GLOBALCLOCK_H
#define GLOBALCLOCK_H

#include <chrono>
#include <thread>
#include <atomic>
#include <iostream>

using namespace std;

class GlobalClock {
public:
    // Constructor with adjustable time increment
    GlobalClock(double increment = 1.0) 
        : currentTime(0), running(true), timeIncrement(increment), clockThread(&GlobalClock::run, this) {}

    ~GlobalClock() {
        running = false;
        if (clockThread.joinable()) {
            clockThread.join();
        }
    }

    // Get the current simulation time
    double getCurrentTime() const {
        return currentTime;
    }

    // Reset the clock
    void reset() {
        currentTime = 0;
        startTime = chrono::steady_clock::now();
    }

    // Get the real elapsed time since the simulation started
    double getRealElapsedTime() const {
        auto now = chrono::steady_clock::now();
        chrono::duration<double> elapsed = now - startTime;
        return elapsed.count();
    }

    // Set the clock speed by adjusting time increment
    void setClockSpeed(double speed) {
        if (speed > 0) {
            timeIncrement = speed;
        }
    }

private:
    atomic<double> currentTime;  // Simulation time
    chrono::steady_clock::time_point startTime;  // Start time of the simulation
    atomic<bool> running;
    atomic<double> timeIncrement;  // How much time to add per tick
    thread clockThread;

    void run() {
        while (running) {
            this_thread::sleep_for(chrono::milliseconds(1000));  // Tick every 1 ms
            currentTime = currentTime + timeIncrement;  // Adjust simulation time by timeIncrement
        }
    }
};

#endif // GLOBALCLOCK_H
