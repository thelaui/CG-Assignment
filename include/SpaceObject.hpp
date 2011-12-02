////////////////////////////////////////////////////////////////////
/// \file
/// \brief A SpaceObject can have any number of SpaceObjects
///        floating in circles around.
//
// CG-Beleg 2, by Felix Lauer (90404) & Simon Schneegans (90405)
////////////////////////////////////////////////////////////////////

# ifndef SPACEOBJECT_HPP
# define SPACEOBJECT_HPP

# include "include/Mesh.hpp"
# include "include/Object.hpp"

# include <gloostMath.h>

# include <vector>
# include <map>

class SpaceScene;
class Billboard;

////////////////////////////////////////////////////////////////////
/// \brief A class an object in space.
///
/// This can either be a sun, a planet, a moon or something else
/// floating in space. It can have any number of satellites as
/// children, which in turn can have other children again.
////////////////////////////////////////////////////////////////////
class SpaceObject {
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Constructor.
        ///
        /// This creates a new SpaceObject.
        ////////////////////////////////////////////////////////////
        SpaceObject(float radius);

        ////////////////////////////////////////////////////////////
        /// \brief Destructor.
        ///
        /// Deletes this SpaceObject.
        ////////////////////////////////////////////////////////////
        virtual ~SpaceObject();

        ////////////////////////////////////////////////////////////
        /// \brief Updates this object.
        ///
        /// This updates the position of the SpaceObject
        ////////////////////////////////////////////////////////////
        virtual void update(double frameTime) = 0;

        ////////////////////////////////////////////////////////////
        /// \brief Draw this object.
        ///
        /// This draws the SpaceObject to the given SpaceScene.
        /// \param scene The scene to draw into.
        ////////////////////////////////////////////////////////////
        virtual void draw() const = 0;

        ////////////////////////////////////////////////////////////
        /// \brief Applies recursive sorting.
        ///
        /// The SpaceObject will insert itself and its distance to
        /// the position in the map.
        /// \param position The position the SpaceObject computes
        ///                 its distance to.
        /// \param sortMap  A map of SpaceObjects sorted by their
        ///                 distance to position.
        ////////////////////////////////////////////////////////////
        void sort(gloost::Vector3 const& position, std::map<float, SpaceObject const*>& sortMap) const;

        ////////////////////////////////////////////////////////////
        /// \brief Returns the transformation of the SpaceObject.
        ///
        /// The transformation will be computed recursively
        /// depending on the SpaceObect's parent(s).
        /// \return Location in world coords.
        ////////////////////////////////////////////////////////////
        gloost::Vector3 const getTransform() const;

        ////////////////////////////////////////////////////////////
        /// \brief Adds a new child object.
        ///
        /// The added child will rotate around this SpaceObject.
        /// \param satellite The new child Planet.
        /// \param distance  The orbiting distance to the object.
        ////////////////////////////////////////////////////////////
        void addSatellite(SpaceObject* satellite);

    protected:
        float radius_;
        mutable gloost::Vector3 position_;

        SpaceObject* parent_;
        std::vector<SpaceObject*> satellites_;

};

# endif //SPACEOBJECT_HPP
