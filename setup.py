from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        "lowpy",  # Name of the extension
        ["lowpy.cpp"],  # Source files
        include_dirs=[pybind11.get_include()],
        language='c++'
    ),
]

setup(
    name="lowpy",
    ext_modules=ext_modules,
    install_requires=['pybind11>=2.4.3'],
    setup_requires=['pybind11>=2.4.3'],
)