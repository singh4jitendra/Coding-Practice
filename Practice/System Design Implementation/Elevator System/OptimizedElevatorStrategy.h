#include "ElevatorStrategy.h"
#include <climits>
#include <cmath>

class OptimizedElevatorStrategy : public ElevatorStrategy {
public:
    LiftController* findBestLift(const CallRequest& request, const std::vector<std::unique_ptr<LiftController>>& lifts) override {
        LiftController* best_candidate = nullptr;
        int min_distance = INT_MAX;

        // Pass 1: Find the most efficient lift (moving in the same direction)
        for (const auto& lift : lifts) {
            bool is_in_path = false;
            if (request.direction == CallRequest::UP && lift->getStatus() == LiftController::Status::MOVING_UP && lift->getCurrentFloor() <= request.floor) {
                is_in_path = true;
            } else if (request.direction == CallRequest::DOWN && lift->getStatus() == LiftController::Status::MOVING_DOWN && lift->getCurrentFloor() >= request.floor) {
                is_in_path = true;
            }

            if (is_in_path) {
                int distance = std::abs(lift->getCurrentFloor() - request.floor);
                if (distance < min_distance) {
                    min_distance = distance;
                    best_candidate = lift.get();
                }
            }
        }

        // Pass 2: If no ideal candidate is found, find the closest idle lift
        if (best_candidate == nullptr) {
            min_distance = INT_MAX;
            for (const auto& lift : lifts) {
                if (lift->getStatus() == LiftController::Status::IDLE) {
                    int distance = std::abs(lift->getCurrentFloor() - request.floor);
                    if (distance < min_distance) {
                        min_distance = distance;
                        best_candidate = lift.get();
                    }
                }
            }
        }

        // Pass 3: If still no candidate, find the closest lift regardless of direction
        if (best_candidate == nullptr) {
            min_distance = INT_MAX;
            for (const auto& lift : lifts) {
                int distance = std::abs(lift->getCurrentFloor() - request.floor);
                if (distance < min_distance) {
                    min_distance = distance;
                    best_candidate = lift.get();
                }
            }
        }

        return best_candidate;
    }
};