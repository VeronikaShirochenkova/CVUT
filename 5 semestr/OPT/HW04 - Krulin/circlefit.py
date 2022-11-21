import scipy.io as sio
import numpy as np
from math import pi
from matplotlib import pyplot as plt
from numpy import linalg as LA
import random

def quad_to_center(d,e,f):
    x0 = -d/2
    y0 = -e/2
    r = (np.sqrt(pow(d,2) + pow(e,2) - 4*f))/2
    return x0, y0, r

def fit_circle_nhom(X):

    row, col = X.shape

    A = np.ones((row, 3))
    for i in range(row):
        A[i][0] = X[i][0]
        A[i][1] = X[i][1]

    b = np.zeros((row,1))
    for q in range(row):
        b[q] = -pow(X[q][0], 2) - pow(X[q][1], 2)
    d,e,f = np.linalg.lstsq(A, b, rcond=None)[0]
    d1 = 0
    d1 = d[0]
    e1 = 0
    e1 = e[0]
    f1 = 0
    f1 = f[0]

    return d1,e1,f1

def fit_circle_hom(X):

    row, col = X.shape
    A = np.ones((row, 4))
    for i in range(row):
        A[i][0] = pow(X[i][0], 2) + pow(X[i][1], 2)
        A[i][1] = X[i][0]
        A[i][2] = X[i][1]

    D, U = LA.eig(np.matmul(A.transpose(), A))

    n = len(D)
    min_num = D[0]
    min_idx = 0
    for j in range(n):
        if min_num > D[j]:
            min_num = D[j]
            min_idx = j

    a = U[0][min_idx]
    d = U[1][min_idx] / a
    e = U[2][min_idx] / a
    f = U[3][min_idx] / a

    return d,e,f

def dist(X, x0, y0, r):
    row, col = X.shape
    d = np.zeros(row)

    for i in range(row):
        d[i] = np.sqrt(pow(X[i][0]-x0, 2) + pow(X[i][1]-y0, 2)) - r;

    return d

def fit_circle_ransac(X, num_iter, threshold):
    n, m = X.shape
    Q = np.zeros((3, 2))
    x_res = 0
    y_res = 0
    r_res = 0
    res_inliers = 0
    S = np.zeros((1, 2))
    for i in range(num_iter):
        point1, point2, point3 = random.sample(range(0, n), 3)
        Q[0] = X[point1]
        Q[1] = X[point2]
        Q[2] = X[point3]

        d,e,f = fit_circle_nhom(Q)
        x0, y0, r = quad_to_center(d,e,f)

        num_of_inliers = 0
        for j in range(n):
            S[0] = X[j]
            distance = dist(S, x0, y0, r)
            if distance < threshold and distance > 0:
                num_of_inliers += 1

        if num_of_inliers > res_inliers:

            res_inliers = num_of_inliers
            x_res = x0
            y_res = y0
            r_res = r

    return x_res, y_res, r_res

def plot_circle(x0,y0,r, color, label):
    t = np.arange(0,2*pi,0.01)
    X = x0 + r*np.cos(t)
    Y = y0 + r*np.sin(t)
    plt.plot(X,Y, color=color, label=label)

if(__name__ == '__main__'):
    data = sio.loadmat('data.mat')
    X = data['X'] # only inliers
    A = data['A'] # X + outliers

    def_nh = fit_circle_nhom(X)
    x0y0r_nh = quad_to_center(*def_nh)
    dnh = dist(X, *x0y0r_nh)

    def_h = fit_circle_hom(X)
    x0y0r_h = quad_to_center(*def_h)
    dh = dist(X, *x0y0r_h)

    results = {'def_nh':def_nh, 'def_h':def_h, 
               'x0y0r_nh' : x0y0r_nh, 'x0y0r_h': x0y0r_nh,
               'dnh': dnh, 'dh':dh}
    
    GT = sio.loadmat('GT.mat')
    for key in results:
        print('max difference',  np.amax(np.abs(results[key] - GT[key])), 'in', key)


    x = fit_circle_ransac(A, 2000, 0.1)

    plt.figure(1)
    plt.subplot(121)
    plt.scatter(X[:,0], X[:,1], marker='.', s=3)
    plot_circle(*x0y0r_h, 'r', 'hom')
    plot_circle(*x0y0r_nh, 'b', 'nhom')
    plt.legend()
    plt.axis('equal')    
    plt.subplot(122)
    plt.scatter(A[:,0], A[:,1], marker='.', s=2)
    plot_circle(*x, 'y', 'ransac')
    plt.legend()
    plt.axis('equal')
    plt.show()
    
