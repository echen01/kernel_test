#include <chrono>
#include <iostream>
#include <vector>
#include <string>

#include <Eigen/Eigen>
#include "sdlp.h"

#include "Mesh.h"
#include "BasicGeometricElements.h"
#include "BasicMeshElements.h"

void computeHalfSpacesFromTriangles(const std::vector<Triangle> &tris, const std::vector<Vertex> &verts, std::vector<HalfSpace> &halfSpaces)
{
    for (int i = 0; i < tris.size(); i++)
    {
        Triangle tri = tris[i];
        HalfSpace halfSpace(verts[tri.corners[0]].coords, verts[tri.corners[1]].coords, verts[tri.corners[2]].coords, false);
        halfSpaces.push_back(halfSpace);
    }
}

/**
 * Returns a kernel point of the given mesh, or NULL if no such point exists.
 * @param meshName The mesh to find a kernel point of, in .obj or .off format.
 * @return A 3D array representing the kernel point, or NULL if no such point exists.
 */
double *kernelPoint(std::string meshName)
{
    Mesh *mesh = new Mesh();

    if (meshName.substr(meshName.length() - 3, 3) == "off")
        mesh->loadOff(meshName.c_str());
    else
        mesh->loadObj(meshName.c_str());

    std::vector<HalfSpace> halfSpaceSet;
    computeHalfSpacesFromTriangles(mesh->getAllTris(), mesh->getAllVerts(), halfSpaceSet);

    int m = halfSpaceSet.size(); // Number of half-spaces
    Eigen::Vector3d x;           // decision variables
    Eigen::Vector3d c;           // objective coefficients
    Eigen::MatrixXd A(m, 3);     // constraint matrix
    Eigen::VectorXd b(m);        // constraint bound

    for (int i = 0; i < m; i++)
    {
        HalfSpace hp = halfSpaceSet[i];
        A.row(i) << hp.ABCD[0], hp.ABCD[1], hp.ABCD[2];
        b.row(i) << -hp.ABCD[3];
    }
    c << 0.0, 0.0, 1.0; // Extreme direction

    // Solves min c^T x subject to Ax <= b
    double minobj = sdlp::linprog<3>(c, A, b, x);

    if (minobj == std::numeric_limits<double>::infinity())
    {
        std::cout << "Kernel Point Infeasible" << std::endl;
        return NULL;
    }
    if (minobj == -std::numeric_limits<double>::infinity())
    {
        std::cout << "Mesh Unbounded" << std::endl;
        return NULL;
    }

    double *kp = new double[3];
    for (int i = 0; i < 3; i++)
    {
        kp[i] = x[i];
    }

    return kp;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <mesh_file_name>" << std::endl;
        std::cerr << "Example: " << argv[0] << " cube.obj" << std::endl;
        return 1; // Indicate an error
    }

    std::string meshName = argv[1];

    auto start = std::chrono::high_resolution_clock::now();
    double *kp = kernelPoint(meshName);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    if (kp != NULL)
    {
        std::cout << "Kernel point found: ["
                  << kp[0] << ", "
                  << kp[1] << ", "
                  << kp[2] << "]" << std::endl;
    }

    std::cout
        << "Time taken: " << duration.count() << " ms" << std::endl;

    return 0;
}