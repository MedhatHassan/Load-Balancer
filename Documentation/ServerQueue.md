# ServerQueue
`ServerQueue` is a C++ class designed to manage a queue of tasks assigned to a server. It processes tasks, tracks server utilization, and logs relevant information for each task and server.

## Features
- **Task Management**: Handles task addition, processing, and queue management.
- **Utilization Tracking**: Calculates and updates server utilization.
- **Logging**: Logs task details and server statistics.
- **Average Calculations**: Computes average wait time and average queue occupancy.

## Usage
### Creating an Instance
Instantiate the `ServerQueue` class with **server ID**, **processing power**, **queue size**, a reference to `GlobalClock`, and a utilization callback function.

```cpp
ServerQueue(int id, double power, int queueSize, GlobalClock* clock, std::function<void(std::pair<int, double>)> utilizationCallback)
```

### Adding a Task
Add a task to the server's queue using the addTask method.
```cpp
bool success = serverQueue.addTask(taskID, serviceTime);
```
### Stopping Processing
Stop the processing of tasks.
```cpp
serverQueue.stopProcessing();
```
### Calculating Averages
Calculate the average wait time and average queue occupancy.
```cpp
serverQueue.calculateAverageWaitTime();
serverQueue.calculateAverageQueueOccupancy();
```

## Example

```cpp
#include <iostream>
#include "ServerQueue.h"
#include "GlobalClock.h"

int main() {
    GlobalClock globalClock(100);
    ServerQueue serverQueue1(1, 50.0, 20, &globalClock, [](std::pair<int, double> utilizationData) {
        int serverID = utilizationData.first;
        double utilization = utilizationData.second;
        std::cout << "Server " << serverID << " utilization: " << utilization * 100 << "%" << std::endl;
    });

    serverQueue1.addTask(1, 3.0); // Add task with 3 seconds service time
    serverQueue1.addTask(2, 1.5); // Add another task

    std::this_thread::sleep_for(std::chrono::seconds(10)); // Let the server process tasks

    serverQueue1.calculateAverageWaitTime();
    serverQueue1.calculateAverageQueueOccupancy();

    serverQueue1.stopProcessing(); // Stop processing
    return 0;
}

```

Explanation:
1. Creating an Instance: Initialize the ServerQueue with server ID, processing power, queue size, GlobalClock, and a utilization callback.
2. Adding Tasks: Add tasks to the server queue.
3. Stopping Processing: Stop task processing after a simulation duration.
4. Calculating Averages: Compute and log average wait time and average queue occupancy.