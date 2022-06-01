#ifndef SERIALIZER_H_
#define SERIALIZER_H_

// TODO: Differentiate with dynamic types that has variable size, and include size
// TODO: Global logging of signals, which can then be plotted, or saved to file

/*
data_logger::logSignal(SIGNAL_SLOT0, x);
data_logger::logSignal("signal_name", x);

*/

constexpr size_t kMaxStringLen = 254;

class Serializer
{
private:
    uint8_t* const data_;
    size_t idx_;

public:
    Serializer() = delete;
    Serializer(uint8_t* const data) : data_(data), idx_(0U) {}

    template <typename T> void serializeStaticType(const T& var)
    {
        const uint8_t* const data = reinterpret_cast<const uint8_t* const>(&var);
        memcpy(&(data_[idx_]), data, sizeof(T));
        idx_ += sizeof(T);
    }

    template <typename T> void serializePointer(const T* ptr, const size_t num_elements)
    {
        const uint8_t* const data = reinterpret_cast<const uint8_t* const>(ptr);

        memcpy(&(data_[idx_]), data, sizeof(T) * num_elements);
        idx_ += sizeof(T) * num_elements;
    }

    template <typename T> void serializeVector(const std::vector<T> vec)
    {
        const size_t num_elements = vec.size();
        const uint8_t* const data = reinterpret_cast<const uint8_t* const>(vec.data());

        memcpy(&(data_[idx_]), data, sizeof(T) * num_elements);
        idx_ += sizeof(T) * num_elements;
    }

    void serializeString(const std::string& s)
    {
        assert(s.length()  <= kMaxStringLen);
        memcpy(&(data_[idx_]), s.data(), s.length());
        idx_ += s.length();

        data_[idx_] = 0; // Null termination of string
        idx_ += 1;
    }

    uint64_t getIdx() const
    {
        return idx_;
    }

    uint8_t* getData() const
    {
        return data_;
    }
};

class Deserializer
{
private:
    const uint8_t* const data_;
    size_t idx_;

public:
    Deserializer() = delete;
    Deserializer(const uint8_t* const data) : data_(data), idx_(0U) {}

    template <typename T> void deserializeToStaticType(T& var)
    {
        uint8_t* data = reinterpret_cast<uint8_t*>(&var);
        memcpy(data, &(data_[idx_]), sizeof(T));
        idx_ += sizeof(T);
    }

    uint64_t getIdx() const
    {
        return idx_;
    }

    const uint8_t* getData() const
    {
        return data_;
    }

};

inline uint64_t sizeOfString(const std::string& s)
{
    return s.length() + 1;
}

#endif
