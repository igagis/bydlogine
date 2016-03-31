/* 
 * File:   Exc.hpp
 * Author: ivan
 *
 * Created on November 1, 2014, 8:22 PM
 */

#pragma once

#include <utki/Exc.hpp>


namespace byd{

class Exc : public utki::Exc{
public:
	Exc(const std::string& message) :
			utki::Exc(message)
	{}
};

}
