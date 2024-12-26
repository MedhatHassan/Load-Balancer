#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <queue>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "SERVERQUEUE.h"  // Include the full definition of ServerQueue

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
    std::vector<std::unique_ptr<ServerQueue>> servers; // Array of server instances
    std::ofstream logFile;

public:
    // Constructor
    LoadBalancer() {
        logFile.open("load_balancer_log.txt", std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file!" << std::endl;
        }
    }

    // Destructor
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
    	taskQueue.push(task);  // Add the task to the queue

    // Find the server with the lowest utilization
    	int bestServer = -1;
    	double minUtilization = 1e9;

    	for (const auto& [serverId, util] : serverUtilization) {
        	if (util < minUtilization) {
            	minUtilization = util;
            	bestServer = serverId;
        	}
    	}

    	// Check if all servers are fully utilized
    	if (minUtilization >= 1.0) {  // Assuming utilization is between 0.0 and 1.0 (100%)
        	std::cerr << "All servers at maximum capacity. Task " 
                  << task.id << " queued for later processing." << std::endl;
        	return;
    	}

    	// Send task to the best available server
    	if (bestServer != -1) {
            std::cout << "Task " << task.id << " sent to Server " << bestServer << std::endl;
        	servers[bestServer - 1]->addTask(task.id, task.time); 
        	logTask(task, bestServer);
        	taskQueue.pop();
    	}else {
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

    // Set servers array
    void setServers(std::vector<std::unique_ptr<ServerQueue>>& serverArray) {
        servers.clear();
        for (auto& server : serverArray) {
            servers.push_back(std::move(server));
    }
}
};

#endif  // LOADBALANCER_H
