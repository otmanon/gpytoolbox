from gpytoolbox_bindings import viewer
import gpytoolbox as gpy
import numpy as np

name = "armadillo"
obj_file = "../unit_tests_data/" + name  +".obj"
[V, F] = gpy.read_mesh(obj_file)
step = 0

v = viewer()


def callback_pre_draw():
    global step
    U = V + 10*np.sin(step/10)
    v.set_vertices(U)
    step += 1
    return False


v.background_color(np.array([1,1,1, 1.0]))
v.set_mesh(V, F)
v.show_lines(False)

v.callback_pre_draw(callback_pre_draw)
v.is_animating(True)
v.max_fps(100)
v.launch()




