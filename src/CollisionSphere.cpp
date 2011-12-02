# include "include/CollisionSphere.hpp"

# include "include/PhysicalScene.hpp"

CollisionSphere::CollisionSphere(gloost::Vector3 const& position, float radius, bool isStatic):
    position_(position),
    radius_(radius) {
        PhysicalScene::pointer()->addSphere(this, isStatic);
    }

gloost::Vector3 const& CollisionSphere::getPosition() const {
    return position_;
}

float CollisionSphere::getRadius() const {
    return radius_;
}

void CollisionSphere::setPosition(gloost::Vector3 const& position) {
    position_=position;
}
