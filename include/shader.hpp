////////////////////////////////////////////////////////////////////
/// \file
/// \brief A namespace which can load shaders from files.
//
// CG-Beleg 2, by Felix Lauer (90404) & Simon Schneegans (90405)
////////////////////////////////////////////////////////////////////

# ifndef SHADER_HPP
# define SHADER_HPP

# include <string>

////////////////////////////////////////////////////////////////////
/// \brief A namespace for loading shaders.
///
/// This namespace can load shaders from files. They will be
/// compiled and on error an appropriate message will be printed.
////////////////////////////////////////////////////////////////////
namespace shader {

    ////////////////////////////////////////////////////////////////
    /// \brief Loads a shader from file.
    ///
    /// Takes a filename and a Shadertype and returns an id of the
    /// compiled shader.
    /// \param fileName    The file to load.
    /// \param shaderType  Either GL_VERTEX_SHADER,
    ///                    GL_FRAGMENT_SHADER or GL_GEOMETRIE_SHADER
    /// \throw std::string An error message.
    /// \return The ID of the compiled shader.
    ////////////////////////////////////////////////////////////////
    unsigned load(std::string const& fileName, unsigned shaderType) throw (std::string);
}

# endif //SHADER_HPP
