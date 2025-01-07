# LowPy
## Access to C++ pointers and low-level memory manipulation. Memory safe.

### Installing:
1. Clone the repository
2. Navigate to the repository on your local harddrive and install it via ```bash pip install .```

### How to use:
```py
import lowpy as lp

# Create a new pointer
ptr = lp.MemoryPtr()

# Allocate memory
ptr.allocate(num_of_bytes_to_allocate)

# Increase the amount of allocated memory
ptr.reallocate(num_of_bytes_to_allocate)

# Free memory (not strictly required, as all allocated data is managed by pythons garbage collector)
ptr.deallocate()

# Writing to memory. LowPy provides a few premade datatypes that can be used but you can also write raw bytes
ptr.write_int(value, offset)
ptr.write_double(value, offset) # Equivalent to pythons 'float'
ptr.write_char(value, offset)
ptr.write_byte(byte, offset)

# Reading from memory. There are a few premade datatypes the bytes can be interpreted as, but you can also read the raw bytes
ptr.read_int(offset)
ptr.read_double(offset) # Equivalent to pythons 'float'
ptr.read_char(offset)
ptr.read_bytes(amount, offset)

# To safely copy the data of one pointer to another do this:
ptr_copy = lp.copy_memory_pointer(ptr)
```

### How can this be used in projects?
This project was mainly a learning project to learn c++ and low-level memory management. I am not sure if LowPy can really be used for large scale python project or if all functionality this offers is completly redundant.