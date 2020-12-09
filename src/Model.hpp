#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <iostream>
#include <string>

#include <QVector3D>
#include <QVector2D>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include "Material.hpp"

class Model {
public:
    const char *name;
    Transform transform;
    Transform aabb_transform;
    
    Material *material;

    Model(const char *path, const char*);
    void loadModel(std::string path);
    void draw(QSharedPointer<Shader> shader);
    void setTransform(const Transform &t) {
        transform = t;
    }

    TriMesh* triMesh;
    TriMesh* getTriMesh() {
        return triMesh;
    }

private:
    std::vector<Mesh> meshes;
    std::vector<Texture2D> textures_loaded;
    std::string directory;

    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
//    std::vector<Texture2D> loadMaterialTextures(
//                                aiMaterial*,
//                                aiTextureType,
//                                std::string
//                            );


};

#endif // MODEL_H
