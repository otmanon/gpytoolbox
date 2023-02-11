from gpytoolbox_bindings import viewer, _unproject_onto_mesh
import gpytoolbox as gpy
import numpy as np
from scipy.spatial.transform import Rotation as R
name = "armadillo"
obj_file = "./unit_tests_data/" + name  +".obj"
[V, F] = gpy.read_mesh(obj_file)

rot = R.from_rotvec(np.pi * np.array([0, 1, 0])).as_matrix()
V = (rot @ V.T).T

[V_plane, F_plane] = gpy.regular_square_mesh(10)
#append a column of 0s to V_palne
V_plane = np.hstack((V_plane, np.zeros((V_plane.shape[0], 1))))
plane_rot = R.from_rotvec(-np.pi/2 * np.array([1, 0, 0])).as_matrix()
plane_s = 1000
plane_t = np.array([0, V[:, 1].min(), 0])
V_plane = plane_s * ((plane_rot @ V_plane.T).T) + plane_t


step = 0

v = viewer()


step = 0
a = 1000.0
period = 10
pos = a*np.array([0.5, 0.5, 0])
def callback_pre_draw():
    global step
    rot = R.from_rotvec(np.array([0, 1, 0])*step/period).as_matrix()
    new_pos = rot @ pos.T
    v.light_position(new_pos)
    step += 1
    return False



v.background_color(np.array([1,1,1, 1.0]))
v.set_mesh(V, F)
v.show_lines(False)
v.set_colors(np.array([0.9, 0.5, 0.5]))



v.is_shadow_mapping(True)
v.is_directional_light(True)
v.is_animating(True)

V_plane_specular = 1*np.ones((V_plane.shape[0], 3))
F_plane_specular = 1*np.ones((F_plane.shape[0], 3))
id = v.append_mesh(V_plane, F_plane)
v.set_mesh(V_plane, F_plane, id)
v.show_lines(False, id)

v.set_colors(np.array([0.6, 0.6, 0.9]), id)


v.look_at(   np.array([0, 5, 10]), np.array([0, 0, 0]) )
v.callback_pre_draw(callback_pre_draw)
v.zoom(10)
v.launch()




