#include "Scene.hpp"

#include "Model.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Geometry.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "Entity.hpp"
#include <QOpenGLContext>

#include <QTimer>
#include <QApplication>

#include <random>

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

void Scene::init(int w, int h)
{
    _camera = new Camera;
    
        entities = new Entity;
        picked = false;
        roughFloat = metalFloat = 0.5f;
    
        sphere_pos = QVector3D(0, 0, 0);
    cubeGeom = new Geometry;

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    unsigned int  quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    QString workingDir = QApplication::applicationDirPath();
    pbrShader = Shader::load(workingDir + "/shaders/pbr.vert", workingDir + "/shaders/pbr.frag");
    cubemapShader = Shader::load(workingDir + "/shaders/cubemap.vert", workingDir + "/shaders/cubemap.frag");
    irradianceShader = Shader::load(workingDir + "/shaders/cubemap.vert", workingDir + "/shaders/env_convolution.frag");
    prefilterShader = Shader::load(workingDir + "/shaders/cubemap.vert", workingDir + "/shaders/prefilter.frag");
    backgroundShader = Shader::load(workingDir + "/shaders/background.vert", workingDir + "/shaders/background.frag");
    brdfShader = Shader::load(workingDir + "/shaders/brdf.vert", workingDir + "/shaders/brdf.frag");
    lineShader = Shader::load(workingDir + "/shaders/line.vert", workingDir + "/shaders/line.frag");
    targetScreenShader = Shader::load(workingDir + "/shaders/screen.vert", workingDir + "/shaders/screen.frag");
    outlineShader = Shader::load(workingDir + "/shaders/outline.vert", workingDir + "/shaders/outline.frag");
    velocityShader = Shader::load(workingDir + "/shaders/velocity.vert", workingDir + "/shaders/velocity.frag");
    geometryShader = Shader::load(workingDir + "/shaders/geometry.vert", workingDir + "/shaders/geometry.frag");
    blurShader = Shader::load(workingDir + "/shaders/blur.vert", workingDir + "/shaders/blur.frag");
    ssaoShader = Shader::load(workingDir + "/shaders/ssao.vert", workingDir + "/shaders/ssao.frag");
    ssaoBlurShader = Shader::load(workingDir + "/shaders/ssaoblur.vert", workingDir + "/shaders/ssaoblur.frag");

    QString sphereModel = workingDir + "/assets/models/iron_sphere.obj";
    // QString mechModel = workingDir + "/assets/lighter/Lighter.obj";
    QString mechModel = workingDir + "/assets/previewer/HelmetMeshTriCentered.obj";
    QString floorModel = workingDir + "/assets/models/Floor1.obj";

    sphere = new Model(sphereModel.toStdString().c_str(), "Sphere");
    // mech = new Model(mechModel.toStdString().c_str(), "Lighter");
    // floor = new Model(floorModel.toStdString().c_str(), "Floor");

    // entities->addEntity(sphere);
    // entities->addEntity(mech);

    // QString loftHDRTex = workingDir + "/assets/HDRIS/2K/modern_buildings_2k.hdr";
    QString loftHDRTex = workingDir + "/assets/HDRIS/wobbly_bridge_1k.hdr";
    LoftHdr = Texture::loadHDR(loftHDRTex.toStdString());

    // QString albedoTexture2 = workingDir + "/assets/Lighter/Lighter_B.png";
    // QString metallicTexture2 = workingDir + "/assets/Lighter/Lighter_M.png";
    // QString roughnessTexture2 = workingDir + "/assets/Lighter/Lighter_R.png";
    // QString normalTexture2 = workingDir + "/assets/Lighter/Lighter_N.png";
    // QString aoTexture2 = workingDir + "/assets/Lighter/Lighter_M.png";

    // albedoMap2 = Texture::load(albedoTexture2.toStdString(), false);
    // metallicMap2 = Texture::load(metallicTexture2.toStdString(), false);
    // roughnessMap2 = Texture::load(roughnessTexture2.toStdString(), false);
    // normalMap2 = Texture::load(normalTexture2.toStdString(), false);
    // aoMap2 = Texture::load(aoTexture2.toStdString(), false);

    // QString albedoTexture3 = workingDir + "/assets/textures/puddle/JPG/groundGravelPuddle_tile_DIF.jpg";
    // QString metallicTexture3 = workingDir + "/assets/textures/puddle/JPG/groundGravelPuddle_tile_DISP.jpg";
    // QString roughnessTexture3 = workingDir + "/assets/textures/puddle/JPG/groundGravelPuddle_tile_DISP.jpg";
    // QString normalTexture3 = workingDir + "/assets/textures/puddle/JPG/groundGravelPuddle_tile_NM.jpg";
    // QString aoTexture3 = workingDir + "/assets/textures/puddle/JPG/groundGravelPuddle_tile_AO.jpg";

    // albedoMap3 = Texture::load(albedoTexture3.toStdString(), false);
    // metallicMap3 = Texture::load(metallicTexture3.toStdString(), false);
    // roughnessMap3 = Texture::load(roughnessTexture3.toStdString(), false);
    // normalMap3 = Texture::load(normalTexture3.toStdString(), false);
    // aoMap3 = Texture::load(aoTexture3.toStdString(), false);

    QString albedoTexture = workingDir + "/assets/textures/bamboo-wood/albedo.png";
    QString metallicTexture = workingDir + "/assets/textures/bamboo-wood/metallic.png";
    QString roughnessTexture = workingDir + "/assets/textures/bamboo-wood/roughness.png";
    QString normalTexture = workingDir + "/assets/textures/bamboo-wood/normal.png";
    QString aoTexture = workingDir + "/assets/textures/bamboo-wood/ao.png";

    // QString albedoTexture = workingDir + "/assets/previewer/_GameMeshTri1_d.jpg";
    // QString metallicTexture = workingDir + "/assets/previewer/_GameMeshTri1_s.jpg";
    // QString roughnessTexture = workingDir + "/assets/previewer/_GameMeshTri1_g.jpg";
    // QString normalTexture = workingDir + "/assets/previewer/_GameMeshTri1_n.jpg";
    // QString aoTexture = workingDir + "/assets/previewer/_GameMeshTri1_o.jpg";

    albedoMap = Texture::load(albedoTexture.toStdString(), false);
    metallicMap = Texture::load(metallicTexture.toStdString(), false);
    roughnessMap = Texture::load(roughnessTexture.toStdString(), false);
    normalMap = Texture::load(normalTexture.toStdString(), false);
    aoMap = Texture::load(aoTexture.toStdString(), false);

    sphere->material->textures.insert(TextureSlot::ALBEDO,    albedoTexture);
    sphere->material->textures.insert(TextureSlot::METALLIC,  metallicTexture);
    sphere->material->textures.insert(TextureSlot::ROUGHNESS, roughnessTexture);
    sphere->material->textures.insert(TextureSlot::NORMAL,    normalTexture);
    sphere->material->textures.insert(TextureSlot::AO,        aoTexture);

    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //     -0.5f,  0.5f, 0.0f,
    //      0.5f,  0.5f, 0.0f,
    // };

    // glGenVertexArrays(1, &vaoId);
    // glGenBuffers(1, &vboId);
    // glBindVertexArray(vaoId);
    // glBindBuffer(GL_ARRAY_BUFFER, vboId);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LESS);
    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glClearColor(.1f, .1f, .1f, .4f);

    QVector3D lightPositions[] = {
        QVector3D(-10,  10, 10),
        QVector3D( 10,  10, 10),
        QVector3D( 10, -10, 10),
        QVector3D(-10, -10, 10),
    };

    // SSAO
    glGenFramebuffers(1, &ssaoFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // and blur stage
    glGenFramebuffers(1, &ssaoBlurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glGenTextures(1, &ssaoColorBufferBlur);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // generate sample kernel
    // ----------------------
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;
    for (unsigned int i = 0; i < 64; ++i)
    {
        QVector3D sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
        sample.normalize();
        sample *= randomFloats(generator);
        float scale = float(i) / 64.0;

        // scale samples s.t. they're more aligned to center of kernel
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);
    }

    // generate noise texture
    // ----------------------
    for (unsigned int i = 0; i < 16; i++)
    {
        QVector3D noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
        ssaoNoise.push_back(noise);
    }

    glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // VELOCITY
    glGenFramebuffers(1, &velocityFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, velocityFrameBuffer);

    glGenTextures(1, &vtexture);
    glBindTexture(GL_TEXTURE_2D, vtexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, vtexture, 0); 

    glGenRenderbuffers(1, &velRboDepthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, velRboDepthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, velRboDepthStencil
    );

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 

    // BLUR?
    glGenFramebuffers(1, &blurFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, blurFrameBuffer);

    glGenTextures(1, &btexture);
    glBindTexture(GL_TEXTURE_2D, btexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, btexture, 0); 

    // GEOMETRY
    glGenFramebuffers(1, &geometryFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, geometryFrameBuffer);

    // NORMAL FOR SSAO
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0); 

    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    glGenRenderbuffers(1, &geomRboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, geomRboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, geomRboDepth
    );

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 

    // SCREEN
    glGenFramebuffers(1, &outputFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, outputFrameBuffer);

    glGenTextures(1, &otexture);
    glBindTexture(GL_TEXTURE_2D, otexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, otexture, 0); 

    glGenRenderbuffers(1, &rboDepthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil
    );

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 

    // SELECTION
    glGenFramebuffers(1, &outlineFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, outlineFrameBuffer);

    glGenTextures(1, &stexture);
    glBindTexture(GL_TEXTURE_2D, stexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, stexture, 0); 

    glGenRenderbuffers(1, &selrboDepthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, selrboDepthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, selrboDepthStencil
    );

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 

    // LINE
    glGenFramebuffers(1, &selectFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, selectFrameBuffer);

    glGenTextures(1, &ltexture);
    glBindTexture(GL_TEXTURE_2D, ltexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ltexture, 0); 

    glGenRenderbuffers(1, &linerboDepthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, linerboDepthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, linerboDepthStencil
    );

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 

    // START
    // pbrShader->setUniform("uLightPos", QVector3D(0, 2, 2));
    pbrShader->use();
    pbrShader->setUniform("lightColor", QVector3D(300.f, 300.f, 300.f));
    for (int i = 0; i < 4; i++) {
        pbrShader->setUniform(QString("lightPositions[%1]").arg(i).toStdString().c_str(),
                                  lightPositions[i]);
    }

    // pbr: setup framebuffer
    // ----------------------
    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    // pbr: setup cubemap to render to and attach to framebuffer
    // ---------------------------------------------------------
    glGenTextures(1, &envCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    QMatrix4x4 captureProjection;
    captureProjection.perspective(90.0f, 1.0f, 0.1f, 10.0f);
    QMatrix4x4 one, two, three, four, five, six;
    one.lookAt(QVector3D(0.0f, 0.0f, 0.0f), QVector3D( 1.0f,  0.0f,  0.0f), QVector3D(0.0f, -1.0f,  0.0f));
    two.lookAt(QVector3D(0.0f, 0.0f, 0.0f), QVector3D(-1.0f,  0.0f,  0.0f), QVector3D(0.0f, -1.0f,  0.0f));
    three.lookAt(QVector3D(0.0f, 0.0f, 0.0f), QVector3D( 0.0f,  1.0f,  0.0f), QVector3D(0.0f,  0.0f,  1.0f));
    four.lookAt(QVector3D(0.0f, 0.0f, 0.0f), QVector3D( 0.0f, -1.0f,  0.0f), QVector3D(0.0f,  0.0f, -1.0f));
    five.lookAt(QVector3D(0.0f, 0.0f, 0.0f), QVector3D( 0.0f,  0.0f,  1.0f), QVector3D(0.0f, -1.0f,  0.0f));
    six.lookAt(QVector3D(0.0f, 0.0f, 0.0f), QVector3D( 0.0f,  0.0f, -1.0f), QVector3D(0.0f, -1.0f,  0.0f));
    QMatrix4x4 captureViews[] = { one, two, three, four, five, six };

    // pbr: convert HDR equirectangular environment map to cubemap equivalent
    // ----------------------------------------------------------------------
    cubemapShader->use();
    cubemapShader->setUniform("equirectangularMap", 0);
    cubemapShader->setUniform("uProj", captureProjection);
    // LoftHdr->bind(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, LoftHdr->ID());

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i) {
        cubemapShader->setUniform("uView", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubeMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cubeGeom->drawCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
    // -----------------------------------------------------------------------------
    irradianceShader->use();
    irradianceShader->setUniform("environmentMap", 0);
    irradianceShader->setUniform("uProj", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);

    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i) {
        irradianceShader->setUniform("uView", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cubeGeom->drawCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, w, h);

    // prefilter
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    prefilterShader->use();
    prefilterShader->setUniform("environmentMap", 0);
    prefilterShader->setUniform("uProj", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth  = 128 * std::pow(0.5, mip);
        unsigned int mipHeight = 128 * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader->setUniform("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            prefilterShader->setUniform("uView", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            cubeGeom->drawCube();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // GENERATE 2D LUT
    glGenTextures(1, &brdfLUTTexture);

    // pre-allocate enough memory for the LUT texture.
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

    glViewport(0, 0, 512, 512);
    brdfShader->use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cubeGeom->drawQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // timerQuery.create();
    // elapsedTimer->start();
    angle = 0;
}

void Scene::internalUpdate(float dt)
{
    _camera->update(dt);
}

void Scene::addModel(QString &modelPath)
{
    auto model = new Model(modelPath.toStdString().c_str(), "Untitled");
    entities->addEntity(model);
}

void Scene::update(float dt)
{

}

void Scene::resize(int width, int height)
{
    _camera->updateProjectionMatrix(width, height);
}

bool Scene::swapTexture(TextureSlot slot, const QString &texture)
{
    switch (slot) {
        case TextureSlot::ALBEDO:
            albedoMap = Texture::load(texture.toStdString(), false);
        break;
        case TextureSlot::METALLIC:
            metallicMap = Texture::load(texture.toStdString(), false);
        break;
        case TextureSlot::ROUGHNESS:
            roughnessMap = Texture::load(texture.toStdString(), false);
        break;
        case TextureSlot::NORMAL:
            normalMap = Texture::load(texture.toStdString(), false);
        break;
        case TextureSlot::AO:
            aoMap = Texture::load(texture.toStdString(), false);
        break;
        case TextureSlot::NONE:
        default: break;
    };

    return true;
}

void Scene::render(float delta, int w, int h)
{
    auto ctx = QOpenGLContext::currentContext();
    internalUpdate(delta);

    // Transform trans1;
    // trans1.translate(QVector3D(0, 0, 0)).scale(1.5f); //.rotate(angle, QVector3D(0.5, 0.5, 0.5));
    Transform trans;
    trans.translate(QVector3D(0, 0, 0)).scale(1.5f); // .rotate(angle, QVector3D(0.0, 1, 0.0));
    ////////////////////////////////////////////////////////////////////////

    if (picked) {
        glBindFramebuffer(GL_FRAMEBUFFER, outlineFrameBuffer);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        lineShader->use();
        lineShader->setUniform("uView", _camera->getView());
        lineShader->setUniform("uProj", _camera->getProjectionMatrix());
        lineShader->setUniform("uModel", pickedNode->transform.transform());
        pickedNode->draw(lineShader);
        lineShader->stop();
        
        glBindFramebuffer(GL_FRAMEBUFFER, selectFrameBuffer);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        outlineShader->use();
        outlineShader->setUniform("screenTexture", 0);
        outlineShader->setUniform("ttextureSize", QVector3D(w, h, 0));
        
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, stexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        outlineShader->stop();
    } else {
        GLuint clearColor[4] = {0, 0, 0, 0};
        glClearTexImage(ltexture, 0, GL_RGBA, GL_UNSIGNED_BYTE, &clearColor);  
    }

    ///////////////////////////////////////////
        // DEFAULT
    glBindFramebuffer(GL_FRAMEBUFFER, geometryFrameBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    geometryShader->use();
    geometryShader->setUniform("view", _camera->getView());
    geometryShader->setUniform("projection", _camera->getProjectionMatrix());
    geometryShader->setUniform("model", trans.transform());
    glEnable(GL_DEPTH_TEST);
    for (auto item : entities->getEntities()) {
        item->draw(geometryShader);
        item->setTransform(trans);
    }
    geometryShader->stop();

    // DEFAULT
    glBindFramebuffer(GL_FRAMEBUFFER, outputFrameBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    pbrShader->use();

    view.setToIdentity();
    // view.lookAt(QVector3D(0, .8f, 1.3),     // pos
    //             QVector3D(0, 0, 0),         // target
    //             QVector3D(0, 1, 0));        // up

    QVector3D viewPos(QVector3D(0, 0, 0));
    view.setToIdentity();
    view.lookAt(viewPos, QVector3D(0, 0, 0), QVector3D(0, 1, 0));

    pbrShader->setUniform("uViewPos", _camera->position);
    pbrShader->setUniform("uView", _camera->getView());
    pbrShader->setUniform("uProj", _camera->getProjectionMatrix());


    pbrShader->setUniform("irradianceMap", 0);
    pbrShader->setUniform("prefilterMap", 1);
    pbrShader->setUniform("brdfLUT", 2);
    pbrShader->setUniform("albedoMap", 3);
    pbrShader->setUniform("metallicMap", 4);
    pbrShader->setUniform("roughnessMap", 5);
    pbrShader->setUniform("normalMap", 6);
    pbrShader->setUniform("aoMap", 7);
    // pbrShader->setUniform("outline", 8);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, albedoMap->ID());
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, metallicMap->ID());
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, roughnessMap->ID());
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, normalMap->ID());
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, aoMap->ID());
    // glActiveTexture(GL_TEXTURE8);
    // glBindTexture(GL_TEXTURE_2D, otexture);

    pbrShader->use();
    pbrShader->setUniform("metalFloat", metalFloat);
    pbrShader->setUniform("roughFloat", roughFloat);
    pbrShader->setUniform("uModel", trans.transform());

    glEnable(GL_DEPTH_TEST);
    for (auto item : entities->getEntities()) {
        item->draw(pbrShader);
        item->setTransform(trans);
    }

    angle += 1132.f * delta;
    if (angle >= 360) angle = 0;

    pbrShader->stop();

    // 2. generate SSAO texture
    // ------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        ssaoShader->use();
        ssaoShader->setUniform("gPosition", 0);
        ssaoShader->setUniform("gNormal", 1);
        ssaoShader->setUniform("texNoise", 2);
        // Send kernel + rotation 
        for (unsigned int i = 0; i < 64; ++i) {
            auto str = "samples[" + QString::number(i) + "]";
            ssaoShader->setUniform(str.toStdString().c_str(), ssaoKernel[i]);
        }
            
        ssaoShader->setUniform("projection", _camera->getProjectionMatrix());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ssaoShader->stop();

    // 3. blur SSAO texture to remove noise
    // ------------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    ssaoBlurShader->use();
    ssaoBlurShader->setUniform("ssaoInput", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ssaoBlurShader->stop();

    // BACKGROUND
    backgroundShader->use();
    backgroundShader->setUniform("uView",  view);
    backgroundShader->setUniform("uProj",  _camera->getProjectionMatrix());
    glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
    backgroundShader->setUniform("equirectangularMap", 0);
    cubeGeom->drawCube();
    backgroundShader->stop();

    // VEL
    glBindFramebuffer(GL_FRAMEBUFFER, velocityFrameBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    velocityShader->use();
    velocityShader->setUniform("uView", _camera->getView());
    velocityShader->setUniform("uProj", _camera->getProjectionMatrix());  
    velocityShader->setUniform("uModel", trans.transform());  
    velocityShader->setUniform("uPrevView", prevView);
    velocityShader->setUniform("uPrevProj", prevProj); 
    velocityShader->setUniform("uPrevModel", prevModel); 
    glEnable(GL_DEPTH_TEST);
    for (auto item : entities->getEntities()) {
        item->draw(velocityShader);
        item->setTransform(trans);
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    velocityShader->stop();    

    // BLUR
    glBindFramebuffer(GL_FRAMEBUFFER, blurFrameBuffer);
    glClear(GL_COLOR_BUFFER_BIT);
    blurShader->use();
    blurShader->setUniform("uTexInput", 0);
    blurShader->setUniform("uTexVelocity", 1);
    blurShader->setUniform("uVelocityScale", 0.61f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, otexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, vtexture);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    blurShader->stop();

    // CUSTOM
    glBindFramebuffer(GL_FRAMEBUFFER, ctx->defaultFramebufferObject());
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    targetScreenShader->use();
    targetScreenShader->setUniform("screenTexture", 0);
    targetScreenShader->setUniform("selectTexture", 1);
    targetScreenShader->setUniform("ssao", 2);
    glBindVertexArray(quadVAO);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, otexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ltexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    targetScreenShader->stop();

    prevView = _camera->getView();
    prevProj = _camera->getProjectionMatrix(); 
    prevModel = trans.transform(); 
}

void Scene::mousePressEvent(QMouseEvent *event)
{
    // clicks should start at the NEAR PLANE
    prevMousePos = event->localPos();
    _camera->onMouseDown(event->button());

    if (event->button() == Qt::LeftButton) {
        QVector3D camera_pos = _camera->position;
        // more dpi fuckery
        QVector3D coords = _camera->calculateMouseRay(event->localPos() * 2);// * 128.f; // scale that bitch??

        auto rayDir = coords * 1024;
        auto segEnd = camera_pos + rayDir;
        // Ray r = Ray(camera_pos, coords);
        // Sphere s = Sphere(sphere_pos, 1.f);

        // hit_record record;
        // if (camera->intersect(r, s, (float) 1e-3, std::numeric_limits<float>::max(), record)) {
        //     qDebug() << "hit at " << record.hit_point;
        // } else {
        //     qDebug() << "???";
        // }
        QList<PickingResult> hitList;

        for (auto model : entities->getEntities()) {
            auto triMesh = model->getTriMesh();
            auto invTransform = model->transform.transform().inverted();
            auto a = invTransform * camera_pos;
            auto b = invTransform * segEnd;
    
            QList<TriangleIntersectionResult> results;
            if (int resultCount = triMesh->getSegmentIntersections(a, b, results)) {
                for (auto triResult : results) {
                    // convert hit to world space
                    auto hitPoint = baseModel * triResult.hitPoint;
    
                    PickingResult pick;
                    pick.hitNode = model;
                    pick.hitPoint = hitPoint;
                    pick.distanceFromCameraSqrd = (hitPoint - camera_pos).lengthSquared();
    
                    hitList.append(pick);
                }
            }
        }

        if (hitList.size() != 0) {
            // sort by distance to camera then return the closest hit node
            qSort(hitList.begin(), hitList.end(), [](const PickingResult& a, const PickingResult& b) {
                return a.distanceFromCameraSqrd > b.distanceFromCameraSqrd;
            });

            pickedNode = hitList.last().hitNode;
            emit nodeSelected(pickedNode);
            picked = true;
            // qDebug() << "Picked " << pickedNode->name;
        } else {
            picked = false;
        }
    }
}

void Scene::mouseReleaseEvent(QMouseEvent *event)
{
    _camera->onMouseUp(event->button());
}

void Scene::focusOutEvent(QFocusEvent* event)
{
    Q_UNUSED(event);
    Input::resetKeys();
}

// this is a simple case of just using the old and new mouse pos to compute distance moved
// we then translate this [x, y] vector into useful values that control the scene camera
void Scene::mouseMoveEvent(QMouseEvent *event)
{
    QPointF localPos = event->localPos();
    QPointF dir = localPos - prevMousePos;
    _camera->onMouseMove(-dir.x(), -dir.y());
    prevMousePos = localPos;
}

void Scene::wheelEvent(QWheelEvent *event)
{
    _camera->onMouseWheel(event->delta());
}