#include "Pathfinder.hpp"

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

namespace py = pybind11;
using namespace std;
PYBIND11_MODULE(pathfinder, m)
{
    py::class_<Pathfinder>(m, "Pathfinder")
        .def(py::init<int, int>())
        .def("search_astar", &Pathfinder::searchAStar)
        .def("clear", &Pathfinder::clearGrid)
        .def("set_grid", &Pathfinder::setGrid)
        .def("get_path", &Pathfinder::getPath)
        .def("get_runs", &Pathfinder::getRuns)
        .def("cell_index", &Pathfinder::index);
}