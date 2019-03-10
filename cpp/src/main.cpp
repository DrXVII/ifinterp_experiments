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

                std::cout << player->get_location()->get_name() << std::endl;
                std::cout << player->get_location()->get_description() << std::endl;
            }
    });

    world.m_one_obj_actions.insert({
            "examine",
            [](Object* obj) {
                if(!obj) {return;}

                std::cout << obj->get_description() << std::endl;
            }
    });

    Object start_room ("start room", "this is where it all begins");
    Object player ("player", "hey, it's you!");
    Object book ("book", "a fancy book");
    Object book_of_jnanin ("book of Jnanin",
            "beautiful golden letters inset in a cover of green leather "
            "read\"Jnanin\"");

    start_room.add_object(&player);
    start_room.add_object(&book);
    start_room.add_object(&book_of_jnanin);

    std::string command;
    while(command != "q") {
        std::cout << "\n> ";
        std::getline(std::cin, command);

        process_command(command, &world, player.get_location());

        //supporting only one word commands for this experiment
        //auto action = world.m_one_obj_actions.find(command);
        //if(action != world.m_one_obj_actions.end()) {
        //    action->second(&player);
        //}
        //else {std::cout << "pardon, I didn't understand that\n";}
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

std::function<void(Object*)>* find_action(std::string str, std::map<std::string, std::function<void(Object*)>>* actions)
{
    auto search = actions->find(str);
    if(search != actions->end()) {return &search->second;}
    else {return nullptr;}
}

void process_command(std::string command, World* world, Object* location)
{
    /*algorhithm logic:
     * typical action signature: '<verb> [article] <noun> [<connector> [article] <noun>]
     * TODO check for aliases
     * TODO (refine) check first word to determine candidate list of actions
     * TODO (refine) check remaining string against local objects (pick longest match)
     * TODO (later) if there's more, check what's left against second word of function(s) in candidate list
     * TODO (later) what's left is our second object - find that among local objects
    */

    std::string action_word = get_nth_word(command, " ");
    command.erase(0, action_word.size() + 1); //removing the action word
    std::function<void(Object*)>* action {nullptr};
    action = find_action(action_word, &world->m_one_obj_actions);
    //if(action) {std::cerr << "action: found\n";}
    //else {std::cerr << "action: not found\n";}

    Object* object1 = find_obj(command, location->get_contents());
    //if(object1) {std::cerr << "object: " << object1->get_name() << std::endl;}
    //else {std::cerr << "object not found\n";}

    if(action && object1) {(*action)(object1);}
}
