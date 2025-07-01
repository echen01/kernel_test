// @author Merve Asiler

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include "BasicMeshElements.h"

class Mesh
{

private:
    std::vector<Vertex> verts;
    std::vector<Triangle> tris;
    std::vector<Edge> edges;

public:
    Mesh() {};
    ~Mesh();
    Mesh(const Mesh &mesh);
    void loadObj(const char *name);
    void loadOff(const char *name);
    void writeOff(std::string offFileName);

    // methods to construct mesh
    void addTriangle(int v1, int v2, int v3);
    void addEdge(int v1, int v2);
    void addVertex(double x, double y, double z);
    int makeVertsNeighbor(int v1i, int v2i);

    // methods to reach mesh elements
    int getNumOfVerts() const { return verts.size(); };
    int getNumOfTris() const { return tris.size(); };
    int getNumOfEdges() const { return edges.size(); };
    Vertex getVertex(int i) const { return verts[i]; };
    Triangle getTriangle(int i) const { return tris[i]; };
    Edge getEdge(int i) const { return edges[i]; };
    const std::vector<Vertex> &getAllVerts() const { return verts; };
    const std::vector<Triangle> &getAllTris() const { return tris; };
    const std::vector<Edge> &getAllEdges() const { return edges; };

    // methods to compute mesh features
    bool isManifold();
    void addVertexColor(int id, double color[3]);
    void changeVertexCoords(int id, double coords[3]);
    void computeTrisAngles();
    double computeVolume();
    void tetrahedralizeSurface();
    std::vector<double> computeCurvaturePerVertex();
    void makeConvex();
    void removeAbnormalTris();
    void rotate(double angles[3], double rotationCenter[3]);
    void translate(double distances[3]);
    void computeCenter(double center[3]);

    std::vector<std::string> split(std::string inputStr, std::string delimiter)
    {

        std::vector<std::string> pieces;
        std::string data = inputStr, subdata = "";

        while (data != "")
        {
            int ind = data.find_first_of(delimiter);

            if (ind < 0)
                subdata = data;
            else
                subdata = data.substr(0, ind);

            if (subdata != "")
                pieces.push_back(subdata);

            if (ind < 0)
                break;
            data = data.substr(ind + 1);
        }

        return pieces;
    }
};
