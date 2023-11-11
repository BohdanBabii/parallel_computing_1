import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('memory_bandwidth.csv')

# Extract the data
array_sizes = df['Array Size (bytes)']
bandwidths = df['Bandwidth (GB/s)']

# Create the plot
plt.figure(figsize=(10, 6))
plt.plot(array_sizes, bandwidths, marker='o', linestyle='-', color='b')
plt.xscale('log', base=2)  # Set x-axis to logarithmic scale
plt.yscale('log')
plt.title('Memory Bandwidth vs Array Size')
plt.xlabel('Array Size (bytes)')
plt.ylabel('Bandwidth (GB/s)')
plt.grid(True, which="both", ls="--", c='gray')
plt.tight_layout()

# Save the plot as a PDF
plt.savefig('memory_bandwidth_plot.png', format='png')

# Show the plot
plt.show()

