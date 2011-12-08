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
    life_(radius*30),
    alive_(true),
    eruptions_(),
    explosion_(NULL),
    sparks_(NULL),
    ring_(NULL) {

    if (collidable) {
        collisionSphere_= new CollisionSphere(gloost::Vector3(), radius *0.5f);

        ParticleTemplate explosion;
        explosion.lifeTime = 1.0;
        explosion.r = AnimatedValue(AnimatedValue::Out, 1.0, 1.0, explosion.lifeTime);
        explosion.g = AnimatedValue(AnimatedValue::Out, 1.0, 0.0, explosion.lifeTime);
        explosion.b = AnimatedValue(AnimatedValue::Linear, 0.5, 0.0, explosion.lifeTime*0.1);
        explosion.a = AnimatedValue(AnimatedValue::Linear, 0.5, 0.0, explosion.lifeTime);
        explosion.size = AnimatedValue(AnimatedValue::Out, 0.0, radius*200, explosion.lifeTime, 2);
        explosion.movementInterpolation = AnimatedValue::Linear;
        explosion.movementMultiplier = 0;
        explosion.texture = new Texture("data/textures/fuel.jpg");
        explosion.rate = AnimatedValue(AnimatedValue::Linear, 20.0, 20.0, 0.0);
        explosion.colliding = false;

        explosion_ = new Emitter(explosion, gloost::Vector3(), gloost::Vector3());

        ParticleTemplate spark;
        spark.lifeTime = 1.0;
        spark.r = AnimatedValue(AnimatedValue::Out, 1.0, 1.0, spark.lifeTime);
        spark.g = AnimatedValue(AnimatedValue::Out, 1.0, 0.0, spark.lifeTime);
        spark.b = AnimatedValue(AnimatedValue::Linear, 0.5, 0.0, spark.lifeTime*0.1);
        spark.a = AnimatedValue(AnimatedValue::Linear, 0.5, 0.0, spark.lifeTime);
        spark.size = AnimatedValue(AnimatedValue::Linear, 5*radius, 0, spark.lifeTime);
        spark.movementInterpolation = AnimatedValue::Out;
        spark.movementMultiplier = 1;
        spark.texture = new Texture("data/textures/fuel.jpg");
        spark.rate = AnimatedValue(AnimatedValue::Linear, 500.0, 500.0, 0.0);
        spark.colliding = false;

        sparks_ = new Emitter(spark, gloost::Vector3(), gloost::Vector3(), gloost::Vector3(1, 1, 1)*radius);

        ParticleTemplate ring;
        ring.lifeTime = 7.0;
        ring.r = AnimatedValue(AnimatedValue::Out, object_->atmoR_, object_->atmoR_, ring.lifeTime);
        ring.g = AnimatedValue(AnimatedValue::Out, object_->atmoG_, object_->atmoG_, ring.lifeTime);
        ring.b = AnimatedValue(AnimatedValue::Linear, object_->atmoB_, object_->atmoB_, ring.lifeTime*0.1);
        ring.a = AnimatedValue(AnimatedValue::Linear, 0.5, 0.0, ring.lifeTime);
        ring.size = AnimatedValue(AnimatedValue::Out, radius*5, radius*5, ring.lifeTime, 2);
        ring.movementInterpolation = AnimatedValue::Linear;
        ring.movementMultiplier = 0;
        ring.texture = new Texture("data/textures/fuel.jpg");
        ring.rate = AnimatedValue(AnimatedValue::Out, 15000.0, 15000.0, 0.0, 2);
        ring.colliding = false;

        ring_ = new Emitter(ring, gloost::Vector3(), gloost::Vector3(), gloost::Vector3(15, 0, 15)*radius);
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

    for(std::list<Emitter*>::iterator eruption = eruptions_.begin(); eruption != eruptions_.end(); ++eruption) {
        if ((*eruption)->isIdle()) {
            delete *eruption;
            eruption = eruptions_.erase(eruption);
            --eruption;
        }
        else (*eruption)->update(frameTime);
    }

    if (collisionSphere_) {
        collisionSphere_->setPosition(getTransform());

        std::list<gloost::Vector3> collisionPositions(collisionSphere_->getCollisionPositions());

        if (collisionPositions.size() > 0) {

            ParticleTemplate eruption;
            eruption.lifeTime = 1.f;
            eruption.r = AnimatedValue(AnimatedValue::Out, object_->atmoR_, object_->atmoR_, eruption.lifeTime);
            eruption.g = AnimatedValue(AnimatedValue::Out, object_->atmoG_, object_->atmoG_, eruption.lifeTime);
            eruption.b = AnimatedValue(AnimatedValue::Linear, object_->atmoB_, object_->atmoB_, eruption.lifeTime*0.1);
            eruption.a = AnimatedValue(AnimatedValue::Out, 1.f, 0.0, eruption.lifeTime);
            eruption.size = AnimatedValue(AnimatedValue::Out, 5, 0, eruption.lifeTime, 2);
            eruption.movementInterpolation = AnimatedValue::Out;
            eruption.movementMultiplier = 10;
            eruption.texture = new Texture("data/textures/fuel.jpg");
            eruption.rate = AnimatedValue(AnimatedValue::Out, 500.0, 0.0, 0.5, 2);
            eruption.colliding = false;

            for (std::list<gloost::Vector3>::iterator position(collisionPositions.begin()); position!=collisionPositions.end(); ++position) {
                eruptions_.push_back(new Emitter(eruption, *position, (*position - position_).normalized() * 0.1, gloost::Vector3(0.05, 0.05, 0.05)));
            }
        }
    }

    for(int i(0); i<satellites_.size(); ++i)
        satellites_[i]->update(frameTime);

    if (collisionSphere_ && collisionSphere_->getCollisionCount() > life_) {
        delete collisionSphere_;
        collisionSphere_ = NULL;
        alive_ = false;
        explosion_->setPosition(getTransform());
        explosion_->setRate(0, 3.f);
        sparks_->setPosition(getTransform());
        sparks_->setRate(0, 1.f);
        ring_->setPosition(getTransform());
        ring_->setRate(0, 0.3f);
    }

    if (!alive_) {
        explosion_->update(frameTime);
        sparks_->update(frameTime);
        ring_->update(frameTime);
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

            else {
                explosion_ -> draw();
                sparks_ -> draw();
                ring_ -> draw();
            }

            for (std::list<Emitter*>::const_iterator eruption = eruptions_.begin(); eruption != eruptions_.end(); ++eruption) {
                (*eruption)->draw();
            }

        transform->pop();

    transform->pop();
}
