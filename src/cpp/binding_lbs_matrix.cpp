#include <igl/lbs_matrix.h>
#include <igl/repdiag.h>
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

void binding_lbs_matrix(py::module& m) {
    m.def("_lbs_matrix", [](EigenDRef<MatrixXd> V,
        EigenDRef<MatrixXd> W, int dim)
        {
            MatrixXd M;
            igl::lbs_matrix_column(V, W, M);
            if (dim > 1)
            {
                M = igl::repdiag(M, dim);
            }
            return M;
        }, py::arg("V"), py::arg("W"), py::arg("dim") = 1);

}

