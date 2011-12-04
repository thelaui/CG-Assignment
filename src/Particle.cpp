# include "include/Particle.hpp"

# include "include/Emitter.hpp"
# include "include/CollisionSphere.hpp"

int Particle::count = 0;

Particle::Particle(ParticleTemplate const& settings, gloost::Vector3 const& from, gloost::Vector3 const& direction, bool collidable):
    a_(settings.a),
    r_(settings.r),
    g_(settings.g),
    b_(settings.b),
    size_(settings.size),
    x_(settings.movementInterpolation, from[0], from[0] + direction[0], settings.lifeTime, settings.movementMultiplier),
    y_(settings.movementInterpolation, from[1], from[1] + direction[1], settings.lifeTime, settings.movementMultiplier),
    z_(settings.movementInterpolation, from[2], from[2] + direction[2], settings.lifeTime, settings.movementMultiplier),
    life_(settings.lifeTime),
    collisionSphere_(NULL) {

    if (collidable)
        collisionSphere_ = new CollisionSphere(from, 0.001f, false);

    ++count;
}

Particle::~Particle() {
    if (collisionSphere_)
        delete collisionSphere_;
    --count;
}

void Particle::update(double frameTime) {
    a_.update(frameTime);
    size_.update(frameTime);
    x_.update(frameTime);
    y_.update(frameTime);
    z_.update(frameTime);
    r_.update(frameTime);
    g_.update(frameTime);
    b_.update(frameTime);
    life_ -= frameTime;

    if (collisionSphere_ ) {
        if (collisionSphere_->getCollisionCount() == 0)
            collisionSphere_->setPosition(getPosition());
        else life_ = 0.f;
    }
}

gloost::Vector3 Particle::getPosition() const {
    return gloost::Vector3(x_.val(), y_.val(), z_.val());
}

gloost::Vector3 Particle::getColor() const {
    return gloost::Vector3(r_.val(), g_.val(), b_.val());
}

float Particle::getAlpha() const {
    return a_.val();
}

float Particle::getSize() const {
    return size_.val();
}

bool Particle::isDead() const {
    return life_ <= 0.0;
}
