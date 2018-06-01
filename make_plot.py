#!/usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
import sys
data1 = np.genfromtxt(sys.argv[1], delimiter=',', names=['x', 'y'])
data2 = np.genfromtxt(sys.argv[2], delimiter=',', names=['x', 'y'])
plt.scatter(data1['x'], data1['y'], color='r', s=0.1)
plt.scatter(data2['x'], data2['y'], color='b', s=10.0)
plt.axis([0, 3600, 0, 100])
plt.show()
