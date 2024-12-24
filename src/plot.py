import matplotlib.pyplot as plt

# Read and parse the results file
results = {}
with open("plot_test.txt", "r") as file:
    for line in file:
        parts = line.split(", ")
        server_id = int(parts[0].split(": ")[1])
        avg_delay = float(parts[1].split(": ")[1])
        avg_waiting = float(parts[2].split(": ")[1])
        avg_queue_length = int(parts[3].split(": ")[1])
        
        results[server_id] = (avg_delay, avg_waiting, avg_queue_length)

# Extract data for plotting
server_ids = list(results.keys())
avg_delays = [results[i][0] for i in server_ids]
avg_waitings = [results[i][1] for i in server_ids]
avg_queue_lengths = [results[i][2] for i in server_ids]

# Plotting
x = range(len(server_ids))
plt.bar(x, avg_delays, width=0.3, label="Avg Delay")
plt.bar([p + 0.3 for p in x], avg_waitings, width=0.3, label="Avg Waiting")
plt.bar([p + 0.6 for p in x], avg_queue_lengths, width=0.3, label="Avg Queue Length")

plt.xlabel("Server ID")
plt.ylabel("Metrics")
plt.title("Servers Performance Metrics")
plt.xticks([p + 0.3 for p in x], server_ids)
plt.legend()
plt.show()
