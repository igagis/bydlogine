
#include "Object.h"

#include <utki/debug.hpp>

#include <algorithm>
#include <utki/Exc.hpp>

namespace byd
{

Object::Object(const std::string & name)
 : id(name),
   parent(nullptr)
{
    TRACE(<< "object ctor" << std::endl)
}

    
Object::~Object() noexcept
{
    TRACE(<< "object dctor" << std::endl)
}

void Object::AddChild(ObjectSharedPtr child)
{
    ASSERT(child->parent == nullptr)
            
    child->parent = this;
    this->children.push_back(std::move(child));    
}

void Object::RemoveFromParent() {
	if(!this->Parent()){
		return;
	}
	
	this->Parent()->RemoveChild(*this);
}


void Object::RemoveChild(Object& child)
{
	if(child.Parent() != this){
		throw utki::Exc("Object::RemoveChild(): child is not added to this parent");
	}
	
    for(auto i = this->children.begin(); i != this->children.end(); ++i){
		if((*i).get() == &child){
			this->children.erase(i);
			break;
		}
	}
}

void Object::AddComponent(ComponentSharedPtr component)
{
    this->components.push_back(component);
    component->setOwner(this);
    
    component->init();
    
    SendMessage(ComponentAddedMessage());
}

ObjectSharedPtr Object::FindRecursiveChildById(const std::string & childId)
{
    for (auto it = this->children.begin(); it != this->children.end(); ++it)
    {
        if ((*it)->id == childId)
        {
            return *it;
        }
        
        if (auto found = (*it)->FindRecursiveChildById(childId))
        {
            return found;
        }
    }
    
    return ObjectSharedPtr();
}

void Object::trace()
{
    TRACE(<< "trace object" << std::endl)
            
    for (auto it = this->children.begin(); it != this->children.end(); ++it)
    {
        (*it)->trace();
    }
}

}
