import re
from jinja2 import Template
lpf = open('./lowPass.txt','r')
bpf = open('./bandPass.txt','r')

lpfData = lpf.readlines()[0]
lpfData = re.split('\t+|\s+',lpfData)
lpfData = [float(x) for x in lpfData]

bpfData = bpf.readlines()[0]
bpfData = re.split('\t+|\s+',bpfData)
bpfData = [float(x) for x in bpfData]

max_int = 1000
#convert values to integers
def convertToInt(data):
    max_float = max(data)
    multiplier = max_int / max_float
    for i in range(len(data)):
        data[i] = int(data[i]*multiplier+0.5)

convertToInt(lpfData)
convertToInt(bpfData)


output = open("./output.c","w")
inputFile = open("ctemplate.c","r")
tm = Template(inputFile.read())
output.write(tm.render(lpf=lpfData, bpf=bpfData, axis_size = len(lpfData), acceleration_size = len(bpfData)))
output.close()
inputFile.close()
lpf.close()
bpf.close()

#print the data to the c files




