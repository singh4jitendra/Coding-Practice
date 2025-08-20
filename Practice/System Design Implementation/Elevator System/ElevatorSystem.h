#ifndef ELEVATOR_SYSTEM_H
#define ELEVATOR_SYSTEM_H

#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <functional>
#include "LiftController.h"
#include "ElevatorStrategy.h"
#include "ClosestElevatorStrategy.h"

class ElevatorSystem {
private:
    static ElevatorSystem* instance;
    std::vector<std::unique_ptr<LiftController>> all_lifts;
    std::unique_ptr<ElevatorStrategy> dispatcher_strategy;

    // Central queues for all incoming requests
    std::priority_queue<int, std::vector<int>, std::greater<int>> up_requests; // Min-heap
    std::priority_queue<int> down_requests; // Max-heap

    ElevatorSystem() {
        dispatcher_strategy = std::make_unique<ClosestElevatorStrategy>();
    }

public:
    static ElevatorSystem* getInstance() {
        if (instance == nullptr) {
            instance = new ElevatorSystem();
        }
        return instance;
    }
    
    // Factory Method to create and register lifts
    void registerLift(int lift_id) {
        all_lifts.push_back(std::make_unique<LiftController>(lift_id));
        std::cout << "Lift " << lift_id << " registered." << std::endl;
    }

    void setDispatcherStrategy(std::unique_ptr<ElevatorStrategy> strategy) {
        dispatcher_strategy = std::move(strategy);
    }
    
    // The public method called when a lift is requested
    void callLift(int floor, CallRequest::Direction direction) {
        if (direction == CallRequest::UP) {
            up_requests.push(floor);
        } else {
            down_requests.push(floor);
        }
        dispatchRequests();
    }
        const std::vector<std::unique_ptr<LiftController>>& getLifts() const {
        return all_lifts;
    }

private:
    void dispatchRequests() {
        if (!up_requests.empty()) {
            int floor = up_requests.top();
            CallRequest request = {floor, CallRequest::UP};
            
            LiftController* best_lift = dispatcher_strategy->findBestLift(request, all_lifts);
            if (best_lift != nullptr) {
                best_lift->receiveCall(request);
                up_requests.pop();
            }
        } else if (!down_requests.empty()) {
            int floor = down_requests.top();
            CallRequest request = {floor, CallRequest::DOWN};
            
            LiftController* best_lift = dispatcher_strategy->findBestLift(request, all_lifts);
            if (best_lift != nullptr) {
                best_lift->receiveCall(request);
                down_requests.pop();
            }
        }
    }
};

ElevatorSystem* ElevatorSystem::instance = nullptr;

#endif // ELEVATOR_SYSTEM_H