#include "test/unit_tests/math/test_data/test_data_rotations.h"

const double rmat0[3][3] = {{0.3020658, -0.6822845, 0.6657658},
                            {0.1786694, 0.7265335, 0.6634955},
                            {-0.9363939, -0.0814673, 0.3413643}};
const double quat0[4] = {-0.2419545, 0.5203613, 0.279627, 0.7697343};
const double ax0[3] = {-0.3790226, 0.8151477, 0.4380366};
const double angle0 = 1.384743;
const double rpy0[3] = {-0.23427, 1.2122101, 0.53414};

const double rmat1[3][3] = {{0.7490382, 0.1062940, -0.6539444},
                            {-0.6624105, 0.1016569, -0.7422117},
                            {-0.0124147, 0.9891246, 0.1465551}};
const double quat1[4] = {0.6125411, -0.2269711, -0.2719651, 0.7066205};
const double ax1[3] = {0.865669, -0.3207652, -0.3843526};
const double angle1 = 1.5721712;
const double rpy1[3] = {1.4237, 0.012415, -0.7241};

const double rmat2[3][3] = {{0.1850022, -0.6043947, -0.7749072},
                            {0.4964692, -0.6230184, 0.6044555},
                            {-0.8481112, -0.4965431, 0.1848036}};
const double quat2[4] = {-0.6370277, 0.0423551, 0.6369497, 0.4320843};
const double ax2[3] = {-0.7063697, 0.0469656, 0.7062833};
const double angle2 = 2.2479873;
const double rpy2[3] = {-1.2145, 1.01241, 1.2141};

const double rmat3[3][3] = {{0.9787245, -0.1969100, -0.0576616},
                            {0.1391616, 0.8435739, -0.5186685},
                            {0.1507728, 0.4996093, 0.8530288}};
const double quat3[4] = {0.2655757, -0.0543615, 0.0876504, 0.9585571};
const double ax3[3] = {0.9321704, -0.1908088, 0.3076528};
const double angle3 = 0.5778049;
const double rpy3[3] = {0.52983, -0.15135, 0.14124};

const double rmat4[3][3] = {{0.5093587, 0.5045460, -0.6971277},
                            {-0.2228927, 0.8597973, 0.4594208},
                            {0.8311874, -0.0786253, 0.5504048}};
const double quat4[4] = {-0.1574457, -0.447223, -0.2128667, 0.8543361};
const double ax4[3] = {-0.3029428, -0.8605062, -0.4095788};
const double angle4 = 1.0930484;
const double rpy4[3] = {-0.14189, -0.98124, -0.41249};

const double rmat5[3][3] = {{0.3411126, 0.9364606, 0.0817548},
                            {-0.9106703, 0.3507729, -0.2182614},
                            {-0.2330706, 0.0, 0.9724599}};
const double quat5[4] = {0.0668577, 0.0964372, -0.5658123, 0.8161411};
const double ax5[3] = {0.1157003, 0.1668889, -0.9791637};
const double angle5 = 1.2321902;
const double rpy5[3] = {0, 0.235234, -1.2124};

const double rmat6[3][3] = {{0.8660448, -0.2648617, 0.4240457},
                            {0.4999664, 0.4587950, -0.7345344},
                            {0.0, 0.8481482, 0.5297589}};
const double quat6[4] = {0.4683724, 0.1254903, 0.22634, 0.8447779};
const double ax6[3] = {0.8752709, 0.2345099, 0.4229728};
const double angle6 = 1.1292927;
const double rpy6[3] = {1.01248, 0.0, 0.52356};

const double rmat7[3][3] = {{0.9673599, -0.0666702, -0.2444788},
                            {0.0, 0.9647696, -0.2630961},
                            {0.2534064, 0.2545086, 0.9332795}};
const double quat7[4] = {0.1316348, -0.1266198, 0.0169553, 0.9830322};
const double ax7[3] = {0.7176167, -0.6902771, 0.0924328};
const double angle7 = 0.3689557;
const double rpy7[3] = {0.26623, -0.2562, 0.0};

const double rmat8[3][3] = {
    {1.0000000, 0.0, 0.0}, {0.0, 0.9726074, -0.2324540}, {0.0, 0.2324540, 0.9726074}};
const double quat8[4] = {0.1170312, 0, 0, 0.9931282};
const double ax8[3] = {1, 0, 0};
const double angle8 = 0.2346;
const double rpy8[3] = {0.2346, 0, 0};

const double rmat9[3][3] = {
    {1.0000000, 0.0, 0.0}, {0.0, 0.1576361, 0.9874973}, {0.0, -0.9874973, 0.1576361}};
const double quat9[4] = {-0.6489853, 0, 0, 0.7608009};
const double ax9[3] = {-1, 0, 0};
const double angle9 = 1.4125;
const double rpy9[3] = {-1.4125, 0, 0};

const double rmat10[3][3] = {
    {0.8118982, 0.0, 0.5837990}, {0.0, 1.0000000, 0.0}, {-0.5837990, 0.0, 0.8118982}};
const double quat10[4] = {0, 0.3066772, 0, 0.9518136};
const double ax10[3] = {0, 1, 0};
const double angle10 = 0.6234;
const double rpy10[3] = {0, 0.6234, 0};

const double rmat11[3][3] = {
    {0.8666242, 0.0, -0.4989615}, {0.0, 1.0000000, 0.0}, {0.4989615, 0.0, 0.8666242}};
const double quat11[4] = {0, -0.25824, 0, 0.9660808};
const double ax11[3] = {0, -1, 0};
const double angle11 = 0.5224;
const double rpy11[3] = {0, -0.5224, 0};

const double rmat12[3][3] = {
    {0.7494655, -0.6620435, 0.0}, {0.6620435, 0.7494655, 0.0}, {0.0, 0.0, 1.0000000}};
const double quat12[4] = {0, 0, 0.3539312, 0.9352715};
const double ax12[3] = {0, 0, 1};
const double angle12 = 0.723542;
const double rpy12[3] = {0, 0, 0.723542};

const double rmat13[3][3] = {
    {0.9234423, 0.3837373, 0.0}, {-0.3837373, 0.9234423, 0.0}, {0.0, 0.0, 1.0000000}};
const double quat13[4] = {0, 0, -0.1956498, 0.9806738};
const double ax13[3] = {0, 0, -1};
const double angle13 = 0.39384;
const double rpy13[3] = {0, 0, -0.39384};

std::vector<const double (*)[3]> rotation_matrix_arrays;
std::vector<const double(*)> quaternion_arrays;
std::vector<const double(*)> axis_arrays;
std::vector<const double(*)> angle_arrays;
std::vector<const double(*)> rpy_arrays;

void TEST_SETUP_DATA_ROTATIONS()
{
    rotation_matrix_arrays.clear();
    quaternion_arrays.clear();
    axis_arrays.clear();
    angle_arrays.clear();
    rpy_arrays.clear();

    rotation_matrix_arrays.push_back(rmat0);
    rotation_matrix_arrays.push_back(rmat1);
    rotation_matrix_arrays.push_back(rmat2);
    rotation_matrix_arrays.push_back(rmat3);
    rotation_matrix_arrays.push_back(rmat4);
    rotation_matrix_arrays.push_back(rmat5);
    rotation_matrix_arrays.push_back(rmat6);
    rotation_matrix_arrays.push_back(rmat7);
    rotation_matrix_arrays.push_back(rmat8);
    rotation_matrix_arrays.push_back(rmat9);
    rotation_matrix_arrays.push_back(rmat10);
    rotation_matrix_arrays.push_back(rmat11);
    rotation_matrix_arrays.push_back(rmat12);
    rotation_matrix_arrays.push_back(rmat13);

    quaternion_arrays.push_back(quat0);
    quaternion_arrays.push_back(quat1);
    quaternion_arrays.push_back(quat2);
    quaternion_arrays.push_back(quat3);
    quaternion_arrays.push_back(quat4);
    quaternion_arrays.push_back(quat5);
    quaternion_arrays.push_back(quat6);
    quaternion_arrays.push_back(quat7);
    quaternion_arrays.push_back(quat8);
    quaternion_arrays.push_back(quat9);
    quaternion_arrays.push_back(quat10);
    quaternion_arrays.push_back(quat11);
    quaternion_arrays.push_back(quat12);
    quaternion_arrays.push_back(quat13);

    axis_arrays.push_back(ax0);
    axis_arrays.push_back(ax1);
    axis_arrays.push_back(ax2);
    axis_arrays.push_back(ax3);
    axis_arrays.push_back(ax4);
    axis_arrays.push_back(ax5);
    axis_arrays.push_back(ax6);
    axis_arrays.push_back(ax7);
    axis_arrays.push_back(ax8);
    axis_arrays.push_back(ax9);
    axis_arrays.push_back(ax10);
    axis_arrays.push_back(ax11);
    axis_arrays.push_back(ax12);
    axis_arrays.push_back(ax13);

    angle_arrays.push_back(&angle0);
    angle_arrays.push_back(&angle1);
    angle_arrays.push_back(&angle2);
    angle_arrays.push_back(&angle3);
    angle_arrays.push_back(&angle4);
    angle_arrays.push_back(&angle5);
    angle_arrays.push_back(&angle6);
    angle_arrays.push_back(&angle7);
    angle_arrays.push_back(&angle8);
    angle_arrays.push_back(&angle9);
    angle_arrays.push_back(&angle10);
    angle_arrays.push_back(&angle11);
    angle_arrays.push_back(&angle12);
    angle_arrays.push_back(&angle13);

    rpy_arrays.push_back(rpy0);
    rpy_arrays.push_back(rpy1);
    rpy_arrays.push_back(rpy2);
    rpy_arrays.push_back(rpy3);
    rpy_arrays.push_back(rpy4);
    rpy_arrays.push_back(rpy5);
    rpy_arrays.push_back(rpy6);
    rpy_arrays.push_back(rpy7);
    rpy_arrays.push_back(rpy8);
    rpy_arrays.push_back(rpy9);
    rpy_arrays.push_back(rpy10);
    rpy_arrays.push_back(rpy11);
    rpy_arrays.push_back(rpy12);
    rpy_arrays.push_back(rpy13);
}