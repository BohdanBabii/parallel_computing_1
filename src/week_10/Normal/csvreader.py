import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file into a DataFrame
df = pd.read_csv('performance.csv')

# Group by Implementation
grouped = df.groupby('M=N=K')

# Create a single figure outside the loop
plt.figure(figsize=(10, 6))

# Group by Implementation and plot separate lines for each
for implementation, group in grouped:
    # Plot the data
    plt.plot(group['Implementation'], group['GFLOPS'], marker='o', label=f'{implementation}')

# Set labels and title
plt.xlabel('Matrix Size')
plt.ylabel('GFLOPS')
plt.title('Matrix-Matrix Multiplication Performance')
plt.xscale('log', base=2)  # Use logarithmic scale for better visualization

# Set x-axis ticks to specific values
plt.xticks([2, 4, 8, 16, 32, 64, 128, 256, 512, 1024])

# Add a legend
plt.legend()

# Save the plot as a PNG image
plt.savefig('performance_plot_all.png')

# Show the plot (optional)
plt.show()
