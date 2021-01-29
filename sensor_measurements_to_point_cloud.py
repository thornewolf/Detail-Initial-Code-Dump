import numpy as np
from uncertainties import unumpy as unp
from uncertainties import umath
import uncertainties
import pandas as pd
import itertools
import copy
import pickle

STEPS_PER_ROTATION = 200*1600
EACH_DIST = 5
LIDAR_UNCERT = 0.025

def unorm(v):
    return (v[0]**2+v[1]**2+v[2]**2)**0.5

def raw_to_xyz(dist, theta_step, phi_step):
    dist = uncertainties.ufloat(dist, LIDAR_UNCERT)

    theta_step = uncertainties.ufloat(theta_step, 0.05)
    phi_step = uncertainties.ufloat(phi_step, 0.05)

    return np.array([
        dist * umath.cos(theta_step * 360 / STEPS_PER_ROTATION * np.pi / 180) * umath.cos(phi_step * 360 / STEPS_PER_ROTATION * np.pi / 180),
        dist * umath.sin(theta_step * 360 / STEPS_PER_ROTATION * np.pi / 180) * umath.cos(phi_step * 360 / STEPS_PER_ROTATION * np.pi / 180),
        dist * umath.sin(phi_step * 360 / STEPS_PER_ROTATION * np.pi / 180)
        ])

def parse_point_measurement_string(point_measurement):
    dist, theta_steps, phi_steps = map(float, point_measurement.split(','))
    return (dist, theta_steps, phi_steps)

def get_sensor_measurements_from_file(file_name):
    raw_measurements = []
    with open(file_name, 'r') as f:
        raw_measurements = f.readlines()
    # dist, theta_steps, phi_steps
    parsed_measurements = list(map(parse_point_measurement_string, raw_measurements))
    return parsed_measurements

def sensor_measurements_to_xyz(measurements):
    return [raw_to_xyz(*measurement) for measurement in measurements]

sensor_measurements = get_sensor_measurements_from_file('sensor_measurement_test_file.csv')
xyzs = sensor_measurements_to_xyz(sensor_measurements)
pickle.dump(xyzs, open('point_cloud_with_uncertainties','wb'))