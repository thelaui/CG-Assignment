# include "include/SpaceScene.hpp"

# include "include/Planet.hpp"
# include "include/Comet.hpp"
# include "include/window.hpp"
# include "include/shader.hpp"
# include "include/Texture.hpp"
# include "include/Billboard.hpp"

# include <GL/glew.h>
# include <GL/freeglut.h>

SpaceScene::SpaceScene():
        modelMatrixUniformLocation_(0),
        viewMatrixUniformLocation_(0),
        projectionMatrixUniformLocation_(0),
        normalMatrixUniformLocation_(0),
        atmoColorUniformLocation_(0),
        isBillboardUniformLocation_(0),
        projectionMatrix_(),
        modelMatrixStack_(),
        universe_(NULL) {

    modelMatrixStack_.loadIdentity();
    projectionMatrix_.setIdentity();

    shaderIds_[3] = { 0u };

    try {
        setupShaders();
    }
    catch (std::string& error) {
        std::cerr << "Failed to set up shaders: " << error << std::endl;
        glutExit();
    }

    setupSpace();
}

SpaceScene::~SpaceScene() {
    glDetachShader(shaderIds_[0], shaderIds_[1]);
    glDetachShader(shaderIds_[0], shaderIds_[2]);
    glDeleteShader(shaderIds_[1]);
    glDeleteShader(shaderIds_[2]);
    glDeleteProgram(shaderIds_[0]);
}

void SpaceScene::update(double frameTime) {
    universe_->update(frameTime);
}

void SpaceScene::draw() {
    glUseProgram(shaderIds_[0]);

    gloost::Matrix cameraTransform(window::getCameraTransform());
    cameraTransform.invert();

    glUniformMatrix4fv(viewMatrixUniformLocation_, 1, GL_FALSE, cameraTransform.data());

    //reset the modelmatrix
    modelMatrixStack_.clear();

    std::map<float, SpaceObject const*> drawMap;
    universe_->sort(window::getCameraTransform().getTranslate(), drawMap);

    for (std::map<float, SpaceObject const*>::reverse_iterator it=drawMap.rbegin(); it!=drawMap.rend(); ++it)
        it->second->draw();
}

void SpaceScene::uploadTransform() {
    glUniformMatrix4fv(modelMatrixUniformLocation_, 1, GL_FALSE, modelMatrixStack_.top().data());

    gloost::Matrix normalMatrix(modelMatrixStack_.top());
    normalMatrix.invert();
    normalMatrix.transpose();
    glUniformMatrix4fv(normalMatrixUniformLocation_, 1, GL_FALSE, normalMatrix.data());
}

gloost::Matrix* SpaceScene::getProjectionMatrix() {
    return &projectionMatrix_;
}

gloost::MatrixStack* SpaceScene::getMatrixStack() {
    return &modelMatrixStack_;
}

unsigned const* SpaceScene::getShaderIds() const {
    return shaderIds_;
}

unsigned SpaceScene::getProjectionMatrixUL() const {
    return projectionMatrixUniformLocation_;
}

unsigned SpaceScene::getAtmoColorUL() const {
    return atmoColorUniformLocation_;
}

unsigned SpaceScene::getIsBillboardUL() const {
    return isBillboardUniformLocation_;
}

unsigned SpaceScene::getIsParticleUL() const {
    return isParticleUniformLocation_;
}

void SpaceScene::setupShaders() throw (std::string) {
    shaderIds_[0] = glCreateProgram(); {
        //takes a (shader) filename and a shader-type and returns and id of the compiled shader
        shaderIds_[1] = shader::load("data/shaders/simpleVertexShader.vert", GL_VERTEX_SHADER);
        shaderIds_[2] = shader::load("data/shaders/simpleFragmentShader.frag", GL_FRAGMENT_SHADER);

        //attaches a shader to a program
        glAttachShader(shaderIds_[0], shaderIds_[1]);
        glAttachShader(shaderIds_[0], shaderIds_[2]);
    }

    glLinkProgram(shaderIds_[0]);

    //describes how the uniforms in the shaders are named and to which shader they belong
    modelMatrixUniformLocation_      = glGetUniformLocation(shaderIds_[0], "modelMatrix");
    viewMatrixUniformLocation_       = glGetUniformLocation(shaderIds_[0], "viewMatrix");
    projectionMatrixUniformLocation_ = glGetUniformLocation(shaderIds_[0], "projectionMatrix");
    normalMatrixUniformLocation_     = glGetUniformLocation(shaderIds_[0], "normalMatrix");
    atmoColorUniformLocation_        = glGetUniformLocation(shaderIds_[0], "atmoColor");
    isBillboardUniformLocation_      = glGetUniformLocation(shaderIds_[0], "isBillboard");
    isParticleUniformLocation_       = glGetUniformLocation(shaderIds_[0], "isParticle");
    unsigned diffuseLocation         = glGetUniformLocation(shaderIds_[0], "diffuse");
    unsigned bumpmapLocation         = glGetUniformLocation(shaderIds_[0], "bumpmap");
    unsigned specularLocation        = glGetUniformLocation(shaderIds_[0], "specular");
    unsigned emitLocation            = glGetUniformLocation(shaderIds_[0], "emit");

    glBindAttribLocation(shaderIds_[0], 0, "in_Position");
    glBindAttribLocation(shaderIds_[0], 1, "in_Normal");
    glBindAttribLocation(shaderIds_[0], 2, "in_TexCoord");
    glBindAttribLocation(shaderIds_[0], 3, "in_PointSize");
    glBindAttribLocation(shaderIds_[0], 4, "in_PointColor");

    glUseProgram(shaderIds_[0]);

    glUniform1i(diffuseLocation, 0);
    glUniform1i(bumpmapLocation, 1);
    glUniform1i(specularLocation, 2);
    glUniform1i(emitLocation, 3);
}

void SpaceScene::setupSpace() {

    Texture* diffuse01 = new Texture("data/textures/01_diffuse.jpg");
    Texture* diffuse02 = new Texture("data/textures/02_diffuse.jpg");
    Texture* diffuse03 = new Texture("data/textures/03_diffuse.jpg");
    Texture* diffuse04 = new Texture("data/textures/04_diffuse.jpg");
    Texture* diffuse05 = new Texture("data/textures/05_diffuse.jpg");
    Texture* diffuse06 = new Texture("data/textures/06_diffuse.jpg");
    Texture* diffuse07 = new Texture("data/textures/07_diffuse.jpg");

    Texture* normal00 = new Texture("data/textures/00_normal.jpg");
    Texture* normal01 = new Texture("data/textures/01_normal.jpg");
    Texture* normal02 = new Texture("data/textures/02_normal.jpg");
    Texture* normal03 = new Texture("data/textures/03_normal.jpg");
    Texture* normal04 = new Texture("data/textures/04_normal.jpg");
    Texture* normal05 = new Texture("data/textures/05_normal.jpg");
    Texture* normal07 = new Texture("data/textures/07_normal.jpg");

    Texture* specular01 = new Texture("data/textures/01_specular.jpg");
    Texture* specular02 = new Texture("data/textures/02_specular.jpg");
    Texture* specular03 = new Texture("data/textures/03_specular.jpg");
    Texture* specular04 = new Texture("data/textures/04_specular.jpg");
    Texture* specular05 = new Texture("data/textures/05_specular.jpg");

    Texture* glow00 = new Texture("data/textures/00_glow.jpg");
    Texture* glow01 = new Texture("data/textures/01_glow.jpg");
    Texture* glow02 = new Texture("data/textures/02_glow.jpg");
    Texture* glow03 = new Texture("data/textures/03_glow.jpg");
    Texture* glow05 = new Texture("data/textures/05_glow.jpg");

    Texture* billboardTex01 = new Texture("data/textures/billboard01.jpg");
    Texture* billboardTex02 = new Texture("data/textures/billboard02.jpg");

    Texture* sky = new Texture("data/textures/skybox.jpg");

    Mesh* sphere = new Mesh("data/objects/sphere.obj");
    Mesh* quad = new Mesh("data/objects/quad.obj");
    Mesh* comet = new Mesh("data/objects/comet.obj");

    Billboard* billboard01 = new Billboard(quad, billboardTex01, 1.7);
    Billboard* billboard02 = new Billboard(quad, billboardTex02, 3.7);
    Billboard* billboard03 = new Billboard(quad, billboardTex02, 3.5);

    Object* universeObject = new Object(sphere, 0, 0, 0, NULL, NULL, normal00, sky);
    Object* sunObject      = new Object(sphere, 1, 1, 1, NULL, NULL, normal00, glow00);
    Object* planetObject01 = new Object(sphere, 0.5, 0.8, 1, diffuse01, specular01, normal01, glow01);
    Object* planetObject02 = new Object(sphere, 1, 0.5, 0.5, diffuse02, specular02, normal02, glow02);
    Object* planetObject03 = new Object(sphere, 0.8, 0.5, 0, diffuse03, specular03, normal03, glow03);
    Object* planetObject04 = new Object(sphere, 0.2, 0.2, 0.2, diffuse04, specular04, normal04, NULL);
    Object* planetObject05 = new Object(sphere, 0.7, 0.2, 0.9, diffuse05, specular05, normal05, glow05);
    Object* planetObject06 = new Object(sphere, 0.8, 0.7, 0.5, diffuse06, NULL, normal00, NULL);
    Object* planetObject07 = new Object(sphere, 0.6, 0.4, 0.3, diffuse07, NULL, normal07, NULL);

    universe_ = new Planet(universeObject, NULL, 3000.0, 0.0, 0.0, 0.0, false);
    Planet* sun = new Planet(sunObject, billboard02, 4.0, 0.0, 2.1, 0.0);

    Planet* planet01 = new Planet(planetObject01, billboard01, 1.5, 0.5, 5.2, 15);
    Planet* planet03 = new Planet(planetObject03, billboard01, 2.5, -0.3, 3.7, 25);
    Planet* planet05 = new Planet(planetObject05, billboard01, 2.0, 0.5, 4.7, 35);
    Planet* planet06 = new Planet(planetObject06, billboard01, 3.5, 0.3, 5.7, 50);
    Planet* planet07 = new Planet(planetObject07, billboard01, 0.5, 4.5, 15.9, 7);

    Planet* moon00 = new Planet(planetObject04, billboard01, 0.5, -3.8, -8.7, 2.5);

    universe_->addSatellite(sun);

    sun->addSatellite(planet07);

    sun->addSatellite(planet01);
        planet01->addSatellite(moon00);
            moon00->addSatellite(new Planet(planetObject04, billboard01, 0.3, 10.8, 10.9, 1.0));

    sun->addSatellite(planet03);
    sun->addSatellite(planet05);
        planet05->addSatellite(new Planet(planetObject02, billboard01, 1.0, 7, -9.5, 3.4));

    sun->addSatellite(planet06);
        planet06->addSatellite(new Planet(planetObject04, billboard01, 0.7, 25, -17, 3.1));
        planet06->addSatellite(new Planet(planetObject04, billboard01, 2.0, -5, 7, 5.2));

    Object* cometObject01 = new Object(comet, 0.8, 0.8, 0.8, diffuse04, specular04, normal04, NULL);

    Comet* comet00 = new Comet(cometObject01, billboard03, 0.2, -2, 15.9, 9, gloost::Vector3(1, 0, 0), gloost::Vector3(1, 0, 0));
    Comet* comet01 = new Comet(cometObject01, billboard03, 0.3, 5, 15.9, 18, gloost::Vector3(0, 0, 1), gloost::Vector3(0, 1, 1));
    Comet* comet02 = new Comet(cometObject01, billboard03, 0.3, -4, 5.9, 20, gloost::Vector3(0, 1, 0), gloost::Vector3(0.2, 1, 0));
    Comet* comet03 = new Comet(cometObject01, billboard03, 0.4, 3, 15.9, 30, gloost::Vector3(1, 0, 1), gloost::Vector3(0.2, 1, 0.1));
    Comet* comet04 = new Comet(cometObject01, billboard03, 0.4, -2, 15.9, 40, gloost::Vector3(0, 1, 1), gloost::Vector3(0, 1, 0.1));
    Comet* comet05 = new Comet(cometObject01, billboard03, 0.4, -2, 15.9, 60, gloost::Vector3(1, 1, 0), gloost::Vector3(0.3, 1, 0));
    sun->addSatellite(comet00);
    sun->addSatellite(comet01);
    sun->addSatellite(comet02);
    sun->addSatellite(comet03);
    sun->addSatellite(comet04);
    sun->addSatellite(comet05);
}
