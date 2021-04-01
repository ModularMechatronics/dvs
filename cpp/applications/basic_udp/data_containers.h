#ifndef DATA_CONTAINERS_H_
#define DATA_CONTAINERS_H_

template <typename T>
class Vector
{
private:
    size_t num_elements_;
    T* data_;

public:
    Vector() : num_elements_(0), data_(nullptr)
    {

    }

    Vector(const size_t num_elements) : num_elements_(num_elements)
    {
        data_ = new T[num_elements_];
    }

    ~Vector()
    {
        num_elements_ = 0;
        delete[] data_;
    }

    size_t size() const
    {
        return num_elements_;
    }

    bool isAllocated() const
    {
        return num_elements_ > 0;
    }
};

#endif
