# ifndef COLLISION_SPHERE_HPP
# define COLLISION_SPHERE_HPP

# include <gloostMath.h>
# include <list>

class CollisionSphere {
    public:
        CollisionSphere(gloost::Vector3 const& position, float radius, bool isStatic = true, bool countCollision = true);

        ~CollisionSphere();

        gloost::Vector3 const& getPosition()        const;
        float                  getRadius()          const;
        int                    getCollisionCount()  const;
        bool                   isCollisionCounted() const;

        void setPosition(gloost::Vector3 const& position);
        void addCollisionPosition(gloost::Vector3 const& position);
        std::list<gloost::Vector3> const& getCollisionPositions() const;
        void clearCollisionPositions();

        void countCollision();

    private:
        gloost::Vector3            position_;
        std::list<gloost::Vector3> collisionPositions_;
        float                      radius_;
        bool                       isStatic_;
        bool                       countCollision_;

        int collisionCount_;
};

# endif //COLLISION_SPHERE_HPP
