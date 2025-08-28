import random
import math

def custom_ceil(x):
    return math.ceil(x * 100) / 100

def mean(weights, squared = False):
    out = 0
    i = 1
    for weight in weights:
        if squared:
            out += weight * (i**2)
        else:
            out += weight * i
        i += 1
    return out

def variance(weights):
    return mean(weights, True) - mean(weights)**2


with open('report.txt', 'w') as file:
    freq = [0] * 350
    trials = 20100
    n = 700
    weights = [1/6, 1/6, 1/6, 1/6, 1/6, 1/6]
    m = mean(weights)
    v = variance(weights)

    for _ in range(trials):
        sum = 0
        for _ in range(n):
            sum += random.choices([1, 2, 3, 4, 5, 6], weights = weights, k = 1)[0]
        z = (sum - n * m) / (math.sqrt(v) * math.sqrt(n))
        #print("z",z)
        i = custom_ceil(z)
        i = max(0, i)
        #print("i", i)
        while int(round(i/0.01)) < 350 and int(round(i/0.01)) >= 0:
            freq[int(round(i/0.01))] += 1
            i += 0.01

    for i in range(len(freq)):
        freq[i] /= trials
    #print(freq)
    j = 0
    file.write("Experimental CDF\n")
    file.write("      .00   .01   .02   .03   .04   .05   .06   .07   .08   .09\n")
    file.write("--------------------------------------------------------------------------------\n")
    for i in range(len(freq)):
        if (j % 10 == 0):
            file.write(f"{j / 100} | ")
        file.write(f"{freq[i]:.4f}   ")
        j += 1
        if (j % 10 == 0):
            file.write("\n")
        