import csv
import numpy as np
import os

file_name_in = 'small long 5'
# file_name_out = 'test1w.csv'
file_name_out = file_name_in + 'w.csv'
file_name_in += ".csv"

bw = 19
rows = 5
np.set_printoptions(suppress=True)

def clear_data(t, x):
    l = []
    for i in range(0, len(x), bw):

        m1 = 123
        m2 = -123
        t1 = 0
        t2 = 0
        step = i+bw
        if step > len(x):
            step = len(x)
        for j in range(i, step):
            # print(j)
            if 0.3 > x[j] > m2:
            # if 0 > x[j] > m2:
                m2 = x[j]
                t2 = t[j]
            if x[j] < m1:
                m1 = x[j]
                t1 = t[j]
        # print(i)
        # m1 = min(x[i:i+bw])
        # m2 = max(x[i:i+bw])
        # if m2 > 0:
        #     m2 = -123
        #     for j in range(i, i+bw):
        #         if x[j] < 0 and x[j] > m2:
        #             m2 = x[j]
        l.append([t1, t2, m1, m2])
    return l

os.chdir('pydata')


# fin = open(file_name_in, 'r')
# fout = open(file_name_out, 'w')

d = np.genfromtxt(file_name_in, delimiter=',')
d1 = np.transpose(d)
a = np.array([[0, 0, 0, 0]])

# v1
# for i in range(0, rows, 2):
#     z = clear_data(d1[i], d1[i+1])
#     for j in range(len(z)):
#         a = np.append(a, z[j])
#     a = np.append(a, np.array([999, 999, 999, 999]))

# v2
for i in range(1, rows):
    z = clear_data(d1[0], d1[i])
    for j in range(len(z)):
        a = np.append(a, z[j])
    a = np.append(a, np.array([999, 999, 999, 999]))

a.shape = (a.size//4, 4)

np.savetxt(file_name_out, a, delimiter=',', fmt="%f")
# fin.close()
# fout.close()