# Load-Balancer

## GlobalClock

`GlobalClock` is a C++ class designed to simulate the passage of time in a controlled and customizable manner. It allows for **parallel execution** with the main application logic, making it useful for various simulation scenarios.

### Features
- Adjustable **simulation** time increment (**speed**).
- Tracks both **simulation** time and **real** elapsed time.
- Runs in a separate thread to provide **parallel execution**.
- Simple interface to get the current simulation time, reset the clock, and set the clock speed.

### Usage
#### Creating an Instance
Instantiate the GlobalClock with an optional time increment parameter (default is 1.0).
```cpp
GlobalClock clock(100); // Custom time increment (speed)
```
***Adjust the speed** of the clock by this equation (simulation time (sec)= actual time (sec) * speed)*
#### Getting Current Time
Get the current simulation time using the getCurrentTime() method.
```cpp
double simTime = clock.getCurrentTime();
```
#### Resetting the Clock
Reset the simulation time to zero.
```cpp
clock.reset();
```
#### Getting Real Elapsed Time
Get the real elapsed time since the simulation started.
```cpp
double realElapsedTime = clock.getRealElapsedTime();
```

## LoadBalancer 
`LoadBalancer` is a C++ class designed to efficiently distribute tasks across multiple servers by tracking their utilization and assigning tasks to the least utilized server. It logs task assignments to a file for auditing and monitoring purposes. 
## Features 
- **Track Server Utilization**: Keeps track of each server's utilization. 
- **Task Queue**: Maintains a queue of tasks to be processed. 
- **Task Assignment**: Sends tasks to the server with the lowest utilization. 
- **Logging**: Logs task assignments, including server ID and current utilization.

### Usage

#### Creating an Instance
Instantiate the LoadBalancer class.
```cpp
LoadBalancer lb;
```
#### Tracking Server Utilization
Track the utilization of a server using the trackUtil method.
```cpp
lb.trackUtil(serverId, utilization);
```
*Should be sent from the server*
#### Sending Tasks
Send tasks to the load balancer using the sendTask method. Tasks should be defined using the Task struct.
```cpp
Task task = {id, time};
lb.sendTask(task);
```
#### Checking Pending Tasks
Check if there are any pending tasks in the queue.
```cpp
bool pending = lb.hasPendingTasks();
```