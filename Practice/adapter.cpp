/* #include <iostream>
#include <memory>
using namespace std;

// ---------------- Target Interface ----------------
class ICharger {
public:
    virtual void charge() = 0;
    virtual ~ICharger() = default;
};

// ---------------- Adapters with Hidden Adaptees ----------------
class IndianAdapter : public ICharger {
    class IndianCharger {   // Hidden adaptee
    public:
        void indian_charging() {
            cout << "✅ Device charging with Indian charger 🇮🇳" << endl;
        }
    } indian;

public:
    void charge() override {
        indian.indian_charging();  // Translate to common interface
    }
};

class USAAdapter : public ICharger {
    class USACharger {   // Hidden adaptee
    public:
        void usa_charging() {
            cout << "✅ Device charging with USA charger 🇺🇸" << endl;
        }
    } usa;

public:
    void charge() override {
        usa.usa_charging();  // Translate to common interface
    }
};

class GermanAdapter : public ICharger {
    class GermanCharger {   // Hidden adaptee
    public:
        void german_charging() {
            cout << "✅ Device charging with German charger 🇩🇪" << endl;
        }
    } german;

public:
    void charge() override {
        german.german_charging();  // Translate to common interface
    }
};

// ---------------- Client Code ----------------
void startCharging(ICharger* charger) {
    cout << "🔌 Plugging into universal charging interface...\n";
    charger->charge();  // Client calls common interface
}

int main() {
    // ⚠️ Let's say only USA charger is available
    //unique_ptr<ICharger> charger = make_unique<USAAdapter>();
    unique_ptr<ICharger> charger = make_unique<IndianAdapter>();


    // Client doesn't care which charger is used underneath
    startCharging(charger.get());

    return 0;
}
 */

/*  #include <iostream>
#include <memory>
#include <string>
using namespace std;

// ---------------- Target Interface ----------------
class ICharger {
public:
    virtual void charge() = 0;
    virtual ~ICharger() = default;
};

// ---------------- Adapters with Hidden Adaptees ----------------
class IndianAdapter : public ICharger {
    class IndianCharger {   // Hidden adaptee
    public:
        void indian_charging() {
            cout << "✅ Device charging with Indian charger 🇮🇳" << endl;
        }
    } indian;
public:
    void charge() override { indian.indian_charging(); }
};

class USAAdapter : public ICharger {
    class USACharger {   // Hidden adaptee
    public:
        void usa_charging() {
            cout << "✅ Device charging with USA charger 🇺🇸" << endl;
        }
    } usa;
public:
    void charge() override { usa.usa_charging(); }
};

class GermanAdapter : public ICharger {
    class GermanCharger {   // Hidden adaptee
    public:
        void german_charging() {
            cout << "✅ Device charging with German charger 🇩🇪" << endl;
        }
    } german;
public:
    void charge() override { german.german_charging(); }
};

// ---------------- Universal Adapter ----------------
class UniversalAdapter : public ICharger {
    unique_ptr<ICharger> charger;  // Holds any charger adapter
public:
    UniversalAdapter(const string& type) {
        if (type == "INDIAN") charger = make_unique<IndianAdapter>();
        else if (type == "USA") charger = make_unique<USAAdapter>();
        else if (type == "GERMAN") charger = make_unique<GermanAdapter>();
        else throw invalid_argument("Unknown charger type!");
    }

    void charge() override {
        if (charger) charger->charge();
        else cout << "No charger available!" << endl;
    }
};

// ---------------- Client ----------------
void startCharging(ICharger* charger) {
    cout << "🔌 Plugging into universal charging interface...\n";
    charger->charge();
}

int main() {
    try {
        // ⚡ Dynamically select charger type
        string chargerType = "USA";  // Can be changed to "INDIAN" or "GERMAN"
        unique_ptr<ICharger> universalCharger = make_unique<UniversalAdapter>(chargerType);

        // Client only interacts with ICharger
        startCharging(universalCharger.get());
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
} */



#include <iostream>
#include <memory>
#include <string>
using namespace std;

// ---------------- Target Interface ----------------
class ICharger {
public:
    virtual void charge() = 0;
    virtual ~ICharger() = default;
};

// ---------------- Adapters ----------------
class IndianAdapter : public ICharger {
    class IndianCharger {   // Legacy class
    public:
        void indian_charging() {
            cout << "✅ Device charging with Indian charger 🇮🇳" << endl;
        }
    } indian;

public:
    void charge() override { indian.indian_charging(); }
};

class USAAdapter : public ICharger {
    class USACharger {
    public:
        void usa_charging() {
            cout << "✅ Device charging with USA charger 🇺🇸" << endl;
        }
    } usa;

public:
    void charge() override { usa.usa_charging(); }
};

class GermanAdapter : public ICharger {
    class GermanCharger {
    public:
        void german_charging() {
            cout << "✅ Device charging with German charger 🇩🇪" << endl;
        }
    } german;

public:
    void charge() override { german.german_charging(); }
};


class PakAdapter : public ICharger {
    class PakCharger {
    public:
        void pak_charging() {
            cout << "✅ Device charging with Pak charger 🇩🇪" << endl;
        }
    } pak;

public:
    void charge() override { pak.pak_charging(); }
};


// ---------------- Universal Adapter ----------------
class UniversalAdapter : public ICharger {
    unique_ptr<ICharger> charger;
public:
    UniversalAdapter(unique_ptr<ICharger> c) : charger(move(c)) {}
    void charge() override { charger->charge(); }
};

// ---------------- Client ----------------
void startCharging(ICharger* charger) {
    cout << "🔌 Plugging into universal charging interface...\n";
    charger->charge();
}



int main() {
    // The client decides which adapter to use
    //unique_ptr<ICharger> adapter = make_unique<USAAdapter>();
    unique_ptr<ICharger> adapter = make_unique<PakAdapter>();
    adapter.get()->charge();


    // Wrap in Universal Adapter
    unique_ptr<ICharger> universal = make_unique<UniversalAdapter>(move(adapter));

    // Client calls through ICharger
    startCharging(universal.get());
}