#include "LoadBalancer.h"

int main() {
    LoadBalancer lb;

    // Simulate server utilization (sent form servers)
    lb.trackUtil(1, 0.5);
    lb.trackUtil(2, 0.3);
    lb.trackUtil(3, 0.7);

    // Create tasks (Note Task is defined in the LoadBalancer.h)
    Task task1 = {101, 2.5};
    Task task2 = {102, 10.4};
    Task task3 = {103, 8.2};
    Task task4 = {104, 6.2};

    // Send tasks
    lb.sendTask(task1);
    lb.sendTask(task2);
    lb.sendTask(task3);
    lb.sendTask(task4);

    // Check for pending tasks
    if (lb.hasPendingTasks()) {
        std::cout << "Some tasks are still pending." << std::endl;
    } else {
        std::cout << "All tasks processed." << std::endl;
    }

    return 0;
}
