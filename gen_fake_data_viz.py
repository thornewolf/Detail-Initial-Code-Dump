import numpy as np
import random
import time

name = 'test_scan'

def gen_fake_data():
    return np.random.rand(10000,3)*10

a = gen_fake_data()

i = 1
while i < 10000:
    b = a[:i, :]
    print(b)
    np.save(name, b)
    time.sleep(random.random())
    i += random.randint(1, 5)