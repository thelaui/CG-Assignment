# ifndef PHYSICAL_SCENE_HPP
# define PHYSICAL_SCENE_HPP

# include "include/KnownObject.hpp"

# include <vector>

class CollisionSphere;

class PhysicalScene : public KnownObject<PhysicalScene> {
    public:
        PhysicalScene();

        void addSphere(CollisionSphere* toBeAdded, bool isStatic);

        void update();

    private:
        std::vector<CollisionSphere*> staticSpheres_;
        std::vector<CollisionSphere*> dynamicSpheres_;

        bool collide(CollisionSphere* first, CollisionSphere* second) const;
        void computeCollision(CollisionSphere* changing, CollisionSphere* untouched);
};

# endif //PHYSICAL_SCENE_HPP

