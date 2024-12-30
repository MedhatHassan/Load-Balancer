#include "GlobalClock.h"
#include "LoadBalancer.h"
#include "Analyzer.h"
#include "SERVERQUEUE.h"
#include "TASKGENERATOR.h"

using namespace std;

int main(int argc, char const *argv[]) {
    double speed = 10; // Control the speed of the clock (eq -> simulation time = actual time * speed)
    double averageServiceTime = 40.0; // Control the average service time of the 
    int NumberofServers = 3; // Conrtol number of servers used

    GlobalClock clock(speed);// Create clock instance with the controled speed
    LoadBalancer LB;// Create load balacer server instance
    TaskGenerator TG(averageServiceTime, "task_log.txt", &clock);// Create task genrator server instance (average service time - log file - clock reference)

    vector<shared_ptr<ServerQueue>> servers;// Create a vector of server instance .
    for (int i = 0; i < NumberofServers; ++i) {
    // (service id - server processing power - qeue size - clock reference - utilization call back function)
        servers.push_back(make_shared<ServerQueue>(
            i + 1, 15.0 + i * 5, 10 + i * 5, &clock,
            [&LB](pair<int, double> utilizationData) {
                LB.trackUtil(utilizationData.first, utilizationData.second);
            }));
    }
    
    LB.setServers(servers);//Conect all the servers to the load balacer
    
    //(Task generate frequency (inter arrival time)- task call back function)
    TG.start(0.3, [&LB](pair<int, double> task) {
        Task tasklb = {task.first, task.second};
        LB.sendTask(tasklb);
    });
    
    double simulationDuration = 2 * 3600;//Set a simulation duration for 2h 
    while (clock.getCurrentTime() < simulationDuration) {
        this_thread::sleep_for(chrono::milliseconds(100));// Avoid busy waiting
    }

    TG.stop(); //Manual stop task genrator server 

    for (int i = 0; i < NumberofServers; ++i) {
        servers[i]->calculateAverageWaitTime();// calculate average waitTime for each server
        servers[i]->calculateAverageQueueOccupancy();// calculate average queue occupancy for each server
        servers[i]->stopProcessing(); // Manual stop each server to avoid destructor being called automatically
    }

    vector<string> logFiles;
    for (int i = 0; i < NumberofServers; ++i) {
        logFiles.push_back("server" + to_string(i + 1) + "_log.txt");
    }
    analyzer(logFiles, "task_log.txt");// Analyze the simulation results

    return 0;
}
