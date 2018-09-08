import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

# create figure
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
#ax.set_autoscaley_on(False)
ax.set_xlim([0,1])
ax.set_ylim([0,1])
ax.set_zlim([0,1])

# initial velocity
v0 = np.array([0.3,0.0,3.0])
# acceleration
a = np.array([0.0,0.0,-4.4])
# time interval
t = 0.0
# start position
points = np.array([np.array([0,0,0])])
# calculate all positions adding new time interval
for n in range(5):
    t += 0.1
    new_point = points[0] + v0*t + (a*(t**2)) / 2
    points = np.vstack((points, new_point))

#print(points)

# plot data
ax.scatter(points[:,0], points[:,1], points[:,2], c='r', marker='o')

# labels
ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

plt.show()