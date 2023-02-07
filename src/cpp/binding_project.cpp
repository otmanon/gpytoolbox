#include <igl/project.h>
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

void binding_project(py::module& m) {
    m.def("_project", [](EigenDRef<MatrixXd> V,
        EigenDRef<MatrixXd> view, EigenDRef<MatrixXd> proj,
        VectorXd& viewport)
        {
        MatrixXd P;
        igl::project(V, view, proj, viewport, P);
        return P;
        });

}
