# /// script
# requires-python = ">=3.13"
# dependencies = [
#     "scipy",
#     "trimesh",
# ]
# ///

import glob
from os import PathLike
import sys
from time import perf_counter_ns

import numpy as np
from scipy.optimize import linprog, OptimizeResult
import trimesh


def compute_half_spaces_from_triangles(triangles: np.ndarray) -> np.ndarray:
    """
    Computes the half spaces formed by a set of triangles.
    Each half space defines a plane [a, b, c, d] satisfying the equation:
    ax + by + cz + d = 0, where (a, b, c) is the normal vector of the plane,
    and d is the distance from the origin to the plane.


    triangles: (n, 3, 3) float array of triangles

    returns: (n, 4) float array of half planes, in point-normal form.
    """

    v1s = triangles[:, 0]  # (n, 3)
    v2s = triangles[:, 1]  # (n, 3)
    v3s = triangles[:, 2]  # (n, 3)

    normals = np.cross(v2s - v1s, v3s - v2s)  # (n, 3)
    normals = normals / np.linalg.norm(normals)  # (n, 3)

    d = -np.vecdot(normals, v1s)[:, np.newaxis]  # (n, 3)

    half_spaces = np.concatenate((normals, d), axis=1)  # (n, 4)

    return half_spaces  # (n, 4)


def kernel_point(mesh_name: PathLike) -> np.ndarray | None:
    """
    Returns a kernel point of the given mesh, or None if no such point exists.

    mesh_name (PathLike): The mesh to find a kernel point of.

    returns: A 3D array representing the kernel point, or None if no such point exists.
    """
    mesh = trimesh.load(mesh_name)

    half_spaces = compute_half_spaces_from_triangles(mesh.triangles)  # (n, 4)

    c = np.array([0.0, 0.0, 1.0])
    A = half_spaces[:, :3]  # (n, 3) # The normals of the half spaces
    b = -half_spaces[:, 3]  # (n, ) # The constant of the half spaces

    result: OptimizeResult = linprog(c, A, b)
    if result.success:
        kp = result.x  # (3, )
    else:
        kp = None

    return kp


def main() -> None:
    path = sys.argv[1]
    if len(sys.argv) != 2:
        print("Usage: python kernel_test.py <mesh_file_name>")
        print("Example: python kernel_test.py cube.obj")

    start = perf_counter_ns()
    kp = kernel_point(path)
    end = perf_counter_ns()

    print("Kernel point found:", kp)
    print(f"Time taken: {(end - start) * 1e-6:.2f}ms")


if __name__ == "__main__":
    main()
