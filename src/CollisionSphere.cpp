# include "include/CollisionSphere.hpp"

# include "include/PhysicalScene.hpp"

CollisionSphere::CollisionSphere(gloost::Vector3 const& position, float radius, bool isStatic, bool countCollision):
    position_(position),
    collisionPositions_(),
    radius_(radius),
    isStatic_(isStatic),
    countCollision_(countCollision),
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

bool CollisionSphere::isCollisionCounted() const {
    return countCollision_;
}

void CollisionSphere::setPosition(gloost::Vector3 const& position) {
    position_=position;
}

void CollisionSphere::addCollisionPosition(gloost::Vector3 const& position) {
    collisionPositions_.push_back(position);
}

std::list<gloost::Vector3> const& CollisionSphere::getCollisionPositions() const {
    return collisionPositions_;
}

void CollisionSphere::clearCollisionPositions() {
    collisionPositions_.clear();
}

void CollisionSphere::countCollision() {
    ++collisionCount_;
}
