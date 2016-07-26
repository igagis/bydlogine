/* 
 * File:   ResMesh.hpp
 * Author: ivan
 *
 * Created on November 1, 2014, 11:59 AM
 */

#pragma once

#include <morda/ResourceManager.hpp>

#include <morda/shaders/PosTexShader.hpp>

#include <morda/resources/ResTexture.hpp>

#include <kolme/Vector2.hpp>

namespace byd{

class ResModel : public morda::Resource{
	friend class morda::ResourceManager;
	
	std::shared_ptr<morda::ResTexture> tex;
	
	std::vector<std::uint16_t> indices;
	std::vector<kolme::Vec3f> vertices;
	std::vector<kolme::Vec2f> texCoords;
	
public:
	ResModel(){}
	
	ResModel(const ResModel&) = delete;
	ResModel& operator=(const ResModel&) = delete;
	
	void Render(morda::PosTexShader& s)const;
	
	void Render(morda::PosShader& s)const;
	
private:
	static std::shared_ptr<ResModel> load(const stob::Node& chain, const papki::File& fi);
};

}
