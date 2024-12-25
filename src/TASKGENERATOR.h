#ifndef TASK_GENERATOR_H
#define TASK_GENERATOR_H
#include <mutex>
#include <map>
#include <random>
#include <ctime>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <thread>
#include <atomic>
#include <chrono>
#include <functional>

class TaskGenerator {
public:
    // Constructor to initialize the Task Generator with average service time
    TaskGenerator(double averageServiceTime, std::string logFilePath, GlobalClock* globalClock)
        : rng(std::random_device{}()), 
          serviceTimeDist(1.0 / averageServiceTime),
          currentTaskID(0), 
          logFile(logFilePath, std::ios::app), 
          globalClock(globalClock),
          stopThread(false) {
        if (!logFile.is_open()) {
            throw std::runtime_error("Failed to open log file.");
        }
        lastGeneratedTask = {0, 0.0};
    }

    // Destructor to close the log file and stop the thread
    ~TaskGenerator() {
        stop();
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    // Start the task generator in a separate thread
    void start(double interArrivalTime, std::function<void(std::pair<int, double>)> taskCallback) {
        stopThread = false;
        generatorThread = std::thread([this, interArrivalTime, taskCallback]() {
            while (!stopThread.load()) {
                auto task = generateAndLogTask();
                if (taskCallback) {
                    taskCallback(task); // Send task to LoadBalancer
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(interArrivalTime * 1000)));
            }
        });
    }

    // Stop the task generator thread
    void stop() {
        stopThread = true;
        if (generatorThread.joinable()) {
            generatorThread.join();
        }
    }

    // Generate a single task manually
    std::pair<int, double> generateTask() {
        currentTaskID++; // Increment task ID
        double serviceTime = generateExponential(1.0 / serviceTimeDist.lambda());
        logTask(currentTaskID, serviceTime);
        lastGeneratedTask = {currentTaskID, serviceTime};
        return lastGeneratedTask;
    }

    // Get the last generated task
    std::pair<int, double> getLastGeneratedTask() const {
        return lastGeneratedTask;
    }

private:

    // Helper function to generate and log a task
std::pair<int, double> generateAndLogTask() {
    currentTaskID++;
    double serviceTime = generateExponential(1.0 / serviceTimeDist.lambda());
    //std::cout << "Generated Service Time: " << serviceTime << " (Exponential Distribution)\n";  // Debugging line
    logTask(currentTaskID, serviceTime);
    lastGeneratedTask = {currentTaskID, serviceTime};
    return lastGeneratedTask;
}

// Helper function to generate exponentially distributed random numbers
double generateExponential(double mean) {
    return serviceTimeDist(rng);  // Generates a random number from exponential distribution
}


    // Log task details with timestamp to a file
    void logTask(int taskID, double serviceTime) {
        if (globalClock) {
            logFile << std::fixed << std::setprecision(2);
            logFile << "[Time: " << globalClock->getCurrentTime() << "] Task ID: " << taskID 
                    << ", Service Time: " << serviceTime << " seconds\n";
        }
    }

    std::mt19937 rng; // Random number generator
    std::exponential_distribution<double> serviceTimeDist; // Exponential distribution for service time
    int currentTaskID; // Counter for unique task IDs
    std::ofstream logFile; // Log file stream
    GlobalClock* globalClock; // Pointer to global clock
    std::atomic<bool> stopThread; // Flag to stop the thread
    std::thread generatorThread; // Thread for generating tasks
    std::pair<int, double> lastGeneratedTask; // Last generated task
};

#endif // TASK_GENERATOR_H
