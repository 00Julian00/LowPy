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

        void reallocate(size_t bytes)
        {
            if (ptr)
            {
                void* new_ptr = realloc(ptr, size + bytes);
                if (new_ptr)
                {
                    ptr = new_ptr;
                    size += bytes;
                }
                else
                {
                    throw std::bad_alloc();
                }
            }
            else
            {
                ptr = malloc(bytes);
                if (!ptr) {
                    throw std::bad_alloc();
                }
                size = bytes;
            }
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

        void read_raw(void* buffer, size_t offset, size_t num_bytes)
        {
            if (offset + num_bytes > size) throw std::out_of_range("Memory access out of bounds");
            memcpy(buffer, static_cast<char*>(ptr) + offset, num_bytes);
        }

        void write_raw(const void* buffer, size_t offset, size_t num_bytes)
        {
            if (offset + num_bytes > size) throw std::out_of_range("Memory access out of bounds");
            memcpy(static_cast<char*>(ptr) + offset, buffer, num_bytes);
        }


    ~MemoryPtr()
    {
        deallocate();
    }
};

PYBIND11_MODULE(lowpy_cpp, m)
{
    py::class_<MemoryPtr>(m, "MemoryPtr")        
        .def(py::init<>())
        .def(py::init<size_t>())
        .def("allocate", &MemoryPtr::allocate)
        .def("reallocate", &MemoryPtr::reallocate)
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

        // For copying pointers
        .def("read_bytes", [](MemoryPtr& self, size_t num_bytes, size_t offset)
        {
            std::string buffer(num_bytes, '\0');  // Create a string buffer
            self.read_raw(&buffer[0], offset, num_bytes);
            return py::bytes(buffer);  // Convert to Python bytes
        })
        .def("write_bytes", [](MemoryPtr& self, const py::bytes& data, size_t offset)
        {
            char* buffer;
            Py_ssize_t length;
            if (PyBytes_AsStringAndSize(data.ptr(), &buffer, &length) != -1)
            {
                self.write_raw(buffer, offset, length);
            }
        })

        .def_readonly("size", &MemoryPtr::size);
}