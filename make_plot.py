#!/usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
import sys
data = np.genfromtxt(sys.argv[1], delimiter=',', names=['x', 'y'])
plt.scatter(data['x'], data['y'], color='r', label='label', s=0.1)
plt.axis([0, 3600, 0, 100])
plt.show()