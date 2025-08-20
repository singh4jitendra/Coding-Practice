#ifndef LIFT_CONTROLLER_H
#define LIFT_CONTROLLER_H

#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <functional> // For std::greater
#include <cmath>      // For std::abs
#include "CallRequest.h"

class LiftController {
private:
    int lift_id;
    int current_floor;

    
    // Min-heap for UP requests (sorts by floor number, lowest first)
    std::priority_queue<int, std::vector<int>, std::greater<int>> up_queue;

    // Max-heap for DOWN requests (sorts by floor number, highest first)
    std::priority_queue<int> down_queue;

public:
    enum Status { IDLE, MOVING_UP, MOVING_DOWN, DOORS_OPEN };
    Status current_status;
    LiftController(int id, int initial_floor = 1) : lift_id(id), current_floor(initial_floor), current_status(IDLE) {}
    
    // Public getters for the dispatcher
    int getCurrentFloor() const { return current_floor; }
    Status getStatus() const { return current_status; }
    
    // This method is a direct command from the central dispatcher
    void receiveCall(const CallRequest& request) {
        if (request.direction == CallRequest::UP) {
            up_queue.push(request.floor);
        } else {
            down_queue.push(request.floor);
        }
        processInternalQueue();
    }
    
    // Adds a floor request from inside the lift
  void addInsideRequest(const InsideLiftRequest& request) { // Changed parameter type
        if (request.floor > current_floor) {
            up_queue.push(request.floor);
        } else if (request.floor < current_floor) {
            down_queue.push(request.floor);
        }
        processInternalQueue();
    }
// In LiftController.h, inside public:
int getLiftId() const { return lift_id; }

private:
    // This private method handles the lift's own movement logic
    void processInternalQueue() {
        if (current_status == IDLE) {
            if (!up_queue.empty()) {
                current_status = MOVING_UP;
                int next_floor = up_queue.top();
                up_queue.pop();
                std::cout << "Lift " << lift_id << " moving up to floor " << next_floor << std::endl;
                current_floor = next_floor;
                current_status = DOORS_OPEN;
                // Simulating door opening and closing time
                current_status = IDLE; 
            } else if (!down_queue.empty()) {
                current_status = MOVING_DOWN;
                int next_floor = down_queue.top();
                down_queue.pop();
                std::cout << "Lift " << lift_id << " moving down to floor " << next_floor << std::endl;
                current_floor = next_floor;
                current_status = DOORS_OPEN;
                current_status = IDLE;
            }
        }
    }
};

#endif // LIFT_CONTROLLER_H