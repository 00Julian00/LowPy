#include <pybind11/pybind11.h>
#include <iostream>
#include <cmath>

using namespace std;
namespace py = pybind11;

void print(string text) // Because cout << text << "\n" is ugly af
{
    cout << text << "\n";
}

class MemoryPtr
{
    public:
        void* ptr = nullptr;
        size_t size = 0;

        MemoryPtr()
        {

        }

        MemoryPtr(size_t bytes)
        {
            allocate(bytes);
        }

        void allocate(size_t bytes)
        {
            ptr = malloc(bytes);
            size += bytes;
        }

        void deallocate()
        {
            if (ptr)
            {
                free(ptr);
                ptr = nullptr;
                size = 0;
            }
        }

        template<typename T>
        void write(const T& value, size_t offset = 0)
        {
            if (offset + sizeof(T) > size) throw std::out_of_range("Memory access out of bounds");
            *reinterpret_cast<T*>(static_cast<char*>(ptr) + offset) = value;
        }

        template<typename T>
        T read(size_t offset = 0)
        {
            if (offset + sizeof(T) > size) throw std::out_of_range("Memory access out of bounds");
            return *reinterpret_cast<T*>(static_cast<char*>(ptr) + offset);
        }

        void copy(MemoryPtr& copy_to) const
        {
            copy_to.deallocate();
            copy_to.allocate(size);
            if (ptr && size > 0)
            {
                memcpy(copy_to.ptr, ptr, size);
            }
        }

    ~MemoryPtr()
    {
        deallocate();
    }
};

PYBIND11_MODULE(lowpy, m)
{
    m.doc() = "LowPy provides low level access to C++ pointers from python in a memory safe way.";

    py::class_<MemoryPtr>(m, "MemoryPtr")        
        .def(py::init<>())
        .def(py::init<size_t>())
        .def("allocate", &MemoryPtr::allocate)
        .def("deallocate", &MemoryPtr::deallocate)
        // Add default values for offset parameters
        .def("write_int", &MemoryPtr::write<int>, py::arg("value"), py::arg("offset") = 0)
        .def("write_float", &MemoryPtr::write<float>, py::arg("value"), py::arg("offset") = 0)
        .def("write_double", &MemoryPtr::write<double>, py::arg("value"), py::arg("offset") = 0)
        .def("write_char", &MemoryPtr::write<char>, py::arg("value"), py::arg("offset") = 0)
        .def("read_int", &MemoryPtr::read<int>, py::arg("offset") = 0)
        .def("read_float", &MemoryPtr::read<float>, py::arg("offset") = 0)
        .def("read_double", &MemoryPtr::read<double>, py::arg("offset") = 0)
        .def("read_char", &MemoryPtr::read<char>, py::arg("offset") = 0)
        .def("copy", &MemoryPtr::copy)
        .def_readonly("size", &MemoryPtr::size);
}
