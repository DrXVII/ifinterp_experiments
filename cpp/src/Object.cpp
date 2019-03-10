#include "Object.hpp"

Object::Object(const std::string& name, const std::string& description)
: m_name {name}
, m_description {description}
, m_contents {}
, m_location {nullptr}
{
}

std::string Object::get_name() {return m_name;}
std::string Object::get_description() {return m_description;}
Object* Object::get_location() {return m_location;}
std::vector<Object*>* Object::get_contents() {return &m_contents;}

void Object::add_object(Object* obj)
{
    m_contents.push_back(obj);
    obj->m_location = this;
}

void Object::list_contents()
{
    if(m_contents.size() == 0) {std::cout << "Empty\n";}
    else {
        std::cout << m_name << " contains:\n";
        for(auto obj : m_contents) {std::cout << obj->m_name << std::endl;}
    }
}
