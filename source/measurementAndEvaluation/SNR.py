import pandas as pd
import matplotlib.pyplot as plt

if __name__ == '__main__':
    """
    The measured raw signal was recorded for three test subjects and then manually looked in the data to see where a touch state is and where it resolves (this was checked with a viedo).
    These sections were then manually inserted into the program and with the help of pandas mean and sd were summed, the results are stored in SNR.xlsx.
    
    """

    signal  = pd.read_csv("data/SNR/PinchPhilipp.csv")
    signal_eval = signal[198-1:256-1]
    print(signal_eval.describe())


    #Eval Data
    data = [[15.56302501,15.26855987 ,15.84783379, 16.25826713, 16.12359948],
            [29.5630812, 30.02705475, 30.01335306, 29.1056005, 29.89302883],
            [29.13336512,28.78022015,28.1838157,29.25744163,28.31306109]]

    fig1, ax1 = plt.subplots(1,3)
    bplot1 = ax1[0].boxplot(data[0], labels=["test subject 1"],patch_artist=True)
    bplot2 = ax1[1].boxplot(data[1], labels=["test subject 2"], patch_artist=True)
    bplot3 = ax1[2].boxplot(data[2], labels=["test subject 3"], patch_artist=True)
    ax1[0].set_ylabel('dB')
    ax1[1].set_ylabel('dB')
    ax1[2].set_ylabel('dB')

    fig1.suptitle("Signal to noise ratio", fontsize=14)
    bplot1['boxes'][0].set_facecolor('lightblue')
    bplot2['boxes'][0].set_facecolor('lightblue')
    bplot3['boxes'][0].set_facecolor('lightblue')

    print(data[0].describe())


    plt.tight_layout()
    plt.show()
