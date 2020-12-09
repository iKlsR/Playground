#ifndef PLAYGROUND_HPP
#define PLAYGROUND_HPP

#include <string>

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

#include <qopengl.h>
#include <queue>
#include <condition_variable>
#include <mutex>

using namespace std;

static condition_variable g_jobs_ready;
static condition_variable g_jobs_done;
static mutex g_mtx;
static bool jobs_available = false;
static bool jobs_done = false;

static mutex l_mtx;
static void logFunc(QString text) {
    std::lock_guard<std::mutex> lock(l_mtx);
    qDebug() << text;
}

enum RenderJob
{
    LoadMesh,
    DrawMesh,
    DrawMeshIndexed,
    CreateProgram,
    CreateShader
};

struct Transform
{

};

enum class VA_Index { POS, NRM, TEX };

struct Vertex {
    QVector3D position;
//    QVector3D normal;
//    QVector2D texCoord;
};

struct Texture2D {
    GLuint textureID;
    std::string type;
//    aiString path;
};

/* encompass all mesh types into one */
struct RawMesh
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
//    std::vector<Texture2D> textures;
};

struct Mesh
{
    unsigned vertex_array = 0;
    unsigned vertex_buffer;
    unsigned index_buffer;

    RawMesh rawMesh;
};

struct Material
{

};

struct Model
{
    Mesh mesh;
    Material material;
    Transform transform;
    Transform aabb_transform;
};

namespace RenderCommands
{
    struct CommandDrawIndexed {
        int command_id;

        uint32_t index_count;
        uint32_t vertex_count;

        Model model;
        unsigned program_id;

        struct attribs {

        };
    };

    struct CommandCreateProgram {
        uint32_t command_id;
        unsigned program_id;
    };

    struct CommandCreateShader {
        uint32_t command_id;
        unsigned shader_id;
    };
}

struct Swapper
{
    Swapper() {
        UpdateDrawCommands = new std::vector<RenderCommands::CommandDrawIndexed>();
        RenderDrawCommands = new std::vector<RenderCommands::CommandDrawIndexed>();

//        UpdateDrawCommands->reserve(1);
//        RenderDrawCommands->reserve(1);
    }

    void swap() {
        auto commands = UpdateDrawCommands;
        RenderDrawCommands = commands;
        UpdateDrawCommands = RenderDrawCommands;
    }

    std::vector<RenderCommands::CommandDrawIndexed> *UpdateDrawCommands;
    std::vector<RenderCommands::CommandDrawIndexed> *RenderDrawCommands;
};

//class Playground
//{
//public:
//    Playground();
//};

#endif // PLAYGROUND_HPP
