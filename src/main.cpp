#include "GlobalClock.h"
#include "LoadBalancer.h"
#include "Analyzer.h"
#include "SERVERQUEUE.h"
#include "TASKGENERATOR.h"

using namespace std;

int main(int argc, char const *argv[])
{
    double speed = 3000;
    double averageServiceTime = 42.0;

    GlobalClock clock(speed);
    LoadBalancer LB;
    TaskGenerator TG(averageServiceTime, "task_log.txt", &clock);

    std::vector<std::unique_ptr<ServerQueue>> servers;
    for (int i = 0; i < 10; ++i) {
    servers.push_back(std::make_unique<ServerQueue>(
        i + 1, 50.0 + i * 5, 20 + i * 5, &clock,
        [&LB](std::pair<int, double> utilizationData) {
            LB.trackUtil(utilizationData.first, utilizationData.second);
        }));
    }
    
    LB.setServers(servers);
    
    //this_thread::sleep_for(chrono::milliseconds(100));//for debugging 
    TG.start(1.0, [&LB](std::pair<int, double> task) {
            Task tasklb = {task.first, task.second};
            LB.sendTask(tasklb);
        });
    
    //cout<<"currunt Time:"<<clock.getCurrentTime()<<endl;//for debugging 
    double simulationDuration = 5*3600;
    while (clock.getCurrentTime() < simulationDuration) {
        //cout<<"2"<<endl;
        //cout<<"currunt Time:"<<clock.getCurrentTime()<<endl;//for debugging 
        
        //cout<<"3"<<endl;

        this_thread::sleep_for(chrono::milliseconds(100));
        //cout<<"4"<<endl;
        
    }

    TG.stop();

    for (int i = 0; i < 10; ++i) {
        servers[i]->calculateAverageWaitTime();
        servers[i]->calculateAverageQueueOccupancy();
        servers[i]->stopProcessing();
    }


    vector<string> logFiles;
    for (int i = 0; i < 10; ++i) {
        logFiles.push_back("server" + to_string(i + 1) + "_log.txt");
    }
    analyzer(logFiles, "task_log.txt");

    return 0;
}
