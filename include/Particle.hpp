////////////////////////////////////////////////////////////////////
/// \file
/// \brief A class which is capable of drawing a Billboard.
//
// CG-Beleg 2, by Felix Lauer (90404) & Simon Schneegans (90405)
////////////////////////////////////////////////////////////////////

# ifndef PARTICLE_HPP
# define PARTICLE_HPP

# include <gloostMath.h>

# include "include/AnimatedValue.hpp"

struct ParticleTemplate;
class  CollisionSphere;

////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////

class Particle {
    public:
        Particle(ParticleTemplate const& settings,
                 gloost::Vector3 const& from,
                 gloost::Vector3 const& direction,
                 bool collidable = false);

        ~Particle();

        void update(double frameTime);

        gloost::Vector3 getPosition() const;
        gloost::Vector3 getColor() const;
        float getAlpha() const;
        float getSize() const;

        bool isDead() const;

    private:
        AnimatedValue r_;
        AnimatedValue g_;
        AnimatedValue b_;
        AnimatedValue a_;
        AnimatedValue size_;
        AnimatedValue x_;
        AnimatedValue y_;
        AnimatedValue z_;
        float life_;
        CollisionSphere* collisionSphere_;
};

# endif //PARTICLE_HPP







