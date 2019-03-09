#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

struct Object {
    std::string m_name;
    std::string m_description;
    std::vector<Object*> m_contents;
    Object* m_location;

    void list_contents();
};

void Object::list_contents()
{
    if(m_contents.size() == 0) {std::cout << "Empty\n";}
    else {
        std::cout << m_name << " contains:\n";
        for(auto obj : m_contents) {std::cout << obj->m_name << std::endl;}
    }
}

//struct Player : Object {
//};

int main()
{
    std::cout << "Hello worlds!\n";

    Object start_room {
            .m_name = "start room",
            .m_description = "this is where it all begins",
            .m_contents {},
            .m_location = nullptr
    };

    Object player {
            .m_name = "player",
            .m_description = "hey, it's you!",
            .m_contents {},
            .m_location = &start_room
    };

    std::map<std::string, std::function<void(Object*, Object*)>> actions;
    actions.insert({
            "look",
            [](Object* player, Object* subject) {
                if(player == nullptr) {return;}
                if(subject == nullptr) {/*it's OK, we dont use it here*/}

                std::cout << player->m_location->m_description << std::endl;
            }
    });

    std::cout << player.m_location->m_name << std::endl;
    std::cout << "\"" << player.m_location->m_description << "\"" << std::endl;

    std::string command;
    while(command != "q") {
        std::cout << "\n> ";
        std::getline(std::cin, command);

        //supporting only one word commands for this experiment
        auto action = actions.find(command);
        if(action != actions.end()) {
            action->second(&player, nullptr);
        }
    }

    return 0;
}
