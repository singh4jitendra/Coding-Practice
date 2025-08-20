#ifndef PARKING_SYSTEM_H
#define PARKING_SYSTEM_H

#include <string>
#include <queue>
#include <iostream>
#include <vector>
#include <map>        // To store occupied spot data
#include <mutex>      // For thread safety
#include <cmath>      // For std::abs if needed
#include <algorithm>  // For std::min, std::max
#include <iomanip>    // For std::put_time

#include "parking_lot_entities.h" // Include the entity definitions

// Constants for parking lot dimensions
const int TOTAL_FLOORS = 4;
const int ROWS_PER_FLOOR = 3;
const int COLS_PER_ROW = 10; // Total spots per row
const int TWO_WHEELER_FLOORS = 2; // Floors 0 and 1 for two-wheelers

class Parking_System {
public:
    Parking_System() {
        // Initialize parking_db (physical layout)
        // Spots on floors 0 and 1 are for TWO_WHEELER, rest for FOUR_WHEELER
        for (int floor = 0; floor < TOTAL_FLOORS; ++floor) {
            for (int row = 0; row < ROWS_PER_FLOOR; ++row) {
                for (int col = 0; col < COLS_PER_ROW; ++col) {
                    int spot_id = generate_spot_id(floor, row, col);
                    vehicle_types type = (floor < TWO_WHEELER_FLOORS) ? TWO_WHEELER : FOUR_WHEELER;
                    parking_db[floor][row][col] = Parking_spot(spot_id, type);
                    
                    // Add spot IDs to respective available heaps
                    if (type == TWO_WHEELER) {
                        available_two_wheeler_spots.push(spot_id);
                    } else {
                        available_four_wheeler_spots.push(spot_id);
                    }
                }
            }
        }
        std::cout << "Parking System initialized with " 
                  << available_two_wheeler_spots.size() << " 2-wheeler spots and "
                  << available_four_wheeler_spots.size() << " 4-wheeler spots." << std::endl;
    }

    // Displays current parking availability (simplified)
    void display_availability() {
        std::cout << "\n--- Parking Availability ---\n";
        // Iterate through physical spots, not just heaps
        for (int floor = 0; floor < TOTAL_FLOORS; ++floor) {
            std::cout << "Floor " << floor << " (" 
                      << ((floor < TWO_WHEELER_FLOORS) ? "2-Wheeler" : "4-Wheeler") << "):\n";
            for (int row = 0; row < ROWS_PER_FLOOR; ++row) {
                std::cout << "  Row " << row << ": ";
                for (int col = 0; col < COLS_PER_ROW; ++col) {
                    const Parking_spot& spot = parking_db[floor][row][col];
                    if (spot.get_is_available()) {
                        std::cout << "A "; // Available
                    } else {
                        std::cout << "O "; // Occupied
                    }
                    if (col % 20 == 19) std::cout << "| "; // Visual separator
                }
                std::cout << std::endl;
            }
            std::cout << "--------------------------------\n";
        }
        std::cout << "Available 2-wheeler spots: " << available_two_wheeler_spots.size() << std::endl;
        std::cout << "Available 4-wheeler spots: " << available_four_wheeler_spots.size() << std::endl;
        std::cout << "--------------------------------\n";
    }

    // Assigns a parking spot to a vehicle
    // Returns spot ID if successful, -1 otherwise
    int assign_parking_spot(vehicle_types vehicle_type, const parking_spot_record& record_details) {
        std::lock_guard<std::mutex> lock(mtx); // Protect critical section

        int assigned_spot_id = -1;
        std::priority_queue<int, std::vector<int>, std::greater<int>>* target_heap = nullptr;

        if (vehicle_type == TWO_WHEELER) {
            target_heap = &available_two_wheeler_spots;
        } else {
            target_heap = &available_four_wheeler_spots;
        }

        if (target_heap && !target_heap->empty()) {
            assigned_spot_id = target_heap->top();
            target_heap->pop();

            // Derive floor, row, col from spot_id
            int floor = assigned_spot_id / (ROWS_PER_FLOOR * COLS_PER_ROW);
            int remaining = assigned_spot_id % (ROWS_PER_FLOOR * COLS_PER_ROW);
            int row = remaining / COLS_PER_ROW;
            int col = remaining % COLS_PER_ROW;

            // Update the Parking_spot in the 3D array
            parking_db[floor][row][col].set_is_available(false);
            
            // Store the record details for this occupied spot
            occupied_spot_records[assigned_spot_id] = record_details;
            occupied_spot_records[assigned_spot_id].set_entry_time(std::chrono::system_clock::now()); // Set actual entry time

            std::cout << "Assigned " << (vehicle_type == TWO_WHEELER ? "2-wheeler" : "4-wheeler")
                      << " spot ID: " << assigned_spot_id 
                      << " (Floor: " << floor << ", Row: " << row << ", Col: " << col << ")" << std::endl;
        } else {
            std::cout << "No available spots for " 
                      << (vehicle_type == TWO_WHEELER ? "2-wheeler" : "4-wheeler") << " vehicles." << std::endl;
        }
        return assigned_spot_id;
    }

    // Frees up a parking spot when a vehicle exits
    // Returns the calculated bill amount, or -1 if spot not found/already free
    double release_parking_spot(int spot_id) {
        std::lock_guard<std::mutex> lock(mtx); // Protect critical section

        double bill_amount = -1.0;

        // Derive floor, row, col from spot_id
        int floor = spot_id / (ROWS_PER_FLOOR * COLS_PER_ROW);
        int remaining = spot_id % (ROWS_PER_FLOOR * COLS_PER_ROW);
        int row = remaining / COLS_PER_ROW;
        int col = remaining % COLS_PER_ROW;

        // Basic validation of spot ID
        if (floor < 0 || floor >= TOTAL_FLOORS || row < 0 || row >= ROWS_PER_FLOOR || col < 0 || col >= COLS_PER_ROW) {
            std::cout << "Invalid spot ID: " << spot_id << std::endl;
            return -1.0;
        }

        Parking_spot& spot = parking_db[floor][row][col];

        if (!spot.get_is_available()) {
            spot.set_is_available(true); // Mark as available
            
            // Add back to the correct available heap
            if (spot.get_spot_type() == TWO_WHEELER) {
                available_two_wheeler_spots.push(spot_id);
            } else {
                available_four_wheeler_spots.push(spot_id);
            }

            // Retrieve and remove record details
            auto it = occupied_spot_records.find(spot_id);
            if (it != occupied_spot_records.end()) {
                parking_spot_record record = it->second;
                record.set_exit_time(std::chrono::system_clock::now()); // Set actual exit time

                // Calculate duration and bill
                auto duration = std::chrono::duration_cast<std::chrono::hours>(record.get_exit_time() - record.get_entry_time());
                long long total_hours = duration.count();
                if (total_hours == 0 && (record.get_exit_time() - record.get_entry_time()).count() > 0) {
                     total_hours = 1; // Minimum 1 hour charge if parked for any duration
                } else if (total_hours == 0) {
                     // Handle case where entry and exit time are same (e.g., for testing, or very quick exit)
                     total_hours = 1; 
                }

                bill_amount = total_hours * record.get_per_hour_rate();
                
                std::cout << "Vehicle " << record.get_vehicle_number() << " exited spot " << spot_id 
                          << ". Parked for " << total_hours << " hours. Bill: $" << bill_amount << std::endl;
                
                occupied_spot_records.erase(it); // Remove from occupied records
            } else {
                std::cout << "Error: Spot " << spot_id << " was marked occupied but no record found." << std::endl;
            }
        } else {
            std::cout << "Spot " << spot_id << " is already available." << std::endl;
        }
        return bill_amount;
    }

    // Admin function: Add a new floor (simplified - assumes extending arrays)
    void add_floor(int floor_number, vehicle_types default_type) {
        std::lock_guard<std::mutex> lock(mtx);
        // In a real system, adding floors dynamically to a C-style array is not feasible.
        // You'd need std::vector<std::vector<std::vector<Parking_spot>>> or dynamic allocation.
        // For this fixed-size array, this would mean re-initializing the whole system or
        // having pre-allocated space.
        std::cout << "Admin: Attempting to add floor " << floor_number << ". (Dynamic floor addition not fully implemented for fixed array size).\n";
        // If parking_db was std::vector, you could resize and add new spots
        // For demonstration, we'll just acknowledge.
    }

    // Admin function: Remove a floor (simplified)
    void remove_floor(int floor_number) {
        std::lock_guard<std::mutex> lock(mtx);
        // Similar to add_floor, removing from a fixed array is complex.
        // Would need to check if floor is empty first in a real system.
        std::cout << "Admin: Attempting to remove floor " << floor_number << ". (Dynamic floor removal not fully implemented for fixed array size).\n";
    }

private:
    // Helper to generate a unique integer ID for a parking spot
    int generate_spot_id(int floor, int row, int col) {
        // Example ID: FFFRRCC (Floor * 10000 + Row * 100 + Col)
        return floor * (ROWS_PER_FLOOR * COLS_PER_ROW) + row * COLS_PER_ROW + col;
    }

    // Mutex for thread-safe access to shared data (heaps and parking_db)
    std::mutex mtx; 

    // Heaps to store available spot IDs for quick retrieval (lowest floor first)
    std::priority_queue<int, std::vector<int>, std::greater<int>> available_two_wheeler_spots;
    std::priority_queue<int, std::vector<int>, std::greater<int>> available_four_wheeler_spots;

    // 3D array representing the physical parking lot layout
    Parking_spot parking_db[TOTAL_FLOORS][ROWS_PER_FLOOR][COLS_PER_ROW];

    // Map to store vehicle details for currently occupied spots (Spot ID -> Record)
    std::map<int, parking_spot_record> occupied_spot_records;
};

#endif // PARKING_SYSTEM_H