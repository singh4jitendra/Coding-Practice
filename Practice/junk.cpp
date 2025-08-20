#include <iostream>
#include <string>    // Added for std::string
#include <memory>    // For std::unique_ptr (modern C++ way for ownership)

using namespace std;

// End product
class Flat {
public:
    int floor;
    int rooms;
    int bathroom;
    string config_name; // Renamed to avoid parameter/member confusion

    // FIX 1: Correct constructor initialization using initializer list
    Flat(string conf_str) : config_name(conf_str), floor(0), rooms(0), bathroom(0)
    {
        // Member variables are now correctly initialized
        // No need for 'config = conf;' in the body with initializer list
    }

    void setfloor(int f) { floor = f; }
    void setrooms(int r) { rooms = r; }
    void setbathrooms(int b) { bathroom = b; }

    int getfloor() { return floor; }
    int getrooms() { return rooms; }
    int getbathrooms() { return bathroom; }
    string getconfig() { return config_name; } // Returns the correctly initialized member

    // Optional: for better output of the entire config
    string toString() const {
        return "Flat Config: " + config_name + ", Floor: " + to_string(floor) +
               ", Rooms: " + to_string(rooms) + ", Bathrooms: " + to_string(bathroom);
    }
};


// Flat build interface (Abstract Builder)
class IFlatBuilder {
public:
    // FIX 3: Initialize pflat to nullptr
    IFlatBuilder() : pflat(nullptr) {}

    // FIX 4: Add virtual destructor for proper cleanup
    virtual ~IFlatBuilder() {
        // If the builder were responsible for owning the Flat object *after* buildFlat returns,
        // you'd delete pflat here. In this case, Prestige::buildFlat takes ownership by returning the raw pointer,
        // so main() is responsible. However, a virtual destructor is essential for base classes.
    }

    // Pure virtual functions
    virtual void buildfloor() = 0;
    virtual void buildrooms() = 0;
    virtual void voidbuildbathrooms() = 0; // Renamed to avoid potential conflict if any

    Flat* getFlat() {
        return pflat;
    }

protected: // Made pflat protected for derived class access
    Flat* pflat;
};


// Concrete Builder for 3BHK
class Flat3bhk : public IFlatBuilder {
public:
    void buildfloor() override // Use override keyword for clarity and safety
    {
        pflat = new Flat("3bhk"); // Create new Flat object
        pflat->setfloor(3);       // FIX 5: Set specific floor for 3bhk
    }
    void buildrooms() override { pflat->setrooms(3); }
    void voidbuildbathrooms() override { pflat->setbathrooms(2); } // Assuming 2 bathrooms for 3bhk
};


// Concrete Builder for 4BHK
class Flat4bhk : public IFlatBuilder {
public:
    void buildfloor() override // Use override keyword
    {
        pflat = new Flat("4bhk"); // Create new Flat object
        pflat->setfloor(4);       // FIX 5: Set specific floor for 4bhk
    }
    void buildrooms() override { pflat->setrooms(4); }
    void voidbuildbathrooms() override { pflat->setbathrooms(3); } // Assuming 3 bathrooms for 4bhk
};

// Director
class Prestige {
public:
    // FIX 7: Removed unused member 'flatbuilder'

    // This function orchestrates the building and returns the constructed Flat object.
    // Consider returning std::unique_ptr<Flat> for automatic memory management.
    Flat* buildFlat(IFlatBuilder* flatbuilder)
    {
        flatbuilder->buildfloor();    // This will call 'new Flat()' inside the builder
        flatbuilder->buildrooms();
        flatbuilder->voidbuildbathrooms(); // Call the corrected method
        return flatbuilder->getFlat(); // Return the raw pointer
    }
};

int main()
{
    Prestige ppg;

    Flat3bhk flat3_builder; // Create instances of concrete builders
    Flat4bhk flat4_builder;

    // Build the Flat objects using the builders via the director
    // FIX 8: Use std::unique_ptr for automatic memory management
    std::unique_ptr<Flat> fl3 = std::unique_ptr<Flat>(ppg.buildFlat(&flat3_builder));
    std::unique_ptr<Flat> fl4 = std::unique_ptr<Flat>(ppg.buildFlat(&flat4_builder));

    // Print the configurations using the toString() method for full details
    cout << fl3->toString() << endl;
    cout << fl4->toString() << endl;

    // No need for 'delete fl3;' and 'delete fl4;' due to std::unique_ptr
    // The Flats will be automatically deleted when fl3 and fl4 go out of scope.

    return 0;
}