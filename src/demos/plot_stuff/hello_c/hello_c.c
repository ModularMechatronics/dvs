
#include "dvs/dvs.h"

int main()
{
    const size_t num_rows = 100, num_cols = 100;
    const MatrixPairD mp = meshgridD(0.0, 4.0, 1.0, 5.0, num_cols, num_rows);

    const MatrixD x = mp.m0, y = mp.m1;

    const MatrixD rx = matrix_sinD(x);
    const MatrixD ry = matrix_cosD(y);
    const MatrixD z = matrix_elementWiseMultiplyD(rx, ry);

    setCurrentElement("hello_c");
    clearView();

    surf(x, y, z, ColorMapJetSoft(), Alpha(0.7));
    setTitle("Hello from C!");
    view(-128, 23);

    free(x.data);
    free(y.data);
    free(z.data);

    free(rx.data);
    free(ry.data);

    return 0;
}