#!/usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
import sys
data1 = np.genfromtxt(sys.argv[1], delimiter=',', names=['x', 'y'])
data2 = np.genfromtxt(sys.argv[2], delimiter=',', names=['x', 'y'])
plt.scatter(data1['x'], data1['y'], color='r', s=0.1)
plt.scatter(data2['x'], data2['y'], color='b', s=10.0, alpha = 0.5)

plt.axis([0, data2['x'][-1], data2['y'].min() - 0.5*data2['y'].min(), data2['y'].max() * 1.1])
plt.show()
