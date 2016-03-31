#include "Application.h"

#include "EngineWidget.h"


using namespace byd;



namespace{

morda::App::WindowParams GetWindowParams()noexcept{
	morda::App::WindowParams wp;

	wp.dim.x = 800;
	wp.dim.y = 800;

	return wp;
}

}


Application::Application() :
		App(GetWindowParams())
{
	this->resMan.MountResPack(*this->CreateResourceFileInterface("res/"));

	this->inflater.AddWidget<EngineWidget>("EngineWidget");

	auto c = morda::App::inst().inflater.Inflate(
			*this->CreateResourceFileInterface("res/test.gui.stob")
		);

	std::dynamic_pointer_cast<EngineWidget>(c->findChildByName("bydlogine_widget"))->StartUpdating(30);

	this->SetRootWidget(c);
}
