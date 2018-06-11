#!/usr/bin/python3
# coding=utf-8
import numpy as np
import matplotlib.pyplot as plt
import sys
interpolated = np.genfromtxt(sys.argv[1], delimiter=',', names=['x', 'y'])
points = np.genfromtxt(sys.argv[2], delimiter=',', names=['x', 'y'])
model = np.genfromtxt(sys.argv[3], delimiter=',', names=['x', 'y'])
plt.figure(figsize=(20,6))
plt.scatter(model['x'], model['y'], color='g', s=0.5, alpha=1.0, label='Wartości wzorcowe')
plt.scatter(interpolated['x'], interpolated['y'], color='r', s=0.1, alpha=1.0, label='Wartości interpolowane')
plt.scatter(points['x'], points['y'], color='b', s=10.0, alpha=0.5, label='Punkty interpolacyjne')
y_min=model['y'].min()
y_max=model['y'].max()
diff=y_max-y_min
y_min-=diff*0.1
y_max+=diff*0.1
plt.axis([0, model['x'][-1], y_min, y_max])
plt.xlabel('Odległość')
plt.ylabel('Wysokość')
# plt.show()
lgnd = plt.legend()
lgnd.legendHandles[0]._sizes = [30]
lgnd.legendHandles[1]._sizes = [30]
lgnd.legendHandles[2]._sizes = [30]
plt.title(sys.argv[4] + str(points.size) + ' punktów interpolacyjnych')
plt.tight_layout()
plt.savefig(str(points.size)+'_plot_' + sys.argv[1] + '.png')