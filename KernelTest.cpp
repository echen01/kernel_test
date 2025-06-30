#include <iostream>
#include <vector>
#include <string>

#include <Eigen/Eigen>
#include "sdlp.hpp"

#include "Mesh.hpp"

using namespace std;

/**
 * Returns a kernel point of the given mesh, or NULL if no such point exists.
 * @param meshName The mesh to find a kernel point i, in .obj or .off format.
 * @return A pointer to an array of 3 doubles representing the kernel point, or NULL if no such point exists.
 */
double *kernelPoint(string MeshName)
{
    Mesh *mesh = new Mesh();

    if (meshName.substr(meshName.length() - 3, 3) == "off")
        mesh->loadOff(meshName.c_str());
    else
        mesh->loadObj(meshName.c_str());

    vector<HalfSpace> halfSpaceSet;
    computeHalfSpaceFromTriangles(mesh.getAllTris(), mesh.getAllVerts(), halfSpaceSet);

    int m = halfSpaceSet.size(); // Number of half-spaces
    int d = 3;                   // Dimension of the space
    Eigen::VectorXd x(d);        // Decision variables
    Eigen::VectorXd c(d);        // Objective coefficients
    Eigen::MatrixXd A(m, d);     // Constraint matrix
    Eigen::VectorXd b(m);        // constraint bound

    for (int i = 0; i < m; i++)
    {
        HalfSpace hp = mesh[i];
        A.row(i) << hp.ABCD[0], hp.ABCD[1], hp.ABCD[2];
        b(i) = -hp.ABCD[3];
    }
    c << 0.0, 0.0, 1.0; // +z axis by convention

    // Solves min c^T x subject to Ax <= b
    double minobj = sdlp::linprog(c, A, b, x);

    if (minobj == numeric_limits<double>::infinity())
    {
        cout << "Kernel Point Infeasible" << endl;
        return NULL;
    }
    if (minobj == -numeric_limits<double>::infinity())
    {
        cout << "Mesh Unbounded" << endl;
        return NULL;
    }

    double *kp = new double[3];
    for (int i = 0; i < 3; i++)
    {
        kp[i] = x(i);
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

    double *kp = kernelPoint(meshName);

    if (kp != NULL)
    {
        std::cout << "Kernel Point found: ("
                  << kp[0] << ", "
                  << kp[1] << ", "
                  << kp[2] << ")" << std::endl;
    }

    return 0;
}