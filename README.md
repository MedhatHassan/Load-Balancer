# Load Balancer

This project is a C++ simulation of a load balancing system designed to efficiently distribute tasks across multiple servers. It models **real-world** server environments with task queues, server utilization tracking, and performance analysis.

## Table of Contents

1. [Features](#features)
2. [Architecture](#architecture)
3. [Getting Started](#getting-started)
   - [Prerequisites](#prerequisites)
   - [Installation](#installation)
   - [Running the Simulation](#running-the-simulation)
   - [Configuration](#configuration)
4. [Log Files](#log-files)
5. [Analyzer Output](#analyzer-output)
6. [Documentation](#documentation)
7. [Contributing](#contributing)
8. [License](#license)

---
## Features
- **Realistic Load Balancing**
  - Tasks are distributed across multiple servers based on their **current utilization**, minimizing queue times and improving throughput.
- **Dynamic Task Generation**
  - The task generator produces tasks based on **exponential distribution** to simulate real-world workload at varying intervals with adjustable service times, simulating fluctuating workloads.
- **Server Queuing and Processing**
  - Each server maintains a queue, processes tasks sequentially, and tracks metrics such as average wait time, queue length, and utilization.
- **Global Time Management**
  - A global clock manages the progression of simulation time, controlling task scheduling and execution.
- **Performance Analysis**
  - The analyzer computes **key performance indicators (KPIs)** such as average delay, wait times, and queue lengths, summarizing each server efficiency post-simulation.
- **Configurable Parameters**: 
  - Users can adjust the simulation speed, average service time, number of servers, task generation frequency, and simulation duration. Additionally, each server’s processing power, queue size, and utilization tracking can be configured to **optimize resource usage and load balancing**.
---
## Architecture
#### The project is structured into modular components, ensuring **maintainability** and **scalability**.

![alt text](<Load Balancer Flowchart.png>)

### Key Components:

- #### GlobalClock
  - Manages and synchronizes the simulation time.

- #### LoadBalancer
  - Routes tasks to the least utilized server.

- #### ServerQueue
  - Represents individual servers that process incoming tasks.

- #### TaskGenerator
  - Creates tasks at specified intervals and forwards them to the load balancer.

- #### Analyzer
  - Parses server logs and computes performance metrics.

---
## Getting Started

### Prerequisites
- C++11 or higher
- g++
- Standard C++ libraries

### Installation

1. Clone the repository:
```bash
git clone https://github.com/MedhatHassan/Load-Balancer.git
```
2. Navigate to the project directory:
```bash
cd Load-Balancer/src
```
3. Build the project using g++:
```bash
g++ mian.cpp -o simulation
```

### Running the Simulation
After building the project, execute the simulation with:
```bash
./simulation
```
The simulation will run for a predefined duration, generating log files and results file.

### Configuration
#### Modify the main.cpp file to adjust the following parameters:

- Simulation Speed: Adjust the `speed` variable.
- Average Service Time: Set the `averageServiceTime`.
- Number of Servers: Update the `NumberofServers` variable.
- Task generate frequency: Update the value `TG.start(0.3)`
- Simulation Duration: Update the value `simulationDuration` in seconds.
---
### Log Files
The simulation generates several log files:

- **Task Log** (`task_log.txt`): Logs task creation time and service time.
- **Server Logs** (`serverX_log.txt`): Logs server activity, including task queue size and utilization.
- **Analyzer Results** (`analyzer_results.txt`): Summarizes the performance metrics of each server post-simulation.
---
### Analyzer Output
The analyzer calculates:
- **Average Delay Time**: The time difference between task generation and completion.
- **Average Waiting Time**: The time tasks spend waiting in the queue.
- **Average Queue Length**: The average number of tasks in the server queue.

Example Output:
```bash
Server ID: 0, Average Delay time: 12.45, Average Waiting time: 48.41, Average Queue length: 4
Server ID: 1, Average Delay time: 15.76, Average Waiting time: 87.74, Average Queue length: 8
Server ID: 2, Average Delay time: 19.32, Average Waiting time: 132.29, Average Queue length: 12
```
---
## Documentation
#### Detailed component (header) documentation can be found at the following links:

- [GlobalClock Documentation](Documentation/GlobalClock.md)
- [LoadBalancer Documentation](Documentation/LoadBalancer.md)
- [ServerQueue Documentation](Documentation/ServerQueue.md)
- [TaskGenerator Documentation](Documentation/TaskGenrator.md)
- [Analyzer Documentation](Documentation/Analyzer.md)

---
## Contributing

Contributions are welcome! Please fork the repository and create a pull request with your changes. Ensure that your code adheres to the project's coding standards and includes appropriate tests.

---
## License

This project is licensed under the [MIT License](LICENSE). See the LICENSE file for details.

*For questions or issues, please open an issue on the GitHub repository.*

