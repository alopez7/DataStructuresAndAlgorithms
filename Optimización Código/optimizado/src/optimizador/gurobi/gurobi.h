#pragma once

#include "../modelamiento/route.h"
#include "gurobi_c.h"

/** Optimiza las cargas de un avion */
void optimize_weight(Route* route);
