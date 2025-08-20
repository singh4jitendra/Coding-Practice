#include <iostream>

using namespace std;

class XML{

    string xmldata;
    public:
        XML(string data)
        {
            xmldata = data;
        }

    string  get()
    {
        return xmldata;
    }
};

class Tool{

    string jsondata;
    public:
        Tool(){};
        Tool(string data)
        {
            jsondata = data;
        }

    virtual void analyze()
    {
        cout << "Analysizing JSON data" << jsondata <<endl;
    }
};

class Client{

    public:
        void analytics(Tool* data)
        {
            cout <<"Client proicesinfg json data" <<endl;
            data->analyze();

        }
};

class Adapter : public Tool{

    XML *xmlbobj;
    public: 
    Adapter(XML *argxmlobj)
    {
        xmlbobj = argxmlobj;
    }
    void analyze()
    {
        cout <<"Now do some proccesing to convert xml data to json data" <<endl;
        Tool::analyze();
    }

};

int main()
{
    XML* xmlobj = new XML("Sample XML data");
    
    Tool *toolobj = new Adapter(xmlobj);

    Client cl;
    cl.analytics(toolobj);
    return 0;
}


