#ifndef DATA_CONTAINERS_H_
#define DATA_CONTAINERS_H_

template <typename T>
class Matrix
{
private:
    size_t rows_;
    size_t cols_;
    T* data_;

public:
    Matrix() : rows_(0), cols_(0), data_(nullptr)
    {

    }

    Matrix(const size_t rows_in, const size_t cols_in) : rows_(rows_in), cols_(cols_in)
    {
        data_ = new T[rows_in * cols_in];
    }

    ~Matrix()
    {
        rows_ = 0;
        cols_ = 0;
        delete[] data_;
    }

    void setInternalPointer(const size_t rows_in, const size_t cols_in, T* data_ptr)
    {
        rows_ = rows_in;
        cols_ = cols_in;
        data_ = data_ptr;
    }

};

#endif
