import numpy as np

g1 = np.array([[1, 0, 1], [1,  1, 0]])
g2 = np.array([[1, 1, 0, 0], [1, 1, 1, 1], [1, 1, 0, 1]])
print(np.kron(g1, g2))