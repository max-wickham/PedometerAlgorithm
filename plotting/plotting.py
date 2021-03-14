import matplotlib.pyplot as plt

data = open("./data.txt", "r")
d = data
d = d.readlines()
data.close()
d = [int(x) for x in d]
data = d
time_data = range(len(data))
plt.plot(time_data, data)
plt.show()
