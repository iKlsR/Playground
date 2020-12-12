#include "Model.hpp"

using std::string;
using std::cerr;

Model::Model(const char *path, const char *n) : name(n) {
    loadModel(path);
    material = new Material;
}

void Model::loadModel(string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(
                                path,
                                aiProcess_Triangulate | aiProcess_FlipUVs);

    if (scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE  ||
        !scene->mRootNode                           ||
        !scene)
    {
        cerr << "Failed to load model " << import.GetErrorString() << "\n";
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::draw(QSharedPointer<Shader> shader) {
    for (auto mesh : meshes) {
        mesh.draw(shader);
    }
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // process all nodes meshes if any
    for (unsigned i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
        std::cout << "Loaded mesh " << mesh->mName.C_Str() << "\n";
    }

    // recursively load any child meshes
    for (unsigned i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }

    std::cout << "Processed " << node->mNumChildren << " children\n";
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    RawMesh rawMesh;
    triMesh = new TriMesh;

    for (unsigned i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        QVector3D vector;
        vector.setX(mesh->mVertices[i].x);
        vector.setY(mesh->mVertices[i].y);
        vector.setZ(mesh->mVertices[i].z);
        vertex.position = vector;

        vector.setX(mesh->mNormals[i].x);
        vector.setY(mesh->mNormals[i].y);
        vector.setZ(mesh->mNormals[i].z);
        vertex.normal = vector;

        // does the mesh contain coords?
        if (mesh->mTextureCoords[0]) {
            QVector2D vec;
            vec.setX(mesh->mTextureCoords[0][i].x);
            vec.setY(mesh->mTextureCoords[0][i].y);
            vertex.texCoord = vec;
        } else {
            vertex.texCoord = QVector2D(0.0f, 0.0f);
        }

        rawMesh.vertices.push_back(vertex);
    }

    // https://en.wikibooks.org/wiki/OpenGL_Programming/Bounding_box
    GLfloat
    min_x, max_x,
    min_y, max_y,
    min_z, max_z;

    min_x = max_x = mesh->mVertices[0].x;
    min_y = max_y = mesh->mVertices[0].y;
    min_z = max_z = mesh->mVertices[0].z;

    for (unsigned i = 0; i < mesh->mNumVertices; i++) {
        if (mesh->mVertices[i].x < min_x) min_x = mesh->mVertices[i].x;
        if (mesh->mVertices[i].x > max_x) max_x = mesh->mVertices[i].x;
        if (mesh->mVertices[i].y < min_y) min_y = mesh->mVertices[i].y;
        if (mesh->mVertices[i].y > max_y) max_y = mesh->mVertices[i].y;
        if (mesh->mVertices[i].z < min_z) min_z = mesh->mVertices[i].z;
        if (mesh->mVertices[i].z > max_z) max_z = mesh->mVertices[i].z;
    }

    QVector3D size = QVector3D(max_x-min_x, max_y-min_y, max_z-min_z);
    QVector3D center = QVector3D((min_x+max_x)/2, (min_y+max_y)/2, (min_z+max_z)/2);
    
    aabb_transform.translate(center).scale(size);
    // QMatrix4x4 transform;
    // transform.translate(center) * transforn.scale(size);

    // walk through each of the mesh's faces (a face is a mesh its triangle)
    // and retrieve the corresponding vertex indices.
    triMesh->triangles.reserve(mesh->mNumFaces);
    for (GLuint i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; j++) {
            rawMesh.indices.push_back(face.mIndices[j]);

            // in threes!
            auto a = mesh->mVertices[face.mIndices[0]];
            auto b = mesh->mVertices[face.mIndices[1]];
            auto c = mesh->mVertices[face.mIndices[2]];
    
            triMesh->addTriangle(QVector3D(a.x, a.y, a.z),
                                 QVector3D(b.x, b.y, b.z),
                                 QVector3D(c.x, c.y, c.z));
        }
    }

    // if (mesh->mMaterialIndex >= 0) {
    //     aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    //     vector<Texture2D> diffuseMaps =
    //         loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse_texture");
    //     rawMesh.textures.insert(rawMesh.textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    //     vector<Texture2D> specularMaps =
    //         loadMaterialTextures(material, aiTextureType_SPECULAR, "specular_texture");
    //     rawMesh.textures.insert(rawMesh.textures.end(), specularMaps.begin(), specularMaps.end());
    // }

    return Mesh(rawMesh);
}

//vector<Texture2D> Model::loadMaterialTextures(
//                    aiMaterial *mat,
//                    aiTextureType type,
//                    string typeName)
//{
//    vector<Texture2D> textures;

//    for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
//        aiString str;
//        mat->GetTexture(type, i, &str);
//        GLboolean skip = false;

//        for (auto j = 0; j < textures_loaded.size(); j++) {
//            if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0) {
//                textures.push_back(textures_loaded[j]);
//                skip = true;
//                break;
//            }
//        }

//        if (!skip) {
//            Texture2D texture;
//            texture.textureID = Texture::loadFromFile(str.C_Str(), directory);
//            texture.type = typeName;
//            texture.path = str;
//            textures.push_back(texture);
//            this->textures_loaded.push_back(texture);
//        }
//    }

//    return textures;
//}
