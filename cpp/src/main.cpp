#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include "Object.hpp"

struct World {
    World();

    //objects in the world
    //std::map<std::string, Object*> m_objects;
    //aliases for objects, actions, etc in the world
    std::map<std::string, std::string> m_aliases;
    //actions operating on one object
    //TODO should be pointer to function obj
    std::map<std::string, std::function<void(Object*)>>
        m_one_obj_actions;
    //actions operation on two objects
    //TODO shoudl be pointer to function obj
    std::map<std::string, std::function<void(Object*, Object*)>>
        m_two_obj_actions;
};

World::World()
: m_aliases {}
, m_one_obj_actions {}
, m_two_obj_actions {}
{}

//struct Player : Object {
//};

void process_command(std::string command, World* world, Object* location);

int main()
{
    std::cout << "Hello worlds!\n";

    World world;
    world.m_one_obj_actions.insert({
            "look",
            [](Object* player) {
                if(player == nullptr) {return;}

                std::cout << player->get_location()->get_name()
                    << std::endl;
                std::cout << player->get_location()->get_description()
                    << std::endl;
            }
    });

    world.m_one_obj_actions.insert({
            "examine",
            [](Object* obj) {
                if(!obj) {return;}

                std::cout << obj->get_description() << std::endl;
                obj->list_contents();
            }
    });

    world.m_two_obj_actions.insert({
            "put into",
            [](Object* obj, Object* cont) {
                if(!obj || !cont) {return;}

                cont->add_object(obj);
            }
    });

    Object start_room ("start room", "this is where it all begins");
    Object player ("player", "hey, it's you!");
    Object book ("book", "a fancy book");
    Object book_of_jnanin ("book of Jnanin",
            "beautiful golden letters inset in a cover of green leather "
            "read\"Jnanin\"");
    Object box ("box", "a simple wooden box");
    box.add_object(&book);

    start_room.add_object(&player);
    start_room.add_object(&box);
    start_room.add_object(&book_of_jnanin);

    std::string command;
    while(command != "q") {
        std::cout << "\n> ";
        std::getline(std::cin, command);

        process_command(command, &world, player.get_location());
    }

    return 0;
}

//TODO this should go to utils.cpp or something
std::string get_nth_word(const std::string& str, const std::string& sep,
        size_t n = 1)
{
    if(str.size() == 0) {return "";}

    size_t word_n {0};
    size_t beg_pos {0};
    size_t ed_pos {0};

    while(word_n < n) {
        ++word_n;
        beg_pos = (word_n > 1)? ed_pos + 1 : 0;
        ed_pos = str.find_first_of(sep, beg_pos);

        if(word_n == n) {
            if(ed_pos == std::string::npos) {return str.substr(beg_pos);}
            else {return str.substr(beg_pos, ed_pos - beg_pos);}
        }
        else if(ed_pos == std::string::npos) {return "";}
    }

    return "";
}

//find object that matches the longest part of the command string from beginning
Object* find_obj(const std::string& str, std::vector<Object*>* objects)
{
    Object* candidate {nullptr};

    for(Object* object : *objects) {
        size_t pos {str.find(object->get_name())};
        if(pos > 0) {continue;}

        //std::cerr << "pos: " << pos << " obj: " << object->get_name() << std::endl;

        if(!candidate || candidate->get_name().size() < object->get_name().size()) {
            candidate = object;
        }
    }

    return candidate;
}

std::function<void(Object*)>* find_action(std::string str, std::map<std::string,
        std::function<void(Object*)>>* actions)
{
    auto search = actions->find(str);
    if(search != actions->end()) {return &search->second;}
    else {return nullptr;}
}

std::function<void(Object*, Object*)>* find_action2(std::string str,
        std::map<std::string, std::function<void(Object*, Object*)>>* actions)
{
    auto search = actions->find(str);
    if(search != actions->end()) {return &search->second;}
    else {return nullptr;}
}

//TODO looks convoluted, should revise
void process_command(std::string command, World* world, Object* location)
{
    /*algorhithm logic:
     * typical action signature: '<verb> [article] <noun> [<connector> [article] <noun>]
     * TODO check for aliases
     * TODO handle articles
    */

    std::string action_word1 = get_nth_word(command, " ");
    command.erase(0, action_word1.size() + 1); //removing the action word
    std::function<void(Object*)>* action {nullptr};

    action = find_action(action_word1, &world->m_one_obj_actions);

    Object* object1 = find_obj(command, location->get_contents());
    if(object1 == nullptr && command.size() > 0) {
        std::cout << "You don't see such an item\n";
        std::cout << "DBG: " << command << std::endl;
        return;
    }

    if(object1) {command.erase(0, object1->get_name().size() + 1);}

    if(command.empty()) {
        if(action && object1) {(*action)(object1);}
        else if(action == nullptr && object1 == nullptr) {
            std::cout << "Sorry, I'm not sure what you mean.\n";
        }
        else if(action) {
            std::cout << "What do you want to " << action_word1 << "?\n";
        }
        else if(object1) {
            std::cout << action_word1 << " is not something you can do to "
                << object1->get_name() << ".\n";
        }

        return;
    }

    /*if something is still left in the command string, we have to find the
     * second action word and the second object*/

    std::string action_word2 = get_nth_word(command, " ");
    command.erase(0, action_word2.size() + 1);

    std::string action_word_combo {action_word1 + " " + action_word2};

    std::function<void(Object*, Object*)>* action2 {
        find_action2(action_word_combo, &world->m_two_obj_actions)
    };

    if(action2 == nullptr) {
        std::cout << action_word_combo << " is not something you can do.\n";
    }

    Object* object2 = find_obj(command, location->get_contents());

    //DBG
    //std::cout << "act: " << action_word_combo << std::endl;
    //if(object1) {std::cout << "o1: " << object1->get_name() << std::endl;}
    //if(object2) {std::cout << "o2: " << object2->get_name() << std::endl;}

    if(action2 && object1 && object2) {(*action2)(object1, object2);}
    else {std::cout << "Sorry, that's not something I can understand.\n";}
}
