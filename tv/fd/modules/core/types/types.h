#ifndef MODULES_CORE_TYPES_TYPES_H_
#define MODULES_CORE_TYPES_TYPES_H_

#include <cstdint>

#include <iostream>

#include "core/serializer.h"

namespace internal
{

constexpr std::uint64_t kMaxNumStaticArrayElements = 100000;

template <typename T> class Type : Serializable
{
private:
    T data_;

public:
    Type() = default;
    Type(const T& other)
    {
        data_ = other;
    }

    size_t serializedSize() const override
    {
        return sizeof(T);
    }

    void serialize(uint8_t* serialized_data) const override
    {
        Serializer serializer(serialized_data);
        serializer.serializeStaticType(data_);
    }

    void deserialize(const uint8_t* const serialized_data) override
    {
        Deserializer deserializer(serialized_data);
        deserializer.deserializeToStaticType(data_);
    }


    operator T() const
    {
        return data_;
    }

    operator T()
    {
        return data_;
    }
};

template <typename T> class TypePtr : Serializable
{
private:
    T* data_{};
    std::uint64_t size_;

public:

    using value_type = std::remove_cv_t<T>;
    TypePtr() : data_(nullptr), size_(0U) {}

    TypePtr(const std::uint64_t num_elements) = delete;

    TypePtr(T* other) = delete;

    TypePtr& operator=(const TypePtr& other) = delete;

    size_t serializedSize() const override
    {
        return sizeof(T) * size_;
    }

    void serialize(uint8_t* serialized_data) const override
    {
        Serializer serializer(serialized_data);
        serializer.serializePointer(data_, size_);
    }

    void deserialize(const uint8_t* const serialized_data) override
    {
        std::cout << "Not implemented yet!" << std::endl;
        exit(-1);
        if(size_ == 0U)
        {
            // TODO: Read size from serialized_data
            // *this = create()
            // size_ = ...
            
        }
        Deserializer deserializer(serialized_data);
        deserializer.deserializePointer(data_, size_);
    }

    operator bool()
    {
        return data_ != nullptr;
    }

    operator T*()
    {
        return data_;
    }

    static TypePtr create(const std::uint64_t num_elements)
    {
        TypePtr tp;

        tp.data_ = new T[num_elements];
        tp.size_ = num_elements;

        return tp;
    }

    std::uint64_t size() const
    {
        return size_;
    }

    T* data() const
    {
        return data_;
    }

    T* begin() const
    {
        return data_;
    }

    T* end() const
    {
        return &(data_[size_]);
    }

};

template <typename T>
void free(const TypePtr<T>& other)
{
    delete[] other.data();
}

template <typename T, std::uint64_t N> class TypeArray : Serializable
{
private:
    T data_[N];
    static_assert(N <= kMaxNumStaticArrayElements, "Attempted to create static TypeArray with N > kMaxNumStaticArrayElements, which is not recommended!");

public:
    TypeArray() = default;

    std::uint64_t size() const
    {
        return N;
    }

    operator T*()
    {
        return data_;
    }

    size_t serializedSize() const override
    {
        return sizeof(T) * N + sizeof(uint64_t);
    }

    void serialize(uint8_t* serialized_data) const override
    {
        Serializer serializer(serialized_data);
        serializer.serializeArray<T, N>(data_);
    }

    void deserialize(const uint8_t* const serialized_data) override
    {
        Deserializer deserializer(serialized_data);
        deserializer.deserializeArray<T, N>(data_);
    }


    T* data() const
    {
        return data_;
    }
};


} // namespace internal

#ifdef PROD

using S8 = std::int8_t;
using S16 = std::int16_t;
using S32 = std::int32_t;
using S64 = std::int64_t;

using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;

using F32 = std::float_t;
using F64 = std::double_t;

using Bool = bool;

using S8Ptr = ::internal::TypePtr<std::int8_t>;
using S16Ptr = ::internal::TypePtr<std::int16_t>;
using S32Ptr = ::internal::TypePtr<std::int32_t>;
using S64Ptr = ::internal::TypePtr<std::int64_t>;

using U8Ptr = ::internal::TypePtr<std::uint8_t>;
using U16Ptr = ::internal::TypePtr<std::uint16_t>;
using U32Ptr = ::internal::TypePtr<std::uint32_t>;
using U64Ptr = ::internal::TypePtr<std::uint64_t>;

using F32Ptr = ::internal::TypePtr<std::float_t>;
using F64Ptr = ::internal::TypePtr<std::double_t>;


using S8Array = ::internal::TypeArray<std::int8_t>;
using S16Array = ::internal::TypeArray<std::int16_t>;
using S32Array = ::internal::TypeArray<std::int32_t>;
using S64Array = ::internal::TypeArray<std::int64_t>;

using U8Array = ::internal::TypeArray<std::uint8_t>;
using U16Array = ::internal::TypeArray<std::uint16_t>;
using U32Array = ::internal::TypeArray<std::uint32_t>;
using U64Array = ::internal::TypeArray<std::uint64_t>;

using F32Array = ::internal::TypeArray<std::float_t>;
using F64Array = ::internal::TypeArray<std::double_t>;



#else

using S8 = ::internal::Type<std::int8_t>;
using S16 = ::internal::Type<std::int16_t>;
using S32 = ::internal::Type<std::int32_t>;
using S64 = ::internal::Type<std::int64_t>;

using U8 = ::internal::Type<std::uint8_t>;
using U16 = ::internal::Type<std::uint16_t>;
using U32 = ::internal::Type<std::uint32_t>;
using U64 = ::internal::Type<std::uint64_t>;

using F32 = ::internal::Type<std::float_t>;
using F64 = ::internal::Type<std::double_t>;

using Bool = ::internal::Type<bool>;

using S8Ptr = ::internal::TypePtr<std::int8_t>;
using S16Ptr = ::internal::TypePtr<std::int16_t>;
using S32Ptr = ::internal::TypePtr<std::int32_t>;
using S64Ptr = ::internal::TypePtr<std::int64_t>;

using U8Ptr = ::internal::TypePtr<std::uint8_t>;
using U16Ptr = ::internal::TypePtr<std::uint16_t>;
using U32Ptr = ::internal::TypePtr<std::uint32_t>;
using U64Ptr = ::internal::TypePtr<std::uint64_t>;

using F32Ptr = ::internal::TypePtr<std::float_t>;
using F64Ptr = ::internal::TypePtr<std::double_t>;

using BoolPtr = ::internal::TypePtr<bool>;


/*using S8Array = ::internal::TypeArray<std::int8_t>;
using S16Array = ::internal::TypeArray<std::int16_t>;
using S32Array = ::internal::TypeArray<std::int32_t>;
using S64Array = ::internal::TypeArray<std::int64_t>;

using U8Array = ::internal::TypeArray<std::uint8_t>;
using U16Array = ::internal::TypeArray<std::uint16_t>;
using U32Array = ::internal::TypeArray<std::uint32_t>;
using U64Array = ::internal::TypeArray<std::uint64_t>;

using F32Array = ::internal::TypeArray<std::float_t>;
using F64Array = ::internal::TypeArray<std::double_t>;
*/

#endif


#endif
