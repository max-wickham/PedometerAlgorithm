import matplotlib.pyplot as plt

data = open("output.txt",'r').read()
data = data.split(" ")[:-1]
data = [int(x) for x in data]

time_data = range(len(data))
plt.plot(time_data, data)
plt.show()
