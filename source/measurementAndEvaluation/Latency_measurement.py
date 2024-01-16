import pandas as pd
from scipy import stats
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator

if __name__ == '__main__':
    df = pd.read_csv("data/latency/Latency.csv")
    print(df[['Frames',
              'Milliseconds'
              ]].describe())


    fig1, ax1 = plt.subplots()
    bplot = ax1.boxplot(df['Milliseconds'], patch_artist=True)
    ax1.set_ylabel('Seconds')
    ax1.set_title('Latency')
    bplot['boxes'][0].set_facecolor('lightblue')

    plt.show()
