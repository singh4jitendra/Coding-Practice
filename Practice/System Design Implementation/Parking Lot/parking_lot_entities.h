#ifndef PARKING_LOT_ENTITIES_H
#define PARKING_LOT_ENTITIES_H

#include <string>
#include <ctime>   // For time_t
#include <chrono>  // For std::chrono

// Enum for vehicle types
enum vehicle_types {TWO_WHEELER = 0, FOUR_WHEELER};

// Struct to hold details of a parked vehicle (receipt info)
class parking_spot_record {
public:
    vehicle_types vehicle_type;
    std::string vehicle_number;
    std::chrono::system_clock::time_point entry_time; // Using chrono for time
    std::chrono::system_clock::time_point exit_time;  // Using chrono for time
    int entrance_gate_id;
    int per_hour_rate;
    std::string customer_name; // Optional

    parking_spot_record() : vehicle_type(FOUR_WHEELER), entrance_gate_id(0), per_hour_rate(0) {} // Default constructor

    // Setters (using const std::string& for efficiency)
    void set_vehicle_type(vehicle_types type) { vehicle_type = type; }
    void set_vehicle_number(const std::string& number) { vehicle_number = number; }
    void set_entry_time(std::chrono::system_clock::time_point time) { entry_time = time; }
    void set_exit_time(std::chrono::system_clock::time_point time) { exit_time = time; }
    void set_entrance_gate_id(int gate) { entrance_gate_id = gate; }
    void set_per_hour_rate(int rate) { per_hour_rate = rate; }
    void set_customer_name(const std::string& name) { customer_name = name; }

    // Getters
    vehicle_types get_vehicle_type() const { return vehicle_type; }
    std::string get_vehicle_number() const { return vehicle_number; }
    std::chrono::system_clock::time_point get_entry_time() const { return entry_time; }
    std::chrono::system_clock::time_point get_exit_time() const { return exit_time; }
    int get_entrance_gate_id() const { return entrance_gate_id; }
    int get_per_hour_rate() const { return per_hour_rate; }
    std::string get_customer_name() const { return customer_name; }
};

// Class representing a physical parking spot
class Parking_spot {
public:
    int id; // Integer ID for consistency with heaps
    bool is_available;
    vehicle_types spot_type; // Type of vehicle this spot can accommodate

    // Default constructor for array initialization
    Parking_spot() : id(0), is_available(true), spot_type(FOUR_WHEELER) {} 

    // Constructor to set initial properties
    Parking_spot(int _id, vehicle_types _type) : id(_id), is_available(true), spot_type(_type) {}
    
    // Setters
    void set_id(int _id) { id = _id; }
    void set_is_available(bool available) { is_available = available; }
    void set_spot_type(vehicle_types type) { spot_type = type; }

    // Getters
    int get_id() const { return id; }
    bool get_is_available() const { return is_available; }
    vehicle_types get_spot_type() const { return spot_type; }
};

#endif // PARKING_LOT_ENTITIES_H