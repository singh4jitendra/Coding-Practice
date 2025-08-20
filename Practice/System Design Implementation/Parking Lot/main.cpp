#include "Parking_System.h"
#include <iostream>
#include <limits> // For numeric_limits
#include <string>
#include <chrono>
#include <thread> // For std::this_thread::sleep_for

// Helper function to format chrono::system_clock::time_point to string
std::string format_time_point(const std::chrono::system_clock::time_point& tp) {
    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    std::tm tm_buf;
    #ifdef _WIN32
        localtime_s(&tm_buf, &tt); // Safe version for Windows
    #else
        localtime_r(&tt, &tm_buf); // Safe version for POSIX
    #endif
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_buf);
    return buffer;
}

int main() {
    Parking_System parking_lot;

    parking_lot.display_availability();

    // --- Simulate parking a 4-wheeler ---
    std::cout << "\n--- Parking a 4-wheeler ---\n";
    parking_spot_record car1_details;
    car1_details.set_vehicle_type(FOUR_WHEELER);
    car1_details.set_vehicle_number("MH12AB1234");
    car1_details.set_entrance_gate_id(1);
    car1_details.set_per_hour_rate(20);
    car1_details.set_customer_name("Alice");

    int spot1_id = parking_lot.assign_parking_spot(FOUR_WHEELER, car1_details);
    if (spot1_id != -1) {
        std::cout << "Car 1 parked successfully at spot ID: " << spot1_id << std::endl;
    }
    parking_lot.display_availability();
    
    // Simulate some time passing
    std::cout << "\nSimulating 2 hours passing...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Use a short sleep for actual demo

    // --- Simulate parking a 2-wheeler ---
    std::cout << "\n--- Parking a 2-wheeler ---\n";
    parking_spot_record bike1_details;
    bike1_details.set_vehicle_type(TWO_WHEELER);
    bike1_details.set_vehicle_number("KA01CD5678");
    bike1_details.set_entrance_gate_id(2);
    bike1_details.set_per_hour_rate(10);
    bike1_details.set_customer_name("Bob");

    int spot2_id = parking_lot.assign_parking_spot(TWO_WHEELER, bike1_details);
    if (spot2_id != -1) {
        std::cout << "Bike 1 parked successfully at spot ID: " << spot2_id << std::endl;
    }
    parking_lot.display_availability();

    // Simulate more time passing
    std::cout << "\nSimulating 3 more hours passing (total 5 for car1, 3 for bike1)...\n";
    std::this_thread::sleep_for(std::chrono::seconds(3)); // Use a short sleep for actual demo

    // --- Simulate exiting vehicles ---
    std::cout << "\n--- Exiting vehicles ---\n";
    if (spot1_id != -1) {
        double bill1 = parking_lot.release_parking_spot(spot1_id);
        std::cout << "Car 1 bill: $" << bill1 << std::endl;
    }
    if (spot2_id != -1) {
        double bill2 = parking_lot.release_parking_spot(spot2_id);
        std::cout << "Bike 1 bill: $" << bill2 << std::endl;
    }
    parking_lot.display_availability();

    // --- Simulate another parking and immediate exit ---
    std::cout << "\n--- Parking and immediate exit ---\n";
    parking_spot_record car2_details;
    car2_details.set_vehicle_type(FOUR_WHEELER);
    car2_details.set_vehicle_number("UP65YZ9999");
    car2_details.set_entrance_gate_id(1);
    car2_details.set_per_hour_rate(25);
    car2_details.set_customer_name("Charlie");

    int spot3_id = parking_lot.assign_parking_spot(FOUR_WHEELER, car2_details);
    if (spot3_id != -1) {
        std::cout << "Car 2 parked successfully at spot ID: " << spot3_id << std::endl;
    }
    parking_lot.display_availability();
    
    // Immediate release to test minimum 1 hour charge
    if (spot3_id != -1) {
        double bill3 = parking_lot.release_parking_spot(spot3_id);
        std::cout << "Car 2 bill: $" << bill3 << std::endl;
    }
    parking_lot.display_availability();

    return 0;
}