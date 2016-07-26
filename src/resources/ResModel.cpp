/* 
 * File:   ResMesh.cpp
 * Author: ivan
 * 
 * Created on November 1, 2014, 11:59 AM
 */

#include "ResModel.hpp"

#include <assimp/scene.h>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <morda/App.hpp>

#include "../Exc.hpp"

using namespace byd;



namespace{


// My own implementation of IOStream
class BydIOStream : public Assimp::IOStream{
	std::unique_ptr<const papki::File> f;
public:
	// Constructor protected for private usage by MyIOSystem
	BydIOStream(std::unique_ptr<const papki::File>&& f) :
			f(std::move(f))
	{}
	
	~BydIOStream(){
		this->f->close();
	}
	
	size_t Read(void* pvBuffer, size_t pSize, size_t pCount)override{
		ASSERT(pSize != 0)
		return this->f->read(utki::wrapBuf(
				reinterpret_cast<std::uint8_t*>(pvBuffer),
				pSize * pCount
			)) / pSize;
	}
	
	size_t Write(const void* pvBuffer, size_t pSize, size_t pCount)override{
		ASSERT(pSize != 0)
//		return this->f->Write(ting::Buffer<const std::uint8_t>(
//				reinterpret_cast<const std::uint8_t*>(pvBuffer), pSize * pCount
//			)) / pSize;
		throw byd::Exc("BydIOStream::Write(): read-only file system");
	}
	
	aiReturn Seek(size_t pOffset, aiOrigin pOrigin)override{
		switch(pOrigin){
			default:
			case aiOrigin::aiOrigin_CUR:
				this->f->seekForward(pOffset);
				break;
			case aiOrigin::aiOrigin_END:
				this->f->seekForward(size_t(-1));
				this->f->seekBackward(pOffset);
				break;
			case aiOrigin::aiOrigin_SET:
				this->f->rewind();
				this->f->seekForward(pOffset);
				break;
		}
		return aiReturn::aiReturn_SUCCESS;
	}
	
	size_t Tell()const override{
		return this->f->curPos();
	}
	
	size_t FileSize()const override{
		size_t curPos = this->f->curPos();
		this->f->rewind();
		size_t fileSize = this->f->seekForward(size_t(-1));
		
		this->f->rewind();
		this->f->seekForward(curPos);
		return fileSize;
	}
	
	void Flush()override{
		//do nothing
	}
};



class BydIOSystem : public Assimp::IOSystem{
	std::unique_ptr<const papki::File> f; //file interface prototype
public:
	BydIOSystem(std::unique_ptr<const papki::File>&& f) :
			f(std::move(f))
	{}
	
	bool Exists(const char* pFile)const override{
		this->f->setPath(pFile);
		return this->f->exists();
	}
	
	// Get the path delimiter character we'd like to see
	char getOsSeparator()const override{
		return '/';
	}
	
	Assimp::IOStream* Open(const char* pFile, const char* pMode)override{
		auto file = this->f->spawn();
		file->setPath(pFile);
		
		ASSERT(pMode)
		
		if(pMode[0] != 'r'){
			throw byd::Exc("BydIOSystem::Open(): read-only file system tried to open for writing");
		}
		
		file->open();
		return new BydIOStream(std::move(file));
	}
	
	void Close(Assimp::IOStream* pFile)override{
		delete pFile;
	}
};


}




std::shared_ptr<ResModel> ResModel::load(const stob::Node& chain, const papki::File& fi){
//	Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE, aiDefaultLogStream::aiDefaultLogStream_STDOUT);
//
//	ting::util::ScopeExit killLoggerOnExit(
//			[](){
//				Assimp::DefaultLogger::kill();
//			}
//		);
	
	Assimp::Importer importer;
	
	importer.SetIOHandler(new BydIOSystem(fi.spawn()));
	
	const aiScene* scene = importer.ReadFile(
			chain.side("file").up().value(),
			aiProcess_CalcTangentSpace
					| aiProcess_Triangulate
					| aiProcess_JoinIdenticalVertices
					| aiProcess_SortByPType
		);
	
	if(!scene){
		throw morda::ResourceManager::Exc("ResMesh::Load(): Assimp: reading file failed");
	}
	
	auto ret = utki::makeShared<ResModel>();
	
	if(!scene->HasMeshes()){
		return ret;
	}
	
	ASSERT(scene->mNumMeshes != 0)
	ASSERT(scene->mMeshes)
	ASSERT(scene->mMeshes[0])
	
	aiMesh& m = *scene->mMeshes[0];
	
	if(!m.HasPositions() || !m.HasFaces()){
		return ret;
	}
	
	ret->vertices.reserve(m.mNumVertices);
	for(auto p = m.mVertices; p != m.mVertices + m.mNumVertices; ++p){
		ret->vertices.push_back(kolme::Vec3f(p->x, p->y, p->z));
	}
	
	for(auto p = m.mFaces; p != m.mFaces + m.mNumFaces; ++p){
		if(p->mNumIndices != 3){
			continue;
		}
		for(auto ind = p->mIndices; ind != p->mIndices + p->mNumIndices; ++ind){
			ret->indices.push_back(std::uint16_t(*ind));
		}
	}
	
	if(m.HasTextureCoords(0)){
		ret->texCoords.reserve(m.mNumVertices);
		for(auto p = m.mTextureCoords[0]; p != m.mTextureCoords[0] + m.mNumVertices; ++p){
			ret->texCoords.push_back(kolme::Vec2f(p->x, p->y));
		}
	}
	
	//if there is texture mapping
	if(auto texMapping = chain.childOfThisOrNext("textureMapping")){
		if(m.mMaterialIndex <= scene->mNumMaterials){
			ASSERT(scene->mMaterials[m.mMaterialIndex])
			aiMaterial& mat = *scene->mMaterials[m.mMaterialIndex];
			if(mat.GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) != 0){
				aiString path;
				mat.GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path);
//				TRACE(<< "ResModel::Load(): texture path = " << path.C_Str() << std::endl)

				if(auto t = texMapping->thisOrNext(path.C_Str()).node()){
					//texture found
					if(t->child()){
						ret->tex = morda::App::inst().resMan.load<morda::ResTexture>(t->child()->value());
					}
				}
			}
		}
	}
	
	return ret;
}

void ResModel::Render(morda::PosTexShader& s)const{
	this->tex->tex().bind();
	
	s.render(
			utki::wrapBuf(this->indices),
			utki::wrapBuf(this->vertices),
			utki::wrapBuf(this->texCoords),
			morda::Render::Mode_e::TRIANGLES
		);
}


void ResModel::Render(morda::PosShader& s)const{
	s.render(
			utki::wrapBuf(this->indices),
			utki::wrapBuf(this->vertices),
			morda::Render::Mode_e::TRIANGLES
		);
}
