# /// script
# requires-python = ">=3.13"
# dependencies = [
#     "scipy",
#     "trimesh",
# ]
# ///

import glob
from os import PathLike
from time import perf_counter

import numpy as np
from scipy.optimize import linprog
import trimesh


def compute_half_space_from_triangles(triangles: np.ndarray) -> np.ndarray:
    """
    Computes the half spaces formed by a set of triangles.
    Each half space defines a plane [a, b, c, d] satisfying the equation:
    ax + by + cz + d = 0, where (a, b, c) is the normal vector of the plane,
    and d is the distance from the origin to the plane.


    triangles: (n, 3, 3) float array of triangles

    returns: (n, 4) float array of half planes, in point-normal form.
    """

    v1s, v2s, v3s = (
        triangles[:, 0],
        triangles[:, 1],
        triangles[:, 2],
    )  # (n, 3, 3) -> (n, 3), (n, 3), (n, 3)

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

    half_spaces = compute_half_space_from_triangles(mesh.triangles)  # (n, 4)

    c = np.array([0.0, 0.0, 1.0])
    A = half_spaces[:, :3]  # (n, 3) # The normals of the half spaces
    b = -half_spaces[:, 3]  # (n, ) # The constant of the half spaces

    result = linprog(c, A, b)
    if result.success:
        kp = result.x  # (3, )
    else:
        kp = None

    return kp


def main() -> None:
    paths = glob.glob("../../polyhedron_kernel/datasets/Thingi/*.off")
    start = perf_counter()
    for path in paths:
        kp = kernel_point(path)

    end = perf_counter()
    print(f"Time taken: {end - start:.2f} seconds for {len(paths)} models.")


if __name__ == "__main__":
    main()
