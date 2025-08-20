#include "ElevatorSystem.h" // Includes all necessary headers
#include <iostream>
#include <string>

int main() {
    // Get the singleton instance of the elevator system
    ElevatorSystem* system = ElevatorSystem::getInstance();

    // Register a few lifts
    system->registerLift(1);
    system->registerLift(2);
    system->registerLift(3);

    // Initial State Output
    std::cout << "--- Elevator System Initialized ---\n";
    for (const auto& lift_ptr : system->getLifts()) {
        std::cout << "Lift " << lift_ptr->getLiftId()
                  << " is at Floor: " << lift_ptr->getCurrentFloor()
                  << " with Status: IDLE" << std::endl;
    }
    std::cout << "-----------------------------------\n";

    // Main interactive loop
    std::string user_input;
    while (true) {
        int floor_request;
        char direction_char;
        CallRequest::Direction direction;
        
        std::cout << "\nEnter a lift call (e.g., '5 U' for floor 5, UP): ";
        std::cin >> floor_request >> direction_char;

        // Validate user input
        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number followed by 'U' or 'D'.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        direction_char = std::toupper(direction_char);
        if (direction_char == 'U') {
            direction = CallRequest::UP;
        } else if (direction_char == 'D') {
            direction = CallRequest::DOWN;
        } else {
            std::cout << "Invalid direction. Please use 'U' for UP or 'D' for DOWN.\n";
            continue;
        }

        if (floor_request < 1 || floor_request > 10) { // Assuming a 10-story building
            std::cout << "Invalid floor. Please enter a floor between 1 and 10.\n";
            continue;
        }
        
        // Process the lift call
        system->callLift(floor_request, direction);
        
        std::cout << "\n--- Current Lift States ---\n";
        for (const auto& lift_ptr : system->getLifts()) {
            std::cout << "Lift " << lift_ptr->getLiftId()
                      << " is at Floor: " << lift_ptr->getCurrentFloor()
                      << " with Status: " << lift_ptr->getStatus() << std::endl;
        }
        std::cout << "---------------------------\n";

        // Optional: Add a check for inside-lift requests
        // This is a bit tricky to simulate in an external loop without more state,
        // so we'll stick to external calls for this interactive demo.
    }

    return 0;
}

/*
(base) jitendrasingh@Jitendras-MacBook-Pro Elevator System % ./main 
Lift 1 registered.
Lift 2 registered.
Lift 3 registered.
--- Elevator System Initialized ---
Lift 1 is at Floor: 1 with Status: IDLE
Lift 2 is at Floor: 1 with Status: IDLE
Lift 3 is at Floor: 1 with Status: IDLE
-----------------------------------

Enter a lift call (e.g., '5 U' for floor 5, UP): 3 D
Lift 1 moving down to floor 3

--- Current Lift States ---
Lift 1 is at Floor: 3 with Status: 0
Lift 2 is at Floor: 1 with Status: 0
Lift 3 is at Floor: 1 with Status: 0
---------------------------

Enter a lift call (e.g., '5 U' for floor 5, UP): 4 U
Lift 1 moving up to floor 4

--- Current Lift States ---
Lift 1 is at Floor: 4 with Status: 0
Lift 2 is at Floor: 1 with Status: 0
Lift 3 is at Floor: 1 with Status: 0
---------------------------

Enter a lift call (e.g., '5 U' for floor 5, UP): 3 D
Lift 1 moving down to floor 3

--- Current Lift States ---
Lift 1 is at Floor: 3 with Status: 0
Lift 2 is at Floor: 1 with Status: 0
Lift 3 is at Floor: 1 with Status: 0
---------------------------

Enter a lift call (e.g., '5 U' for floor 5, UP): 2 U
Lift 1 moving up to floor 2

--- Current Lift States ---
Lift 1 is at Floor: 2 with Status: 0
Lift 2 is at Floor: 1 with Status: 0
Lift 3 is at Floor: 1 with Status: 0
---------------------------

Enter a lift call (e.g., '5 U' for floor 5, UP): 1 UP
Lift 2 moving up to floor 1

--- Current Lift States ---
Lift 1 is at Floor: 2 with Status: 0
Lift 2 is at Floor: 1 with Status: 0
Lift 3 is at Floor: 1 with Status: 0
---------------------------

Enter a lift call (e.g., '5 U' for floor 5, UP): Invalid input. Please enter a number followed by 'U' or 'D'.

Enter a lift call (e.g., '5 U' for floor 5, UP): 1 U
Lift 2 moving up to floor 1

--- Current Lift States ---
Lift 1 is at Floor: 2 with Status: 0
Lift 2 is at Floor: 1 with Status: 0
Lift 3 is at Floor: 1 with Status: 0
---------------------------

Enter a lift call (e.g., '5 U' for floor 5, UP): 
*/