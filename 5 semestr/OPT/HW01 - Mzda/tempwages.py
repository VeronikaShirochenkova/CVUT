import numpy as np
import matplotlib.pyplot as plt
import math

# The method of least squares
def fit_wages(t, M):
    A = np.vstack([np.ones(len(t)), t]).T                   # Create matrix
    x1, x2 = np.linalg.lstsq(A, M, rcond=None)[0]
    return x1, x2

# Result of M(t) = x1 + x2t
def quarter2_2009(x):
    t = 2009.25
    M = x[0] + x[1]*t
    return M

# create to arrays for sin/cos
def create_arrays(t, omega):
    sinus = []
    cosinus = []
    for i in range(len(t)):
        sinus.append(math.sin(omega * t[i]))
        cosinus.append(math.cos(omega * t[i]))
    return sinus, cosinus

# The method of least squares
def fit_temps(t, T, omega):
    sinus, cosinus = create_arrays(t, omega)
    A = np.vstack([np.ones(len(t)), t, sinus, cosinus]).T   # Create matrix
    x1, x2, x3, x4 = np.linalg.lstsq(A, T, rcond=None)[0]
    return x1, x2, x3, x4


# ============== MAIN ===================
if __name__ == '__main__':

    # # SALARY PART
    years_list = []
    salaries_list = []

    # Open file with data
    file_1 = open('mzdy.txt', 'r')

    # Read input data
    for line in file_1:
        num_1 = []
        num_1.append([float(x) for x in line.split()])
        years_list.append(num_1[0][0])
        salaries_list.append(num_1[0][1])

    # Create two vectors
    years = np.array(years_list)
    salaries = np.array(salaries_list)

    # Call a function, that will return two parameters for linear function
    x1, x2 = fit_wages(years, salaries)
    print("Solution is: M(t) =",x1, "+", x2, "* t")
    x_s = []
    x_s.append(x1)
    x_s.append(x2)

    # Call a function, that will return salary estimation in the second quarter of 2009
    point = quarter2_2009(x_s)
    print("Estimate in the second quarter of 2009 is: ", point)


    # TEMPERATURE PART
    days_list = []
    temp_list = []
    omega = (2 * math.pi) / 365

    # Open file with data
    file_2 = open('teplota.txt', 'r')

    # Read input data
    for line in file_2:
        num_2 = []
        num_2.append([float(x) for x in line.split()])
        days_list.append(num_2[0][0])
        temp_list.append(num_2[0][1])

    # Create two vectors
    days = np.array(days_list)
    temp = np.array(temp_list)

    # Call a function, that will return four parameters for non-linear function
    x_1, x_2, x_3, x_4 = fit_temps(days, temp, omega)
    print("Solution is: T(t) =", x_1, "+", x_2, "* t +", x_3, "* sin(wt) *", x_4, "* cos(wt)")

    # # =================== DRAWING PART [SALARY] =====================
    fig, ax = plt.subplots()

    ax.scatter(years, salaries, c='blue')
    ax.scatter(2009.25, point, c='deeppink')

    ax.set_facecolor('white')  # background color
    ax.set_title('Prediction of the average gross wage')  # title

    fig.set_figwidth(8)  # width
    fig.set_figheight(8)  # height
    plt.plot(years, x1 + x2 * years, 'r') # estimated linear function

    plt.show()


    # =================== DRAWING PART [TEMPERATURE] =====================
    fig_2, ax_2 = plt.subplots()

    ax_2.scatter(days, temp, c='blue')

    ax_2.set_facecolor('white')  # background color
    ax_2.set_title('Temperature')  # title

    fig_2.set_figwidth(8)  # width
    fig_2.set_figheight(8)  # height

    sinus, cosinus = create_arrays(days, omega)
    sinus1 = np.array(sinus)
    cosinus1 = np.array(cosinus)

    plt.plot(days, x_1 + x_2 + x_3 * sinus1 + x_4 * cosinus1, 'r')  # estimated non-linear function

    plt.show()
