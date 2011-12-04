////////////////////////////////////////////////////////////////////
/// \file
/// \brief A SpaceObject can have any number of SpaceObjects
///        floating in circles around.
//
// CG-Beleg 2, by Felix Lauer (90404) & Simon Schneegans (90405)
////////////////////////////////////////////////////////////////////

# ifndef PLANET_HPP
# define PLANET_HPP

# include "include/SpaceObject.hpp"

class CollisionSphere;
class Emitter;

////////////////////////////////////////////////////////////////////
/// \brief A class an object in space.
///
/// This can either be a sun, a planet, a moon or something else
/// floating in space. It can have any number of satellites as
/// children, which in turn can have other children again.
////////////////////////////////////////////////////////////////////
class Planet: public SpaceObject {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Constructor.
        ///
        /// This creates a new Planet.
        /// \param object           The geometry of this object
        ///                         with its textures.
        /// \param billboard        The billboard attached to this
        ///                         object.
        /// \param radius           The size of this object.
        /// \param rotationspeed    A factor indicating the speed of
        ///                         rotation around its parent.
        /// \param ownRotationspeed A factor indicating the speed of
        ///                         self rotation.
        ////////////////////////////////////////////////////////////
        Planet(Object* object, Billboard* billboard, float radius, float rotationSpeed, float ownRotationSpeed, float orbit,
               bool collidable = true, gloost::Vector3 const& rotationAxis = gloost::Vector3(0, 1, 0));

        virtual ~Planet();

        ////////////////////////////////////////////////////////////
        /// \brief Updates this object.
        ///
        /// This updates the position of the Planet
        ////////////////////////////////////////////////////////////
        virtual void update(double frameTime);

        ////////////////////////////////////////////////////////////
        /// \brief Draw this object.
        ///
        /// This draws the Planet and all of its children to
        /// the given SpaceScene.
        /// \param scene The scene to draw into.
        ////////////////////////////////////////////////////////////
        virtual void draw() const;

    private:
        Object* object_;
        CollisionSphere* collisionSphere_;

        Billboard* billboard_;

        float rotationSpeed_, ownRotationSpeed_;
        float orbit_;
        gloost::Vector3 rotationAxis_;

        int  life_;
        bool alive_;

        Emitter *explosion_;
};

# endif //PLANET_HPP

