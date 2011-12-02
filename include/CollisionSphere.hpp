# ifndef COLLISION_SPHERE_HPP
# define COLLISION_SPHERE_HPP

# include <gloostMath.h>

class CollisionSphere {
    public:
        CollisionSphere(gloost::Vector3 const& position, float radius, bool isStatic = true);

        ~CollisionSphere();

        gloost::Vector3 const& getPosition()       const;
        float                  getRadius()         const;
        int                    getCollisionCount() const;

        void setPosition(gloost::Vector3 const& position);
        void countCollision();

    private:
        gloost::Vector3 position_;
        float           radius_;
        bool            isStatic_;

        int collisionCount_;
};

# endif //COLLISION_SPHERE_HPP
