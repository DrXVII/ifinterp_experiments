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

struct World {
    World();

    //objects in the world
    std::map<std::string, Object*> m_objects;
    //actions operating on one object
    std::map<std::string, std::function<void(Object*)>>
        m_one_obj_actions;
    //actions operation on two objects
    std::map<std::string, std::function<void(Object*, Object*)>>
        m_two_obj_actions;
    //aliases for objects, actions, etc in the world
    std::map<std::string, std::string> m_aliases;
};

World::World()
: m_one_obj_actions {}
, m_two_obj_actions {}
{}

//struct Player : Object {
//};

void process_command(const std::string& command);

int main()
{
    std::cout << "Hello worlds!\n";

    World world;
    world.m_one_obj_actions.insert({
            "look",
            [](Object* player) {
                if(player == nullptr) {return;}

                std::cout << player->m_location->m_description << std::endl;
            }
    });

    Object start_room {
            .m_name = "start room",
            .m_description = "this is where it all begins",
            .m_contents = {},
            .m_location = nullptr
    };

    Object player {
            .m_name = "player",
            .m_description = "hey, it's you!",
            .m_contents = {},
            .m_location = &start_room
    };

    std::cout << player.m_location->m_name << std::endl;
    std::cout << "\"" << player.m_location->m_description << "\"" << std::endl;

    std::string command;
    while(command != "q") {
        std::cout << "\n> ";
        std::getline(std::cin, command);

        //supporting only one word commands for this experiment
        auto action = world.m_one_obj_actions.find(command);
        if(action != world.m_one_obj_actions.end()) {
            action->second(&player);
        }
        else {std::cout << "pardon, I didn't understand that\n";}
    }

    return 0;
}
