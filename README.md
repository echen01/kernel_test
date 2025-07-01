# A simple kernel emptiness test

Returns a kernel point if the kernel of a mesh is non-empty, and null otherwise. The C++ version is about 2x faster than the Python version.x


**Definition (Geometric Kernel):** The kernel $K$ of an object is the set of point from which the entire shape $S$ is visible. Formally, it is defined as 
$$K = \{p \in S \mid (1 - t) p + tq \in S, \forall q\in S, \forall t \in [0, 1]\}.$$

If the kernel is non-empty, then it is convex. The shape is called *star shaped* if the kernel is non-empty.


## Cloning
To begin, clone this repo with the Eigen submodule.
```bash
git clone --recursive https://github.com/echen01/kernel_test.git
```
If you forgot to clone recursively, run
```bash
cd kernel_test
git submodule update --init --recursive
```

## C++ 
First compile the program. 
```bash
cd cpp
make
```
Then run the program with a `.off` or `.obj` mesh.
Then 
```bash
./kernel_point path/to/mesh
```

## Python
The Python version requires `scipy` and `trimesh`.
```bash
cd python
python kernel_test.py path/to/mesh
```
