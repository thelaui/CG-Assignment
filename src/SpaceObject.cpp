# include "include/SpaceObject.hpp"

SpaceObject::SpaceObject(float radius):
    radius_(radius),
    position_(),
    parent_(NULL) {}

SpaceObject::~SpaceObject() {
    // clean up
    for(int i(0); i<satellites_.size(); ++i)
        delete satellites_[i];
}

void SpaceObject::sort(gloost::Vector3 const& position, std::map<float, SpaceObject const*>& sortMap) const{
    sortMap.insert(std::make_pair(std::abs((position - getTransform()).length2() - radius_*radius_), this));

    for(int i(0); i<satellites_.size(); ++i)
        satellites_[i]->sort(position, sortMap);
}

gloost::Vector3 const SpaceObject::getTransform() const {
    if (parent_)
        return parent_->getTransform() + position_;
    return position_;
}

void SpaceObject::addSatellite(SpaceObject* satellite) {
    satellite->parent_ = this;
    satellites_.push_back(satellite);
}
