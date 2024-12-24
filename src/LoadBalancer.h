#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <queue>
#include <map>
#include <string>
#include <fstream>
#include <iostream>

// Struct to represent a task
struct Task {
    int id;
    double time;
};

// LoadBalancer Class Definition
class LoadBalancer {
private:
    std::queue<Task> taskQueue;
    std::map<int, double> serverUtilization;  // Server ID -> Utilization
    std::ofstream logFile;

public:
    // Constructor
    LoadBalancer() {
        logFile.open("load_balancer_log.txt", std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file!" << std::endl;
        }
    }

    // Destructor (Closes the log file when the LoadBalancer object is destroyed, preventing resource leaks)
    ~LoadBalancer() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    // Track servers utilization
    void trackUtil(int serverId, double utilization) {
        serverUtilization[serverId] = utilization;
    }

    // Send task to the least utilized server
    void sendTask(const Task& task) {
        taskQueue.push(task); // Add the task to the queue
        
        // Find the server with the lowest utilization
        int bestServer = -1;
        double minUtilization = 1e9;

        for (const auto& [serverId, util] : serverUtilization) {
            if (util < minUtilization) {
                minUtilization = util;
                bestServer = serverId;
            }
        }

        if (bestServer != -1) {
            std::cout << "Task " << task.id << " sent to Server " << bestServer << std::endl;
            trackUtil(bestServer, minUtilization + 0.1);  // Simulate load increase
            logTask(task, bestServer);
            taskQueue.pop();
        } else {
            std::cerr << "No available servers to handle the task." << std::endl;
        }
    }

    // Log task to file
    void logTask(const Task& task, int serverId) {
        if (logFile.is_open()) {
            logFile << "Task ID: " << task.id 
                    << ", Assigned to Server: " << serverId 
                    << ", Server Current Utilization: " << serverUtilization[serverId] 
                    << "\n";
        }
    }

    // Check if tasks are pending
    bool hasPendingTasks() const {
        return !taskQueue.empty();
    }
};

#endif  // LOADBALANCER_H
