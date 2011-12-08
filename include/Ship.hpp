////////////////////////////////////////////////////////////////////
/// \file
/// \brief A SpaceObject can have any number of SpaceObjects
///        floating in circles around.
//
// CG-Beleg 2, by Felix Lauer (90404) & Simon Schneegans (90405)
////////////////////////////////////////////////////////////////////

# ifndef SHIP_HPP
# define SHIP_HPP

# include "include/AnimatedValue.hpp"

# include <gloostMath.h>
# include <MatrixStack.h>

class Object;
class CollisionSphere;
class Emitter;

class Ship {
    public:
        Ship(Object* object, gloost::Vector3 const& rotation, gloost::Vector3 const& position);
        ~Ship();

        void draw();
        void update(double frameTime);

        gloost::Matrix getCameraTransform();

        void accelerate(bool accel);

        void shoot(bool shoot);

        void rotateLeft(bool rotate);
        void rotateRight(bool rotate);

        void turn(double amount);
        void tilt(double amount);


    private:
        Object* object_;
        CollisionSphere* collisionSphere_;

        AnimatedValue speed_;
        AnimatedValue rotX_, rotY_, rotZ_;
        AnimatedValue camRotX_, camRotY_, camRotZ_;

        gloost::MatrixStack transform_;

        Emitter *fuel1_, *fuel2_, *fuel3_;
        Emitter *cannon1_, *cannon2_;
};

# endif //SHIP_HPP

