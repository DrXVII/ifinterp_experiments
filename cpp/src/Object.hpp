#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <vector>
#include <iostream>

class Object {
public:
    Object(const std::string& name, const std::string& description = "");

    std::string get_name();
    std::string get_description();
    Object* get_location();
    std::vector<Object*>* get_contents();

    void add_object(Object* obj);
    void list_contents();

private:
    std::string m_name;
    std::string m_description;
    std::vector<Object*> m_contents;
    Object* m_location;
};

#endif //define OBJECT_HPP
