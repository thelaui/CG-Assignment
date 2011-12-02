////////////////////////////////////////////////////////////////////
/// \file
/// \brief A struct which is storing Object data.
//
// CG-Beleg 2, by Felix Lauer (90404) & Simon Schneegans (90405)
////////////////////////////////////////////////////////////////////

# ifndef OBJECT_HPP
# define OBJECT_HPP

# include <string>

class Mesh;
class Texture;

////////////////////////////////////////////////////////////////////
/// \brief A struct representing an Object.
///
/// This struct stores information about an Object which is ment to
/// be attached to a SpaceObject
////////////////////////////////////////////////////////////////////
struct Object {

    ////////////////////////////////////////////////////////////////
    /// \brief Constructor.
    ///
    /// This constructs a new Object.
    /// \param mesh     The mesh the Object stores.
    /// \param atmoR    The amount of red in the atmospherical
    ///                 glow.
    /// \param atmoG    The amount of green in the atmospherical
    ///                 glow.
    /// \param atmoB    The amount of blue in the atmospherical
    ///                 glow.
    /// \param diffuse  The texture responsible for diffuse
    ///                 reflection.
    /// \param specular The texture responsible for specular
    ///                 reflection.
    /// \param normal   The texture responsible for bump mapping.
    /// \param emit     The texture responsible for unlit Object
    ///                 spots.
    ////////////////////////////////////////////////////////////////
    Object(Mesh* mesh, float atmoR, float atmoG, float atmoB,
           Texture* diffuse = NULL,
           Texture* specular = NULL,
           Texture* normal = NULL,
           Texture* emit = NULL);

    Mesh* mesh_;
    Texture* diffuse_;
    Texture* specular_;
    Texture* normal_;
    Texture* emit_;

    float atmoR_, atmoG_, atmoB_;
};

# endif //OBJECT_HPP




