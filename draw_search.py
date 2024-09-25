import csv
import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import interp1d

exampleFile = open('data_search.csv')  # 打开csv文件
exampleReader = csv.reader(exampleFile)  # 读取csv文件
exampleData = list(exampleReader)  # csv数据转换为列�?
length_zu = len(exampleData)  # 得到数据行数
length_yuan = len(exampleData[0])  # 得到每行长度



x = list()
y = list()


for i in range(0, length_yuan):  # 从第1行开始读�?
    exampleData[0][i]=float(exampleData[0][i]) # 转换数据为浮点数，否则结果图横纵坐标不递增排序
    exampleData[1][i]=float(exampleData[1][i])
    x.append(exampleData[0][i])  # 将第一行数据从�?1列读取到最后一列赋给列表x
    y.append(exampleData[1][i])  # 将第2行数据从�?1列读取到最后一列赋给列表y\
plt.xlabel('x/n')
plt.ylabel('t/ms*10^(-7)')
plt.title("insertion",fontsize=10)
plt.plot(x, y)  # 绘制x,y的折线图
plt.legend()
plt.show()  # 显示折线�?

