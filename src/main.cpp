////////////////////////////////////////////////////////////////////
/// \file
/// \brief The main.cpp file.
//
// CG-Beleg 1, by Felix Lauer (90404) & Simon Schneegans (90405)
////////////////////////////////////////////////////////////////////

# include "include/window.hpp"
# include "include/SpaceScene.hpp"

# include <IL/il.h>

int main(int argc, char* argv[]) {

    ilInit();

    try {
        window::init(1440, 900, argc, argv);
    }
    catch (std::string& error) {
        std::cout << "Failed to open window: " << error << std::endl;
    }

    SpaceScene scene;
    window::draw(&scene);

    return 0;
}
