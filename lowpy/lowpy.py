from . import lowpy_cpp

class MemoryPtr:
    def __init__(self, bytes : int = 0) -> None:
        """
        Create a new pointer.

        Arguments:
            bytes (int): How many bytes should be allocated in memory. Defaults to 0.
        """
        if bytes == 0:
            self._ptr = lowpy_cpp.MemoryPtr()
        else:
            self._ptr = lowpy_cpp.MemoryPtr(bytes)

    @property
    def size(self) -> int:
        return self._ptr.size
    
    # Memory management:
    def allocate(self, bytes: int) -> None:
        """
        Allocates memory. Comparable to C++ "malloc()" function.

        Arguments:
            bytes (int): How many bytes should be allocated in memory.
        """
        self._ptr.allocate(bytes)

    def reallocate(self, bytes : int) -> None:
        """
        Increases the amount of allocated memory. Comparable to C++ "realloc()" function.

        Arguments:
            bytes (int): How many bytes should be allocated in memory.
        """
        self._ptr.reallocate(bytes)

    def deallocate(self) -> None:
        """
        Frees all memory the pointer holds. All saved data in the pointer will be lost.
        """
        self._ptr.deallocate()

    # Writing:
    def write_int(self, value: int, offset: int = 0) -> None:
        """
        Save an integer to the allocated memory.

        Arguments:
            value (int): The integer to be saved.
            offset (int): How many bytes from the start the data should be offset. Defaults to 0.
        """
        self._ptr.write_int(value, offset)
    
    def write_double(self, value: float, offset: int = 0) -> None:
        """
        Save a double to the allocated memory.

        Arguments:
            value (float): The double to be saved.
            offset (int): How many bytes from the start the data should be offset. Defaults to 0.
        """
        self._ptr.write_double(value, offset)

    def write_char(self, value: str, offset: int = 0) -> None:
        """
        Save a char to the allocated memory.

        Arguments:
            value (string): The char to be saved.
            offset (int): How many bytes from the start the data should be offset. Defaults to 0.
        """
        self._ptr.write_char(value, offset)

    def write_byte(self, value: bytes, offset: int = 0) -> None:
        """
        Write a raw byte to memory.

        Arguments:
            value (bytes): The byte that should be written to memory.
            offset (int): How many bytes from the start the data should be offset. Defaults to 0.
        """
        self._ptr.write_bytes(value, offset)

    # Reading:
    def read_int(self, offset: int = 0) -> int:
        """
        Read data from memory interpreted as an integer.

        Arguments:
            offset (int): How many bytes offset from the start should the read beginn.

        Returns:
            int: The read data interpreted as an integer.
        """
        return self._ptr.read_int(offset)

    def read_double(self, offset: int = 0) -> float:
        """
        Read data from memory interpreted as a double.

        Arguments:
            offset (int): How many bytes offset from the start should the read beginn.

        Returns:
            float: The read data interpreted as a double.
        """
        return self._ptr.read_double(offset)

    def read_char(self, offset: int = 0) -> str:
        """
        Read data from memory interpreted as a char.

        Arguments:
            offset (int): How many bytes offset from the start should the read beginn.

        Returns:
            string: The read data interpreted as a char.
        """
        return self._ptr.read_char(offset)
    
    def read_bytes(self, amount: int, offset: int = 0) -> bytes:
        """
        Read data from memory as a raw byte.

        Arguments:
            amount (int): How many bytes should be read.
            offset (int): How many bytes offset from the start should the read beginn.
        
        Returns:
            bytes: The read data as a bytes object.
        """
        return self._ptr.read_bytes(amount, offset)

def copy_memory_pointer(source: MemoryPtr) -> MemoryPtr:
    """
    Safely copies the data of one memory pointer to another.

    Arguments:
        source (MemoryPtr): The pointer that should be copied.

    Returns:
        MemoryPtr: A new pointer containing a copy of the data of the source pointer.
    """

    if not source._ptr:
        return MemoryPtr()

    data = source._ptr.read_bytes(source._ptr.size, 0)

    copy = MemoryPtr(source.size)

    copy._ptr.write_bytes(data, 0)

    return copy