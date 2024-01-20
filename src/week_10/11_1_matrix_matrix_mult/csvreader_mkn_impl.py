import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('performance.csv')

# Filter data for the "MNK" implementation
subset = df[df['Implementation'] == 'MKN']

# Extract the data
size = subset['M=N=K']
gflops = subset['GFLOPS']

# Create the plot
plt.figure(figsize=(10, 6))
plt.plot(size, gflops, marker='o', linestyle='-', color='b')
plt.xscale('log', base=2)  # Set x-axis to logarithmic scale
plt.yscale('log')
plt.title('Matrix-Matrix Multiplication - MKN Implementation')
plt.xlabel('Size')
plt.ylabel('GFLOPS')
plt.grid(True, which="both", ls="--", c='gray')
plt.tight_layout()

# Save the plot as a PNG
plt.savefig('mkn_plot.png', format='png')

# Show the plot
plt.show()
