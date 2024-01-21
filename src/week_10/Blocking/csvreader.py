import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file into a DataFrame
df = pd.read_csv('matrix_matrix_blocking.csv')

# Create a single figure outside the loop
plt.figure(figsize=(10, 6))

# Plot the data
plt.plot(df['Implementation'], df['GFLOPS'], marker='o', label='GFLOPS')

# Set labels and title
plt.xlabel('Matrix Size')
plt.ylabel('GFLOPS')
plt.title('Matrix-Matrix Blocking Multiplication Performance')
plt.xscale('log', base=2)  # Use logarithmic scale for better visualization

# Set x-axis ticks to specific values
plt.xticks([32, 64, 128, 256, 512, 1024, 2048, 4096, 8192])

# Add a legend
plt.legend()

# Save the plot as a PNG image
plt.savefig('matrix_matrix_blocking_performance_plot.png')

# Show the plot (optional)
plt.show()
