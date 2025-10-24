# for Development
# use shared-object-file from the build folder (faster workflow)
import sys
import os
import numpy as np
sys.path.append(os.path.join(os.path.dirname(__file__), "../build"))
from bla import Matrix
# ~ for Development

#from ASCsoft.bla import Matrix

n = 8
A = Matrix(n,n)

# Set elements
for i in range(n):
    for j in range(n):
        A[i,j] = i*n + j

A_np = np.asarray(A)

print(A)
print("\n")
print(A_np)