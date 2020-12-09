#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <QSharedPointer>
#include <vector>
#include <memory>
#include <string>

#include "../extern/gl3w/include/GL/gl3w.h"

struct TexInfo {
    std::string name;
    std::string path;
    GLuint textureID;
};

using std::string;

class Texture {
public:
    static QSharedPointer<Texture> load(const string&, bool flip = true);
    static QSharedPointer<Texture> loadHDR(const string&);
    static std::vector<TexInfo> textureSlate;
    // TODO - eliminate the need for this
    // static GLint loadFromFile(const char *path, std::string directory);
    void bind(int slot = 0);
    int dynbind();
    void unbind();
    int ID();

private:
    Texture(const string&, bool flip);
    Texture(const string&);
    int width, height;
    int boundSlots;
    GLuint textureID;
};

#endif // TEXTURE_HPP
