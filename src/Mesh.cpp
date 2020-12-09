#include "Mesh.hpp"
#include <iostream>

Mesh::Mesh(RawMesh rawMesh) : rawMesh(rawMesh) {
    setupMesh();
}

void Mesh::draw(QSharedPointer<Shader> shader) {
    // GLuint diffuseNr = 1;
    // GLuint specularNr = 1;
    // GLuint roughNr = 1;

    // for (auto i = 0; i < rawMesh.textures.size(); i++) {
    //     // activate the corresponding proper texture channel
    //     glActiveTexture(GL_TEXTURE0 + i);

    //     // retrieve the texture #
    //     std::string name = rawMesh.textures[i].type;
    //     std::string number;

    //     if (name == "diffuse_texture") {
    //         number = to_string(diffuseNr++);
    //     } else if (name == "specular_texture") {
    //         number = to_string(specularNr++);
    //     } else if (name == "roughness_texture") {
    //         number = to_string(roughNr++);
    //     }

    //     glUniform1i(glGetUniformLocation(shader->program(), (name + number).c_str()), i);
    //     glBindTexture(GL_TEXTURE_2D, rawMesh.textures[i].textureID);
    // }

    drawMesh();

    // reset everything back to 0
    // for (GLuint i = 0; i < rawMesh.textures.size(); i++) {
    //     glActiveTexture(GL_TEXTURE0 + i);
    //     glBindTexture(GL_TEXTURE_2D, 0);
    // }
}

void Mesh::drawMesh() {
//    auto gl = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();

    glBindVertexArray(vertexArrayObjID);
    glDrawElements(GL_TRIANGLES, rawMesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh() {
//    auto gl = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();

    glGenVertexArrays(1, &vertexArrayObjID);
    glBindVertexArray(vertexArrayObjID);

    glGenBuffers(1, &vertexBufferObjID);
    glGenBuffers(1, &elementBufferObjID);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
    glBufferData(
        GL_ARRAY_BUFFER,
        rawMesh.vertices.size() * sizeof(Vertex),
        &rawMesh.vertices.front(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObjID);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        rawMesh.indices.size() * sizeof(GLuint),
        &rawMesh.indices.front(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 3,
        GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (GLvoid*) 0
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3,
        GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal)
    );

   glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 2,
        GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (GLvoid*) offsetof(Vertex, texCoord)
    );

    glBindVertexArray(0);
}
