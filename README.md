# A simple kernel emptiness test

Returns a kernel point if the kernel of a mesh is non-empty, and null otherwise.


## C++ 
First compile the program. Then run the program with a `.off` or `.obj` mesh.
```bash
cd cpp
make
```
Then 
```bash
./kernel_point path/to/mesh
```

## Python
Requires `scipy` and `trimesh`. 
```bash
cd python
python kernel_test.py path/to/mesh
```
