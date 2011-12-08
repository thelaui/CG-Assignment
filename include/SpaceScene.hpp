////////////////////////////////////////////////////////////////////
/// \file
/// \brief A SpaceScene manages all SpaceObjects and the
///        shaders.
//
// CG-Beleg 2, by Felix Lauer (90404) & Simon Schneegans (90405)
////////////////////////////////////////////////////////////////////

# ifndef SPACESCENE_HPP
# define SPACESCENE_HPP

# include "include/Mesh.hpp"

# include "include/KnownObject.hpp"

# include <Matrix.h>
# include <MatrixStack.h>

class SpaceObject;

namespace gloost {
    class Mesh;
}

////////////////////////////////////////////////////////////////////
/// \brief A class representing a set of SpaceObjects.
///
/// It encapsulates drawing the objects and copes with the shaders.
////////////////////////////////////////////////////////////////////
class SpaceScene : public KnownObject<SpaceScene>{
    public:

        ////////////////////////////////////////////////////////////
        /// \brief Constructor.
        ///
        /// This constructs a new SpaceScene.
        ////////////////////////////////////////////////////////////
        SpaceScene();

        ////////////////////////////////////////////////////////////
        /// \brief Destructor.
        ///
        /// Cleans up.
        ////////////////////////////////////////////////////////////
        ~SpaceScene();

        ////////////////////////////////////////////////////////////
        /// \brief Updates the entire scene.
        ////////////////////////////////////////////////////////////
        void update(double frameTime);

        ////////////////////////////////////////////////////////////
        /// \brief Draws the entire Scene.
        ///
        /// This will draw all SpaceObjects of this scene.
        ////////////////////////////////////////////////////////////
        void draw();

        ////////////////////////////////////////////////////////////
        /// \brief Uploads the top transformation of the matrix
        /// stack to the GPU.
        ////////////////////////////////////////////////////////////
        void uploadTransform();

        ////////////////////////////////////////////////////////////
        /// \brief Get the projection matrix.
        /// \return The projection matrix.
        ////////////////////////////////////////////////////////////
        gloost::Matrix* getProjectionMatrix();

        ////////////////////////////////////////////////////////////
        /// \brief Get the matrix stack.
        /// \return The matrix stack of the scene.
        ////////////////////////////////////////////////////////////
        gloost::MatrixStack* getMatrixStack();

        ////////////////////////////////////////////////////////////
        /// \brief Get the shaders.
        /// \return A pointer to the shaders of this scene.
        ////////////////////////////////////////////////////////////
        unsigned const* getShaderIds() const;

        ////////////////////////////////////////////////////////////
        /// \brief Get uniform location of the projection matrix.
        /// \return The uniform location.
        ////////////////////////////////////////////////////////////
        unsigned getProjectionMatrixUL() const;

        ////////////////////////////////////////////////////////////
        /// \brief Get uniform location of the atmospheric color.
        /// \return The uniform location.
        ////////////////////////////////////////////////////////////
        unsigned getAtmoColorUL() const;

        ////////////////////////////////////////////////////////////
        /// \brief Get uniform location of the isBillboard uniform.
        /// \return The uniform location.
        ////////////////////////////////////////////////////////////
        unsigned getIsBillboardUL() const;

        ////////////////////////////////////////////////////////////
        /// \brief Get uniform location of the isParticleuniform.
        /// \return The uniform location.
        ////////////////////////////////////////////////////////////
        unsigned getIsParticleUL() const;

    private:
        void setupShaders() throw (std::string);
        void setupSpace();

        unsigned projectionMatrixUniformLocation_;
        unsigned modelMatrixUniformLocation_;
        unsigned viewMatrixUniformLocation_;
        unsigned normalMatrixUniformLocation_;
        unsigned atmoColorUniformLocation_;
        unsigned isBillboardUniformLocation_;
        unsigned isParticleUniformLocation_;

        unsigned shaderIds_[];

        gloost::Matrix      projectionMatrix_;
        gloost::MatrixStack modelMatrixStack_;

        SpaceObject* universe_;
};

# endif //SPACESCENE_HPP


