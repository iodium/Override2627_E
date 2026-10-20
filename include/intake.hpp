#pragma once
#include "main.h"
#include "pros/misc.h"

void liftAscend(std::optional<int32_t> power = std::nullopt);
void liftDescend(std::optional<int32_t> power = std::nullopt);
void liftStop();
bool liftTooFar(bool checkingMin);