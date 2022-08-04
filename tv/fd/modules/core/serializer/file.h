#ifndef SERIALIZABLE_FILE_H_
#define SERIALIZABLE_FILE_H_

#include <fstream>

#include <stdint.h>

#include "core/logging.h"
#include "serializable.h"
#include "serializable_types.h"
#include "serialize_types.h"

template <typename T>
class SerializableType
{
private:

public:
    SerializableType() = delete;
    
    SerializableType(const T& var);
    SerializableType(const T* var, const size_t num_elements);

};

template <typename T>
SerializableType<T>::SerializableType(const T& var)
{

}


class SimpleFileWriter final
{
private:
    uint8_t* data_;
    uint64_t buffer_size_;
    uint64_t idx_;
    uint64_t num_entries_;
    std::string file_path_;
    bool is_open_;

    static constexpr uint64_t kInitialBufferSize = 1024U;

    void writeToFile();
    void bufferReallocation(const uint64_t new_data_size);

public:
    SimpleFileWriter() : data_(nullptr), buffer_size_(0U), idx_(0U), num_entries_(0U), file_path_(""), is_open_(false) {}
    SimpleFileWriter(const std::string& file_path);
    SimpleFileWriter(const std::string& file_path, const uint64_t initial_buffer_size);
    SimpleFileWriter(SimpleFileWriter&& other);
    SimpleFileWriter(const SimpleFileWriter& other) = delete;
    SimpleFileWriter&& operator=(SimpleFileWriter&& other);

    ~SimpleFileWriter();

    template <typename T>
    void append(const T& data);
    template <typename T>
    void append(const std::vector<T>& data);
    template <typename T>
    void append(const T* data, const uint64_t num_elements);

    void close();
    void open(const std::string& file_path);
    void open(const std::string& file_path, const uint64_t initial_buffer_size);
};

/*
uint64_t num_bytes
uint16_t type (enum)
bool has_name
(optional) uint8_t num_name_bytes
(optional) name (num_name_bytes)
*/

inline void SimpleFileWriter::open(const std::string& file_path)
{
    data_ = new uint8_t[kInitialBufferSize];
    buffer_size_ = kInitialBufferSize;
    idx_ = 0U;
    file_path_ = file_path;
    is_open_ = true;
}

inline void SimpleFileWriter::open(const std::string& file_path, const uint64_t initial_buffer_size)
{
    data_ = new uint8_t[initial_buffer_size];
    buffer_size_ = initial_buffer_size;
    idx_ = 0U;
    file_path_ = file_path;
    is_open_ = true;
}

inline void SimpleFileWriter::close()
{
    if(is_open_)
    {
        writeToFile();
        delete[] data_;
        idx_ = 0;
        is_open_ = false;
    }
}

inline void SimpleFileWriter::writeToFile()
{
    auto myfile = std::fstream(file_path_, std::ios::out | std::ios::binary);
    std::cout << "Writing " << idx_ << " bytes..." << std::endl;
    myfile.write((char*)&(idx_), sizeof(idx_));
    myfile.write((char*)&(num_entries_), sizeof(num_entries_));
    myfile.write((char*)&(data_[0]), idx_);
    myfile.close();
}

// template <typename T>
// void SimpleFileWriter::append(const std::string& name, const T& data)

template <typename T>
void fillBufferWithObject(uint8_t* data_buffer, const T& input_data)
{
    const uint8_t* const tmp_ptr = reinterpret_cast<const uint8_t* const>(&input_data);

    memcpy(data_buffer, tmp_ptr, sizeof(T));
}

// template <typename T>
// void SimpleFileWriter::append(const std::string& data_name, const T& data)

template <typename T>
void SimpleFileWriter::append(const T& data)
{
    num_entries_++;

    const uint64_t serialized_data_size = sizeOfType(data);
    const uint64_t new_data_size = sizeof(uint64_t) + serialized_data_size;

    if((idx_ + new_data_size) > buffer_size_)
    {
        bufferReallocation(new_data_size);
    }

    serialize(data, &(data_[idx_]));
    idx_ += serialized_data_size;
}

template <typename T>
void SimpleFileWriter::append(const T* data, const uint64_t num_elements)
{
    if(num_elements == 0)
    {
        return;
    }

    num_entries_++;

    uint64_t num_bytes = 0;

    for(size_t k = 0; k < num_elements; k++)
    {
        num_bytes += sizeOfType(data[k]);
    }

    const uint64_t new_data_size = sizeof(uint64_t) * 2 + num_bytes;

    memcpy(&(data_[idx_]), &num_elements, sizeof(num_elements));
    idx_ += sizeof(uint64_t);

    memcpy(&(data_[idx_]), &num_bytes, sizeof(num_elements));
    idx_ += sizeof(uint64_t);

    if((idx_ + new_data_size) > buffer_size_)
    {
        bufferReallocation(new_data_size);
    }

    for(size_t k = 0; k < num_elements; k++)
    {
        serialize(data[k], &(data_[idx_]));
        idx_ += sizeOfType(data[k]);
    }
}

inline void SimpleFileWriter::bufferReallocation(const uint64_t new_data_size)
{
    while((idx_ + new_data_size) > buffer_size_)
    {
        buffer_size_ *= 2U;
    }

    constexpr uint64_t kFourPointThreeGbInBytes = 4294967296;

    if(buffer_size_ > kFourPointThreeGbInBytes)
    {
        EXIT(-1) << "You probably don't want to allocate more than 4.3GB... Exiting.";
    }

    uint8_t* new_ptr = new uint8_t[buffer_size_];

    memcpy(new_ptr, data_, idx_);
    delete[] data_;
    data_ = new_ptr;
}

template <typename T>
void SimpleFileWriter::append(const std::vector<T>& data)
{
    if(data.size() == 0)
    {
        return;
    }

    num_entries_++;

    const uint64_t num_elements = data.size();
    uint64_t num_bytes = 0;

    for(size_t k = 0; k < num_elements; k++)
    {
        num_bytes += sizeOfType(data[k]);
    }

    const uint64_t new_data_size = sizeof(uint64_t) * 2 + num_bytes;

    memcpy(&(data_[idx_]), &num_elements, sizeof(num_elements));
    idx_ += sizeof(uint64_t);

    memcpy(&(data_[idx_]), &num_bytes, sizeof(num_elements));
    idx_ += sizeof(uint64_t);

    if((idx_ + new_data_size) > buffer_size_)
    {
        bufferReallocation(new_data_size);
    }

    for(size_t k = 0; k < data.size(); k++)
    {
        serialize(data[k], &(data_[idx_]));
        idx_ += sizeOfType(data[k]);
    }

}

inline SimpleFileWriter&& SimpleFileWriter::operator=(SimpleFileWriter&& other)
{
    std::cout << (&other) << " is being move ASSIGNED to " << this << std::endl;
    data_ = other.data_;
    buffer_size_ = other.buffer_size_;
    idx_ = other.idx_;
    num_entries_ = other.num_entries_;
    file_path_ = other.file_path_;
    is_open_ = other.is_open_;

    other.data_ = nullptr;
    other.buffer_size_ = 0;
    other.idx_ = 0;
    other.num_entries_ = 0;
    other.is_open_ = false;

    return std::move(*this);
}

inline SimpleFileWriter::SimpleFileWriter(SimpleFileWriter&& other)
{
    std::cout << (&other) << " is being moved to " << this << std::endl;
    data_ = other.data_;
    buffer_size_ = other.buffer_size_;
    idx_ = other.idx_;
    num_entries_ = other.num_entries_;
    file_path_ = other.file_path_;
    is_open_ = other.is_open_;

    other.data_ = nullptr;
    other.buffer_size_ = 0;
    other.idx_ = 0;
    other.num_entries_ = 0;
    other.is_open_ = false;
}

inline SimpleFileWriter::SimpleFileWriter(const std::string& file_path) :
    data_(new uint8_t[kInitialBufferSize]), buffer_size_(kInitialBufferSize), idx_(0U), num_entries_(0U), file_path_(file_path), is_open_(true)
{
    std::cout << "Constructor of" << this << std::endl;
}

inline SimpleFileWriter::SimpleFileWriter(const std::string& file_path, const uint64_t initial_buffer_size) :
    data_(new uint8_t[initial_buffer_size]), buffer_size_(initial_buffer_size), idx_(0U), num_entries_(0U), file_path_(file_path), is_open_(true)
{}

inline SimpleFileWriter::~SimpleFileWriter()
{
    close();
}


class SimpleFileReader final
{
private:
    uint8_t* data_;
    uint64_t num_bytes_;
    uint64_t num_entries_;
    uint64_t idx_;
    std::ifstream file_;

public:
    SimpleFileReader() : data_(nullptr) {}
    SimpleFileReader(const std::string& file_path);
    ~SimpleFileReader();

    template <typename T>
    bool readNext(T& data);
    template <typename T>
    bool readNext(std::vector<T>& data);

    uint64_t bytesLeftToRead() const;
    uint64_t numEntries() const;
};

inline SimpleFileReader::~SimpleFileReader()
{
    delete[] data_;
}

inline SimpleFileReader::SimpleFileReader(const std::string& file_path) : idx_(0U)
{
    file_ = std::ifstream(file_path, std::ios::out | std::ios::binary);

    if(!file_)
    {
        std::cout << "Cannot open file!" << std::endl;
    }

    file_.read(reinterpret_cast<char*>(&num_bytes_), sizeof(num_bytes_));
    file_.read(reinterpret_cast<char*>(&num_entries_), sizeof(num_entries_));

    std::cout << "Reading " << num_bytes_ << " bytes..." << std::endl;
    data_ = new uint8_t[num_bytes_];

    file_.read(reinterpret_cast<char*>(data_), num_bytes_);

    file_.close();
}

inline uint64_t SimpleFileReader::numEntries() const
{
    return num_entries_;
}

inline uint64_t SimpleFileReader::bytesLeftToRead() const
{
    return (idx_ > num_bytes_) ? 0U : (num_bytes_ - idx_);
}

template <typename T>
bool SimpleFileReader::readNext(T& data)
{
    if(idx_ == num_bytes_)
    {
        return false;
    }
    // if((idx_ + sizeof(T)) > num_bytes_)
    // {
    //     return false;
    // }
    // else
    // {
        deserialize(&(data_[idx_]), data);
        idx_ += sizeOfType(data);
        return true;
    // }
}

template <typename T>
bool SimpleFileReader::readNext(std::vector<T>& data)
{
    if(idx_ == num_bytes_)
    {
        return false;
    }
    uint64_t num_read_elements, num_read_bytes;
    memcpy(&num_read_elements, &(data_[idx_]), sizeof(num_read_elements));
    idx_ += sizeof(uint64_t);

    memcpy(&num_read_bytes, &(data_[idx_]), sizeof(num_read_bytes));
    idx_ += sizeof(uint64_t);

    if((idx_ + num_read_bytes) > num_bytes_)
    {
        return false;
    }
    else
    {
        data.resize(num_read_elements);

        for(size_t k = 0; k < num_read_elements; k++)
        {
            deserialize(&(data_[idx_]), data[k]);
            idx_ += sizeOfType(data[k]);
        }

        return true;
    }
}


#endif
