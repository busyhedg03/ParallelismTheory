import matplotlib as mpl
import matplotlib.pyplot as plt
import math
import matplotlib.colors as mcolors
import numpy as np
# cat_par = [f"{i}*{i}" for i in [128, 256, 512, 1024]]

# g1 = [0.72, 8.3, 106, 0] 
# g2 = [0.6, 1.7, 8.7, 71.4]

# width = 0.3

# x = np.arange(len(cat_par))

# fig, ax = plt.subplots()

# rects1 = ax.bar(x - width/2, g1, width, label='Onecore')
# rects2 = ax.bar(x + width/2, g2, width, label='Multicore')

# ax.set_title('Onecore and multicore time, s')
# ax.set_xticks(x)
# ax.set_xticklabels(cat_par)
# ax.set_axisbelow(True)
# ax.legend()
# fig.set_figheight(7)
# plt.grid(axis = 'y')
# fig.savefig('cpu.png')



# cat_par = [f"{i+1}" for i in range(3)]

# g1 = [0.28, 0.24, 0.22]

# width = 0.3

# x = np.arange(len(cat_par))

# fig, ax = plt.subplots()

# rects1 = ax.bar(x - width/2, g1, width, label='GPU time, s')

# ax.set_title('Этапы оптимизации')
# ax.set_xticks(x)
# ax.set_xticklabels(cat_par)
# ax.set_axisbelow(True)
# ax.legend()
# fig.set_figheight(3)
# plt.grid(axis = 'y')
# fig.savefig('opt.png')





cat_par = [f"{i}*{i}" for i in [128, 256, 512, 1024]]

g1 = [0.72, 8.3, 106, 0] 
g2 = [0.6, 1.7, 8.7, 71.4]
g3 = [2, 10.6, 66.5, 400]
width = 0.3

x = np.arange(len(cat_par))

fig, ax = plt.subplots()

rects1 = ax.bar(x - width, g1, width, label='Onecore')
rects2 = ax.bar(x, g2, width, label='Multicore')
rects3 = ax.bar(x + width, g3, width, label='GPU')
ax.set_title('Onecore and multicore time, s')
ax.set_xticks(x)
ax.set_xticklabels(cat_par)
ax.set_axisbelow(True)
ax.legend()
fig.set_figheight(7)
plt.grid(axis = 'y')
fig.savefig('gpu.png')