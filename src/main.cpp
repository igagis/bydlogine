#include <morda/AppFactory.hpp>

#include "Application.h"


std::unique_ptr<morda::App> morda::createApp(int argc, const char** argv, const utki::Buf<std::uint8_t> savedState){
	return utki::makeUnique<byd::Application>();
}
