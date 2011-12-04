# include "include/window.hpp"

# include "include/SpaceScene.hpp"
# include "include/Billboard.hpp"
# include "include/Object.hpp"
# include "include/Texture.hpp"
# include "include/Ship.hpp"
# include "include/PhysicalScene.hpp"

# include <GL/glew.h>
# include <GL/freeglut.h>

# include <gloostGlUtil.h>


namespace window {

namespace {

    int width_ (0), height_(0), frameCount_(0), time_(0);
    double mouseX_(0.0), mouseY_(0.0);
    bool fullScreen_(false), showHelp_(true);
    bool accel_(false);
    gloost::Vector2 mouseDrag_(0, 0);
    Billboard* helpText_(NULL);
    Ship* ship_;

    SpaceScene* scene_(NULL);
    PhysicalScene* physicalScene_(NULL);

    void timerFunction(int value) {
        if(0 != value) {
            int fps = frameCount_ * 4;
            glutSetWindowTitle( (gloost::toString(fps) + " fps").c_str());
        }

        frameCount_ = 0;
        glutTimerFunc(250, timerFunction, 1);
    }

    void renderFunction() {
        ++frameCount_;

        int now = glutGet(GLUT_ELAPSED_TIME);
        int frameTime = now - time_;
        time_ = now;

        ship_->tilt(-mouseY_);
        ship_->turn(-mouseX_);

        if (!showHelp_) {
            glutSetCursor(GLUT_CURSOR_NONE);
            glutWarpPointer(width_/2, height_/2);
            mouseX_ = 0.0;
            mouseY_ = 0.0;
        }

        else glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

        ship_->update(frameTime*0.001);
        scene_->update(frameTime*0.001);
        physicalScene_->update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene_->draw();

        ship_->draw();

        if (showHelp_)
            helpText_->drawInCameraSpace();

        glBindVertexArray(0);
        glUseProgram(0);

        glutSwapBuffers();
        glutPostRedisplay();

    }

    void onResize(int width, int height) {
        width_  = width;
        height_ = height;
        glViewport(0, 0, width_, height_);

        gloost::gloostPerspective(*scene_->getProjectionMatrix(), 60.0f, static_cast<float>(width_) / height_, 0.01f, 4000.0f);

        glUseProgram(scene_->getShaderIds()[0]);
        glUniformMatrix4fv(scene_->getProjectionMatrixUL(), 1, GL_FALSE, scene_->getProjectionMatrix()->data());
        glUseProgram(0);
    }

    void toggleFullscreen() {
        fullScreen_ = !fullScreen_;

        if (fullScreen_)
            glutFullScreen();
        else
            glutReshapeWindow(1440, 900);
    }

    void onSpecialKeyUp(int key, int x, int y) {
        if (!showHelp_) {
            switch (key) {
                case GLUT_KEY_LEFT :
                    ship_->rotateLeft(false);
                    break;
                case GLUT_KEY_RIGHT :
                    ship_->rotateRight(false);
                    break;
                case GLUT_KEY_UP :
                    ship_->accelerate(false);
                    break;
            }
        }
    }

    void onSpecialKeyDown(int key, int x, int y) {
        switch (key) {
            case GLUT_KEY_LEFT :
                if (!showHelp_)
                    ship_->rotateLeft(true);
                break;
            case GLUT_KEY_RIGHT :
                if (!showHelp_)
                    ship_->rotateRight(true);
                break;
            case GLUT_KEY_UP :
                if (!showHelp_)
                    ship_->accelerate(true);
                break;
            case GLUT_KEY_F11:
                toggleFullscreen();
                break;
        }
    }

    void onKey(unsigned char key, int x, int y) {
        switch (key) {
            case 27:
                glutExit();
                break;
            case 'h':
                showHelp_ = !showHelp_;
                break;
        }
    }

    void onMove(int x, int y) {
        if (!showHelp_) {
            mouseX_ = 1.0*x/width_ - 0.5;
            mouseY_ = 1.0*y/height_- 0.5;
        }
    }

    void onClick(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON && !showHelp_) {
            ship_->shoot(state == GLUT_DOWN);
        }
    }

    void idleFunction(void) {
        glutPostRedisplay();
    }

    void initWindow(int argc, char** argv) throw (std::string) {
        glutInit(&argc, argv);

        glutInitContextVersion(3, 3);
        glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
        glutInitContextProfile(GLUT_CORE_PROFILE);

        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

        glutInitWindowSize(width_, height_);

        glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

        if(glutCreateWindow("") < 1) {
            fprintf(stderr, "ERROR: Could not create a new rendering window.\n");
            glutExit();
        }

        glutIgnoreKeyRepeat(true);
        glutSetCursor(GLUT_CURSOR_NONE);
        glutWarpPointer(width_/2, height_/2);

        //Glut function callbacks
        glutTimerFunc(0, timerFunction, 0);
        glutDisplayFunc(renderFunction);
        glutReshapeFunc(onResize);
        glutIdleFunc(idleFunction);
        glutSpecialFunc(onSpecialKeyDown);
        glutSpecialUpFunc(onSpecialKeyUp);
        glutKeyboardFunc(onKey);
        glutPassiveMotionFunc(onMove);
        glutMotionFunc(onMove);
        glutMouseFunc(onClick);
    }
}


void init(int width, int height, int argc, char* argv[]) throw (std::string) {
    width_  = width;
    height_ = height;

    GLenum GlewInitResult;

    initWindow(argc, argv);

    glewExperimental = GL_TRUE;
    GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
         throw std::string("Failed to initialize GLEW!");

    std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glGetError();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    physicalScene_ = new PhysicalScene();

    Texture* billboardTex = new Texture("data/textures/helpText.jpg");
    Mesh* quad = new Mesh("data/objects/quad.obj");
    helpText_ = new Billboard(quad, billboardTex, 0.02);

    Texture* shipDiffuse = new Texture("data/textures/ship_diffuse.jpg");
    Texture* shipNormal = new Texture("data/textures/ship_normal.jpg");
    Texture* shipSpecular = new Texture("data/textures/ship_specular.jpg");
    Texture* shipGlow = new Texture("data/textures/ship_glow.jpg");
    Mesh* shipMesh = new Mesh("data/objects/ship.obj");
    Object* shipObject = new Object(shipMesh, 1, 1, 1, shipDiffuse, shipSpecular, shipNormal, shipGlow);
    ship_ = new Ship(shipObject, gloost::Vector3(0.35, M_PI*0.5, 0.0), gloost::Vector3(60.0, 5.0, 0.0));
}

void draw(SpaceScene* scene) {
    scene_ = scene;
    glutMainLoop();
}

int getWidth() {
    return width_;
}

int getHeight() {
    return height_;
}

gloost::Matrix const& getCameraTransform() {
    ship_->getCameraTransform();
}

}

