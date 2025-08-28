import random
import math

def custom_ceil(x):
    return math.ceil(x * 10) / 10
with open('report.txt', 'w') as file:
    freq = [0] * 100
    trials = 200000

    for _ in range(trials):
        x1 = random.random()
        x2 = random.random()
        if x1 == 0 or x2 == 0:
            print("HALLEJUAH")
        y1 = math.log(1/(1-x1))
        y2 = math.log(1/(1-x2))
        z = y1 + y2
        i = custom_ceil(z)
        while int(round(i/0.1)) < 100 and int(round(i/0.1)) >= 0:
            freq[int(round(i/0.1))] += 1
            i += 0.1

    for i in range(len(freq)):
        freq[i] /= trials
    
    j = 0
    file.write("Sum of Exponentials CDF:\n")
    file.write("     .0   .1   .2   .3   .4   .5   .6   .7   .8   .9\n")
    file.write("--------------------------------------------------------------------------------\n")
    for i in range(len(freq)):
        if (j % 10 == 0):
            file.write(f"{j / 10} | ")
        file.write(f"{freq[i]:.4f}   ")
        j += 1
        if (j % 10 == 0):
            file.write("\n")
        

