from gpytoolbox_bindings import viewer, _unproject_onto_mesh, _lbs_matrix
import gpytoolbox as gpy
import numpy as np
from scipy.spatial.transform import Rotation as R

name = "life_of_ocean"
obj_file = "./unit_tests_data/" + name  +"_tex.obj"
png_file = "./unit_tests_data/" + name  +"_tex.png"
[V, F, UV, F_UV] = gpy.read_mesh(obj_file, return_UV=True)

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
v.background_color(np.array([1,1,1, 1.0]))

v.set_mesh(V, F)
v.show_lines(False)
v.is_shadow_mapping(True)
v.is_directional_light(True)
v.light_position(np.array([1, 1, 1]))
v.is_animating(True)


v.set_uv(UV, F_UV)
v.set_texture(png_file)
v.show_texture(True)

W = np.ones((V.shape[0], 1))
J = _lbs_matrix(V, W, dim=1)
def guizmo_callback(T):
    # print("guizmo Transform : ", T)
    P = T[0:3, :4]
    p = P.flatten(order="F")
    X = (J @ p).reshape((V.shape[0], 3), order="F")
    v.set_vertices(X, 0);

T0 = np.identity(4, dtype=np.float64)
v.attach_guizmo(True, T0, guizmo_callback, "translate" )
# v.set_colors(np.array([0.6, 0.6, 0.9]), id)
#
#
# v.look_at(   np.array([0, 5, 10]), np.array([0, 0, 0]) )
#
# v.zoom(10)
v.is_animating(True)
v.max_fps(60)
v.launch()




