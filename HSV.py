import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import cv2
img = cv2.imread('test.jpg')
RGB_value = img[50:140,70:150]

RGB_value = img[50:140,70:150]
HSV_value = cv2.cvtColor(img[50:140,70:150],cv2.COLOR_BGR2HSV)
data = np.reshape(HSV_value, (-1, 3))
# 生成数据
x = data[:,0]
y = data[:,1]
z = data[:,2]

# 创建3D图形对象
fig = plt.figure()
ax = Axes3D(fig)

# 绘制散点图
ax.scatter(x, y, z, c='r', marker='o')

# 设置坐标轴标签
ax.set_xlabel('H Label')
ax.set_ylabel('V Label')
ax.set_zlabel('S Label')
# 显示图形
plt.show()