
void testHyperboloid()
{
    // Grid lines disappear when this function is plotted
    const std::string project_file_path = "../../project_files/small.duoplot";
    openProjectFile(project_file_path);
    const size_t n_elems{100U};

    /*
    [x,y]=meshgrid(-10:10);
    r_sq=x.^2+y.^2;
    z=sqrt(r_sq+1);
    surf(x,y,z)
    */

    const std::pair<Matrix<double>, Matrix<double>> xy_mats =
        meshGrid<double>(-10.0, 10.0, -10.0, 10.0, n_elems, n_elems);
    Matrix<double> x{xy_mats.first}, y{xy_mats.second};

    const Matrix<double> r = elementWiseMultiply(x, x) + elementWiseMultiply(y, y);
    const Matrix<double> z = dvs::sqrt(1.0 - r);

    setCurrentElement("p_view_0");
    clearView();
    surf(x, y, z, properties::EdgeColor(0, 0, 0), properties::ColorMap::JET_BRIGHT);
}
