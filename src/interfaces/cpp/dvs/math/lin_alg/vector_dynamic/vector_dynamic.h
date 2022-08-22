#ifndef DVS_MATH_LIN_ALG_VECTOR_DYNAMIC_VECTOR_DYNAMIC_H_
#define DVS_MATH_LIN_ALG_VECTOR_DYNAMIC_VECTOR_DYNAMIC_H_

#include <cmath>
#include <cstring>
#include <type_traits>
#include <utility>

#include "dvs/math/lin_alg/vector_dynamic/class_def/vector_dynamic.h"
#include "dvs/math/misc/math_macros.h"

namespace dvs
{
template <typename T> Vector<T>::Vector() : data_(nullptr), size_(0), is_allocated_(false) {}

template <typename T> Vector<T>::Vector(const size_t vector_length) : is_allocated_(true)
{
    size_ = vector_length;

    DATA_ALLOCATION(data_, vector_length, T, "Vector");
}

template <typename T> Vector<T>::Vector(const Vector<T>& v) : is_allocated_(true)
{
    size_ = v.size();

    DATA_ALLOCATION(data_, v.size(), T, "Vector");

    for (size_t k = 0; k < v.size(); k++)
    {
        data_[k] = v(k);
    }
}

template <typename T> Vector<T>::Vector(Vector<T>&& v)
{
    DVS_ASSERT(v.isAllocated()) << "Input vector not allocated!";
    data_ = v.data();
    size_ = v.size();
    is_allocated_ = true;

    v.setInternalData(nullptr, 0);
}

template <typename T> Vector<T>& Vector<T>::operator=(Vector<T>&& v)
{
    if (this != &v)
    {
        DVS_ASSERT(v.isAllocated()) << "Input vector not allocated before assignment!";

        if (is_allocated_)
        {
            delete[] data_;
        }

        size_ = v.size();
        is_allocated_ = true;

        data_ = v.data();

        v.setInternalData(nullptr, 0);
    }

    return *this;
}

template <typename T> Vector<T>::~Vector()
{
    if (is_allocated_)
    {
        delete[] data_;
        is_allocated_ = false;
    }
}

template <typename T> void Vector<T>::fillBufferWithData(uint8_t* const buffer) const
{
    const uint8_t* const internal_ptr = reinterpret_cast<uint8_t*>(data_);
    const size_t num_bytes = size_ * sizeof(T);

    std::memcpy(buffer, internal_ptr, num_bytes);
}

template <typename T> Vector<T>& Vector<T>::operator=(const Vector<T>& v)
{
    DVS_ASSERT(v.isAllocated()) << "Input vector not allocated before assignment!";

    if (this != &v)
    {
        if (is_allocated_)
        {
            if (v.size() != size_)
            {
                delete[] data_;
                DATA_ALLOCATION(data_, v.size(), T, "Vector");
            }
        }
        else
        {
            DATA_ALLOCATION(data_, v.size(), T, "Vector");
        }
        size_ = v.size();

        is_allocated_ = true;

        for (size_t k = 0; k < v.size(); k++)
        {
            data_[k] = v(k);
        }
    }

    return *this;
}

template <typename T> template <typename Y> Vector<T>::Vector(const Vector<Y>& v) : is_allocated_(true)
{
    size_ = v.size();

    DATA_ALLOCATION(data_, v.size(), T, "Vector");

    for (size_t k = 0; k < v.size(); k++)
    {
        data_[k] = static_cast<T>(v(k));
    }
}

template <typename T> template <typename Y> Vector<T>& Vector<T>::operator=(const Vector<Y>& v)
{
    if (is_allocated_)
    {
        if (v.size() != size_)
        {
            delete[] data_;
            DATA_ALLOCATION(data_, v.size(), T, "Vector");
        }
    }
    else
    {
        DATA_ALLOCATION(data_, v.size(), T, "Vector");
    }
    size_ = v.size();
    is_allocated_ = true;

    for (size_t k = 0; k < v.size(); k++)
    {
        data_[k] = static_cast<T>(v(k));
    }

    return *this;
}

template <typename T> Vector<T>::Vector(const std::initializer_list<T>& il)
{
    DVS_ASSERT(il.size() > 0) << "Tried to initialize with empty vector!";

    DATA_ALLOCATION(data_, il.size(), T, "Vector");
    is_allocated_ = true;

    size_ = il.size();

    size_t idx = 0;
    for (auto list_element : il)
    {
        data_[idx] = list_element;
        idx++;
    }
}

template <typename T> Vector<T>::Vector(const std::vector<T>& v)
{
    if (v.size() == 0)
    {
        size_ = 0;
        is_allocated_ = false;
    }
    else
    {
        size_ = v.size();

        DATA_ALLOCATION(data_, v.size(), T, "Vector");
        is_allocated_ = true;

        size_t idx = 0;
        for (auto vec_element : v)
        {
            data_[idx] = vec_element;
            idx++;
        }
    }
}

template <typename T> T& Vector<T>::operator()(const size_t idx)
{
    assert(idx < size_ && is_allocated_);
    return data_[idx];
}

template <typename T> const T& Vector<T>::operator()(const size_t idx) const
{
    assert(idx < size_ && is_allocated_);
    return data_[idx];
}

template <typename T> size_t Vector<T>::size() const
{
    return size_;
}

template <typename T> size_t Vector<T>::numElements() const
{
    return size_;
}

template <typename T> size_t Vector<T>::numBytes() const
{
    return size_ * sizeof(T);
}

template <typename T> bool Vector<T>::isAllocated() const
{
    return is_allocated_;
}

template <typename T> void Vector<T>::fill(const T& val)
{
    assert(is_allocated_ && "Tried to fill unallocated vector!");
    for (size_t k = 0; k < size_; k++)
    {
        data_[k] = val;
    }
}

template <typename T> void Vector<T>::resize(const size_t new_size)
{
    if ((new_size != size_) && (new_size != 0))
    {
        if (is_allocated_)
        {
            delete[] data_;
        }

        if (new_size == 0)
        {
            is_allocated_ = false;
            size_ = 0;
        }
        else
        {
            is_allocated_ = true;
            DATA_ALLOCATION(data_, new_size, T, "Vector");
            size_ = new_size;
        }
    }
}

template <typename T> size_t Vector<T>::endIndex() const
{
    assert(size_ != 0U);
    return size_ - 1;
}

template <typename T> T* Vector<T>::data() const
{
    return data_;
}

template <typename T> void Vector<T>::setInternalData(T* const input_ptr, const size_t num_elements)
{
    if (input_ptr == nullptr)
    {
        is_allocated_ = false;
    }
    else
    {
        is_allocated_ = true;
    }

    data_ = input_ptr;
    size_ = num_elements;
}

template <typename T> T* Vector<T>::begin() const
{
    return &data_[0];
}

template <typename T> T* Vector<T>::end() const
{
    return &data_[size_];
}

template <typename T> T Vector<T>::min() const
{
    ASSERT_VEC_VALID_INTERNAL();
    T min_val = data_[0];

    for (size_t k = 1; k < size_; k++)
    {
        min_val = std::min(data_[k], min_val);
    }

    return min_val;
}

template <typename T> T Vector<T>::max() const
{
    ASSERT_VEC_VALID_INTERNAL();
    T max_val = data_[0];

    for (size_t k = 1; k < size_; k++)
    {
        max_val = std::max(data_[k], max_val);
    }

    return max_val;
}

template <typename T> T Vector<T>::sum() const
{
    ASSERT_VEC_VALID_INTERNAL();
    T s = 0.0;

    for (size_t k = 0; k < size_; k++)
    {
        s = s + data_[k];
    }

    return s;
}

// Non class methods

template <typename T> std::ostream& operator<<(std::ostream& os, const Vector<T>& v)
{
    std::string s = "[ ";
    for (size_t k = 0; k < v.size(); k++)
    {
        s = s + std::to_string(v(k));
        if (k != v.size() - 1)
        {
            s = s + ", ";
        }
    }
    s = s + " ]";
    os << s;

    return os;
}

template <typename T> T operator*(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    T d = 0.0;
    for (size_t k = 0; k < v0.size(); k++)
    {
        d = d + v0(k) * v1(k);
    }
    return d;
}

template <typename T> Vector<T> operator%(const Vector<T>& v, const int i)
{
    Vector<T> vout(v.size());

    for (size_t k = 0; k < v.size(); k++)
    {
        vout(k) = static_cast<T>(static_cast<int>(v(k)) % i);
    }

    return vout;
}

template <typename T> Vector<T> operator*(const T f, const Vector<T>& v)
{
    Vector<T> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = f * v(k);
    }
    return v_res;
}

template <typename T> Vector<T> operator*(const Vector<T>& v, const T f)
{
    Vector<T> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = f * v(k);
    }
    return v_res;
}

template <typename T> Vector<T> operator^(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    Vector<T> v_res(v0.size());
    for (size_t k = 0; k < v0.size(); k++)
    {
        v_res(k) = v0(k) * v1(k);
    }
    return v_res;
}

template <typename T> Vector<T> operator/(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    Vector<T> v_res(v0.size());
    for (size_t k = 0; k < v0.size(); k++)
    {
        v_res(k) = v0(k) / v1(k);
    }
    return v_res;
}

template <typename T> Vector<T> operator/(const Vector<T>& v, const T f)
{
    Vector<T> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) / f;
    }
    return v_res;
}

template <typename T> Vector<T> operator/(const T f, const Vector<T>& v)
{
    Vector<T> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = f / v(k);
    }
    return v_res;
}

template <typename T> Vector<T> operator+(const Vector<T>& v, const T f)
{
    Vector<T> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) + f;
    }
    return v_res;
}

template <typename T> Vector<T> operator+(const T f, const Vector<T>& v)
{
    Vector<T> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) + f;
    }
    return v_res;
}

template <typename T> Vector<T> operator-(const Vector<T>& v, const T f)
{
    Vector<T> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) - f;
    }
    return v_res;
}

template <typename T> Vector<T> operator-(const T f, const Vector<T>& v)
{
    Vector<T> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = f - v(k);
    }
    return v_res;
}

template <typename T> Vector<T> operator-(const Vector<T>& v)
{
    Vector<T> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = -v(k);
    }
    return v_res;
}

template <typename T> Vector<T> operator+(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    Vector<T> v_res(v0.size());
    for (size_t k = 0; k < v0.size(); k++)
    {
        v_res(k) = v0(k) + v1(k);
    }
    return v_res;
}

template <typename T> Vector<T> operator-(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    Vector<T> v_res(v0.size());
    for (size_t k = 0; k < v0.size(); k++)
    {
        v_res(k) = v0(k) - v1(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator==(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    Vector<bool> v_res(v0.size());
    for (size_t k = 0; k < v0.size(); k++)
    {
        v_res(k) = v0(k) == v1(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator==(const Vector<T>& v, const T s)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) == s;
    }
    return v_res;
}

template <typename T> Vector<bool> operator==(const T s, const Vector<T>& v)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) == s;
    }
    return v_res;
}

template <typename T> Vector<bool> operator!=(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    Vector<bool> v_res(v0.size());
    for (size_t k = 0; k < v0.size(); k++)
    {
        v_res(k) = v0(k) != v1(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator!=(const Vector<T>& v, const T s)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) != s;
    }
    return v_res;
}

template <typename T> Vector<bool> operator!=(const T s, const Vector<T>& v)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) != s;
    }
    return v_res;
}

template <typename T> Vector<bool> operator<(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    Vector<bool> v_res(v0.size());
    for (size_t k = 0; k < v0.size(); k++)
    {
        v_res(k) = v0(k) < v1(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator<(const Vector<T>& v, const T s)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) < s;
    }
    return v_res;
}

template <typename T> Vector<bool> operator<(const T s, const Vector<T>& v)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = s < v(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator>(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    Vector<bool> v_res(v0.size());
    for (size_t k = 0; k < v0.size(); k++)
    {
        v_res(k) = v0(k) > v1(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator>(const Vector<T>& v, const T s)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) > s;
    }
    return v_res;
}

template <typename T> Vector<bool> operator>(const T s, const Vector<T>& v)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = s > v(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator<=(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    Vector<bool> v_res(v0.size());
    for (size_t k = 0; k < v0.size(); k++)
    {
        v_res(k) = v0(k) <= v1(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator<=(const Vector<T>& v, const T s)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) <= s;
    }
    return v_res;
}

template <typename T> Vector<bool> operator<=(const T s, const Vector<T>& v)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = s <= v(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator>=(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    Vector<bool> v_res(v0.size());
    for (size_t k = 0; k < v0.size(); k++)
    {
        v_res(k) = v0(k) >= v1(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator>=(const Vector<T>& v, const T s)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) >= s;
    }
    return v_res;
}

template <typename T> Vector<bool> operator>=(const T s, const Vector<T>& v)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = s >= v(k);
    }
    return v_res;
}

template <typename T> Vector<T> operator&(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    Vector<T> v_res(v0.size());
    for (size_t k = 0; k < v0.size(); k++)
    {
        v_res(k) = v0(k) & v1(k);
    }
    return v_res;
}

template <typename T> Vector<T> operator&(const Vector<T>& v, const T s)
{
    Vector<T> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) & s;
    }
    return v_res;
}

template <typename T> Vector<T> operator&(const T s, const Vector<T>& v)
{
    Vector<T> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = s & v(k);
    }
    return v_res;
}

template <typename T> Vector<T> operator|(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    Vector<T> v_res(v0.size());
    for (size_t k = 0; k < v0.size(); k++)
    {
        v_res(k) = v0(k) | v1(k);
    }
    return v_res;
}

template <typename T> Vector<T> operator|(const Vector<T>& v, const T s)
{
    Vector<T> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) | s;
    }
    return v_res;
}

template <typename T> Vector<T> operator|(const T s, const Vector<T>& v)
{
    Vector<T> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = s | v(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator&&(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    Vector<bool> v_res(v0.size());
    for (size_t k = 0; k < v0.size(); k++)
    {
        v_res(k) = v0(k) && v1(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator&&(const Vector<T>& v, const T s)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) && s;
    }
    return v_res;
}

template <typename T> Vector<bool> operator&&(const T s, const Vector<T>& v)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = s && v(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator||(const Vector<T>& v0, const Vector<T>& v1)
{
    assert(v0.size() == v1.size());
    Vector<bool> v_res(v0.size());
    for (size_t k = 0; k < v0.size(); k++)
    {
        v_res(k) = v0(k) || v1(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator||(const Vector<T>& v, const T s)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = v(k) || s;
    }
    return v_res;
}

template <typename T> Vector<bool> operator||(const T s, const Vector<T>& v)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = s || v(k);
    }
    return v_res;
}

template <typename T> Vector<bool> operator!(const Vector<T>& v)
{
    Vector<bool> v_res(v.size());
    for (size_t k = 0; k < v.size(); k++)
    {
        v_res(k) = !v(k);
    }
    return v_res;
}

template <typename Y> void fillWithPtr(Vector<Y>& v, const void* const ptr, const size_t vector_length)
{
    v.is_allocated_ = true;
    v.size_ = vector_length;
    DATA_ALLOCATION(v.data_, vector_length, Y, "Vector");

    std::memcpy(v.data_, ptr, sizeof(Y) * vector_length);
}

}  // namespace dvs

#endif // DVS_MATH_LIN_ALG_VECTOR_DYNAMIC_VECTOR_DYNAMIC_H_
