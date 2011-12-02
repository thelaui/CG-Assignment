# include "include/Mesh.hpp"

# include <ObjLoader.h>
# include <Mesh.h>

Mesh::Mesh(std::string const& file):
    bufferIds_({0u}){

    // load the mesh
    gloost::ObjLoader loader(file);
    mesh_ = loader.getMesh();

    mesh_->takeReference();
    mesh_->generateNormals();
    mesh_->scaleToSize(1.0);
    mesh_->interleave();
    //mesh_->printMeshInfo();

    //create VAO which holds the state of our Vertex Attributes and VertexBufferObjects - a control structure
    glGenVertexArrays(1, &bufferIds_[0]);
    glBindVertexArray(bufferIds_[0]);

    //Create two VertexBufferObject and bind the first one to set its data
    glGenBuffers(2, &bufferIds_[1]);
    glBindBuffer(GL_ARRAY_BUFFER, bufferIds_[1]);

    //set the vertex data for the actual buffer; the second parameter is the size in bytes of all Vertices together
    //the third parameter is a pointer to the vertexdata
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_->getInterleavedAttributes().size(),
                 &mesh_->getInterleavedAttributes().front(), GL_STATIC_DRAW);

    //enables a VertexAttributeArray
    glEnableVertexAttribArray(0);


    //specifies where in the GL_ARRAY_BUFFER our data(the vertex position) is exactly
    glVertexAttribPointer(0,
                        GLOOST_MESH_NUM_COMPONENTS_VERTEX,
                        GL_FLOAT, GL_FALSE,
                        mesh_->getInterleavedInfo().interleavedPackageStride,//mesh_->getInterleavedInfo().interleavedVertexStride,
                        (GLvoid*)(mesh_->getInterleavedInfo().interleavedVertexStride));


    //enables a VertexAttributeArray
    glEnableVertexAttribArray(1);

    //specifies where in the GL_ARRAY_BUFFER our data(the vertex position) is exactly
    glVertexAttribPointer(1,
                        GLOOST_MESH_NUM_COMPONENTS_NORMAL,
                        GL_FLOAT, GL_FALSE,
                        mesh_->getInterleavedInfo().interleavedPackageStride,
                        (GLvoid*)(mesh_->getInterleavedInfo().interleavedNormalStride));

    //enables a VertexAttributeArray
    glEnableVertexAttribArray(2);

    //specifies where in the GL_ARRAY_BUFFER our data(the vertex position) is exactly
    glVertexAttribPointer(2,
                        GLOOST_MESH_NUM_COMPONENTS_TEXCOORD,
                        GL_FLOAT, GL_FALSE,
                        mesh_->getInterleavedInfo().interleavedPackageStride,
                        (GLvoid*)(mesh_->getInterleavedInfo().interleavedTexcoordStride));


    // the seceond VertexBufferObject ist bound
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIds_[2]);
    // its data are the indices of the vertices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(gloost::TriangleFace) * mesh_->getTriangles().size(),
               &mesh_->getTriangles().front(),
               GL_STATIC_DRAW);

    // unbind the VertexArray
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    // clean up
    glDeleteBuffers(2, &bufferIds_[1]);
    glDeleteVertexArrays(1, &bufferIds_[0]);
}

void Mesh::draw() {
    // bind the geometry and draw it
    glBindVertexArray(bufferIds_[0]);
    glDrawElements(GL_TRIANGLES, mesh_->getTriangles().size()*3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
