# Elevator System Design

This document outlines the high-level and low-level design of a C++-based elevator dispatch and control system. It aims to efficiently manage multiple elevators in a building, responding to both external call requests and internal destination selections.

FR:
- user will be able to call lift.
- lift should go up/down
- each user request must have direction to go and location.
- Inside lift, option to select the floors
- Lift should go in roder of floors not the one who pressed first. 
- List system can manage 4 lifts at a time.
- If user call any lift button , nearby lifts which has capacity only should stop.
- Each lift must maintain capacity, total persons, availabe place, status, 
location, going direction.

NFR:
- Able to add/remove new lifts to system
- Each lift must have some id.

Back of the envelope:
- NA

---

## High-Level Design (HLD)

The system operates on a centralized dispatching model. 

* **User Interaction:** Users initiate requests by pressing UP/DOWN buttons on a floor or destination buttons inside an elevator.
* **Central `ElevatorSystem`:** This is the core component that receives all external call requests. It maintains global queues for UP and DOWN requests.
* **Dispatching Strategy:** The `ElevatorSystem` uses a configurable strategy to determine the "best" available `LiftController` for each incoming request.
* **Individual `LiftController`s:** Each physical elevator car is managed by its own `LiftController`. This controller maintains internal UP and DOWN queues for floors it needs to visit and manages the elevator's movement and state.
* **Communication Flow:**
    1.  External call requests are sent to the `ElevatorSystem`.
    2.  The `ElevatorSystem` places these requests into its central UP/DOWN queues.
    3.  A dispatching mechanism within `ElevatorSystem` picks requests from its central queues.
    4.  It consults the current `ElevatorStrategy` to identify the most suitable `LiftController`.
    5.  The selected `LiftController` receives the request and adds it to its internal queue.
    6.  Users inside a lift interact directly with their `LiftController` to select a destination floor.

---

## Low-Level Design (LLD)

### Classes and Their Responsibilities

* **`CallRequest` (Struct):**
    * Represents a request from outside an elevator (e.g., a person on floor 5 wants to go UP).
    * Contains: `floor` (the floor where the call originated) and `direction` (UP or DOWN).

* **`InsideLiftRequest` (Struct):**
    * Represents a destination request made by a passenger already inside an elevator.
    * Contains: `floor` (the destination floor).

* **`LiftController` (Class):**
    * Manages the state and movement of a **single physical elevator**.
    * **Attributes:** `lift_id`, `current_floor`, `current_status` (IDLE, MOVING_UP, MOVING_DOWN, DOORS_OPEN).
    * **Internal Queues:**
        * `up_queue`: A `min-heap` (`std::priority_queue<int, std::vector<int>, std::greater<int>>`) to store floors to visit in ascending order (lowest first) while moving up.
        * `down_queue`: A `max-heap` (`std::priority_queue<int>`) to store floors to visit in descending order (highest first) while moving down.
    * **Methods:**
        * `receiveCall(const CallRequest& request)`: Receives an external call request from `ElevatorSystem` and adds the floor to its internal `up_queue` or `down_queue`.
        * `addInsideRequest(const InsideLiftRequest& request)`: Receives a destination request from inside the lift and adds the floor to its internal `up_queue` or `down_queue`.
        * `processInternalQueue()`: The core logic for the elevator's movement (implements the **SCAN Algorithm**). It processes requests from its internal queues, moves the elevator, opens/closes doors, and updates its status. In a multi-threaded system, this would run continuously in a dedicated thread for each lift.
        * `getCurrentFloor()`, `getStatus()`, `getLiftId()`: Getters for its current state, used by the dispatching strategy.

* **`ElevatorStrategy` (Abstract Base Class/Interface):**
    * Defines the interface for **elevator dispatching algorithms**.
    * **Method:** `findBestLift(const CallRequest& request, const std::vector<std::unique_ptr<LiftController>>& lifts)`: A pure virtual method that concrete strategies must implement to determine the most suitable `LiftController` for a given `CallRequest`.

* **`OptimizedElevatorStrategy` (Concrete Strategy Class):**
    * An implementation of `ElevatorStrategy` that prioritizes lifts based on efficiency.
    * **Logic:**
        1.  **Pass 1:** Finds the most efficient lift currently `MOVING` in the correct `direction` and whose `current_floor` is "in the path" of the `request_floor` (e.g., moving up, and requested floor is above current floor). It picks the closest among these.
        2.  **Pass 2:** If no ideal candidate is found in Pass 1, it then looks for the `closest IDLE` lift.
        3.  **Pass 3:** As a last resort, if still no candidate, it picks the `physically closest` lift, regardless of its current direction.

* **`ElevatorSystem` (Singleton Class):**
    * The **centralized controller** for all elevators in the building.
    * Implements the **Singleton pattern** to ensure only one instance exists.
    * **Global Queues:**
        * `up_requests`: A `min-heap` for all external UP call requests.
        * `down_requests`: A `max-heap` for all external DOWN call requests.
    * **Methods:**
        * `getInstance()`: Returns the single instance of `ElevatorSystem`.
        * `registerLift(int lift_id)`: A **Factory Method** that creates and registers a new `LiftController` object, adding it to the system's list of `all_lifts`.
        * `setDispatcherStrategy(std::unique_ptr<ElevatorStrategy> strategy)`: Allows switching the dispatching algorithm at runtime using the **Strategy pattern**.
        * `callLift(int floor, CallRequest::Direction direction)`: Receives external call requests, places them into the central `up_requests` or `down_requests` queue, and then triggers `dispatchRequests()`.
        * `dispatchRequests()`: This method acts as the **central dispatching worker**. It pulls the next request from the central queues (prioritizing UP then DOWN for now), uses the `ElevatorStrategy` to find the best `LiftController`, and then commands that `LiftController` to `receiveCall()`.

---

## Design Patterns Used

* **Singleton Pattern:** Ensures a single, globally accessible instance of `ElevatorSystem`.
* **Factory Method Pattern:** Used by `ElevatorSystem` to create `LiftController` objects.
* **Strategy Pattern:** Allows for interchangeable dispatching algorithms (e.g., `OptimizedElevatorStrategy`) to be set in the `ElevatorSystem`.
* **Producer-Consumer Pattern (Implicit):** The central `ElevatorSystem` acts as a "producer" of tasks (dispatched requests) and `LiftController`s act as "consumers." This becomes more explicit with multithreading.
* **Observer Pattern (Conceptual/Initial Thought):** While not explicitly implemented with `std::observer`, the concept of the `ElevatorSystem` notifying lifts of requests was discussed, and the `receiveCall` method acts as the "update" when a lift is chosen.

---

## How to Compile and Run

To compile and run this C++ elevator system:

1.  **Save the files:**
    * `CallRequest.h`
    * `LiftController.h`
    * `ElevatorStrategy.h`
    * `OptimizedElevatorStrategy.h`
    * `ElevatorSystem.h`
    * `main.cpp`
2.  **Compile:** Use a C++ compiler (like g++):
    ```bash
    g++ main.cpp -o elevator_sim -std=c++17
    ```
    *(Note: Using `-std=c++17` or higher is recommended for `std::unique_ptr` and other modern C++ features.)*
3.  **Run:**
    ```bash
    ./elevator_sim
    ```

The program will then prompt you to enter lift calls in the console.

**Note:** For a truly dynamic and concurrent simulation that processes internal queues continuously, each `LiftController`'s `processInternalQueue` would need to run in its own separate thread, and the `ElevatorSystem`'s `dispatchRequests` would also run in a continuous thread. The current setup is a simplified sequential demonstration for clarity.