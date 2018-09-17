'''
?????????????
'''
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

# create figure
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# cubic Bazier curve with 4 control points
P = np.array( [ np.array([0.0, 0, 0.0]),
                np.array([3.0, 0, 3.0]),
                np.array([7.0, 0, 4.0]),
                np.array([10.0, 0, 3.0]),
              ])

# plot data
ax.scatter(P[:,0], P[:,1], P[:,2], c='r', marker='o')
#ax.plot(P[:,0], P[:,1], P[:,2])
# labels
ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

ax.set_xlim([0,10])
ax.set_ylim([0,10])
ax.set_zlim([0,10])

plt.show()