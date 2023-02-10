#include <igl/bbw.h>
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

//TODO: expose data, to leverage any precopmutation/additional results
void binding_bbw(py::module& m) {
    m.def("_bbw", [](EigenDRef<MatrixXd> V,
        EigenDRef<MatrixXi> F, VectorXi& b, 
        EigenDRef<MatrixXd> bc)
   {
        MatrixXd W;
        igl::BBWData data;

        MatrixXd Vd = V;
        MatrixXi Fd = F;
        MatrixXd bcd = bc;
        VectorXi bd = bd;
        igl::bbw(Vd, Fd, bd, bcd, data,  W);
        return W;
   });

}

