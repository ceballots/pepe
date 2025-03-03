#include <iostream>
#include <stdexcept>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "KVStore.h" 

namespace py = pybind11;

KVStore::KVStore(const std::string& filename) : fd(-1), mmap_data(nullptr), mmap_size(0) {
    fd = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        throw std::runtime_error("Failed to open mmap file.");
    }

    // TODO: not allocate max 
    mmap_size = MAX_ITEMS * ITEM_SIZE;
    ftruncate(fd, mmap_size);

    mmap_data = (uint8_t*) mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mmap_data == MAP_FAILED) {
        close(fd);
        throw std::runtime_error("Failed to mmap file.");
    }
}

KVStore::~KVStore() {
    if (mmap_data != nullptr) {
        munmap(mmap_data, mmap_size);
    }
    if (fd != -1) {
        close(fd);
    }
}
// TODO: Add batch write
void KVStore::add_item(uint32_t key, const std::vector<KVItem>& value) {
    if (key >= MAX_ITEMS) {
        throw std::out_of_range("Key is out of range.");
    }

    uint8_t* ptr = mmap_data + key * ITEM_SIZE;
    uint32_t size = value.size();
    
    std::memcpy(ptr, &size, sizeof(uint32_t));
    ptr += sizeof(uint32_t);

    for (const auto& item : value) {
        std::memcpy(ptr, &item.id, sizeof(uint32_t)); 
        ptr += sizeof(uint32_t);
        std::memcpy(ptr, &item.score, sizeof(float));
        ptr += sizeof(float);
    }
}

void KVStore::save() {
    msync(mmap_data, mmap_size, MS_SYNC);
}

void KVStore::load() {
}
// TODO: Add batch search
std::vector<KVItem> KVStore::search(uint32_t key) {
    if (key >= MAX_ITEMS) {
        throw std::out_of_range("Key is out of range.");
    }

    uint8_t* ptr = mmap_data + key * ITEM_SIZE;
    uint32_t size;
    std::memcpy(&size, ptr, sizeof(uint32_t));
    ptr += sizeof(uint32_t);

    std::vector<KVItem> result(size);
    for (uint32_t i = 0; i < size; ++i) {
        std::memcpy(&result[i].id, ptr, sizeof(uint32_t));
        ptr += sizeof(uint32_t);
        std::memcpy(&result[i].score, ptr, sizeof(float));
        ptr += sizeof(float);
    }
    
    return result;
}

PYBIND11_MODULE(KVStoreModule, m) {
    py::class_<KVStore>(m, "KVStore")
        .def(py::init<const std::string&>())
        .def("add_item", &KVStore::add_item)
        .def("save", &KVStore::save)
        .def("load", &KVStore::load)
        .def("search", &KVStore::search);

    py::class_<KVItem>(m, "KVItem")
        .def(py::init<uint32_t, float>())
        .def_readwrite("id", &KVItem::id)
        .def_readwrite("score", &KVItem::score)
        .def("__repr__", [](const KVItem &item) {
            std::ostringstream stream;
            stream << "KVItem(id=" << item.id << ", score=" << item.score << ")";
            return stream.str();
        });
}
