# ifndef KNOWN_OBJECT_HPP
# define KNOWN_OBJECT_HPP

# include <vector>

template <typename T>

class KnownObject {
    public:
        KnownObject() {
            existingObjects_.push_back(reinterpret_cast<T*>(this));
        }

        static T* pointer(int index = existingObjects_.size()-1) {
            if (index >= 0 && index < existingObjects_.size())
                return existingObjects_[index];
            return NULL;
        }

    private:
        static std::vector<T*> existingObjects_;

};


template <typename T>
std::vector<T*> KnownObject<T>::existingObjects_;


# endif //KNOWN_OBJECT_HPP
