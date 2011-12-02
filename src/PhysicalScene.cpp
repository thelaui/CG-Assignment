# include "include/PhysicalScene.hpp"

# include "include/CollisionSphere.hpp"

PhysicalScene::PhysicalScene(){}

void PhysicalScene::addSphere(CollisionSphere* toBeAdded, bool isStatic) {
    if (isStatic) addAtFreePosition(toBeAdded, staticSpheres_);
    else          addAtFreePosition(toBeAdded, dynamicSpheres_);
}

void PhysicalScene::removeSphere(CollisionSphere* toBeRemoved, bool isStatic) {
    if (isStatic) freePosition(toBeRemoved, staticSpheres_);
    else          freePosition(toBeRemoved, dynamicSpheres_);
}

void PhysicalScene::update() {
    for (std::vector<CollisionSphere*>::iterator dynamicSphere(dynamicSpheres_.begin()); dynamicSphere != dynamicSpheres_.end(); ++dynamicSphere) {
        if (*dynamicSphere) {
            for (std::vector<CollisionSphere*>::iterator staticSphere(staticSpheres_.begin()); staticSphere != staticSpheres_.end(); ++staticSphere) {
                if (*staticSphere) {
                    if (collide(*dynamicSphere, *staticSphere))
                        computeCollision(*dynamicSphere, *staticSphere);
                }
            }
        }
    }
}

bool PhysicalScene::collide(CollisionSphere* first, CollisionSphere* second) const {
    return (first->getPosition() - second->getPosition()).length() < (first->getRadius() + second->getRadius());
}

void PhysicalScene::computeCollision(CollisionSphere* changing, CollisionSphere* untouched) {
    gloost::Vector3 correctionDirection(changing->getPosition() - untouched->getPosition());
    correctionDirection.normalize();

    changing->countCollision();
    untouched->countCollision();

    changing->setPosition(untouched->getPosition() + correctionDirection*(changing->getRadius() + untouched->getRadius()));
}

void PhysicalScene::addAtFreePosition(CollisionSphere* toBeAdded, std::vector<CollisionSphere*>& sphereVector) {
    bool foundFree(false);
    for (std::vector<CollisionSphere*>::iterator sphere(sphereVector.begin()); sphere != sphereVector.end(); ++sphere) {
        if (*sphere == NULL) {
            foundFree = true;
            *sphere = toBeAdded;
            break;
        }
    }

    if (!foundFree) sphereVector.push_back(toBeAdded);
}

void PhysicalScene::freePosition(CollisionSphere* toBeRemoved, std::vector<CollisionSphere*>& sphereVector) {
    for (std::vector<CollisionSphere*>::iterator sphere(sphereVector.begin()); sphere != sphereVector.end(); ++sphere) {
        if (*sphere == toBeRemoved) {
            *sphere = NULL;
            break;
        }
    }
}
