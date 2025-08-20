#include <iostream>

using namespace std;

//end product
class Flat{
    public:
        int floor;
        int rooms;
        int bathroom;
        string config;

        Flat(string conf)
        {
            this->config = conf;
        }
        void setfloor(int f) {floor = f;}
        void setrooms(int r) {rooms = r;}
        void setbathrooms(int b){bathroom =b;}

        int getfloor() {return floor;}
        int getrooms() {return rooms;}
        int getbathrooms(){return bathroom;}
        string getconfig() {return config;}

};


//flat build interface
class IFlatBuilder{
    protected:
        Flat* pflat;
    public:
        IFlatBuilder()
        {
            pflat=nullptr;
        }
        virtual void buildfloor()=0;
        virtual void buildrooms()=0;
        virtual void buildbathrooms()=0;
        virtual ~IFlatBuilder(){}
        Flat* getFlat()
        {
            return pflat;
        }
};


class Flat3bhk : public IFlatBuilder{
    public:
    void buildfloor() override
    { 
        pflat = new Flat("3bhk");      
    }
    void buildrooms()override{ pflat->setrooms(3);};
    void buildbathrooms()override{ pflat->setbathrooms(3);};
};


class Flat4bhk : public IFlatBuilder{
    public:
    void buildfloor() override
    { 
        pflat = new Flat("4bhk");      
    }
    void buildrooms()override{ pflat->setrooms(4);};
    void buildbathrooms()override{ pflat->setbathrooms(4);};
};

class Prestige{
    public:
        Flat* buildFlat(IFlatBuilder *flatbuilder)
        {
            flatbuilder->buildfloor();
            flatbuilder->buildrooms();
            flatbuilder->buildbathrooms();
            return flatbuilder->getFlat();
        }
};

int main()
{
    Prestige ppg;

    Flat3bhk flat3;
    Flat4bhk flat4;

    //Flat *fl3 = ppg.buildFlat(&flat3);
    //Flat *fl4 = ppg.buildFlat(&flat4);

    std::unique_ptr<Flat> fl3 = std::unique_ptr<Flat>(ppg.buildFlat(&flat3));
    std::unique_ptr<Flat> fl4 = std::unique_ptr<Flat>(ppg.buildFlat(&flat4));


        
    cout << fl3->getconfig() << endl;
    cout << fl4->getconfig() <<endl;

    return 0;
}