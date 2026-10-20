#include "main.h"
#include <cstdint> // for int32_t

const double UPPER_BOUND = 2;
const double LOWER_BOUND = 2;

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

bool liftTooFar(bool checkingMin) {
    std::vector<double> positions = lift.get_position_all();
    double max = 360 * UPPER_BOUND;
    double min = -360 * LOWER_BOUND + 20;
    double avg = 0;
    for (double p : positions) {
        avg += p;
    }
    avg = avg / positions.size();

    if (checkingMin) {
        if (avg < min) {
            return true;
        }
    }
    else {
        if (avg > max) {
            return true;
        }
    }
    return false;
}

// class Lift {
// public:
//     Lift(pros::v5::Motor m1, pros::v5::Motor m2,
//          pros::v5::Motor m3, double upper, double lower) 
//         : m1_(m1),
//           m2_(m2),
//           m3_(m3),
//           upper_(upper),
//           lower_(lower) {}

// private:
//     pros::v5::Motor m1_;
//     pros::v5::Motor m2_;
//     pros::v5::Motor m3_;
//     const double upper_;
//     const double lower_;
// };

