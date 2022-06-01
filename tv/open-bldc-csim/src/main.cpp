/*
 * Open-BLDC CSim - Open BrushLess DC Motor Controller C Simulator
 * Copyright (C) 2012 by Piotr Esden-Tempski <piotr@esden.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdbool.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>
#include <iostream>
#include <fstream>

#include <dvs/dvs.h>
#include <nlohmann/json.hpp>

#include "logging.h"

#include "dyn_model.h"
#include "code.h"
#include "controller.h"

struct motor motor = {
	motor.inertia = 0.01,
	motor.damping = 0.001,
	motor.static_friction = 0.2,
	motor.Kv = 1./32.3*1000,
	motor.L = 0.00207,
	motor.M = -0.00069,
	motor.R = 11.9,
	motor.VDC = 10,
	motor.NbPoles = 4
};

struct command_vector cv = {
	cv.hu = NULL,
	cv.lu = NULL,
	cv.hv = NULL,
	cv.lv = NULL,
	cv.hw = NULL,
	cv.lw = NULL
};

struct perturbation_vector pv = {
	pv.torque = 0.0
};

struct parameters params = {
	params.m = &motor,
	params.cv = &cv,
	params.pv = &pv
};

struct setpoint setpoint = {
        setpoint.pwm_frequency = 16000,
        setpoint.pwm_duty = 1
};

void exp0()
{
    using namespace dvs;

    const size_t num_points = 10000;
    const Vector<float> angle = linspaceFromPointsAndCount(-20.0f, 20.0f, num_points);
    Vector<float> wrapped_angle(num_points);

    for(size_t k = 0; k < angle.size(); k++)
    {
        wrapped_angle(k) = wrapAngle(angle(k));
    }

    setCurrentElement("view_00");
    clearView();

    // axis({0.0, 16.0, -1.0}, {50.0, 64.0, 1.0});
    stairs(angle, wrapped_angle);
    plot(angle, wrapped_angle, properties::Color(255, 0, 0));
}

struct SimConfig
{
    double sim_freq;
    double t_start;
    double t_end;
    double controller_freq;
    uint64_t save_stride;

    bool plot;

    SimConfig() = default;
    SimConfig(const std::string& file_path)
    {
        std::ifstream file(file_path);
        nlohmann::json j = nlohmann::json::parse(file);

        sim_freq = j["sim_freq"];
        t_start = j["t_start"];
        t_end = j["t_end"];
        controller_freq = j["controller_freq"];
        plot = j["plot"];
        save_stride = j["save_stride"];
    }

    void print() const
    {
        std::cout << "sim_freq: " << sim_freq << std::endl;
        std::cout << "t_start: " << t_start << std::endl;
        std::cout << "t_end: " << t_end << std::endl;
    }
};


void logTest()
{
    
    MC_LOG("This is a %s message, and this is a number: %f", "LOG", 42.3f);
    MC_INFO("This is an %s message, and this is a number: %f", "INFO", 42.3f);
    MC_ERROR("This is an %s message, and this is a number: %f", "ERROR", 42.3f);
    MC_FATAL("This is a %s message, and this is a number: %f", "FATAL", 42.3f);
    MC_DEBUG("This is a %s message, and this is a number: %f", "DEBUG", 42.3f);
    MC_WARNING("This is a %s message, and this is a number: %f", "WARNING", 42.3f);
    // MC_ASSERT(false, "This is an %s and this is a number %f", "ASSERTION", 634.23f);
    MC_EXIT("This is %s and this is a number %f", "EXIT", 52.47f);
}

int main(void)
{
	gsl_odeiv2_system sys = {modelDynamics, NULL, 5, &params};

	// prams: system, driver, initial step size, absolute error, relative error
	gsl_odeiv2_driver *d = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk4, 1e-6, 1e-6, 0.0);

    const SimConfig sc("../src/sim_config.json");
    sc.print();

	const uint64_t num_sim_steps = static_cast<uint64_t>((sc.t_end - sc.t_start) * sc.sim_freq);
	const double t_step = (sc.t_end - sc.t_start) / num_sim_steps;
    double t = sc.t_start;

	struct state_vector sv;

	initState(&sv);
	runController(t, t + t_step, &setpoint, &motor, &sv, &cv);

    if((num_sim_steps % sc.save_stride) != 0)
    {
        MC_WARNING("Save stride %lu is not integer multiple of the number of simulation steps %lu", sc.save_stride, num_sim_steps);
    }

    dvs::Vector<double> t_vec(num_sim_steps / sc.save_stride), theta_vec(num_sim_steps / sc.save_stride);

    uint32_t idx = 0;

	for (uint64_t i = 1; i < num_sim_steps; i++)
	{
		const double ti = static_cast<double>(i) * t_step;
		int status = gsl_odeiv2_driver_apply(d, &t, ti, (double *)&sv);

		if (status != GSL_SUCCESS)
		{
			MC_ERROR("GSL return value=%d", status);
			break;
		}

        if((i % sc.save_stride) == 0)
        {
            t_vec(idx) = t;
            theta_vec(idx) = sv.theta;
            idx++;
        }
        

		runController(t, t + t_step, &setpoint, &motor, &sv, &cv);
	}

	gsl_odeiv2_driver_free (d);

    if(sc.plot)
    {
        dvs::setCurrentElement("view_00");
        dvs::clearView();

        // dvs::axis({0.0, 16.0, -1.0}, {50.0, 64.0, 1.0});
        dvs::plot(t_vec, theta_vec);
    }

	return 0;
}
