// Copyright Mihai Preda
#pragma once

#include "common.h"

#include <vector>
#include <tuple>

using std::vector;

// Generate a P-1 second stage plan.
// that covers the primes in (B1, B2], with a block of size D.
// Returns the index of the first block, the total nb. of points selected, and vectors of selected bits per block.
std::tuple<u32, u32, vector<vector<bool>>> makePm1Plan(u32 D, u32 B1, u32 B2);
