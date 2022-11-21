import numpy as np
import scipy.io.wavfile as wav
from matplotlib import pyplot as plt
import warnings
warnings.filterwarnings("ignore")

def ar_fit_model(y: np.ndarray, p: int) -> np.ndarray:
    """Computes the parameters of the autogression model

    Args:
        y: np.ndarray: sound signal
        p: int: required order of AR model

    Return:
        np.ndarray: estimated parameters of AR model 


    Shape:
       - Input: (N,)
       - Output: (p+1,)

    """

    M = np.zeros((len(y)-p, p+1))

    for i in M:
        i[0] = 1

    for r in range(len(y)-p):
        for c in range(1, p+1):
            M[r][c] = y[p+r-c]

    b = []
    for nums in range(p, len(y)):
        b.append(y[nums])
    b1 = np.array(b)

    a = np.linalg.lstsq(M, b1, rcond=None)[0]

    return a

def ar_predict(a: np.ndarray, y0: np.ndarray, N:int) -> np.ndarray:
    """ computes the rest of elements of y, starting from (p+1)-th 
        one up to N-th one. 

    Args:
        a: np.ndarray: estimated parameters of AR model
        y0: np.ndarray: beginning of sequence to be predicted
        N: int:  required length of predicted sequence, including the 
                 beginning represented by y0. 
    Return:
        np.ndarray: the predicted sequence 


    Shape:
       - Input: (p+1,), (p,)
       - Output: (N,)

    """
    p = y0.shape[0]
    y_pred = np.zeros(N)
    y_pred[:p] = y0

    a_k = []
    for koef in range(1, p+1):
        a_k.append(a[koef])
    a_koef = np.array(a_k)

    for idx in range(p, N):
        y_predch = []
        for arr in range(p):
            y_predch.append(y_pred[idx-1-arr])
        y_predchozi = np.array(y_predch)
        y_pred[idx] = a[0] + np.dot(a_koef,y_predchozi)

    return y_pred

if(__name__ == '__main__'):

    fs,y=wav.read('gong.wav')
    y = y.copy()/32767
    p = 300      # size of history considered for prediction
    N = len(y)   # length of the sequence
    K = 10000    # visualize just K first elements

    a = ar_fit_model(y, p)


    y0 = y[:p]
    y_pred = ar_predict(a, y0, N)

    wav.write('gong_predicted.wav', fs, y_pred)

    plt.plot(y[:K], 'b', label = 'original')
    plt.plot(y_pred[:K], 'r', label = 'AR model')
    plt.legend()
    plt.show()

