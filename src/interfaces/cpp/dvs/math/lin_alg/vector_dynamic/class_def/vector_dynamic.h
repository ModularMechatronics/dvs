#ifndef DVS_MATH_LIN_ALG_VECTOR_DYNAMIC_CLASS_DEF_VECTOR_DYNAMIC_H_
#define DVS_MATH_LIN_ALG_VECTOR_DYNAMIC_CLASS_DEF_VECTOR_DYNAMIC_H_

#include <iterator>

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> class Vector
{
protected:
    T* data_;
    size_t size_;
    bool is_allocated_;

public:
    Vector();
    explicit Vector(const size_t vector_length);
    Vector(const Vector<T>& v);
    Vector(Vector<T>&& v);
    template <typename Y> Vector(const Vector<Y>& v);

    template <typename Y> friend void fillWithPtr(Vector<Y>& v, const void* const ptr, const size_t vector_length);

    Vector(const std::initializer_list<T>& il);
    Vector(const std::vector<T>& v);

    ~Vector();

    void fillBufferWithData(uint8_t* const buffer) const;

    Vector<T>& operator=(const Vector<T>& v);
    Vector<T>& operator=(Vector<T>&& v);
    T& operator()(const size_t idx);
    const T& operator()(const size_t idx) const;

    size_t size() const;
    size_t numElements() const;
    size_t numBytes() const;
    bool isAllocated() const;
    void fill(const T& val);
    void resize(const size_t new_size);
    size_t endIndex() const;
    T* data() const;

    void setInternalData(T* const input_ptr, const size_t num_elements);

    T* begin() const;
    T* end() const;
    template <typename Y> Vector<T>& operator=(const Vector<Y>& rhs);

    T max() const;
    T min() const;
    T sum() const;
};

}  // namespace dvs

#endif // DVS_MATH_LIN_ALG_VECTOR_DYNAMIC_CLASS_DEF_VECTOR_DYNAMIC_H_
