import numpy as np
import matplotlib.pyplot as plt

NoOfHops = ['0', '1', '2', '3', '4']
Count = [142128, 232047, 590572, 45081, 172]

plot = plt.bar(NoOfHops, Count, color='royalblue', alpha =0.6, width = 0.4, edgecolor='black')

for value in plot:
    height = value.get_height()
    plt.text(value.get_x() + value.get_width()/2., 1.002*height, '%d' % int(height), ha='center', va='bottom')
    
#length = len(Count)
#fig, ax = plt.subplots()
#width = 0.2
#x_pos = np.arange(len(NoOfHops))
#ax.bar(x, Count, width, color='#000080', yerr=NoOfHops)

#plt.bar(NoOfHops, Count)

plt.title('Part I: Hop Distribution while querying')
#plt.bar(x_pos, height, color=(0.1, 0.1, 0.1, 0.1), edgeColor='Black')
plt.xlabel('No. of Hops')

#ax.set_ylabel('Count')

plt.ylabel('Count')
#fig.tight_layout()
#plt.xticks(NoOfHops, Count)
plt.show()
