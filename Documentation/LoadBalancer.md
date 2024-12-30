# LoadBalancer
`LoadBalancer` is a C++ class designed to distribute tasks efficiently across multiple servers by tracking their utilization and assigning tasks to the least utilized server. It logs task assignments to a file for auditing and monitoring purposes.

## Features
- **Track Server Utilization**: Keeps track of each server's utilization.
- **Task Queue**: Maintains a queue of tasks to be processed.
- **Task Assignment**: Sends tasks to the server with the lowest utilization.
- **Logging**: Logs task assignments, including server ID and current utilization.

## Usage
### Creating an Instance
Instantiate the `LoadBalancer` class.

```cpp
LoadBalancer lb;
``` 

### Tracking Server Utilization
Track the utilization of a server using the trackUtil method.
```cpp
lb.trackUtil(serverId, utilization);
```

### Sending Tasks
Send tasks to the load balancer using the sendTask method. Tasks should be defined using the Task struct.

```cpp
Task task = {id, time};
lb.sendTask(task);
```

### Checking Pending Tasks
Check if there are any pending tasks in the queue.
``` cpp
bool pending = lb.hasPendingTasks();
```
### Setting Servers
Assign the servers to the load balancer.

```cpp
std::vector<std::shared_ptr<ServerQueue>> servers = /* initialize servers */;
lb.setServers(servers); 
```
## Example
```cpp
#include <iostream>
#include "LoadBalancer.h"
#include "GlobalClock.h"
#include "ServerQueue.h"

int main() {
    GlobalClock clock(100); // Create an instance of GlobalClock with a custom speed
    LoadBalancer lb; // Create an instance of LoadBalancer

    // Initialize servers and add them to the load balancer
    std::vector<std::shared_ptr<ServerQueue>> servers;
    for (int i = 0; i < 3; ++i) {
        double processingPower = std::min(50.0 + i * 10, 100.0);
        servers.push_back(std::make_shared<ServerQueue>(
            i + 1, processingPower, 20 + i * 5, &clock,
            [&lb](std::pair<int, double> utilizationData) {
                lb.trackUtil(utilizationData.first, utilizationData.second);
            }));
    }
    lb.setServers(servers);

    // Create and send tasks
    for (int i = 0; i < 5; ++i) {
        Task task = {i, 1.0 * (i + 1)};
        lb.sendTask(task);
    }

    // Check for pending tasks
    if (lb.hasPendingTasks()) {
        std::cout << "There are pending tasks in the queue." << std::endl;
    } else {
        std::cout << "All tasks have been assigned." << std::endl;
    }

    return 0;
}
```
Explanation:
1. Creating an Instance: Initialize the LoadBalancer and GlobalClock.
2. Setting Servers: Initialize servers with dynamic processing power and assign them to the LoadBalancer.
3. Sending Tasks: Create tasks and send them to the LoadBalancer.
4. Checking Pending Tasks: Verify if there are any pending tasks in the queue.