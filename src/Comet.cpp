# include "include/Comet.hpp"

# include "include/Texture.hpp"
# include "include/Emitter.hpp"

Comet::Comet(Object* object, Billboard* billboard, float radius, float rotationSpeed,
             float ownRotationSpeed, float orbit, gloost::Vector3 const& color):

    Planet::Planet(object, billboard, radius, rotationSpeed, ownRotationSpeed, orbit, true),
    trail_(NULL) {

    ParticleTemplate trail;
    trail.lifeTime = 2;
    trail.r = AnimatedValue(AnimatedValue::Out, 1.0, color[0], trail.lifeTime);
    trail.g = AnimatedValue(AnimatedValue::Out, 1.0, color[1], trail.lifeTime);
    trail.b = AnimatedValue(AnimatedValue::Out, 1.0, color[2], trail.lifeTime);
    trail.a = AnimatedValue(AnimatedValue::Out, 1.0, 0.0, trail.lifeTime);
    trail.size = AnimatedValue(AnimatedValue::Linear, 200*radius, 0.0, trail.lifeTime);
    trail.movementInterpolation = AnimatedValue::Linear;
    trail.movementMultiplier = 0;
    trail.texture = new Texture("data/textures/fuel.jpg");
    trail.rate = AnimatedValue(AnimatedValue::Linear, 100.0, 60.0, 0.0);
    trail.colliding = false;

    trail_ = new Emitter(trail, gloost::Vector3(), gloost::Vector3());
}


Comet::~Comet() {
    if (trail_)
        delete trail_;
}

void Comet::update(double frameTime) {
    Planet::update(frameTime);

    trail_->setPosition(getTransform());
    trail_->setDirection(getTransform().normalized()*20);

    trail_->update(frameTime);
}

void Comet::draw() const {
    Planet::draw();
    trail_->draw();
}
