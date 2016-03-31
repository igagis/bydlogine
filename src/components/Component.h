#pragma once

#include <memory>

namespace byd

{

class Object;
    
typedef std::shared_ptr	<class Component>   ComponentSharedPtr;
typedef std::weak_ptr	<class Component>   ComponentWeakPtr;
    
class Component 
{
public:

    Component();
    virtual ~Component() noexcept;

    virtual void init();
    
    inline void setOwner(Object * o)
    {
	owner = o;
    }

    inline Object * getOwner() const
    {
	return owner;
    }
    
    virtual void GotMessage(Object * sender, const std::string & message)
    {
    
    }
    
private:
    
    Object * owner;
    
};

}
