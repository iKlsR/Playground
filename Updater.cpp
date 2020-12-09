#include "Updater.hpp"
#include "Playground.hpp"
#include <chrono>
#include <thread>
#include <QDebug>

Updater::Updater(std::condition_variable *_job_ready, std::mutex *_mtx, Swapper *swpr)
                    : job_ready(_job_ready),
                      mtx(_mtx),
                      swapper(swpr)
{

}

Updater::~Updater()
{

}

void Updater::start()
{
    qDebug() << "Updater started!";

    Vertex vertex0;
    QVector3D vector0 = { -0.5, -0.5, 0.0 };
    vertex0.position = vector0;

    Vertex vertex1;
    QVector3D vector1 = { 0.0,  0.5, 0.0 };
    vertex1.position = vector1;

    Vertex vertex2;
    QVector3D vector2 = { 0.5, -0.5, 0.0 };
    vertex2.position = vector2;

    RawMesh RawQuadMesh;
    RawQuadMesh.vertices.push_back(vertex0);
    RawQuadMesh.vertices.push_back(vertex1);
    RawQuadMesh.vertices.push_back(vertex2);

    RawQuadMesh.indices = {
        0, 1, 2
    };

    Mesh QuadMesh;
    QuadMesh.rawMesh = RawQuadMesh;

    Model QuadModel;
    QuadModel.mesh = QuadMesh;

    RenderCommands::CommandDrawIndexed Quad;
    Quad.command_id     = 99; // use a static counter
    Quad.vertex_count   = 9;
    Quad.index_count    = QuadModel.mesh.rawMesh.indices.size();
    Quad.model          = QuadModel;
    Quad.program_id     = 0;

    RenderCommands::CommandDrawIndexed Quadq;
    Quadq.command_id     = 8; // use a static counter
    Quadq.vertex_count   = 9;
    Quadq.index_count    = QuadModel.mesh.rawMesh.indices.size();
    Quadq.model          = QuadModel;
    Quadq.program_id     = 0;

    forever {
        {
            std::unique_lock<std::mutex> lock(g_mtx);
//            for (int i = 0; i < 1; i++) {
                swapper->UpdateDrawCommands->push_back(Quad);
//                swapper->UpdateDrawCommands->push_back(Quadq);
//            }
            jobs_available = true;
        }

        g_jobs_ready.notify_one();

        {
            std::unique_lock<std::mutex> lock(g_mtx);
            /*while (!jobs_done) */g_jobs_done.wait(lock, []() { return jobs_done; });
//            jobs_done = false;
            logFunc(QString("[ waiting on render thread ]"));
        }
    }
}

void Updater::run()
{

}
