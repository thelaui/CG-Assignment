# include "include/Emitter.hpp"

# include "include/SpaceScene.hpp"
# include "include/Texture.hpp"
# include "include/window.hpp"
# include "include/Particle.hpp"

# include <ObjLoader.h>
# include <Mesh.h>

Emitter::Emitter(ParticleTemplate const& settings,
                 gloost::Vector3 const& position,
                 gloost::Vector3 const& direction):

    SpaceObject(0.f),
    position_(position),
    lastFramePosition_(position),
    direction_(direction),
    lastFrameDirection_(direction),
    settings_(settings),
    bufferIds_({0u}),
    spawnTimer_(0),
    spawnDelay_(0) {

    //create VAO which holds the state of our Vertex Attributes and VertexBufferObjects - a control structure
    glGenVertexArrays(1, &bufferIds_[0]);
    glBindVertexArray(bufferIds_[0]);

    //Create two VertexBufferObject and bind the first one to set its data
    glGenBuffers(1, &bufferIds_[1]);
    glBindBuffer(GL_ARRAY_BUFFER, bufferIds_[1]);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)(3*sizeof(float)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)(4*sizeof(float)));

    // unbind the VertexArray
    glBindVertexArray(0);
}

Emitter::~Emitter() {
    // clean up
    glDeleteBuffers(1, &bufferIds_[1]);
    glDeleteVertexArrays(1, &bufferIds_[0]);
}

void Emitter::update(double frameTime) {

    for(std::list<Particle*>::iterator it = particles_.begin(); it != particles_.end(); ++it) {
        if ((*it)->isDead()) {
            delete *it;
            it = particles_.erase(it);
            --it;
        }
        else (*it)->update(frameTime);
    }

    spawnTimer_ -= frameTime;

    if (spawnTimer_ <= 0) {
        int spawnAmount(-spawnTimer_/(spawnDelay_)+1);

        gloost::Vector3 posStep(position_ - lastFramePosition_);
        gloost::Vector3 dirStep(direction_ - lastFrameDirection_);

        for (int i(0); i<spawnAmount; ++i) {
            Particle* newPart = new Particle(settings_, position_ - 1.0*i/spawnAmount*posStep, direction_ - 1.0*i/spawnAmount*dirStep, settings_.colliding);
            newPart->update(frameTime*i/spawnAmount);
            particles_.push_back(newPart);
        }

        spawnTimer_ = 1.0/window::getParticleAmount()/settings_.rate.val();
        spawnDelay_ = 1.0/window::getParticleAmount()/settings_.rate.val();
    }

    settings_.rate.update(frameTime);
    if (spawnTimer_ > 1.0/window::getParticleAmount()/settings_.rate.val()) {
        spawnTimer_ = 1.0/window::getParticleAmount()/settings_.rate.val();
        spawnDelay_= 1.0/window::getParticleAmount()/settings_.rate.val();
    }

    lastFramePosition_ = position_;
    lastFrameDirection_ = direction_;
}

void Emitter::draw() const {
    SpaceScene* scene(SpaceScene::pointer());

    glUniform1i(scene->getIsParticleUL(), true);

    std::vector<float> vertices(8*particles_.size());
    int index(0);

    for(std::list<Particle*>::iterator it = particles_.begin(); it != particles_.end(); ++it) {
        gloost::Vector3 pos((*it)->getPosition());
        gloost::Vector3 col((*it)->getColor());

        vertices[index*8+0] = pos[0];
        vertices[index*8+1] = pos[1];
        vertices[index*8+2] = pos[2];
        vertices[index*8+3] = (*it)->getSize();
        vertices[index*8+4] = col[0];
        vertices[index*8+5] = col[1];
        vertices[index*8+6] = col[2];
        vertices[index*8+7] = (*it)->getAlpha();
        ++index;
    }

    glBindBuffer(GL_ARRAY_BUFFER, bufferIds_[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &(*vertices.begin()), GL_DYNAMIC_DRAW);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    settings_.texture->bind(0);

    glBindVertexArray(bufferIds_[0]);
    glDrawArrays(GL_POINTS, 0, particles_.size());
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glUniform1i(scene->getIsParticleUL(), false);
}

void Emitter::setPosition(gloost::Vector3 const& position) {
    position_ = position;
}

void Emitter::setDirection(gloost::Vector3 const& direction) {
    direction_ = direction;
}

void Emitter::setRate(double rate, double time) {
    settings_.rate.resetTarget(rate, time);
}
