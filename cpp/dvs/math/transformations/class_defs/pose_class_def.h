#ifndef DVS_POSE_CLASS_H_
#define DVS_POSE_CLASS_H_

namespace dvs
{
template <typename T> class PoseSE3
{
private:
    Matrix<T> rotation_matrix;
    Vec3D<T> translation_vector;

public:
    Matrix<T> getRotationMatrix() const;
    Vec3D<T> getTranslationVector() const;
    Matrix<T> getPoseMatrix() const;
    Matrix<T> getInversePoseMatrix() const;
    void invert();

    PoseSE3(const Matrix<T>& rotation_matrix, const Vec3D<T>& translation_vector);
    PoseSE3(const Matrix<T>& pose_matrix);
    PoseSE3();
};
}  // namespace dvs

#endif
