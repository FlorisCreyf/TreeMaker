/* 
 * Copyright (C) 2016 Floris Creyf
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef MESH_SIZE_H
#define MESH_SIZE_H

#include "node.h"

static inline int get_branch_vcount(node *stem)
{
	return stem->cross_sections * stem->branch_resolution * 3;
}

static inline int get_branch_ecount(node *stem)
{
	return stem->cross_sections * stem->branch_resolution * 6;
}

#endif /* MESH_SIZE_H */
