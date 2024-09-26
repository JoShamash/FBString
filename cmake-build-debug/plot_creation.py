import matplotlib.pyplot as plt
x = ['small', 'medium', 'large']
std_data = [0.0181526, 0.164603, 1.424]
fb_data = [0.010261, 0.0036397, 0.0837351]
plt.plot(x, std_data, 'r-', label='std::string')
plt.plot(x, fb_data, 'b-', label='FBString')
plt.xlabel('String Size')
plt.ylabel('Time (seconds)')
plt.title('Performance Comparison: creation (Iterations: 100000)')
plt.legend()
plt.savefig('plot_creation.png')
plt.close()
