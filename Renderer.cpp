#include "Renderer.hpp"
#include "OpenGLWidget.hpp"
#include "Playground.hpp"

#include <QOpenGLFunctions_3_3_Core>
#include <thread>
#include <QDebug>

//std::map<int, GlobalRenderData::Mesh*> GlobalRenderData::meshes;
//std::vector<RenderJob*> Renderer::jobs;

Renderer::Renderer(OpenGLWidget *widget,
                   std::condition_variable *_job_ready,
                   std::mutex *_mtx,
                   Swapper *swpr)
                   : surfaceWindow(widget),
                     job_ready(_job_ready),
                     mtx(_mtx),
                     swapper(swpr)
{
    // pass
    threadContext = 0;
}

Renderer::~Renderer()
{
    surfaceWindow->destroy();
}

bool Renderer::createContext()
{
    threadContext = new QOpenGLContext;

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setVersion(3, 3);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);

    threadContext->setFormat(format);

    return threadContext->create();
}

void Renderer::start()
{
    if (!createContext()) return;

    forever {
        {
            // wait for a job from updater
            std::unique_lock<std::mutex> lock(g_mtx);
            /*while (jobs_available) */
            g_jobs_ready.wait(lock, [](){ return !jobs_available;});
//            jobs_available = false;
        }

        {
            std::unique_lock<std::mutex> lock(g_mtx);
            swapper->swap();
            jobs_done = true;
        }

        g_jobs_done.notify_one();

        run();
//        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void Renderer::run()
{
    // Save some cycles and only run if the window is being exposed
    if (surfaceWindow->isExposed()) {
        threadContext->makeCurrent(surfaceWindow);

        auto gl = threadContext->versionFunctions<QOpenGLFunctions_3_3_Core>();

        gl->glViewport(0, 0, surfaceWindow->width(), surfaceWindow->height());
        gl->glClearColor(.1f, .1f, .1f, .4f);
        gl->glClear(GL_COLOR_BUFFER_BIT);

        logFunc(QString("size ") + QString::number(swapper->RenderDrawCommands->size()));

//        // do some stuff
        std::vector<RenderCommands::CommandDrawIndexed>::iterator iter;
        for (iter = swapper->RenderDrawCommands->begin();
             iter != swapper->RenderDrawCommands->end();
             iter++)
        {
            // we are using the mesh bucket!
            // do a lookup for the id.
            if (vaoStore.value((*iter).command_id)) {
                // draw the model
//                logFunc(QString("drawing cached model vao id = %1").arg((*iter).model.mesh.vertex_array));


                program->bind();

                QMatrix4x4 model;
                QMatrix4x4 view;
                QMatrix4x4 projection;

                projection.setToIdentity();
                projection.perspective(45.f, (GLfloat) surfaceWindow->width() / surfaceWindow->height(), .1f, 32.f);
                model.setToIdentity();
                model.translate(QVector3D(0, 0, 0));
                view.setToIdentity();
                view.lookAt(QVector3D(0, 0, 2),
                            QVector3D(0, 0, 0),
                            QVector3D(0, 1, 0));

                program->setUniformValue("uModel", model);
                program->setUniformValue("uView",  view);
                program->setUniformValue("uProj",  projection);


                gl->glBindVertexArray((*iter).model.mesh.vertex_array);
                gl->glDrawElements(GL_TRIANGLES, (*iter).index_count, GL_UNSIGNED_INT, 0);
                gl->glBindVertexArray(0);

                program->release();
            }
            else {


                QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
                QString vertexShader =
                        "#version 330\n"
                        "uniform mat4 uProj;\n"
                        "uniform mat4 uView;\n"
                        "uniform mat4 uModel;\n"
                        "layout (location = 0) in vec3 position;"
                        "void main() {\n"
                        "   gl_Position = uProj * uView * uModel * vec4(position, 1.0);"
                        "}";
                vshader->compileSourceCode(vertexShader.toStdString().c_str());

                QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
                QString fragmentShader =
                        "#version 330\n"
                        "out vec4 FragColor;\n"
                        "void main() {\n"
                        "   FragColor = vec4(0.3, 1, 0, 1);\n"
                        "}";
                fshader->compileSourceCode(fragmentShader.toStdString().c_str());

                program = new QOpenGLShaderProgram;
                program->addShader(vshader);
                program->addShader(fshader);

                program->link();
                program->bind();
                program->release();

                // generate and cache it
                gl->glGenVertexArrays(1, &(*iter).model.mesh.vertex_array);
                gl->glBindVertexArray((*iter).model.mesh.vertex_array);

                gl->glGenBuffers(1, &(*iter).model.mesh.vertex_buffer);
                gl->glBindBuffer(GL_ARRAY_BUFFER, (*iter).model.mesh.vertex_buffer);
                gl->glBufferData(GL_ARRAY_BUFFER,
                                 (*iter).model.mesh.rawMesh.vertices.size() * sizeof(Vertex),
                                 (*iter).model.mesh.rawMesh.vertices.data(),
                                 GL_STATIC_DRAW);

                gl->glGenBuffers(1, &(*iter).model.mesh.index_buffer); // Generate 1 buffer
                gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*iter).model.mesh.index_buffer);
                gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                 (*iter).model.mesh.rawMesh.indices.size() * sizeof(GLuint),
                                 (*iter).model.mesh.rawMesh.indices.data(),
                                 GL_STATIC_DRAW);

                GLint posAttrib = program->attributeLocation("position");

//                gl->glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
//                gl->glEnableVertexAttribArray(posAttrib);
//                gl->glBindVertexArray(0);

//                 iter over attribs
                gl->glEnableVertexAttribArray(posAttrib);
                gl->glVertexAttribPointer(posAttrib, (*iter).index_count, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
                gl->glBindVertexArray(0);

                vaoStore.insert((*iter).command_id, (*iter).model.mesh.vertex_array);

                logFunc(QString("cached a model vao id = %1").arg((*iter).model.mesh.vertex_array));
            }
        }

//        swapper->RenderDrawCommands->clear();

        threadContext->swapBuffers(surfaceWindow);
    }
}

//void Renderer::render(RenderJob *job)
//{
//    RenderOp *op = job->op;

//    auto gl = threadContext->versionFunctions<QOpenGLFunctions_3_3_Core>();

//    switch ((int) op) {
//        case static_cast<int>(RenderOp::CreateMesh): {
//            // create space for a new mesh

//            GlobalRenderData::Mesh *mesh = new GlobalRenderData::Mesh;
//            // new (mesh) GlobalRenderData::Mesh;
//            GlobalRenderData::meshes[1] = mesh;

////            mesh->id =

//            gl->glGenVertexArrays(1, &mesh->vertex_array);
//            gl->glBindVertexArray(mesh->vertex_array);

//            gl->glGenBuffers(1, &mesh->vertex_buffer);
//            gl->glBindBuffer(mesh->vertex_buffer, GL_STATIC_DRAW);

//            gl->glBufferData(GL_ARRAY_BUFFER,
//                             mesh->vertices.size() * sizeof(GLfloat),
//                             mesh->vertices.data(),
//                             GL_STATIC_DRAW);

//            gl->glGenBuffers(1, &mesh->index_buffer); // Generate 1 buffer
//            gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
//            gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//                             mesh->indices.size() * sizeof(GLint),
//                             mesh->indices.data(),
//                             GL_STATIC_DRAW);

//            qDebug() << "created mesh";

//            break;
//        }

//        case static_cast<unsigned>(RenderOp::DrawIndexed): {
//            GlobalRenderData::Mesh *mesh = GlobalRenderData::meshes[job->id];

//            gl->glBindVertexArray(mesh->vertex_array);
//            gl->glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
//            gl->glBindVertexArray(0);

//            break;
//        }
//    }
//}
