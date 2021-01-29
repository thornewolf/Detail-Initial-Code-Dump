import numpy as np
from uncertainties import unumpy as unp

m1 = unp.uarray([[1,2,3],[3,4,5],[5,6,7]], [[0.1,0.1,0.1],[0.1,0.1,0.1],[0.1,0.1,0.1]])
m2 = unp.uarray([[3,2,3],[3,3,3],[9,10,15]], [[0.1,0.1,0.1],[0.1,0.1,0.1],[0.1,0.1,0.1]])

print(f'{np.matmul(m1, m2)=}')