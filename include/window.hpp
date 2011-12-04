////////////////////////////////////////////////////////////////////
/// \file
/// \brief Declaration of a namespace capable of opening a
///        window with an OpenGL context.
//
// CG-Beleg 2, by Felix Lauer (90404) & Simon Schneegans (90405)
////////////////////////////////////////////////////////////////////

# ifndef WINDOW_HPP
# define WINDOW_HPP

# include <gloostMath.h>

class SpaceScene;

////////////////////////////////////////////////////////////////////
/// \brief A window namespace.
///
/// The methods of this namespace can open a window and display a
/// SpaceScene on it.
////////////////////////////////////////////////////////////////////
namespace window {

    ////////////////////////////////////////////////////////////////
    /// \brief Initialize the window.
    ///
    /// This has to be called before a window can be opened.
    /// \param width  The width of the window.
    /// \param height The height of the window.
    /// \param argc   The count of command line arguments.
    /// \param argv   Command line arguments.
    /// \throw std::string An error message.
    ////////////////////////////////////////////////////////////////
    void init(int width, int height, int argc, char* argv[]) throw (std::string);

    ////////////////////////////////////////////////////////////////
    /// \brief Draw a scene.
    ///
    /// This method launches the main loop of the previously
    /// initialized window.
    /// \param scene The SpaceScene to be drawn.
    ////////////////////////////////////////////////////////////////
    void draw(SpaceScene* scene);

    ////////////////////////////////////////////////////////////////
    /// \brief Ǵet the width of the window.
    /// \return The width of the window.
    ////////////////////////////////////////////////////////////////
    int getWidth();

    ////////////////////////////////////////////////////////////////
    /// \brief Ǵet the height of the window.
    /// \return The height of the window.
    ////////////////////////////////////////////////////////////////
    int getHeight();

    double getParticleAmount();

    ////////////////////////////////////////////////////////////////
    /// \brief The window's camera.
    ///
    /// A camera is represented by its position and rotation.
    /// \return The camera of this window.
    ////////////////////////////////////////////////////////////////
    gloost::Matrix const& getCameraTransform();
}

# endif //WINDOW_HPP

