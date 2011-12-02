# ifndef PHYSICAL_SCENE_HPP
# define PHYSICAL_SCENE_HPP

# include "include/KnownObject.hpp"

# include <vector>

class CollisionSphere;

class PhysicalScene : public KnownObject<PhysicalScene> {
    public:
        PhysicalScene();

        void addSphere(CollisionSphere* toBeAdded, bool isStatic);
        void removeSphere(CollisionSphere* toBeAdded, bool isStatic);

        void update();

    private:
        std::vector<CollisionSphere*> staticSpheres_;
        std::vector<CollisionSphere*> dynamicSpheres_;

        bool collide(CollisionSphere* first, CollisionSphere* second) const;
        void computeCollision(CollisionSphere* changing, CollisionSphere* untouched);
        void addAtFreePosition(CollisionSphere* toBeAdded, std::vector<CollisionSphere*>& sphereVector);
        void freePosition(CollisionSphere* toBeRemoved, std::vector<CollisionSphere*>& sphereVector);
};

# endif //PHYSICAL_SCENE_HPP

