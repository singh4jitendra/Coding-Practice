#ifndef CLOSEST_ELEVATOR_STRATEGY_H
#define CLOSEST_ELEVATOR_STRATEGY_H

#include "ElevatorStrategy.h"
#include <climits>
#include <cmath>

class ClosestElevatorStrategy : public ElevatorStrategy {
public:
    LiftController* findBestLift(const CallRequest& request, const std::vector<std::unique_ptr<LiftController>>& lifts) override {
        LiftController* best_lift = nullptr;
        int min_distance = INT_MAX;

        for (const auto& lift : lifts) {
            // This simple strategy only considers idle lifts
            if (lift->getStatus() == LiftController::Status::IDLE) {
                int distance = std::abs(lift->getCurrentFloor() - request.floor);
                if (distance < min_distance) {
                    min_distance = distance;
                    best_lift = lift.get();
                }
            }
        }
        return best_lift;
    }
};

#endif // CLOSEST_ELEVATOR_STRATEGY_H