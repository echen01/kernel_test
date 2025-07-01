# A kernel emptiness test for meshes

Returns a kernel point if the kernel of a mesh is non-empty, and null otherwise.
The C++ and Python versions perform at about the same speed. The C++ version uses Seidel's LP algorithm for small dimensions, while Python uses the HiGHS Simplex solver.


**Definition (Geometric Kernel):** The kernel $K$ of an object is the set of point from which the entire shape $S$ is visible. Formally, it is defined as 
$$K = \{p \in S \mid (1 - t) p + tq \in S, \forall q\in S, \forall t \in [0, 1]\}.$$

If the kernel is non-empty, then it is convex. The shape is called *star shaped* if the kernel is non-empty.

## Thea algorithm
We use linear programming to test if the kernel of a mesh is non-empty.
Each face of the mesh corresponds to a plane $(a_i, b_i, c_i, d_i)$ satisfying the equation $a_ix + b_iy + c_iz + d_i = 0$. Then points $x \in \mathbb{R}^3$ satisfying the visibility constraint below, for all faces, are elements of the kernel. 
$$ Ax \leq b,$$

$$A = \begin{bmatrix} a_1 & b_1 & c_1 \\ \vdots &\vdots& \vdots \\ a_n & b_n & c_n\end{bmatrix}, \quad  b = \begin{bmatrix} -d_1 \\ \vdots \\ -d_n \end{bmatrix}$$

If no feasible solutions exist, the kernel is empty. 



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
