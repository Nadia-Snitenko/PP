import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
#
m = []
with open("C:/Users/N_20/source/repos/PP_1/matrix_1.txt") as f:
    for line in f:
        m.append([int(x) for x in line.split()])
m_ = np.array(m, dtype=object,)


n = []
with open("C:/Users/N_20/source/repos/PP_1/matrix_2.txt") as f:
    for line in f:
        n.append([int(x) for x in line.split()])
n_ = np.array(n, dtype=object,)

result_p = []
result_p = np.dot(m_, n_)
print(result_p)

result = []
with open("C:/Users/N_20/source/repos/PP_1/matrix_3.txt") as f:
    for line in f:
        result.append([int(x) for x in line.split()])
result_c = np.array(result, dtype=object,)


if not np.array_equal(result_c, result_p):
    print("not equal")
else:
    print("equal")
#

times = []
with open("C:/Users/N_20/source/repos/PP_1/time.txt") as f:
    for line in f:
        result.append([float(x) for x in line.split()])
times_ = np.array(times, dtype=object,)

time = [0, 0.001, 0.009, 0.035, 0.075, 0.133, 0.189,0.355, 0.488, 1.056, 2.281, 4.271, 6.979, 11.133 ,16.508 ]
counts = [10, 50, 100, 150, 200, 250, 300,350, 400, 500, 600, 700, 800 ,900, 1000 ]
plt.plot( counts,time, color='pink')
plt.xlabel('count of numbers')
plt.ylabel('time')
plt.show()
#
