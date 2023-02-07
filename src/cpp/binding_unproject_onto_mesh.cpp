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

void binding_unproject_onto_mesh(py::module& m) {
    m.def("_unproject_onto_mesh", [](EigenDRef<MatrixXd> V,
        EigenDRef<MatrixXd> view, EigenDRef<MatrixXd> proj,
        VectorXd& viewport)
        {
            MatrixXd U;
            MatrixXi F;
            Vector3d bary;
            int fid;

            MatrixXf Vf = V.cast<float>();
            MatrixXf viewf = view.cast<float>();
            MatrixXf projf = proj.cast<float>();
            VectorXf viewportf = viewport.cast<float>();
            igl::unproject_onto_mesh(Vf, viewf, projf, viewportf, U, F, fid, bary);
            return std::make_tuple(U, F, fid, bary);
        });

}

