#include <pybind11/pybind11.h>
#include "KVStore.h"

namespace py = pybind11;

PYBIND11_MODULE(KVStoreModule, m) {
    py::class_<KVItem>(m, "KVItem")
        .def(py::init<>())
        .def_readwrite("id", &KVItem::id)
        .def_readwrite("score", &KVItem::score);

    py::class_<KVStore>(m, "KVStore")
        .def(py::init<std::string>())
        .def("add_item", &KVStore::add_item)
        .def("save", &KVStore::save)
        .def("load", &KVStore::load)
        .def("search", &KVStore::search);
}
