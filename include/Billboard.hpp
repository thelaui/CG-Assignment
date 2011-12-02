////////////////////////////////////////////////////////////////////
/// \file
/// \brief A class which is capable of drawing a Billboard.
//
// CG-Beleg 2, by Felix Lauer (90404) & Simon Schneegans (90405)
////////////////////////////////////////////////////////////////////

# ifndef BILLBOARD_HPP
# define BILLBOARD_HPP

class Mesh;
class Texture;

////////////////////////////////////////////////////////////////////
/// \brief A class representing a Billboard.
///
/// This class allows to generate a Billboard which is always turned
/// to the camera position
////////////////////////////////////////////////////////////////////
class Billboard {
    public:
        ////////////////////////////////////////////////////////////
        /// \brief Constructor.
        ///
        /// This constructs a new Billboard.
        /// \param mesh    The mesh the Billboard will be attached to.
        /// \param texture The texture of the Billboard.
        /// \param scale   The scale of the Billboard.
        ////////////////////////////////////////////////////////////
        Billboard(Mesh* mesh, Texture* texture, float scale);

        ////////////////////////////////////////////////////////////
        /// \brief Draws the Billboard.
        ///
        /// This draws the Billboard and makes sure it faces
        /// the camera.
        /// \param scene The scene the Billboard is placed in.
        ////////////////////////////////////////////////////////////
        void draw();

        void drawInCameraSpace();

    private:
        Mesh* mesh_;
        Texture* texture_;
        float scale_;
};

# endif //BILLBOARD_HPP





