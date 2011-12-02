# ifndef COLLISION_SPHERE_HPP
# define COLLISION_SPHERE_HPP

# include <gloostMath.h>

class CollisionSphere {
    public:
        CollisionSphere(gloost::Vector3 const& position, float radius, bool isStatic = true);

        gloost::Vector3 const& getPosition() const;
        float                  getRadius()   const;

        void setPosition(gloost::Vector3 const& position);

    private:
        gloost::Vector3 position_;
        float radius_;
};

# endif //COLLISION_SPHERE_HPP
