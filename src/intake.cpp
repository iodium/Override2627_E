#include "main.h"
#include <cstdint> // for int32_t

void liftAscend(std::optional<int32_t> power) {
    int32_t p = power.value_or(127);
    lift.move(p);
}

void liftDescend(std::optional<int32_t> power) {
    int32_t p = power.value_or(-127);
    lift.move(p);
}

void liftStop() {
    lift.move(0);
}