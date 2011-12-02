# include "include/Object.hpp"

Object::Object(Mesh* mesh, float atmoR, float atmoG, float atmoB,
               Texture* diffuse, Texture* specular,
               Texture* normal, Texture* emit):
    mesh_(mesh),
    diffuse_(diffuse),
    specular_(specular),
    normal_(normal),
    emit_(emit),
    atmoR_(atmoR),
    atmoG_(atmoG),
    atmoB_(atmoB) {}
