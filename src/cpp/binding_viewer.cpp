
#include <igl/opengl/glfw/Viewer.h>
#include <igl/unproject_onto_mesh.h>
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include <string>

using namespace Eigen;
namespace py = pybind11;
using EigenDStride = Stride<Eigen::Dynamic, Eigen::Dynamic>;
template <typename MatrixType>
using EigenDRef = Ref<MatrixType, 0, EigenDStride>; //allows passing column/row order matrices easily


namespace iglv = igl::opengl::glfw;

// Pybind does not like libigl's default viewer constructor.
//Instead of changing the igl one, just wrap it. 


void data_list_check(iglv::Viewer& v, int id)
{
    if (v.data_list.size() < id) {
        printf("Data list entry not initialized, please \n \
                    add mesh with gpytoolbox.append_mesh(V, F), \n  \
                    or ensure data list has the correct size \n"); 
        //TODO: expose the data list as something a 
        //viewer can easily browse through
    }
}

//TODO: make it known to the user what types of colormaps exist
igl::ColorMapType string_to_colormap_type(std::string str)
{
    if (str == "viridis")
        return igl::COLOR_MAP_TYPE_VIRIDIS;
    else if (str == "inferno")
        return igl::COLOR_MAP_TYPE_INFERNO;
    else if (str == "magma")
        return igl::COLOR_MAP_TYPE_MAGMA;
    else if (str == "parula")
        return igl::COLOR_MAP_TYPE_PARULA;
    else if (str == "plasma")
        return igl::COLOR_MAP_TYPE_PLASMA;
    else if (str=="turbo")
        return  igl::COLOR_MAP_TYPE_TURBO;
    else if (str =="jet")
        return  igl::COLOR_MAP_TYPE_JET;
    
}

void binding_viewer(py::module& m) {
	py::class_<iglv::Viewer>(m, "viewer")
		.def(py::init<>())
        // setting mesh
        .def("set_mesh", [&](iglv::Viewer& v, EigenDRef<MatrixXd> V, EigenDRef<MatrixXi> F, int id=0) {
            data_list_check(v, id);
            v.data_list[id].set_mesh(V, F);
            }, py::arg("V"), py::arg("F"), py::arg("id") = 0)

        .def("set_vertices", [](iglv::Viewer& v, EigenDRef<MatrixXd> V, int id) {
            data_list_check(v, id);
            v.data_list[id].set_vertices(V);
        }, py::arg("V"), py::arg("id")=0)
        
        .def("compute_normals", [](iglv::Viewer& v, int id) {
            data_list_check(v, id);
            v.data_list[id].compute_normals();
        }, py::arg("id") = 0)

        .def("set_normals", [](iglv::Viewer& v, EigenDRef<MatrixXd> N, int id) {
            data_list_check(v, id);
            v.data_list[id].set_normals(N);
        }, py::arg("N"), py::arg("id") = 0)

        .def("set_points", [](iglv::Viewer& v, EigenDRef<MatrixXd> P, EigenDRef<MatrixXd> C, int id) {
            data_list_check(v, id);
            v.data_list[id].set_points(P, C);
        }, py::arg("P"), py::arg("C"), py::arg("id") = 0)


        .def("set_points", [](iglv::Viewer& v, EigenDRef<MatrixXd> V, EigenDRef<MatrixXi> E, EigenDRef<MatrixXd> C, int id) {
            data_list_check(v, id);
            v.data_list[id].set_edges(V, E, C);
        }, py::arg("V"), py::arg("E"), py::arg("C"), py::arg("id") = 0)


        .def("clear", [](iglv::Viewer& v, int id) {
            v.data_list[id].clear();
        },  py::arg("id") = 0)


        .def("append_mesh", [&](iglv::Viewer& v, EigenDRef<MatrixXd> V, EigenDRef<MatrixXi> F) {
            v.append_mesh();
            v.data_list[v.data_list.size() - 1].set_mesh(V, F);
            return v.data_list.size() - 1;
        })

        .def("append_mesh", [&](iglv::Viewer& v) {
            v.append_mesh();
            return v.data_list.size() - 1;
        })

        //color manipulation
        .def("background_color", [&](iglv::Viewer& v, Eigen::Vector4d & color) {
            v.core().background_color = color.cast<float>(); 
            })
        

        .def("set_colors", [&](iglv::Viewer& v, EigenDRef<MatrixXd> color, int id)
        {
            data_list_check(v, id);
            if (color.size() == 3)
            {
                RowVector3d c = Map<RowVector3d>(color.data(),3);
                v.data_list[id].set_colors(c);
            }
            else
            {
                v.data_list[id].set_colors(color);
            }
        }, py::arg("color"), py::arg("id")=0)
 

        .def("set_data", [&](iglv::Viewer& v, Eigen::VectorXd& d, int id,
            std::string colormap, int num_steps)
        {
            data_list_check(v, id);
            igl::ColorMapType cmap = string_to_colormap_type(colormap);
            v.data_list[id].set_data(d, cmap, num_steps);
        }, py::arg("d"), py::arg("id") = 0, 
            py::arg("colormap") = "viridis", py::arg("num_steps")=21)

        .def("set_data", [&](iglv::Viewer& v, Eigen::VectorXd& d, double caxis_min, 
            double caxis_max, int id,  std::string colormap, int num_steps)
        {
            data_list_check(v, id);
            igl::ColorMapType cmap = string_to_colormap_type(colormap);
            v.data_list[id].set_data(d, caxis_min, caxis_max, cmap, num_steps );
        }, py::arg("d"), py::arg("caxis_min"), py::arg("caxis_max"), py::arg("id") = 0, 
            py::arg("colormap") = "viridis", py::arg("num_steps") = 21)
        
        .def("set_colormap", [&](iglv::Viewer& v, EigenDRef<MatrixXd> CM, int id){
            data_list_check(v, id);
            v.data_list[id].set_colormap(CM);
        }, py::arg("CM"), py::arg("id")=0)
        
            
        .def("use_matcap", [&](iglv::Viewer& v, bool use_matcap, int id){
            data_list_check(v, id);
            v.data_list[id].use_matcap = use_matcap;
        }, py::arg("use_matcap"), py::arg("id")=0)

        .def("show_texture", [&](iglv::Viewer& v, bool show_texture, int id) {
            data_list_check(v, id);
            v.data_list[id].show_texture = show_texture;
        }, py::arg("show_texture"), py::arg("id") = 0)

        

        

        //miscallaneous
        .def("show_lines", [&](iglv::Viewer& v, bool show_lines, int id){
            data_list_check(v, id); 
            v.data_list[id].show_lines = show_lines;
            }, py::arg("show_lines"), py::arg("id")=0)

        .def("show_faces", [&](iglv::Viewer& v, bool show_faces, int id) {
            data_list_check(v, id);
            v.data_list[id].show_faces = show_faces;
            }, py::arg("show_faces"), py::arg("id") = 0)

        .def("double_sided", [&](iglv::Viewer& v, bool double_sided, int id) {
            data_list_check(v, id);
            v.data_list[id].double_sided = double_sided;
            }, py::arg("double_sided"), py::arg("id") = 0)

       .def("invert_normals", [&](iglv::Viewer& v, bool invert_normals, int id) {
            data_list_check(v, id);
            v.data_list[id].invert_normals = invert_normals;
            }, py::arg("invert_normals"), py::arg("id") = 0)

       .def("is_visible", [&](iglv::Viewer& v, bool is_visible, int id) {
            data_list_check(v, id);
            v.data_list[id].is_visible = is_visible;
            }, py::arg("is_visible"), py::arg("id") = 0)

       .def("show_faces", [&](iglv::Viewer& v, bool show_faces, int id) {
            data_list_check(v, id);
            v.data_list[id].show_faces = show_faces;
            }, py::arg("show_faces"), py::arg("id") = 0)

        .def("face_based", [&](iglv::Viewer& v, bool face_based, int id) {
            data_list_check(v, id);
            v.data_list[id].face_based = face_based;
        }, py::arg("face_based"), py::arg("id") = 0)

        .def("shininess", [&](iglv::Viewer& v, float shininess, int id) {
            data_list_check(v, id);
            v.data_list[id].shininess = shininess;
        }, py::arg("shininess"), py::arg("id") = 0)



                

       .def("launch", [&](iglv::Viewer& v) 
       	    {v.launch(); })
		
    
        //camera
        .def("look_at", [&](iglv::Viewer& v, Eigen::RowVector3d& eye, Eigen::RowVector3d& center)
        {
            v.core().camera_eye = eye.cast<float>();
            v.core().camera_center = center.cast<float>();
        })

        .def("zoom", [&](iglv::Viewer& v, double zoom)
        {
            v.core().camera_zoom = zoom;
        })



        // callbacks
        .def("callback_pre_draw", [&](iglv::Viewer& v, std::function<bool(void)>& func){
            {
                auto wrapperFunc = [=](igl::opengl::glfw::Viewer&) -> bool {
                    return func();
                };
                v.callback_pre_draw = wrapperFunc; 
            };})

        .def("callback_post_draw", [&](iglv::Viewer& v, std::function<bool(void)>& func)
            {
                auto wrapperFunc = [=](igl::opengl::glfw::Viewer&) -> bool {
                return func();
            };
            v.callback_post_draw = wrapperFunc;
            })

        .def("callback_key_pressed", [&](iglv::Viewer& v, std::function<bool(unsigned int, int)>& func)
            {
                 auto wrapperFunc = [=](igl::opengl::glfw::Viewer&, unsigned int button, int modifier) -> bool {
                    return func(button, modifier);
                };
                v.callback_key_pressed = wrapperFunc; 
            })

        .def("callback_key_down", [&](iglv::Viewer& v, std::function<bool(unsigned int, int)>& func)
            {
                auto wrapperFunc = [=](igl::opengl::glfw::Viewer&, unsigned int button, int modifier) -> bool {
                return func(button, modifier);
            };
            v.callback_key_down = wrapperFunc;
            })

        .def("callback_key_up", [&](iglv::Viewer& v, std::function<bool(unsigned int, int)>& func)
            {
                auto wrapperFunc = [=](igl::opengl::glfw::Viewer&, unsigned int button, int modifier) -> bool {
                return func(button, modifier);
            };
            v.callback_key_up = wrapperFunc;
            })

        .def("callback_mouse_down", [&](iglv::Viewer& v, std::function<bool(unsigned int, int)>& func)
            {
                auto wrapperFunc = [=](igl::opengl::glfw::Viewer&, unsigned int button, int modifier) -> bool {
                return func(button, modifier);
            };
            v.callback_mouse_down = wrapperFunc;
            })

        .def("callback_mouse_up", [&](iglv::Viewer& v, std::function<bool(unsigned int, int)>& func)
            {
                auto wrapperFunc = [=](igl::opengl::glfw::Viewer&, unsigned int button, int modifier) -> bool {
                return func(button, modifier);
            };
            v.callback_mouse_up = wrapperFunc;
            })

        .def("callback_mouse_move", [&](iglv::Viewer& v, std::function<bool(unsigned int, int)>& func)
            {
                auto wrapperFunc = [=](igl::opengl::glfw::Viewer&, unsigned int button, int modifier) -> bool {
                return func(button, modifier);
            };
            v.callback_mouse_move = wrapperFunc;
            })


        .def("callback_mouse_move", [&](iglv::Viewer& v, std::function<bool(unsigned int, int)>& func)
            {
                auto wrapperFunc = [=](igl::opengl::glfw::Viewer&, unsigned int button, int modifier) -> bool {
                return func(button, modifier);
            };
            v.callback_mouse_move = wrapperFunc;
         })
                
        .def("callback_mouse_scroll", [&](iglv::Viewer& v, std::function<bool(float)>& func)
        {
            auto wrapperFunc = [=](igl::opengl::glfw::Viewer&,float dy) -> bool {
            return func(dy);
        };
        v.callback_mouse_scroll = wrapperFunc;
        })
        
        .def("is_animating", [&](iglv::Viewer& v, bool is_animating)
        {
            v.core().is_animating = is_animating;
        })

        .def("max_fps", [&](iglv::Viewer& v, int max_fps)
        {
            v.core().animation_max_fps = max_fps;
        })

        .def("view", [&](iglv::Viewer& v)
        {
            return v.core().view.cast<double>();
        })

        .def("proj", [&](iglv::Viewer& v)
        {
            return v.core().proj.cast<double>();
        })

        .def("viewport", [&](iglv::Viewer& v)
        {
            return v.core().viewport.cast<double>();
        })

        .def("current_mouse", [&](iglv::Viewer& v)
        {
            return Vector2d(v.current_mouse_x, v.current_mouse_y);
        })

        .def("get_selection", [&](iglv::Viewer& v)
        {
            RowVector3f last_mouse  = Eigen::RowVector3f(
                v.current_mouse_x, v.core().viewport(3) - v.current_mouse_y, 0);
                // Find closest point on mesh to mouse position
            int hit_id = -1;
            int fid;
            Eigen::Vector3f bary;
            for (int i = 0; i < v.data_list.size(); i++)
            {
         
                VectorXd u =
                    v.data_list[i].V;

                MatrixXd U = Map<MatrixXd>(u.data(), u.rows() / 3, 3);

                MatrixXi F = v.data_list[i].F;
                if (igl::unproject_onto_mesh(
                    last_mouse.head(2),
                    v.core().view,
                    v.core().proj,
                    v.core().viewport,
                    U, F,
                    fid, bary))
                {
                    hit_id = i;
                    break;
                }
            }

            return std::make_tuple(hit_id, fid, bary);
        });

        ;






        

       

}