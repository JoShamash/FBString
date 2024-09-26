import matplotlib.pyplot as plt
x = ['small', 'medium', 'large']
std_data = [4.7e+06, 1.52e+07, 1.952e+08]
fb_data = [2.5e+06, 7e+06, 5.2e+07]
plt.plot(x, std_data, 'r-', label='std::string')
plt.plot(x, fb_data, 'b-', label='FBString')
plt.xlabel('String Size')
plt.ylabel('Memory Usage (bytes)')
plt.title('Performance Comparison: memory (Iterations: 100000)')
plt.legend()
plt.savefig('plot_memory.png')
plt.close()
