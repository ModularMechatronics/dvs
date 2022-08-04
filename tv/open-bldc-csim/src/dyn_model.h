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

#ifndef _DYN_MODEL_H
#define _DYN_MODEL_H

#include "structures.h"

#if defined(__cplusplus)
extern "C" {
#endif

int backEmf(struct state_vector *sv, struct motor *m, double thetae_offset, double *bemf);

void initState(struct state_vector *sv);
int modelDynamics(double t, const double asv[], double aov[], void *params);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* _DYN_MODEL_H */
