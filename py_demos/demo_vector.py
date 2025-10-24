# search for libraray like bla.cpython-312-darwin.so in the build directory:
# import sys
# sys.path.append('/Users/joachim/texjs/lva/ws2324/ScientificComputing/ASC-bla/build')
# from bla import Vector

# for Development
# use shared-object-file from the build folder (faster workflow)
import sys
import os
import numpy as np
sys.path.append(os.path.join(os.path.dirname(__file__), "../build"))
from bla import Vector
# ~ for Development

# import from the installed ASCsoft package:
#from ASCsoft.bla import Vector

x = Vector(3)
y = Vector(3)

for i in range(len(x)):
    x[i] = i
y[:] = 2    

x_np = np.asarray(x)

print(x)
print(x_np)



