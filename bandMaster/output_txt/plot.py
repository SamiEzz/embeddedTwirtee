import matplotlib.pyplot as plt
import csv
from math import pi
from random import randint

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
        theta[angle]=theta[angle]*180.0/pi
with open('/home/samie/Documents/git/emebeddedTwirtee/bandMaster/output_txt/mission.txt','r') as csvfile:
    plots2 = csv.reader(csvfile, delimiter=',')
    for row in plots2:
        x_node.append(float(row[1]))
        y_node.append(float(row[2]))

plt.figure(1)
plt.title("Twirtee Simulation Tools\nIRT SAINT EXUPERY")
plt.subplot(121)
plt.plot(x,y,'--',color="silver", label="Twirtee's Position")
plt.plot(x_node,y_node, "v",markersize=5,label="Mission's nodes")
plt.axis('equal')

plt.xlabel('x')
plt.ylabel('y')

plt.subplot(122)
plt.xlabel('iterations')
plt.ylabel('Theta')

plt.plot(iterations,theta, label="Theta angle")
#plt.legend("IRT SAINT EXUPERY")
numstr=str(randint(0,10))
print("Saved file : ./output_txt/out"+numstr+".png")
plt.savefig("./output_txt/out"+numstr+".png")
plt.show()
