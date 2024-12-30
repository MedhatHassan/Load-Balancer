# Analyzer
`Analyzer` is a C++ class designed to analyze server logs and task generation logs to compute various metrics such as average delay time, average waiting time, and average queue length. It saves the analysis results to a file for further review.

## Features
- **Extract Values**: Extract specific values from log lines containing specific keywords.
- **Save Results**: Save computed results such as average delay time, average waiting time, and average queue length to a file.
- **Log Analysis**: Process server logs and task generation logs to compute relevant metrics.

## Usage
### Extracting Values
Extract a value from a line containing a specific keyword using the `extractValue` function.

```cpp
double value = extractValue(line, keyword);
```

### Saving Results
Save analysis results to a file using the saveResults function.
```cpp
saveResults(serverId, avgDelay, avgWaiting, avgQueueLength);
```
### Running the Analyzer
Run the analyzer to process server log files and the task generator log file.
```cpp
vector<string> serverLogFiles = { "server1_log.txt", "server2_log.txt", "server3_log.txt" };
string taskGeneratorLogFile = "task_log.txt";
analyzer(serverLogFiles, taskGeneratorLogFile);
```

## Example
```cpp
#include <iostream>
#include "Analyzer.h"

int main() {
    // Define log files
    vector<string> serverLogFiles = { "server1_log.txt", "server2_log.txt", "server3_log.txt" };
    string taskGeneratorLogFile = "task_log.txt";

    // Run the analyzer
    analyzer(serverLogFiles, taskGeneratorLogFile);

    std::cout << "Analysis complete. Results saved to analyzer_results.txt" << std::endl;

    return 0;
}
```
Explanation:
1. Extracting Values: Use the extractValue function to extract specific values from log lines.
2. Saving Results: Use the saveResults function to save the computed metrics to a file.
3. Running the Analyzer: Define the server log files and task generator log file, then call the analyzer function to process the logs and compute the metrics.

### Logs and results Example 
Below are examples of server log, task generator log, and analysis results:
#### Server Log
```txt
Task ID: 1 Task Finished Time: 3.09
Task ID: 2 Task Finished Time: 18.44
Average Waiting Time: 4.2
Average Queue Length: 5
```
#### Task Generator Log
```txt
[Time: 0.90] Task ID: 1, Service Time: 2.19 seconds
[Time: 1.80] Task ID: 2, Service Time: 14.64 seconds
```
#### Analysis Results
```txt
Server ID: 0, Average Delay time: 9.415, Average Waiting time: 4.2, Average Queue length: 5
```