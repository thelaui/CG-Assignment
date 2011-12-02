////////////////////////////////////////////////////////////////////
/// \file
/// \brief A class which is capable of drawing a Billboard.
//
// CG-Beleg 2, by Felix Lauer (90404) & Simon Schneegans (90405)
////////////////////////////////////////////////////////////////////

# ifndef EMITTER_HPP
# define EMITTER_HPP

# include "include/SpaceObject.hpp"
# include "include/Particle.hpp"

# include <list>

struct ParticleTemplate {
    AnimatedValue r;
    AnimatedValue g;
    AnimatedValue b;
    AnimatedValue a;
    AnimatedValue size;
    AnimatedValue::Direction movementInterpolation;
    float movementMultiplier;
    float lifeTime;
    Texture* texture;
    AnimatedValue rate;
};

////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////
class Emitter: public SpaceObject {
    public:
        Emitter(ParticleTemplate const& settings,
                gloost::Vector3 const& position,
                gloost::Vector3 const& rotation);

        ~Emitter();

        void update(double frameTime);

        void draw() const;

        void setPosition(gloost::Vector3 const& position);
        void setDirection(gloost::Vector3 const& direction);
        void setRate(double rate, double time);

    private:
        mutable std::list<Particle> particles_;

        gloost::Vector3 position_, lastFramePosition_;
        gloost::Vector3 direction_, lastFrameDirection_;

        ParticleTemplate settings_;
        float spawnTimer_, spawnDelay_;

        unsigned bufferIds_[2];
};

# endif //EMITTER_HPP






