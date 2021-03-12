import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import re
_length = 16
class Accleration:
    a = [0 for i in range(_length)]
    alpha = [0.00355773007577022,	0.0204161415535754,	0.0614216670105817,	0.124327248354339,	0.185162263681329,	0.210696541719535,	0.185162263681329,	0.124327248354339,	0.0614216670105817,	0.0204161415535754,	0.00355773007577022]
    fir_data = 0

    def __init__(self):
        f = open("lowPass.txt", "r")
        data = f.readlines()
        data[0] = re.split('\t+|\s+',data[0])
        self.alpha = [float(x) for x in data[0]]
        num = len(data[0])
        self.a = [0 for i in range(num)]

    def append(self,accleration):
        self.a.insert(0,accleration)
        self.a = self.a[:-1]
        return

    def fir(self):
        total = 0
        for x in [self.a[i]*self.alpha[i] for i in range(11)]:
            total += x
        self.fir_data = total

    def userA(self):
        return self.a[0] - self.fir_data

    def gravitaionalA(self):
        return self.fir_data

class Accelerometer:
    x = Accleration()
    y = Accleration()
    z = Accleration()
    a = []
    alpha = []

    def __init__(self):
        f = open("bandPass.txt", "r")
        data = f.readlines()
        data[0] = re.split('\t+|\s+',data[0])
        self.alpha = [float(x) for x in data[0]]
        num = len(data[0])
        self.a = [0 for i in range(num)]


    def append(self,accleration):
        self.x.append(accleration[0])
        self.y.append(accleration[1])
        self.z.append(accleration[2])
        return
    
    def userAccelerationGravitationalDirection(self):
        self.x.fir()
        self.y.fir()
        self.z.fir()
        x_sum = self.x.userA() * self.x.gravitaionalA()
        y_sum = self.y.userA() * self.y.gravitaionalA()
        z_sum = self.z.userA() * self.z.gravitaionalA()
        return x_sum + y_sum + z_sum

    def fir(self,a,alpha):
        total = 0
        for x in [a[i]*alpha[i] for i in range(_length)]:
            total += x
        return total

    def filter(self):
        self.a.insert(0,(self.userAccelerationGravitationalDirection()))
        self.a = self.a[:-1]
        result = self.fir(self.a,self.alpha)
        return result
        

class StepDetection:
    a = Accelerometer()
    step_data = []

    def __init__(self,data):
        self.data = pd.read_csv(data)#,sep="\t")
        #self.data.to_csv("data4.csv")

    def run_filter(self):
        for i in range(len(self.data)):
            self.a.append([self.data["x_data"][i],self.data["y_data"][i],self.data["z_data"][i]])
            self.step_data.append(self.a.filter())
    
    def plot_data(self):
        time_data = range(len(self.step_data))
        plt.plot(time_data,self.step_data)
        plt.show()

    def smoothdata(self):
        filterprev = self.step_data[0]
        filter_constant = 100
        filtered = []
        for x in self.step_data:
            filt = (x+filterprev*filter_constant)/(filter_constant+1)
            filtered.append(filt)
            filterprev = filt
        self.step_data = filtered

    def count_peaks(self):
        count = 0
        last_min = 0
        last_max = 0
        goingup = False
        band = 0.0002
        min_change = 0.00001
        prevx = 0
        for x in self.step_data:
            if x < prevx:
                if goingup:
                    goingup = False
                    last_max = prevx
                    if (last_max - last_min) > band:
                        count += 1
            if x > prevx:
                if goingup == False:
                    goingup = True
                    last_min = prevx
            prevx = x       

        # count = 0
        # below = True
        # peak_mag = 0.0003
        # low_mag = 0.0001
        # for x in self.step_data:
        #     if below:
        #         if x > peak_mag:
        #             count += 1
        #             below = False
        #     else:
        #         if x < low_mag:
        #             below = True
        print(count)


step = StepDetection('data3.csv')
step.run_filter()
step.smoothdata()
step.count_peaks()
step.plot_data()