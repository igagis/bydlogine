#include "Application.h"

#include "EngineWidget.h"


using namespace byd;



namespace{

morda::App::WindowParams GetWindowParams()noexcept{
	morda::App::WindowParams wp(kolme::Vec2ui(800, 800));

	return wp;
}

}


Application::Application() :
		App(GetWindowParams())
{
	this->initStandardWidgets();
	
	this->resMan.mountResPack(*this->createResourceFileInterface("res/"));

	this->inflater.addWidget<EngineWidget>("EngineWidget");

	auto c = morda::App::inst().inflater.inflate(
			*this->createResourceFileInterface("res/test.gui.stob")
		);

	std::dynamic_pointer_cast<EngineWidget>(c->findChildByName("bydlogine_widget"))->startUpdating(30);

	this->setRootWidget(c);
}
