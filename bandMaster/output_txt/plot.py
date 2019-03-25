import matplotlib.pyplot as plt
import csv
from math import pi

x = []
y = []
theta = []
iterations = []
x_node=[]
y_node=[]

with open('/home/samie/Documents/git/emebeddedTwirtee/bandMaster/output_txt/path.txt','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        iterations.append(float(row[0]))
        x.append(float(row[1]))
        y.append(float(row[2]))
        theta.append(float(row[3]))
    for angle in range(len(theta)):
        theta[angle]=theta[angle]*180.0/3.14
with open('/home/samie/Documents/git/emebeddedTwirtee/bandMaster/output_txt/mission.txt','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x_node.append(float(row[1]))
        y_node.append(float(row[2]))

plt.figure(1)
plt.subplot(211)
plt.plot(x,y,'--', label="Twirtee's Position",color="silver")
plt.plot(x_node,y_node, "+",markersize=10,label="Mission's nodes")
plt.axis('equal')

plt.xlabel('x')
plt.ylabel('y')

plt.subplot(212)
plt.plot(iterations,theta, label="Theta angle")
#plt.title("Twirtee Simulation Tools\nIRT SAINT EXUPERY")
#plt.legend("IRT SAINT EXUPERY")
plt.show()
