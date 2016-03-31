
#pragma once

#include "Object.h"

#include <utki/Singleton.hpp>

#include <memory>

namespace byd

{

class Engine : public utki::Singleton<Engine>
{
public:
    
    Engine();
    
    ~Engine()noexcept;
    
    void check();

// temp.!!!
//private:
    
    Object root;
    
};

}
