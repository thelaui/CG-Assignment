////////////////////////////////////////////////////////////////////
/// \file
/// \brief
//
// CG-Beleg 2, by Felix Lauer (90404) & Simon Schneegans (90405)
////////////////////////////////////////////////////////////////////

# ifndef COMET_HPP
# define COMET_HPP

# include "include/Planet.hpp"

class CollisionSphere;
class Emitter;

////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////
class Comet: public Planet {
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
        Comet(Object* object, Billboard* billboard, float radius, float rotationSpeed,
              float ownRotationSpeed, float orbit, gloost::Vector3 const& color, gloost::Vector3 const& rotationAxis);

        virtual ~Comet();

        ////////////////////////////////////////////////////////////
        /// \brief Updates this object.
        ///
        /// This updates the position of the Comet
        ////////////////////////////////////////////////////////////
        void update(double frameTime);

        ////////////////////////////////////////////////////////////
        /// \brief Draw this object.
        ///
        /// This draws the Comet the given SpaceScene.
        /// \param scene The scene to draw into.
        ////////////////////////////////////////////////////////////
        void draw() const;

    private:
        Emitter *trail_;
};

# endif //PLANET_HPP


