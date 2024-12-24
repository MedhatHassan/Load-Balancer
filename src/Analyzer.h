#ifndef Analyzer_H
#define Analyzer_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

// Helper function to extract a value from a line containing a specific keyword
double extractValue(const string& line, const string& keyword) {
    size_t pos = line.find(keyword);
    if (pos != string::npos) {
        stringstream ss(line.substr(pos + keyword.length()));
        double value;
        ss >> value;
        return value;
    }
    return 0.0;
}

// Helper function to save results to a file
void saveResults(int serverId, double avgDelay, double avgWaiting, double avgQueueLength) {
    ofstream resultsFile("analyzer_results.txt", ios::app);
    if (!resultsFile.is_open()) {
        cerr << "Failed to open log file!" << endl;
        return;
    }
    resultsFile << "Server ID: " << serverId
                << ", Average Delay time: " << avgDelay
                << ", Average Waiting time: " << avgWaiting
                << ", Average Queue length: " << avgQueueLength
                << "\n";
    resultsFile.close();
}

inline void analyzer(vector<string> serverLogFiles, string taskGeneratorLogFile) {
    int serverId = 0;

    // Map to store task generation times from TaskGeneratorLogFile
    map<int, double> taskGenTimes;
    
    // Read Task Generator Log to populate taskGenTimes
    ifstream generatorLog(taskGeneratorLogFile);
    if (!generatorLog.is_open()) {
        cerr << "Failed to open Task Generator Log: " << taskGeneratorLogFile << endl;
        return;
    }

    string line;
    while (getline(generatorLog, line)) {
        if (line.find("Task ID:") != string::npos && line.find("Time:") != string::npos) {
            int taskId;
            double genTime;
            size_t timePos = line.find("Time: ");
            size_t idPos = line.find("Task ID: ");
            
            // Extract Task Generation Time
            if (timePos != string::npos) {
                stringstream ss(line.substr(timePos + 6));
                ss >> genTime;
            }

            // Extract Task ID
            if (idPos != string::npos) {
                stringstream ss(line.substr(idPos + 8));
                ss >> taskId;
            }

            taskGenTimes[taskId] = genTime;  // Store Task ID and Generation Time
        }
    }
    generatorLog.close();

    // Process each server log file
    for (const auto& file : serverLogFiles) {
        ifstream serverLogFile(file);
        if (!serverLogFile.is_open()) {
            cerr << "Failed to open server log file: " << file << endl;
            continue;
        }

        double totalDelay = 0.0;
        int taskCount = 0;
        double avgWaiting = 0.0;
        int avgQueueLength = 0;

        // Read and analyze the server log
        while (getline(serverLogFile, line)) {
            // Extract Average Waiting Time
            if (line.find("Average Waiting Time") != string::npos) {
                avgWaiting = extractValue(line, "Average Waiting Time:");
            }

            // Extract Average Queue Length
            if (line.find("Average Queue Length") != string::npos) {
                avgQueueLength = static_cast<int>(extractValue(line, "Average Queue Length:"));
            }

            // Match Task ID and Calculate Delay
            if (line.find("Task ID:") != string::npos && line.find("Task Finished Time") != string::npos) {
                int taskId;
                double finishTime;
                size_t timePos = line.find("Time: ");
                size_t idPos = line.find("Task ID: ");
                
                // Extract Finished Time
                if (timePos != string::npos) {
                    stringstream ss(line.substr(timePos + 6));
                    ss >> finishTime;
                }

                // Extract Task ID
                if (idPos != string::npos) {
                    stringstream ss(line.substr(idPos + 8));
                    ss >> taskId;
                }

                // Calculate delay if task ID exists in generator log
                if (taskGenTimes.find(taskId) != taskGenTimes.end()) {
                    double startTime = taskGenTimes[taskId];
                    totalDelay += (finishTime - startTime);
                    taskCount++;
                }
            }
        }

        serverLogFile.close();

        // Calculate average delay
        double avgDelay = (taskCount > 0) ? (totalDelay / taskCount) : 0.0;

        // Save results
        saveResults(serverId, avgDelay, avgWaiting, avgQueueLength);
        serverId++;
    }
}

#endif  // Analyzer_H
