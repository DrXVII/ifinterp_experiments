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
    if(obj->m_location) {obj->m_location->remove_object(obj);}
    obj->m_location = this;
}

void Object::remove_object(Object* obj)
{
    for(size_t i {0}; i < m_contents.size(); ++i) {
        if(obj == m_contents[i]) {
            m_contents[i] = m_contents.back();
            m_contents.pop_back();
        }
    }
}

void Object::list_contents(bool silent)
{
    if(m_contents.size() == 0) {
        if(silent == false) {std::cout << "Empty\n";}
    }
    else {
        std::cout << m_name << " contains:\n";
        for(auto obj : m_contents) {std::cout << obj->m_name << std::endl;}
    }
}
