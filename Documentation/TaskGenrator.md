# TaskGenerator
`TaskGenerator` is a C++ class designed to generate tasks with an exponential distribution of service times. It logs each task's details and can run in a separate thread to continuously generate tasks.

## Features
- **Task Generation**: Generates tasks with exponentially distributed service times.
- **Logging**: Logs task details with timestamps to a file.
- **Threaded Execution**: Runs in a separate thread to continuously generate tasks.
- **Manual Task Generation**: Allows for manual generation of tasks.

## Usage
### Creating an Instance
Instantiate the `TaskGenerator` class with the average service time, log file path, and a reference to `GlobalClock`.

```cpp
TaskGenerator(double averageServiceTime, std::string logFilePath, GlobalClock* globalClock)
```

### Starting the Task Generator
Start the task generator in a separate thread.
```cpp
taskGenerator.start(interArrivalTime, taskCallback);
```
### Stopping the Task Generator
Stop the task generator thread.
```cpp
taskGenerator.stop();
```

### Generating a Single Task
Generate a single task manually.
```cpp
std::pair<int, double> task = taskGenerator.generateTask();
```

### Getting the Last Generated Task
Retrieve the last generated task.
```cpp
std::pair<int, double> lastTask = taskGenerator.getLastGeneratedTask();
```

## Example
```cpp
#include <iostream>
#include "TaskGenerator.h"
#include "GlobalClock.h"

int main() {
    GlobalClock clock(100); // Create an instance of GlobalClock with a custom speed
    TaskGenerator taskGenerator(42.0, "task_log.txt", &clock); // Create an instance of TaskGenerator

    // Start the task generator with an inter-arrival time of 1.0 seconds and a simple task callback
    taskGenerator.start(1.0, [](std::pair<int, double> task) {
        std::cout << "Generated Task ID: " << task.first << ", Service Time: " << task.second << " seconds" << std::endl;
    });

    // Simulate running for a duration
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Stop the task generator
    taskGenerator.stop();

    // Generate a single task manually
    std::pair<int, double> task = taskGenerator.generateTask();
    std::cout << "Manually Generated Task ID: " << task.first << ", Service Time: " << task.second << " seconds" << std::endl;

    std::cout << "Last Generated Task ID: " << taskGenerator.getLastGeneratedTask().first << std::endl;

    std::cout << "Simulation complete." << std::endl;

    return 0;
}
```

Explanation:
1. Creating an Instance: Initialize the TaskGenerator with the average service time, log file path, and GlobalClock.
2. Starting the Task Generator: Start the generator in a separate thread with a specified inter-arrival time and a task callback.
3. Stopping the Task Generator: Stop the generator after a simulation duration.
4. Manual Task Generation: Generate a single task manually and retrieve the last generated task.