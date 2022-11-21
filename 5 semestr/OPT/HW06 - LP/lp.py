import numpy as np
from scipy.optimize import linprog

def vyhra(koef, k):
    c = np.array([0, 0, 0, 0, 0, -1])
    A_ub = np.array(  [[-koef[0], -koef[1], 0, 0, 0, 1],
                    [0, -koef[1], -koef[2], -koef[3], 0, 1],
                    [0, 0, 0, -koef[3], -koef[4], 1]])
    b_ub = np.array([0,0,0])
    Aeq = np.array([[1, 1, 1, 1, 1, 0]])
    beq = k
    lb = [(0, None),
          (0, None),
          (0, None),
          (0, None),
          (0, None),
          (2000, None)]
    # x = linprog(c, A_ub=A_ub, b_ub=b_ub, A_eq=Aeq, b_eq=beq, bounds=lb)
    x = linprog(c, A_ub=A_ub, b_ub=b_ub, A_eq=Aeq, b_eq=beq)
    res = x.x[:-1]
    return res

def vyhra2(koef,k,m):
    c = np.array([0, 0, 0, -1])

    A_ub = np.array(  [ [-koef[0],   0,         0,          1],
                        [0,         -koef[1],   0,          1],
                        [0,         0,          -koef[2],   1]])

    b_ub = np.array([0, 0, 0])

    Aeq = np.array([[1, 1, 1, 0]])
    beq = k

    lb = [(m, None),
          (m, None),
          (m, None),
          (None, None)]

    x = linprog(c, A_ub=A_ub, b_ub=b_ub, A_eq=Aeq, b_eq=beq, bounds=lb)
    res = x.x[:-1]

    return res

def minimaxfit(x,y):
    x = x.T
    n = x.shape[0]  # 5
    m = x.shape[1]  # 3

    A_ub = np.zeros((2 * n, m + 2))

    # print(x)
    for i in range(0,n):
        for j in range(0,m):
            A_ub[i][j] = x[i][j]
        A_ub[i][m] = 1.0
        A_ub[i][m+1] = -1.0


    for k in range(n, n*2):
        for l in range(0,m):
            A_ub[k][l] = x[k - n][l] * (-1)
        A_ub[k][m] = -1.0
        A_ub[k][m+1] = -1.0


    b_ub = np.vstack((y.T, (-1.0)*y.T))

    c = np.zeros(m+2)
    c[m+2-1] = 1.0


    res = linprog(c, A_ub=A_ub, b_ub=b_ub, bounds=(None,None))

    a = list(res.x[0:m])
    b = res.x[m]
    r = res.x[m+1]

    return a, b, r

if(__name__ == '__main__'):

    # #=============VYHRA 1================
    # c = np.array([1.27, 1.02, 4.70, 3.09, 9.00])
    # k = 3000
    # x = vyhra(c, k)
    #
    # # x = array([3.40905816e-06, 2.69461077e+03, 9.28004584e-07, 1.68174891e-08, 3.05389222e+02]) or
    # # x = [3.40905816e-06, 2.69461077e+03, 9.28004584e-07, 1.68174891e-08, 3.05389222e+02] etc.
    #
    # x2 = [3.40905816e-06, 2.69461077e+03, 9.28004584e-07, 1.68174891e-08, 3.05389222e+02]
    # # res = x.x[:-1]
    # # print(x.x[0]*1.27+x.x[1]*1.02)
    # # print(x2)
    # # print(x)
    # print(np.matmul(x2, c.T))
    # print(np.matmul(x, c.T))
    #
    #
    # #================VYHRA 2===========================
    # c = np.array([1.27, 4.70, 9.00])
    # k = 3000
    # m = 400
    # x = vyhra2(c, k, m)
    # #
    # # # x = array([2046.90108498,  553.09881711,  400.00004477]) or
    # # # x = [2046.90108498,  553.09881711,  400.00004477] etc.
    #
    # x2 = [2046.90108498, 553.09881711, 400.00004477]
    #
    # # print(np.matmul(x2, c.T))
    # # print(np.matmul(x, c.T))


    #===============MINIMAXFIT=============================
    x = np.array([[1, 2, 3, 3, 2], [4, 1, 2, 5, 6], [7, 8, 9, -5, 7]])
    y = np.array([[7, 4, 1, 2, 5]])
    a, b, r = minimaxfit(x, y)
    # print(a, b, r)
    # a = [-2.776, 0.194, -0.030]
    # b = 9.403
    # r = 0.194