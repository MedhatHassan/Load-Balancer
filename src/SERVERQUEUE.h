#ifndef SERVER_QUEUE_H
#define SERVER_QUEUE_H

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <functional>
#include <vector>
#include <fstream>
#include <cmath>
#include "GlobalClock.h"

class ServerQueue {
private:
    struct Task {
        int taskID;
        double serviceTime;
        double arrivalTime;
        double finishTime;
    };

    int serverID;
    double processingPower;
    int fixedQueueSize;
    std::queue<Task> taskQueue;
    GlobalClock* globalClock;
    std::mutex queueMutex;
    std::condition_variable taskNotifier;
    std::atomic<bool> isRunning;
    std::thread processingThread;

    double totalWaitTime = 0.0;
    int processedTasks = 0;

    int totalQueueSize = 0;
    int queueSizeUpdates = 0;

    std::function<void(std::pair<int, double>)> utilizationCallback;

    std::ofstream logFile;
    static std::mutex terminalMutex;

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(terminalMutex);

        // Output to the terminal in real-time
        std::cout << message << std::endl;

        // Write to the file
        if (logFile.is_open()) {
            logFile << message << std::endl;
        }
    }

    void recordQueueSize() {
        totalQueueSize += taskQueue.size();
        ++queueSizeUpdates;
    }

    void processTasks() {
        while (isRunning) {
            std::unique_lock<std::mutex> lock(queueMutex);
            taskNotifier.wait(lock, [this]() { return !taskQueue.empty() || !isRunning; });

            if (!isRunning) break;

            Task task = taskQueue.front();
            taskQueue.pop();

            recordQueueSize();
            log("Server " + std::to_string(serverID) + 
                " current task queue size: " + std::to_string(taskQueue.size()));

            lock.unlock();

            double currentTime = globalClock->getCurrentTime();
            double waitTime = currentTime - task.arrivalTime;
            totalWaitTime += waitTime;
            processedTasks++;

            log("Server " + std::to_string(serverID) + 
                " is processing task " + std::to_string(task.taskID) +
                " with service time: " + std::to_string(task.serviceTime) +
                " seconds, waited: " + std::to_string(waitTime) +
                " seconds at time: " + std::to_string(currentTime) + " secs.");

            double adjustedServiceTime = task.serviceTime / processingPower;

            double startSimProcessingTime = globalClock->getCurrentTime();
            double endSimProcessingTime = startSimProcessingTime + adjustedServiceTime;

            while(globalClock->getCurrentTime() < endSimProcessingTime) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Small sleep to avoid busy waiting
            }

            task.finishTime = globalClock->getCurrentTime();

            log("TaskID: " + std::to_string(task.taskID) + 
                " Task Finished Time: " + std::to_string(task.finishTime) + " secs.");

            calculateQueueUtilization();

        }
    }

    void calculateQueueUtilization() {
        double occupiedQueueUtilization = static_cast<double>(taskQueue.size()) / fixedQueueSize;

        double totalServiceTime = 0.0;
        std::queue<Task> tempQueue = taskQueue;
        while (!tempQueue.empty()) {
            const Task& task = tempQueue.front();
            totalServiceTime += task.serviceTime;
            tempQueue.pop();
        }

        double serviceTimeUtilization = totalServiceTime / (processingPower * fixedQueueSize);
        double finalUtilization = (occupiedQueueUtilization + serviceTimeUtilization) / 2.0;
        log( "Server " + std::to_string(serverID) + " utilization updated: "  + std::to_string(finalUtilization * 100 )+ "%.");


        if (utilizationCallback) {
            utilizationCallback(std::make_pair(serverID, finalUtilization));
        }
    }

public:
    ServerQueue()
        : serverID(1), processingPower(10), fixedQueueSize(20), globalClock(nullptr),
        utilizationCallback(nullptr), isRunning(false), totalQueueSize(0), queueSizeUpdates(0) {

        // Optional: Open a default log file
        logFile.open("default_log.txt", std::ios::out | std::ios::app);
    }

    ServerQueue(int id, double power, int queueSize, GlobalClock* clock, std::function<void(std::pair<int, double>)> utilizationCallback)
        : serverID(id), globalClock(clock), utilizationCallback(utilizationCallback), isRunning(true),
          totalQueueSize(0), queueSizeUpdates(0), fixedQueueSize(queueSize) {

        processingPower = std::clamp(power, 1.0, 100.0);

        logFile.open("server" + std::to_string(serverID) + "_log.txt", std::ios::out | std::ios::app);

        processingThread = std::thread(&ServerQueue::processTasks, this);
        calculateQueueUtilization();
    }

    ~ServerQueue() {
        stopProcessing();
        if (processingThread.joinable()) {
            processingThread.join();
        }

        if (logFile.is_open()) {
            logFile.close();
        }
    }

    bool addTask(int taskID, double serviceTime) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            double arrivalTime = globalClock->getCurrentTime();
            taskQueue.push(Task{taskID, serviceTime, arrivalTime});

            log("Server " + std::to_string(serverID) + 
                " added task " + std::to_string(taskID) +
                " with service time: " + std::to_string(serviceTime) +
                " at time: " + std::to_string(arrivalTime) + " secs.");

            recordQueueSize();
            log("Server " + std::to_string(serverID) + 
                " current task queue size: " + std::to_string(taskQueue.size()));
            calculateQueueUtilization();
        }

        taskNotifier.notify_one();
        return true;
    }

    void stopProcessing() {
        isRunning = false;
        taskNotifier.notify_all();

        if (processingThread.joinable()) {
            processingThread.join();
        }
    }

    void calculateAverageWaitTime() {
        double averageWaitTime = (processedTasks > 0) ? totalWaitTime / processedTasks : 0.0;
        log("Server " + std::to_string(serverID) + 
            " Average Waiting Time: " + std::to_string(averageWaitTime) + " seconds.");
    }

    void calculateAverageQueueOccupancy() {
        double averageOccupancy = (queueSizeUpdates > 0) ? static_cast<double>(totalQueueSize) / queueSizeUpdates : 0.0;
        int flooredOccupancy = static_cast<int>(std::floor(averageOccupancy));
        log("Server " + std::to_string(serverID) + 
            " Average Queue Length: " + std::to_string(flooredOccupancy) + " tasks.");
    }
};

std::mutex ServerQueue::terminalMutex;

#endif // SERVER_QUEUE_H
