#include <iostream>
#include <chrono>
#include <thread>
#include "GlobalClock.h"

using namespace std;

int main() {
    double speed = 100; //Control the speed of the clock (eq -> simulation time = actual time * speed)

    // Create an instance of GlobalClock with user-defined speed
    GlobalClock clock(speed);

    auto start = chrono::steady_clock::now();

    // Print actual and simulation time for 10 seconds of actual time
    for (int i = 0; i < 10; ++i) {
        this_thread::sleep_for(chrono::seconds(1));  // Sleep for 1 second

        auto now = chrono::steady_clock::now();
        chrono::duration<double> actual_elapsed = now - start;

        cout << "Actual time: " << actual_elapsed.count() << " seconds" << endl;
        cout << "Simulation time: " << clock.getCurrentTime() << " seconds" << endl;
    }

    return 0;
}
