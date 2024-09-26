import matplotlib.pyplot as plt
x = ['small', 'medium', 'large']
std_data = [0.002372, 0.0201443, 0.164061]
fb_data = [0.0017231, 0.0121867, 0.0802016]
plt.plot(x, std_data, 'r-', label='std::string')
plt.plot(x, fb_data, 'b-', label='FBString')
plt.xlabel('String Size')
plt.ylabel('Time (seconds)')
plt.title('Performance Comparison: find (Iterations: 100000)')
plt.legend()
plt.savefig('plot_find.png')
plt.close()
