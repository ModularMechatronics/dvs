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

#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#include "switch.h"
#include "dyn_model.h"
#include "misc_utils.h"

#include "controller.h"

bool command_steps_init = false;


struct switch_modes command_steps[12];

void init_steps(void)
{
	if (!command_steps_init) {

		/* 1b */
		command_steps[0].hu=SM_OFF;
		command_steps[0].lu=SM_OFF;
		command_steps[0].hv=SM_OFF;
		command_steps[0].lv=SM_ON;
		command_steps[0].hw=SM_ON;
		command_steps[0].lw=SM_OFF;

		/* 2a */
		command_steps[1].hu=SM_ON;
		command_steps[1].lu=SM_OFF;
		command_steps[1].hv=SM_OFF;
		command_steps[1].lv=SM_ON;
		command_steps[1].hw=SM_OFF;
		command_steps[1].lw=SM_OFF;

		/* 2b */
		command_steps[2].hu=SM_ON;
		command_steps[2].lu=SM_OFF;
		command_steps[2].hv=SM_OFF;
		command_steps[2].lv=SM_ON;
		command_steps[2].hw=SM_OFF;
		command_steps[2].lw=SM_OFF;

		/* 3a */
		command_steps[3].hu=SM_ON;
		command_steps[3].lu=SM_OFF;
		command_steps[3].hv=SM_OFF;
		command_steps[3].lv=SM_OFF;
		command_steps[3].hw=SM_OFF;
		command_steps[3].lw=SM_ON;

		/* 3b */
		command_steps[4].hu=SM_ON;
		command_steps[4].lu=SM_OFF;
		command_steps[4].hv=SM_OFF;
		command_steps[4].lv=SM_OFF;
		command_steps[4].hw=SM_OFF;
		command_steps[4].lw=SM_ON;

		/* 4a */
		command_steps[5].hu=SM_OFF;
		command_steps[5].lu=SM_OFF;
		command_steps[5].hv=SM_ON;
		command_steps[5].lv=SM_OFF;
		command_steps[5].hw=SM_OFF;
		command_steps[5].lw=SM_ON;

		/* 4b */
		command_steps[6].hu=SM_OFF;
		command_steps[6].lu=SM_OFF;
		command_steps[6].hv=SM_ON;
		command_steps[6].lv=SM_OFF;
		command_steps[6].hw=SM_OFF;
		command_steps[6].lw=SM_ON;

		/* 5a */
		command_steps[7].hu=SM_OFF;
		command_steps[7].lu=SM_ON;
		command_steps[7].hv=SM_ON;
		command_steps[7].lv=SM_OFF;
		command_steps[7].hw=SM_OFF;
		command_steps[7].lw=SM_OFF;

		/* 5b */
		command_steps[8].hu=SM_OFF;
		command_steps[8].lu=SM_ON;
		command_steps[8].hv=SM_ON;
		command_steps[8].lv=SM_OFF;
		command_steps[8].hw=SM_OFF;
		command_steps[8].lw=SM_OFF;

		/* 6a */
		command_steps[9].hu=SM_OFF;
		command_steps[9].lu=SM_ON;
		command_steps[9].hv=SM_OFF;
		command_steps[9].lv=SM_OFF;
		command_steps[9].hw=SM_ON;
		command_steps[9].lw=SM_OFF;

		/* 6b */
		command_steps[10].hu=SM_OFF;
		command_steps[10].lu=SM_ON;
		command_steps[10].hv=SM_OFF;
		command_steps[10].lv=SM_OFF;
		command_steps[10].hw=SM_ON;
		command_steps[10].lw=SM_OFF;

		/* 1a */
		command_steps[11].hu=SM_OFF;
		command_steps[11].lu=SM_OFF;
		command_steps[11].hv=SM_OFF;
		command_steps[11].lv=SM_ON;
		command_steps[11].hw=SM_ON;
		command_steps[11].lw=SM_OFF;
	}
}

void config_switch(struct ssn **ssn, double t, double t1, const struct setpoint *sp, enum switch_mode mode)
{
	switch (mode) {
	case SM_OFF:
		switchSet(ssn, t, false);
		break;
	case SM_ON:
		switchSet(ssn, t, true);
		break;
	case SM_PWM:
		switchPwmGen(ssn, t, t1, sp->pwm_frequency, sp->pwm_duty, false);
		break;
	}
}

int runController(double t, double t1, const struct setpoint *sp, const struct motor *m, const struct state_vector *sv, struct command_vector *cv)
{
	/* create a virtual encoder */
	double elec_angle = norm_angle(sv->theta * (m->NbPoles/2)); // + ((M_PI/6)*8);
	const int slice = angle_slice(elec_angle, 12);

	init_steps();

	struct switch_modes *selected_sm = &command_steps[11-slice];
    cv->u_h = selected_sm->hu == SM_ON;
    cv->u_l = selected_sm->lu == SM_ON;
    cv->v_h = selected_sm->hv == SM_ON;
    cv->v_l = selected_sm->lv == SM_ON;
    cv->w_h = selected_sm->hw == SM_ON;
    cv->w_l = selected_sm->lw == SM_ON;

    if(t > 10.0)
    {
        cv->u_h = false;
        cv->u_l = false;
        cv->v_h = false;
        cv->v_l = false;
        cv->w_h = false;
        cv->w_l = false;
    }

	return 1;
}
