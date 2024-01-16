import matplotlib.pyplot as plt
import numpy as np
import csv

# Sampling frequency
freq =10000
# Samples of FFT
samples = 128

#Read measurements
path1 = 'data/pinch/closed.csv'
path2 = 'data/pinch/open.csv'
with open(path1, 'r') as f:
    reader_touch = csv.reader(f, delimiter=',')
    headers = next(reader_touch)
    data_touch = np.array(list(reader_touch)).astype(float)/128

with open(path2, 'r') as f:
    reader_no_touch = csv.reader(f, delimiter=',')
    headers = next(reader_no_touch)
    data_no_touch = np.array(list(reader_no_touch)).astype(float)/128


#data x-axis are freq and y-axis average magnitude of FFT
x = np.linspace(freq*2/samples,freq/2,num=int(samples/2-2))
ave_touch = np.mean(data_touch, axis=0)
ave_no_touch = np.mean(data_no_touch, axis=0)

#Plot data
fig, axs = plt.subplots(2, 1, constrained_layout=True)
fig.suptitle('DFT coefficient of raw measurement signal', fontsize=16)

for i in range(data_touch.shape[0]):
    axs[0].plot(x,data_touch[i,1:],'k',alpha= 0.02)
axs[0].plot(x, ave_touch[1:])
axs[0].set_title('Pinch')
axs[0].set_xlabel('Hertz [Hz]')
axs[0].set_ylabel('Magnitude [dB]')
#axs[0].set_yscale('log')


for i in range(data_no_touch.shape[0]):
    axs[1].plot(x,data_no_touch[i,1:],'k',alpha= 0.02)
axs[1].plot(x,ave_no_touch[1:])
axs[1].set_xlabel('Hertz [Hz]')
axs[1].set_title('No pinch')
axs[1].set_ylabel('Magnitude of DFT coefficient')
#axs[1].set_yscale('log')

plt.show()


indices = (-ave_touch).argsort()[:6]
print((indices+1)*freq/samples)