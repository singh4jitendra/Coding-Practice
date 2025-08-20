#include <iostream>

using namespace std;

class INotify
{
public:
    virtual void notify(string notice) = 0;
};
class User : public INotify
{
public:
    string uname;
    User(string name)
    {
        uname = name;
    }
    void notify(string notice)
    {
        cout << "Received notice for group : " << notice << endl;
    }
};

class Group
{
private:
    vector<User *> users;

public:
    string gname;
    Group(string name)
    {
        gname = name;
    }
    void subscribe(User *uobj)
    {
        users.push_back(uobj);
        cout << "User id : " << uobj->uname << endl;
    }
    void unsubscribe(User *uobj)
    {
        auto it = find(users.begin(), users.end(), uobj);
        if (it != users.end())
        {
            users.erase(it);
        }
        cout << "User id : " << uobj->uname << "unsuncribed " << endl;
    }
    void notify(string notice)
    {
        for (auto user : users)
        {
            user->notify(notice);
        }
    }
};

int main()
{
    Group *group = new Group("Playes");

    User *user1 = new User("user1");
    User *user2 = new User("user2");
    User *user3 = new User("user3");

    group->subscribe(user1);
    group->subscribe(user2);
    group->subscribe(user3);

    group->notify("Match at 3pm");

    cout << "User 3 is not well, removing him from group" << endl;

    group->unsubscribe(user3);

    group->notify("Match started...");

    return 0;
}