#include <iostream>
#include <memory>
using namespace std;

// Interfcae to create lift  classess using abstract factory
class Interface_Lifts
{
    public:
        int lift_id;
        string lift_company;
        int lift_capacity;
        enum lift_status {DOOR_OPEN=0, DOOR_CLOSE, GOING_UP, GOING_DOWN, IDLE, NOT_WORKING};
        priority_queue<Lift_class, greater, Lift_class> UP; //min heap
        priority_queue<Lift_class> DOWN;
};


class Lift_class : public Interface_Lifts{
 // < What to keep here, not sure >
};


// This will be having request obhect when user when call lift 
class Call_lift{
    int requested_from_floor;
    enum direction {UP=0, DOWN};
};


// This will be having request obhect when user entered to lift and now want to press floor.
class Inside_lift_request_floor{
    int requested_floor;
};


// Centralized system to allow new lifts to register under it so that it can manage.
class Elevator_system{

    private:
        static Elevator_system *obj;
        vector<Lift_class> all_lifts;
        Elevator_system()
        {};
    public:
        static Elevator_system* get_elevator_obj()
        {
            if(obj == nullptr)
            {
                obj= new Elevator_system();
            }
            return obj;
        }
        priority_queue<Call_lift, greater, Lift_class> UP;  //min heap
        priority_queue<Call_lift> DOWN;  // max heap
        void register_lift(Lift_class new_lift)
        {
            all_lifts.push_back(new_lift);

        } 

};
