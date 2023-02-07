from gpytoolbox_bindings import viewer, _unproject_onto_mesh
import gpytoolbox as gpy
import numpy as np

name = "armadillo"
obj_file = "../unit_tests_data/" + name  +".obj"
[V, F] = gpy.read_mesh(obj_file)
step = 0

v = viewer()


def callback_pre_draw():
    global step

    return False

P = np.zeros((0, 3), dtype=np.float64)
def callback_mouse_down(button, modifier):
    global P
    print("mouse_down")

    view = v.view()
    proj = v.proj()
    viewport = v.viewport()
    mouse = v.current_mouse()
    curr_mouse = np.array([mouse[0], viewport[3] - mouse[1]])

    [fid, bary] = _unproject_onto_mesh(curr_mouse, view, proj, viewport, V, F)
    if fid > 0:
        newV = V[F[fid, 0]].reshape((1, 3))
        P = np.vstack((P, newV))

        v.set_points(P, np.array([[1.0, 0, 0]]))
    return False

v.background_color(np.array([1,1,1, 1.0]))
v.set_mesh(V, F)
v.show_lines(False)

v.set_colors(np.array([0.5, 0.5, 0.5]))
v.callback_pre_draw(callback_pre_draw)
v.callback_mouse_down(callback_mouse_down)
v.launch()




