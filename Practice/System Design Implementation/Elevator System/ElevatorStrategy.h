#ifndef ELEVATOR_STRATEGY_H
#define ELEVATOR_STRATEGY_H

#include <vector>
#include <memory>
#include "LiftController.h"

class LiftController; // Forward declaration

class ElevatorStrategy {
public:
    virtual ~ElevatorStrategy() = default;
    virtual LiftController* findBestLift(const CallRequest& request, const std::vector<std::unique_ptr<LiftController>>& lifts) = 0;
};

#endif // ELEVATOR_STRATEGY_H