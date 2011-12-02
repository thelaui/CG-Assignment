# include "include/Billboard.hpp"

# include "include/SpaceScene.hpp"
# include "include/Texture.hpp"
# include "include/window.hpp"

# include <MatrixStack.h>
# include <GL/glew.h>
# include <GL/freeglut.h>

Billboard::Billboard(Mesh* mesh, Texture* texture, float scale):
    texture_(texture),
    mesh_(mesh),
    scale_(scale) {}

void Billboard::draw() {
    SpaceScene* scene(SpaceScene::pointer());

    glUniform1i(scene->getIsBillboardUL(), true);
    gloost::MatrixStack* transform(scene->getMatrixStack());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    transform->push();

        transform->scale(scale_, scale_, scale_);

        float flip(window::getCameraTransform().getTranslate()[1] >= 0 ? -1 : 1);

        gloost::Vector3 position = transform->top().getTranslate();
        gloost::Vector3 direction = window::getCameraTransform().getTranslate() - position;
        gloost::Vector3 axis(gloost::cross(direction, gloost::Vector3(0, flip, 0)));

        direction.normalize();
        axis.normalize();

        transform->rotate(std::acos(gloost::Vector3(0, -flip, 0)*direction), axis[0], axis[1], axis[2]);

        scene->uploadTransform();

        texture_->bind(0);

        mesh_->draw();

    transform->pop();

    glDisable(GL_BLEND);
    glUniform1i(scene->getIsBillboardUL(), false);
}

void Billboard::drawInCameraSpace() {
    SpaceScene* scene(SpaceScene::pointer());

    glUniform3f(scene->getAtmoColorUL(), 1, 1, 1);
    glUniform1i(scene->getIsBillboardUL(), true);
    gloost::MatrixStack* transform(scene->getMatrixStack());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    transform->push();

        transform->loadMatrix(window::getCameraTransform());
        transform->translate(gloost::Vector3(0, 0, -0.02));
        transform->scale(scale_, scale_, scale_);
        transform->rotate(M_PI*0.5, 0.0, M_PI);
        scene->uploadTransform();

        texture_->bind(0);

        mesh_->draw();

    transform->pop();

    glDisable(GL_BLEND);
    glUniform1i(scene->getIsBillboardUL(), false);
}





