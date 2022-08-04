#ifndef TEST_DATA_ROTATIONS_H
#define TEST_DATA_ROTATIONS_H

#include <vector>

// Quaternion listed as [x, y, z, w]

extern const double rmat0[3][3];
extern const double quat0[4];
extern const double ax0[3];
extern const double angle0;
extern const double rpy0[3];

extern const double rmat1[3][3];
extern const double quat1[4];
extern const double ax1[3];
extern const double angle1;
extern const double rpy1[3];

extern const double rmat2[3][3];
extern const double quat2[4];
extern const double ax2[3];
extern const double angle2;
extern const double rpy2[3];

extern const double rmat3[3][3];
extern const double quat3[4];
extern const double ax3[3];
extern const double angle3;
extern const double rpy3[3];

extern const double rmat4[3][3];
extern const double quat4[4];
extern const double ax4[3];
extern const double angle4;
extern const double rpy4[3];

extern const double rmat5[3][3];
extern const double quat5[4];
extern const double ax5[3];
extern const double angle5;
extern const double rpy5[3];

extern const double rmat6[3][3];
extern const double quat6[4];
extern const double ax6[3];
extern const double angle6;
extern const double rpy6[3];

extern const double rmat7[3][3];
extern const double quat7[4];
extern const double ax7[3];
extern const double angle7;
extern const double rpy7[3];

extern const double rmat8[3][3];
extern const double quat8[4];
extern const double ax8[3];
extern const double angle8;
extern const double rpy8[3];

extern const double rmat9[3][3];
extern const double quat9[4];
extern const double ax9[3];
extern const double angle9;
extern const double rpy9[3];

extern const double rmat10[3][3];
extern const double quat10[4];
extern const double ax10[3];
extern const double angle10;
extern const double rpy10[3];

extern const double rmat11[3][3];
extern const double quat11[4];
extern const double ax11[3];
extern const double angle11;
extern const double rpy11[3];

extern const double rmat12[3][3];
extern const double quat12[4];
extern const double ax12[3];
extern const double angle12;
extern const double rpy12[3];

extern const double rmat13[3][3];
extern const double quat13[4];
extern const double ax13[3];
extern const double angle13;
extern const double rpy13[3];

extern std::vector<const double (*)[3]> rotation_matrix_arrays;
extern std::vector<const double(*)> quaternion_arrays;
extern std::vector<const double(*)> axis_arrays;
extern std::vector<const double(*)> angle_arrays;
extern std::vector<const double(*)> rpy_arrays;

void TEST_SETUP_DATA_ROTATIONS();

#endif
