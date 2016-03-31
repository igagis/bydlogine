/* 
 * File:   Application.h
 * Author: ivan
 *
 * Created on October 30, 2014, 4:58 PM
 */

#pragma once

#include <morda/App.hpp>

#include "Engine.h"


namespace byd{


class Application : public morda::App{
public:
	Application();
                   
private:
    
    byd::Engine engine;
    
};

}
