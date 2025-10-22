# for Development
# use shared-object-file from the build folder (faster workflow)
import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), "../build"))
from bla import Vector
# ~ for Development

#from ASCsoft.bla import Matrix


A = Matrix(2,2)

# Set elements
A[0,0] = 1
A[0,1] = 2
A[1,0] = 3
A[1,1] = 4

print("Matrix A:")
print(A)