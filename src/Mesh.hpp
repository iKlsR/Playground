#ifndef MESH_HPP
#define MESH_HPP

#include "../extern/gl3w/include/GL/gl3w.h"

#include <vector>
#include <string>
#include <sstream>
#include <memory>

#include "assimp/scene.h"

#include "Shader.hpp"
#include "Trimesh.hpp"

using namespace std;

enum class VA_Index { POS, NRM, TEX };

struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector2D texCoord;
};

struct Texture2D {
    GLuint textureID;
    string type;
    aiString path;
};

/* encompass all mesh types into one */
struct RawMesh {
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture2D> textures;
};

class Mesh
{
public:
    Mesh(RawMesh rawMesh);
    ~Mesh() {}
    void draw(QSharedPointer<Shader> shader);
    void drawMesh();

private:
    GLuint vertexArrayObjID, vertexBufferObjID, elementBufferObjID;
    RawMesh rawMesh;
    void setupMesh();
};

#endif // MESH_HPP
