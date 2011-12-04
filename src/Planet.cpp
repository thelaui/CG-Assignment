# include "include/Planet.hpp"

# include "include/SpaceScene.hpp"
# include "include/Billboard.hpp"
# include "include/Texture.hpp"
# include "include/window.hpp"
# include "include/CollisionSphere.hpp"
# include "include/Emitter.hpp"

# include <MatrixStack.h>
# include <GL/glew.h>
# include <GL/freeglut.h>

Planet::Planet(Object* object, Billboard* billboard, float radius, float rotationSpeed,
                         float ownRotationSpeed, float orbit, bool collidable, gloost::Vector3 const& rotationAxis):
    SpaceObject::SpaceObject(radius),
    object_(object),
    billboard_(billboard),
    rotationSpeed_(rotationSpeed),
    ownRotationSpeed_(ownRotationSpeed),
    orbit_(orbit),
    rotationAxis_(rotationAxis),
    collisionSphere_(NULL),
    life_(radius*100),
    alive_(true),
    explosion_(NULL) {

    if (collidable) {
        collisionSphere_= new CollisionSphere(gloost::Vector3(), radius *0.5f);

        ParticleTemplate explosion;
        explosion.lifeTime = 0.1;
        explosion.r = AnimatedValue(AnimatedValue::Out, 1.0, 1.0, explosion.lifeTime);
        explosion.g = AnimatedValue(AnimatedValue::Out, 1.0, 0.0, explosion.lifeTime);
        explosion.b = AnimatedValue(AnimatedValue::Linear, 0.5, 0.0, explosion.lifeTime*0.1);
        explosion.a = AnimatedValue(AnimatedValue::Linear, 1.0, 0.0, explosion.lifeTime);
        explosion.size = AnimatedValue(AnimatedValue::Linear, 50, 0.0, explosion.lifeTime);
        explosion.movementInterpolation = AnimatedValue::Linear;
        explosion.movementMultiplier = 0;
        explosion.texture = new Texture("data/textures/fuel.jpg");
        explosion.rate = AnimatedValue(AnimatedValue::Linear, 0.0, 0.0, 0.0);
        explosion.colliding = false;

        explosion_ = new Emitter(explosion, gloost::Vector3(), gloost::Vector3());
    }
}

Planet::~Planet() {
    if (collisionSphere_)
        delete collisionSphere_;

    if (explosion_)
        delete explosion_;
}

void Planet::update(double frameTime) {
    gloost::MatrixStack transform;
    transform.rotate(glutGet(GLUT_ELAPSED_TIME)*0.00005*rotationSpeed_, rotationAxis_[0], rotationAxis_[1], rotationAxis_[2]);
    transform.translate(0, 0, orbit_);
    position_ = transform.top().getTranslate();

    if (collisionSphere_)
        collisionSphere_->setPosition(getTransform());

    for(int i(0); i<satellites_.size(); ++i)
        satellites_[i]->update(frameTime);

    if (collisionSphere_ && collisionSphere_->getCollisionCount() > life_) {
        delete collisionSphere_;
        collisionSphere_ = NULL;
        alive_ = false;
        explosion_->setPosition(getTransform());
        explosion_->setRate(100, 3.f);
    }

    if (!alive_) {
        explosion_->setDirection(gloost::Vector3(0.f, 0.1f, 0.f));
        explosion_->update(frameTime);
    }
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
            if (alive_ && billboard_) {
                billboard_->draw();
            }

            transform->rotate(0.0, ownRotationSpeed_*glutGet(GLUT_ELAPSED_TIME)*0.00005, 0.0);

            scene->uploadTransform();

            // draw sphere
            if (alive_ && object_) {
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

            else explosion_ -> draw();

        transform->pop();

    transform->pop();
}
