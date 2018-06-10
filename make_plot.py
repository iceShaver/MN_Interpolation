#!/usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
import sys
interpolated = np.genfromtxt(sys.argv[1], delimiter=',', names=['x', 'y'])
points = np.genfromtxt(sys.argv[2], delimiter=',', names=['x', 'y'])
model = np.genfromtxt(sys.argv[3], delimiter=',', names=['x', 'y'])
plt.figure(figsize=(20,7))
plt.scatter(points['x'], points['y'], color='b', s=10.0, alpha=0.5)
plt.scatter(model['x'], model['y'], color='g', s=0.5, alpha=0.9)
plt.scatter(interpolated['x'], interpolated['y'], color='r', s=0.1)
y_min=model['y'].min()
y_max=model['y'].max()
plt.axis([0, model['x'][-1], 0.9*y_min, y_max * 1.1])
plt.xlabel('Odległość')
plt.ylabel('Wysokość')
# plt.show()
plt.tight_layout()
plt.savefig('plot_' + sys.argv[1] + '.png')