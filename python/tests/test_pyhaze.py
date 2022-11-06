import pyhaze
import numpy as np


def pyhaze_smoothnn_iter():
    mesh = [] # TODO
    pvd_data = []
    res = pyhaze.smoothnn(mesh, pvd_data, num_iter=5)
    assert res.size == 146852
    assert isinstance(res, np.ndarray)
