import numpy as np
from uncertainties import unumpy as unp
from uncertainties import umath
import uncertainties
import pandas as pd
import itertools
import copy

# p1 = np.reshape([-0.99,0.01,0], (3,))
# p2 = np.reshape([1,0,0], (3,))

# xax = np.reshape([1,0,0], (3,))
# p2p1 = p2-p1


# A = xax
# B = p2p1
# A = A / np.linalg.norm(A)
# B = B / np.linalg.norm(B)

# v = np.cross(A, B)
# s = np.linalg.norm(v)
# c = np.dot(A, B)

# ssm = np.array([
#     [0, -v[2], v[1]],
#     [v[2], 0, -v[0]],
#     [-v[1], v[0], 0]
# ])

# R = np.eye(3) + ssm + np.matmul(ssm, ssm) / (1+c)


# print(f'{np.linalg.norm(R)=}')
# print(f'{R=}')
# print(f'{B - np.matmul(R,A)=}')

STEPS_PER_ROTATION = 200*1600
EACH_DIST = 5
LIDAR_UNCERT = 0.025

def unorm(v):
    return (v[0]**2+v[1]**2+v[2]**2)**0.5

def raw_to_xyz(dist, theta_step, phi_step):
    dist = uncertainties.ufloat(dist, LIDAR_UNCERT)
    # I want 45 degrees to the left so I reverse engineer it to the propogate uncertainty
    # 5% uncertainty in the step but relative to it so I do 0.05

    theta_step = uncertainties.ufloat(theta_step, 0.05)
    # 20 degrees up
    phi_step = uncertainties.ufloat(phi_step, 0.05)

    # print(f'{(dist, theta_step, phi_step)=}')
    # print(f'    cos theta = { umath.cos(theta_step * 360 / STEPS_PER_ROTATION * np.pi / 180)}')
    # print(f'    sin theta = { umath.sin(theta_step * 360 / STEPS_PER_ROTATION * np.pi / 180)}')
    # print(f'    cos phi = { umath.cos(phi_step * 360 / STEPS_PER_ROTATION * np.pi / 180)}')
    # print(f'    sin phi = { umath.sin(phi_step * 360 / STEPS_PER_ROTATION * np.pi / 180)}')
    return np.array([
        dist * umath.cos(theta_step * 360 / STEPS_PER_ROTATION * np.pi / 180) * umath.cos(phi_step * 360 / STEPS_PER_ROTATION * np.pi / 180),
        dist * umath.sin(theta_step * 360 / STEPS_PER_ROTATION * np.pi / 180) * umath.cos(phi_step * 360 / STEPS_PER_ROTATION * np.pi / 180),
        dist * umath.sin(phi_step * 360 / STEPS_PER_ROTATION * np.pi / 180)
        ])


measured_distance_target1 = EACH_DIST
# I want 45 degrees to the left so I reverse engineer it to the propogate uncertainty
# 5% uncertainty in the step but relative to it so I do 0.05
measured_motor_step_theta_target1 = 0
# 20 degrees up
measured_motor_step_phi_target1 = 20

measured_distance_target2 = EACH_DIST
# I want 45 degrees to the left so I reverse engineer it to the propogate uncertainty
# 5% uncertainty in the step but relative to it so I do 0.05
measured_motor_step_theta_target2 = 45
# 20 degrees up
measured_motor_step_phi_target2 = 20

p1 = raw_to_xyz(measured_distance_target1, measured_motor_step_theta_target1  / 360 * STEPS_PER_ROTATION, measured_motor_step_phi_target1 / 360 * STEPS_PER_ROTATION)
p2 = raw_to_xyz(measured_distance_target2, measured_motor_step_theta_target2  / 360 * STEPS_PER_ROTATION, measured_motor_step_phi_target1 / 360 * STEPS_PER_ROTATION)
# p1 = np.array([
#     measured_distance_target1 * umath.cos(measured_motor_step_theta_target1 * 360 / 200 * np.pi / 180) * umath.cos(measured_motor_step_phi_target1 * 360 / 200 * np.pi / 180),
#     measured_distance_target1 * umath.sin(measured_motor_step_theta_target1 * 360 / 200 * np.pi / 180) * umath.cos(measured_motor_step_phi_target1 * 360 / 200 * np.pi / 180),
#     measured_distance_target1 * umath.sin(measured_motor_step_phi_target1 * 360 / 200 * np.pi / 180)
#     ])

# p2 = np.array([
#     measured_distance_target2 * umath.cos(measured_motor_step_theta_target2 * 360 / 200 * np.pi / 180) * umath.cos(measured_motor_step_phi_target2 * 360 / 200 * np.pi / 180),
#     measured_distance_target2 * umath.sin(measured_motor_step_theta_target2 * 360 / 200 * np.pi / 180) * umath.cos(measured_motor_step_phi_target2 * 360 / 200 * np.pi / 180),
#     measured_distance_target2 * umath.sin(measured_motor_step_phi_target2 * 360 / 200 * np.pi / 180)
#     ])

print(f'{p1=}')
print(f'{p2=}')


# p1 = np.reshape(unp.uarray([0,0,0], [0.015,0.015,0.015]), (3,))
# p2 = np.reshape(unp.uarray([10,0.05,0], [0.015,0.015,0.015]), (3,))

xax = np.reshape(unp.uarray([1,0,0], [0.0,0.0,0.0]), (3,))
p2p1 = p2-p1

TARGET_COUNT = 4
NUM_PAIRS = TARGET_COUNT * (TARGET_COUNT - 1)
print(f'{p2p1=}')
p2p1 = sum(copy.deepcopy(p2p1) for i in range(NUM_PAIRS))/NUM_PAIRS
print(f'{p2p1=}')


# exit()

A = p2p1
B = xax
A = A / unorm(A)
# print(f'{A=}')
B = B / unorm(B)

v = np.cross(A, B)
s = unorm(v)
c = np.dot(A, B)

ssm = np.array([
    [0, -v[2], v[1]],
    [v[2], 0, -v[0]],
    [-v[1], v[0], 0]
])

R = np.eye(3) + ssm + np.matmul(ssm, ssm) / (1+c)
R = unp.matrix(R)

# print(f'{(R.shape, A.shape, np.reshape(A, (1,3)).shape)=}')
print(f'{R=}')
# print(f'{B - np.matmul(R,A)=}')

# q = np.reshape(unp.uarray([1,0,-0.5], [0.015,0.015,0.015]), (3,))
my_position = np.matmul(R,-p1.copy())
print(f'{my_position=}')

uxs = []
uys = []
uzs = []
thetas = []
phis = []
for theta, phi in itertools.product(range(0, STEPS_PER_ROTATION, STEPS_PER_ROTATION//40),range(0, STEPS_PER_ROTATION, STEPS_PER_ROTATION//40)):
    random_reading = my_position + np.matmul(R,raw_to_xyz(1, theta, phi))
    uxs.append(random_reading[0,0].std_dev)
    uys.append(random_reading[0,1].std_dev)
    uzs.append(random_reading[0,2].std_dev)
    thetas.append(theta)
    phis.append(phi)
    # print(f'{random_reading}')

ucomb = [max(x,y,z) for x,y,z in zip(uxs,uys, uzs)]
df = pd.DataFrame(ucomb)
print(df.describe())

# from matplotlib import pyplot as plt

# groups = itertools.groupby(zip(thetas, ucomb), lambda x: x[0])
# for key, group in groups:
#     group = np.array(list(group))
#     # print(group)
#     plt.scatter(key, np.max(group[:,1]))
#     plt.scatter(key, np.percentile(group[:,1], 95))
#     plt.scatter(key, np.percentile(group[:,1], 75))
#     plt.scatter(key, np.mean(group[:,1]))
#     plt.scatter(key, np.min(group[:,1]))
# # plt.scatter(phis, uxs)
# # plt.legend(['theta', 'phi'])
# plt.savefig('angles vs ux')