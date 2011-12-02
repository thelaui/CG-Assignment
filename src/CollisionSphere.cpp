# include "include/CollisionSphere.hpp"

# include "include/PhysicalScene.hpp"

CollisionSphere::CollisionSphere(gloost::Vector3 const& position, float radius, bool isStatic):
    position_(position),
    radius_(radius),
    isStatic_(isStatic),
    collisionCount_(0) {
        PhysicalScene::pointer()->addSphere(this, isStatic);
}

CollisionSphere::~CollisionSphere() {
    PhysicalScene::pointer()->removeSphere(this, isStatic_);
}

gloost::Vector3 const& CollisionSphere::getPosition() const {
    return position_;
}

float CollisionSphere::getRadius() const {
    return radius_;
}

int CollisionSphere::getCollisionCount() const {
    return collisionCount_;
}

void CollisionSphere::setPosition(gloost::Vector3 const& position) {
    position_=position;
}

void CollisionSphere::countCollision() {
    ++collisionCount_;
}
