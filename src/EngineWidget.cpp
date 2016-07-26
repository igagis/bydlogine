/* 
 * File:   EngineWidget.cpp
 * Author: ivan
 * 
 * Created on October 30, 2014, 5:02 PM
 */

#include "EngineWidget.h"
#include "Engine.h"

#include <morda/App.hpp>

#include "components/MeshRendererComponent.h"

using namespace byd;


EngineWidget::EngineWidget(const stob::Node* chain) :
		Widget(chain)
{
	this->mesh = morda::App::inst().resMan.load<ResModel>("mesh_duck");
	
	this->tex = morda::App::inst().resMan.load<morda::ResTexture>("tex_sample");
}



void EngineWidget::update(std::uint32_t dt){
	this->rot %= kolme::Quatf().initRot(kolme::Vec3f(1, 2, 1).normalize(), 1.5f * (float(dt) / 1000));

	//byd::Engine::inst().check();
}


void EngineWidget::render(const morda::Matr4r& matrix)const{
	
	//отвязываем буферы если они привязаны, ибо утку рендерим не из ГЛных буферов
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	morda::Matr4r matr(matrix);
	matr.scale(this->rect().d / 2);
	matr.translate(1, 1);
	matr.frustum(-2, 2, -1.5, 1.5, 2, 1000);

	morda::Matr4r m(matr);
	m.translate(0, 0, -500);

	m.rotate(this->rot);

//	this->tex->Tex().bind();

	auto& s = morda::App::inst().shaders.posTexShader;
//	s.SetColor(morda::Vec3f(0, 1, 0));
	s.setMatrix(m);

	glEnable(GL_CULL_FACE);
//	glEnable(GL_DEPTH_TEST);
    
	this->mesh->Render(s);
	
	
	glDisable(GL_CULL_FACE);
        
	auto renderers = byd::Engine::inst().root.FindRecursiveComponents<MeshRendererComponent>();

	TRACE(<< "renderers size " << renderers.size() << std::endl)

	for(auto& r : renderers){
		r->Render();
	}
}