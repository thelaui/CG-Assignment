# include "include/PhysicalScene.hpp"

# include "include/CollisionSphere.hpp"

PhysicalScene::PhysicalScene(){}

void PhysicalScene::addSphere(CollisionSphere* toBeAdded, bool isStatic) {
    if (isStatic)
        staticSpheres_.push_back(toBeAdded);
    else dynamicSpheres_.push_back(toBeAdded);
}

void PhysicalScene::update() {
    for (std::vector<CollisionSphere*>::iterator dynamicSphere(dynamicSpheres_.begin()); dynamicSphere != dynamicSpheres_.end(); ++dynamicSphere) {
        for (std::vector<CollisionSphere*>::iterator staticSphere(staticSpheres_.begin()); staticSphere != staticSpheres_.end(); ++staticSphere) {
            if (collide(*dynamicSphere, *staticSphere))
                computeCollision(*dynamicSphere, *staticSphere);
        }
    }
}

bool PhysicalScene::collide(CollisionSphere* first, CollisionSphere* second) const {
    return (first->getPosition() - second->getPosition()).length() < (first->getRadius() + second->getRadius());
}

void PhysicalScene::computeCollision(CollisionSphere* changing, CollisionSphere* untouched) {
    gloost::Vector3 correctionDirection(changing->getPosition() - untouched->getPosition());
    correctionDirection.normalize();

    changing->setPosition(untouched->getPosition() + correctionDirection*(changing->getRadius() + untouched->getRadius()));
}
