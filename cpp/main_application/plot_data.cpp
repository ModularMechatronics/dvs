#include "main_application/plot_data.h"

#include "math/math.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "plot_objects/plot_object_base.h"
#include "plot_objects/plot_objects.h"
#include "utils.h"

PlotDataHandler::PlotDataHandler() {}

void PlotDataHandler::clear()
{
    for (size_t k = 0; k < plot_datas_.size(); k++)
    {
        delete plot_datas_[k];
    }
    plot_datas_.clear();
}

void PlotDataHandler::addData(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
{
    const FunctionHeaderObject fcn_obj = hdr.getObjectAtIdx(0);
    Function fcn = fcn_obj.getAs<Function>();

    switch (fcn)
    {
        case Function::PLOT2:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new Plot2D(std::move(received_data), hdr)));
            break;

        case Function::PLOT3:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new Plot3D(std::move(received_data), hdr)));
            break;

        case Function::SCATTER2:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new Scatter2D(std::move(received_data), hdr)));
            break;

        case Function::SCATTER3:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new Scatter3D(std::move(received_data), hdr)));
            break;

        case Function::SURF:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new Surf(std::move(received_data), hdr)));
            break;

        case Function::IM_SHOW:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new ImShow(std::move(received_data), hdr)));
            break;

        case Function::POLYGON_FROM_4_POINTS:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new DrawPolygon4Points(std::move(received_data), hdr)));
            break;

        case Function::PLANE_XY:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new DrawPlaneXY(std::move(received_data), hdr)));
            break;

        case Function::PLANE_XZ:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new DrawPlaneXZ(std::move(received_data), hdr)));
            break;

        case Function::PLANE_YZ:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new DrawPlaneYZ(std::move(received_data), hdr)));
            break;

        case Function::DRAW_TRIANGLES_3D:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new DrawTriangles3D(std::move(received_data), hdr)));
            break;

        case Function::DRAW_TRIANGLE_3D:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new DrawTriangle3D(std::move(received_data), hdr)));
            break;

        case Function::DRAW_MESH:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new DrawMesh(std::move(received_data), hdr)));
            break;

        case Function::DRAW_LINE3D:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new DrawLine3D(std::move(received_data), hdr)));
            break;

        case Function::DRAW_LINE_BETWEEN_POINTS_3D:
            plot_datas_.push_back(dynamic_cast<PlotObjectBase*>(new DrawLine3D(std::move(received_data), hdr)));
            break;

            break;
        default:
            throw std::runtime_error("Invalid function!");
            break;
    }
}

void PlotDataHandler::visualize() const
{
    for (size_t k = 0; k < plot_datas_.size(); k++)
    {
        plot_datas_[k]->visualize();
    }
}

std::pair<Vec3Dd, Vec3Dd> PlotDataHandler::getMinMaxVectors() const
{
    if (plot_datas_.size() == 0)
    {
        return std::pair<Vec3Dd, Vec3Dd>(Vec3Dd(-1, -1, -1), Vec3Dd(1, 1, 1));
    }
    else
    {
        const std::pair<Vec3Dd, Vec3Dd> min_max = plot_datas_[0]->getMinMaxVectors();
        size_t num_dimensions = plot_datas_[0]->getNumDimensions();
        Vec3Dd min_vec = min_max.first;
        Vec3Dd max_vec = min_max.second;

        bool z_is_set = num_dimensions == 3 ? true : false;

        for (size_t k = 1; k < plot_datas_.size(); k++)
        {
            const std::pair<Vec3Dd, Vec3Dd> current_min_max = plot_datas_[0]->getMinMaxVectors();

            const size_t current_num_dimensions = plot_datas_[k]->getNumDimensions();
            num_dimensions = std::max(num_dimensions, current_num_dimensions);

            if (current_num_dimensions == 3)
            {
                if (!z_is_set)
                {
                    min_vec.z = current_min_max.first.z;
                    max_vec.z = current_min_max.second.z;
                    z_is_set = true;
                }
                else
                {
                    min_vec.z = std::min(min_vec.z, current_min_max.first.z);
                }
            }

            min_vec.x = std::min(min_vec.x, current_min_max.first.x);
            min_vec.y = std::min(min_vec.y, current_min_max.first.y);
        }

        if (num_dimensions == 2)
        {
            min_vec.z = -1.0;
            max_vec.z = 1.0;
        }

        const Vec3Dd diff_vec = max_vec - min_vec;

        const Vectord v = {diff_vec.x, diff_vec.y, diff_vec.z};

        const double largest_diff = dvs::max(v);

        // If some of the axes turns out to have a very small difference
        // between min and max, we have to modify this
        if (diff_vec.x < largest_diff * 0.01)
        {
            min_vec.x = -largest_diff * 0.01;
            max_vec.x = largest_diff * 0.01;
        }
        if (diff_vec.y < largest_diff * 0.01)
        {
            min_vec.y = -largest_diff * 0.01;
            max_vec.y = largest_diff * 0.01;
        }
        if (diff_vec.z < largest_diff * 0.01)
        {
            min_vec.z = -largest_diff * 0.01;
            max_vec.z = largest_diff * 0.01;
        }

        return std::pair<Vec3Dd, Vec3Dd>(min_vec, max_vec);
    }
}

void PlotDataHandler::softClear()
{
    std::vector<PlotObjectBase*> new_plot_datas;
    for (size_t k = 0; k < plot_datas_.size(); k++)
    {
        if (plot_datas_[k]->isPersistent())
        {
            new_plot_datas.push_back(plot_datas_[k]);
        }
        else
        {
            delete plot_datas_[k];
        }
    }
    plot_datas_ = new_plot_datas;
}
