import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('performance.csv')

# Unique implementations
implementations = df['Implementation'].unique()

# Create the plot
plt.figure(figsize=(10, 6))

# Iterate over implementations and plot
for impl in implementations:
    subset = df[df['Implementation'] == impl]
    size = subset['M=K']
    gflops = subset['GFLOPS']
    plt.plot(size, gflops, marker='o', linestyle='-', label=impl)

plt.xscale('log', base=2)  # Set x-axis to logarithmic scale
plt.yscale('log')
plt.title('Matrix-Matrix Multiplication')
plt.xlabel('Size')
plt.ylabel('GFLOPS')
plt.legend()
plt.grid(True, which="both", ls="--", c='gray')
plt.tight_layout()

# Save the plot as a PNG
plt.savefig('blocked.png', format='png')

# Show the plot
plt.show()
