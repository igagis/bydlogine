/* 
 * File:   EngineWidget.h
 * Author: ivan
 *
 * Created on October 30, 2014, 5:02 PM
 */

#pragma once

#include <morda/widgets/Widget.hpp>

#include <morda/resources/ResTexture.hpp>

#include <morda/Updateable.hpp>

#include <kolme/Quaternion.hpp>

#include "resources/ResModel.hpp"

namespace byd{


class EngineWidget : public morda::Widget, public morda::Updateable{
	std::shared_ptr<morda::ResTexture> tex;
	
	std::shared_ptr<ResModel> mesh;
	
	kolme::Quatf rot = kolme::Quatf().identity();
public:
	EngineWidget(const stob::Node* chain);
	
	void Update(std::uint32_t dt) override;
	
	void render(const morda::Matr4r& matrix)const override;
};


}
