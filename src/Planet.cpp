# include "include/Planet.hpp"

# include "include/SpaceScene.hpp"
# include "include/Billboard.hpp"
# include "include/Texture.hpp"
# include "include/window.hpp"
# include "include/CollisionSphere.hpp"

# include <MatrixStack.h>
# include <GL/glew.h>
# include <GL/freeglut.h>

Planet::Planet(Object* object, Billboard* billboard, float radius, float rotationSpeed,
                         float ownRotationSpeed, float orbit, bool collidable):
    SpaceObject::SpaceObject(radius),
    object_(object),
    billboard_(billboard),
    rotationSpeed_(rotationSpeed),
    ownRotationSpeed_(ownRotationSpeed),
    orbit_(orbit),
    collisionSphere_(NULL) {

    if (collidable)
        collisionSphere_= new CollisionSphere(gloost::Vector3(), radius *0.5f);
}

void Planet::update(double frameTime) {
    position_ = gloost::Vector3(orbit_*std::cos(glutGet(GLUT_ELAPSED_TIME)*0.00005*rotationSpeed_), 0.0, orbit_*std::sin(glutGet(GLUT_ELAPSED_TIME)*0.00005*rotationSpeed_));

    if (collisionSphere_)
        collisionSphere_->setPosition(getTransform());

    for(int i(0); i<satellites_.size(); ++i)
        satellites_[i]->update(frameTime);
}

void Planet::draw() const {
    SpaceScene* scene(SpaceScene::pointer());

    gloost::MatrixStack* transform(scene->getMatrixStack());
    transform->push();

        // apply transformations of this object
        if (collisionSphere_)
            transform->translate(collisionSphere_->getPosition());
        else transform->translate(getTransform());

        transform->push();

            transform->scale(radius_, radius_, radius_);

            glUniform3f(scene->getAtmoColorUL(), object_->atmoR_, object_->atmoG_, object_->atmoB_);

            // draw billboard
            if (billboard_) {
                billboard_->draw();
            }

            transform->rotate(0.0, ownRotationSpeed_*glutGet(GLUT_ELAPSED_TIME)*0.00005, 0.0);

            scene->uploadTransform();

            // draw sphere
            if (object_) {
                if (object_->diffuse_)   object_->diffuse_->bind(0);
                else                     Texture::unbind(0);

                if (object_->normal_)    object_->normal_->bind(1);
                else                     Texture::unbind(1);

                if (object_->specular_)  object_->specular_->bind(2);
                else                     Texture::unbind(2);

                if (object_->emit_)      object_->emit_->bind(3);
                else                     Texture::unbind(3);

                object_->mesh_->draw();
            }



        transform->pop();

    transform->pop();
}
