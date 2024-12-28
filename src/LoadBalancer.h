#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <queue>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include "SERVERQUEUE.h"

// Struct to represent a task
struct Task {
    int id;
    double time;
};

// Forward Declaration
class ServerQueue;

class LoadBalancer {
private:
    std::queue<Task> taskQueue;
    std::map<int, double> serverUtilization;  // Server ID -> Utilization
    std::vector<std::shared_ptr<ServerQueue>> servers; // Array of server instances
    std::ofstream logFile;

public:
    LoadBalancer() {
        logFile.open("load_balancer_log.txt", std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file!" << std::endl;
        }
    }

    ~LoadBalancer() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void trackUtil(int serverId, double utilization) {
        serverUtilization[serverId] = utilization;
    }

    void sendTask(const Task& task) {
        taskQueue.push(task);

        int bestServer = -1;
        double minUtilization = 1e9;

        for (const auto& [serverId, util] : serverUtilization) {
            if (util < minUtilization) {
                minUtilization = util;
                bestServer = serverId;
            }
        }

        if (minUtilization >= 1.0) {
            std::cerr << "All servers at maximum capacity. Task " 
                      << task.id << " queued for later processing." << std::endl;
            return;
        }

        if (bestServer != -1) {
            std::cout << "Task " << task.id << " sent to Server " << bestServer << std::endl;
            servers[bestServer - 1]->addTask(task.id, task.time); 
            logTask(task, bestServer);
            taskQueue.pop();
        } else {
            std::cerr << "No available servers to handle the task." << std::endl;
        }
    }

    void logTask(const Task& task, int serverId) {
        if (logFile.is_open()) {
            logFile << "Task ID: " << task.id 
                    << ", Assigned to Server: " << serverId 
                    << ", Server Current Utilization: " << serverUtilization[serverId] 
                    << "\n";
        }
    }

    bool hasPendingTasks() const {
        return !taskQueue.empty();
    }

    void setServers(const std::vector<std::shared_ptr<ServerQueue>>& serverArray) {
        servers = serverArray;
    }
};

#endif  // LOADBALANCER_H
