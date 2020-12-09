#ifndef PL_SCENE_HPP
#define PL_SCENE_HPP

#include <QSharedPointer>

#include "../extern/gl3w/include/GL/gl3w.h"

class Shader;
class Texture;
class Geometry;
class Camera;
class Entity;
class Model;

class QMouseEvent;
class QWheelEvent;
class QFocusEvent;

class Camera;

#include <QMatrix4x4>
#include <QVector3D>

#include "Material.hpp"

struct PickingResult
{
    Model *hitNode;
    QVector3D hitPoint;
    // this is often used for comparisons so it's not necessary to find the root
    float distanceFromCameraSqrd;
};

class Scene : public QObject
{
    Q_OBJECT

public:
    Scene() = default;

    void init(int w, int height);
    void update(float dt);
    void resize(int width, int height);
    void render(float, int, int);

    float metalFloat, roughFloat;
    
    void internalUpdate(float);
    void addModel(QString &modelPath);

    void focusOutEvent(QFocusEvent* event);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);

    bool swapTexture(TextureSlot slot, const QString &texture);

signals:
    void nodeSelected(Model*);

private:

    GLuint vaoId, vboId;
    QMatrix4x4 model, view;
    QMatrix4x4 baseModel;

    bool picked;
    Model *pickedNode;

    QPointF prevMousePos;
    Camera *_camera;

    Model *sphere;
    Model *dragon;
    Model *floor;
    Model *mech;

    Entity *entities;

    QVector3D sphere_pos;

    Geometry *cubeGeom;

    QSharedPointer<Texture> floorTexture;

    QSharedPointer<Shader> pbrShader;
    QSharedPointer<Shader> cubemapShader;
    QSharedPointer<Shader> irradianceShader;
    QSharedPointer<Shader> prefilterShader;
    QSharedPointer<Shader> brdfShader;
    QSharedPointer<Shader> backgroundShader;
    QSharedPointer<Shader> velocityShader;
    QSharedPointer<Shader> blurShader;

    QSharedPointer<Shader> lineShader;

    QSharedPointer<Shader> targetScreenShader;
    QSharedPointer<Shader> outlineShader;

    QSharedPointer<Shader> geometryShader;
    QSharedPointer<Shader> ssaoShader;
    QSharedPointer<Shader> ssaoBlurShader;

    QSharedPointer<Texture> LoftHdr;

    QSharedPointer<Texture> albedoMap;
    QSharedPointer<Texture> metallicMap;
    QSharedPointer<Texture> roughnessMap;
    QSharedPointer<Texture> normalMap;
    QSharedPointer<Texture> aoMap;

    QSharedPointer<Texture> albedoMap2;
    QSharedPointer<Texture> metallicMap2;
    QSharedPointer<Texture> roughnessMap2;
    QSharedPointer<Texture> normalMap2;
    QSharedPointer<Texture> aoMap2;

    QSharedPointer<Texture> albedoMap3;
    QSharedPointer<Texture> metallicMap3;
    QSharedPointer<Texture> roughnessMap3;
    QSharedPointer<Texture> normalMap3;
    QSharedPointer<Texture> aoMap3;

    // pbr shit
    unsigned int envCubeMap;
    unsigned int irradianceMap;
    unsigned int prefilterMap;
    unsigned int brdfLUTTexture;

    float angle;

    unsigned int outputFrameBuffer;
    unsigned int outlineFrameBuffer;
    unsigned int selectFrameBuffer;
    unsigned int geometryFrameBuffer;
    unsigned int velocityFrameBuffer;
    unsigned int blurFrameBuffer;
    unsigned int otexture;
    unsigned int stexture;
    unsigned int ltexture;
    unsigned int vtexture;
    unsigned int btexture;
    unsigned int gPosition;
    unsigned int gNormal;
    unsigned int rboDepthStencil;
    unsigned int selrboDepthStencil;
    unsigned int linerboDepthStencil;
    unsigned int velRboDepthStencil;
    unsigned int geomRboDepth;
    unsigned int ssaoFBO, ssaoBlurFBO;
    unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
    std::vector<QVector3D> ssaoKernel;
    std::vector<QVector3D> ssaoNoise;
    unsigned int noiseTexture;

    unsigned int quadVAO;
    float sinI;
    QMatrix4x4 prevModel, prevView, prevProj;
    // QSharedPointer<Camera> _camera;
};

#endif // PL_SCENE_HPP