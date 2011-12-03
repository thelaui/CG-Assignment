# include "include/Ship.hpp"

# include "include/Object.hpp"
# include "include/Texture.hpp"
# include "include/window.hpp"
# include "include/CollisionSphere.hpp"
# include "include/Emitter.hpp"
# include "include/SpaceScene.hpp"
# include "include/PhysicalScene.hpp"

# include <GL/glew.h>
# include <GL/freeglut.h>

Ship::Ship(Object* object, gloost::Vector3 const& rotation, gloost::Vector3 const& position):
    object_(object),
    collisionSphere_(new CollisionSphere(position, 0.05f, false)),
    speed_(AnimatedValue::Out, 0, 0, 2, 0.5),
    rotX_(AnimatedValue::Linear, 0, 0, 0),
    rotY_(AnimatedValue::Linear, 0, 0, 0),
    rotZ_(AnimatedValue::Linear, 0, 0, 0),
    camRotX_(AnimatedValue::Linear, 0, 0, 0),
    camRotY_(AnimatedValue::Linear, 0, 0, 0),
    camRotZ_(AnimatedValue::InOut, 0, 0, 0),
    fuel1_(NULL),
    fuel2_(NULL),
    fuel3_(NULL),
    cannon1_(NULL),
    cannon2_(NULL) {

    // ###### fuel setup #######
    ParticleTemplate fuel;
    fuel.lifeTime = 0.1;
    fuel.r = AnimatedValue(AnimatedValue::Out, 1.0, 1.0, fuel.lifeTime);
    fuel.g = AnimatedValue(AnimatedValue::Out, 1.0, 0.0, fuel.lifeTime);
    fuel.b = AnimatedValue(AnimatedValue::Linear, 0.5, 0.0, fuel.lifeTime*0.1);
    fuel.a = AnimatedValue(AnimatedValue::Linear, 1.0, 0.0, fuel.lifeTime);
    fuel.size = AnimatedValue(AnimatedValue::Linear, 1.5, 0.0, fuel.lifeTime);
    fuel.movementInterpolation = AnimatedValue::Linear;
    fuel.movementMultiplier = 0;
    fuel.texture = new Texture("data/textures/fuel.jpg");
    fuel.rate = AnimatedValue(AnimatedValue::Linear, 0.0, 0.0, 0.0);
    fuel.colliding = false;

    fuel1_ = new Emitter(fuel, gloost::Vector3(), gloost::Vector3());
    fuel2_ = new Emitter(fuel, gloost::Vector3(), gloost::Vector3());
    fuel3_ = new Emitter(fuel, gloost::Vector3(), gloost::Vector3());



    // ###### cannon setup #######
    ParticleTemplate cannon;
    cannon.lifeTime = 3.0;
    cannon.r = AnimatedValue(AnimatedValue::Linear, 0.9, 0.9, cannon.lifeTime);
    cannon.g = AnimatedValue(AnimatedValue::Linear, 0.8, 0.8, cannon.lifeTime);
    cannon.b = AnimatedValue(AnimatedValue::Linear, 0.7, 0.7, cannon.lifeTime);
    cannon.a = AnimatedValue(AnimatedValue::Out, 1.0, 0.0, cannon.lifeTime);
    cannon.size = AnimatedValue(AnimatedValue::Linear, 10.0, 10.0, cannon.lifeTime);
    cannon.movementInterpolation = AnimatedValue::Linear;
    cannon.movementMultiplier = 0;
    cannon.texture = new Texture("data/textures/bullet.jpg");
    cannon.rate = AnimatedValue(AnimatedValue::Linear, 10.0, 10.0, 0.0);
    cannon.colliding = true;

    cannon1_ = new Emitter(cannon, gloost::Vector3(), gloost::Vector3());
    cannon2_ = new Emitter(cannon, gloost::Vector3(), gloost::Vector3());

    transform_.translate(position);
    transform_.rotate(rotation);
}

Ship::~Ship() {
    if (fuel1_) delete fuel1_;
    if (fuel2_) delete fuel2_;
    if (fuel3_) delete fuel3_;
    if (cannon1_) delete cannon1_;
    if (cannon2_) delete cannon2_;
}

void Ship::draw() {
    if (object_) {

        SpaceScene* scene(SpaceScene::pointer());

        gloost::MatrixStack* transform(scene->getMatrixStack());
        transform->push();

            glUniform3f(scene->getAtmoColorUL(), object_->atmoR_, object_->atmoG_, object_->atmoB_);

            transform_.top().setTranslate(collisionSphere_->getPosition());

            transform->loadMatrix(transform_.top());
            transform->scale(0.1, 0.1, 0.1);
            scene->uploadTransform();

            if (object_->diffuse_)   object_->diffuse_->bind(0);
            else                     Texture::unbind(0);

            if (object_->normal_)    object_->normal_->bind(1);
            else                     Texture::unbind(1);

            if (object_->specular_)  object_->specular_->bind(2);
            else                     Texture::unbind(2);

            if (object_->emit_)      object_->emit_->bind(3);
            else                     Texture::unbind(3);

            cannon1_->draw();
            cannon2_->draw();

            object_->mesh_->draw();

            fuel1_->draw();
            fuel2_->draw();
            fuel3_->draw();

        transform->pop();
    }
}

void Ship::update(double frameTime) {
    transform_.translate(0, 0, speed_.val()*frameTime);
    transform_.rotate(rotX_.val()*frameTime, rotY_.val()*frameTime, rotZ_.val()*frameTime);

    speed_.update(frameTime);
    rotX_.update(frameTime);
    rotY_.update(frameTime);
    rotZ_.update(frameTime);
    camRotX_.update(frameTime);
    camRotY_.update(frameTime);
    camRotZ_.update(frameTime);

    gloost::Vector3 direction = (transform_.top() * gloost::Vector3(0, 0, 0.01));

    transform_.push();
        transform_.translate(0.036, -0.007 , 0.042);
        fuel1_->setPosition(transform_.top().getTranslate());
        fuel1_->setDirection(direction);
    transform_.pop();
    transform_.push();
        transform_.translate(-0.036, -0.007 , 0.042);
        fuel2_->setPosition(transform_.top().getTranslate());
        fuel2_->setDirection(direction);
    transform_.pop();
    transform_.push();
        transform_.translate(0.0, -0.012 , 0.018);
        fuel3_->setPosition(transform_.top().getTranslate());
        fuel3_->setDirection(direction);
    transform_.pop();
    transform_.push();
        transform_.translate(0.036, -0.007 , -0.018);
        cannon1_->setPosition(transform_.top().getTranslate());
        cannon1_->setDirection(1000*direction*(speed_.val()-1));
    transform_.pop();
    transform_.push();
        transform_.translate(-0.036, -0.007 , -0.018);
        cannon2_->setPosition(transform_.top().getTranslate());
        cannon2_->setDirection(1000*direction*(speed_.val()-1));
    transform_.pop();

    fuel1_->update(frameTime);
    fuel2_->update(frameTime);
    fuel3_->update(frameTime);
    cannon1_->update(frameTime);
    cannon2_->update(frameTime);

    collisionSphere_->setPosition(transform_.top().getTranslate());
}

gloost::Matrix Ship::getCameraTransform() {
    transform_.push();
        transform_.rotate(-camRotX_.val()*0.5, -camRotY_.val()*0.5, -camRotZ_.val()*0.5);
        transform_.translate(0, 0.02 + std::sin(glutGet(GLUT_ELAPSED_TIME)*0.001)*0.001, 0.1-speed_.val()*0.05);
        gloost::Matrix cameraTransform(transform_.top());
    transform_.pop();

    return cameraTransform;
}

void Ship::accelerate(bool accel) {
    if (accel) {
        speed_.resetTarget(-3, 3.0);
        fuel1_->setRate(1000, 0.3);
        fuel2_->setRate(1000, 0.3);
        fuel3_->setRate(1000, 0.3);
    } else {
        speed_.resetTarget(0, 2.0);
        fuel1_->setRate(0, 1.0);
        fuel2_->setRate(0, 1.0);
        fuel3_->setRate(0, 1.0);
    }
}

void Ship::rotateLeft(bool rotate) {
    if (rotate) {
        rotZ_.resetTarget(2, 1.0);
        camRotZ_.resetTarget(2, 2.0);
    } else {
        rotZ_.resetTarget(0, 1.0);
        camRotZ_.resetTarget(0, 2.0);
    }
}

void Ship::rotateRight(bool rotate) {
    if (rotate) {
        rotZ_.resetTarget(-2, 1.0);
        camRotZ_.resetTarget(-2, 2.0);
    } else {
        rotZ_.resetTarget(0, 1.0);
        camRotZ_.resetTarget(0, 2.0);
    }
}

void Ship::turn(double amount) {
    rotY_.resetTarget(amount*100, 0.2);
    camRotY_.resetTarget(amount*10, 0.5);
}

void Ship::tilt(double amount) {
    rotX_.resetTarget(amount*100, 0.2);
    camRotX_.resetTarget(amount*10, 0.5);
}
