import re
from jinja2 import Template
import numpy as np
import pandas as pd
import math

max_int = 10000
data_multiplier = 5

lpf = open('./lowPass.txt','r')
bpf = open('./bandPass.txt','r')

lpfData = lpf.readlines()[0]
lpfData = re.split('\t+|\s+',lpfData)
lpfData = [float(x) for x in lpfData]

bpfData = bpf.readlines()[0]
bpfData = re.split('\t+|\s+',bpfData)
bpfData = [float(x) for x in bpfData]
#convert values to integers
def convertToInt(data):
    max_float = max(data)
    multiplier = max_int / max_float
    for i in range(len(data)):
        data[i] = int(data[i]*multiplier+0.5)
lpfBitScalar = int(math.log(max_int/max(lpfData),2)+0.5)
bpfBitScalar = int(math.log(max_int/max(bpfData),2)+0.5)
convertToInt(lpfData)
convertToInt(bpfData)


data = pd.read_csv("data4.csv")
xData = [int(pow(2,data_multiplier)*x + 0.5) for x in data["x_data"]]
yData = [int(pow(2,data_multiplier)*x + 0.5) for x in data["y_data"]]
zData = [int(pow(2,data_multiplier)*x + 0.5) for x in data["z_data"]]

output = open("./output.c","w")
inputFile = open("ctemplatetest.c","r")
tm = Template(inputFile.read())
output.write(tm.render(lpf=lpfData, bpf=bpfData, axis_size = len(lpfData), 
    acceleration_size = len(bpfData), xData = xData, yData = yData, zData = zData,
    xdatalength = len(xData), ydatalength = len(yData), zdatalength = len(zData),
    lpfBitScalar = lpfBitScalar, bpfBitScalar = bpfBitScalar))
output.close()
inputFile.close()
lpf.close()
bpf.close()

#print the data to the c files




