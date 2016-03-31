
#include "MeshComponent.h"

namespace byd
{

MeshComponent::MeshComponent()
{
    // pos
    this->vertices.push_back(0.0f);
    this->vertices.push_back(0.0f);
    this->vertices.push_back(-1.0f);
    
    //color
    this->vertices.push_back(1.0f);
    this->vertices.push_back(0.0f);
    this->vertices.push_back(0.0f);

    //pos    
    this->vertices.push_back(1.0f);
    this->vertices.push_back(0.0f);
    this->vertices.push_back(-1.0f);

    //color
    this->vertices.push_back(0.0f);
    this->vertices.push_back(1.0f);
    this->vertices.push_back(0.0f);
    
    // pos
    this->vertices.push_back(0.5f);
    this->vertices.push_back(1.0f);
    this->vertices.push_back(-1.0f);

    //color
    this->vertices.push_back(0.0f);
    this->vertices.push_back(0.0f);
    this->vertices.push_back(1.0f);

    this->indices.push_back(0);
    this->indices.push_back(1);
    this->indices.push_back(2);
}

MeshComponent::~MeshComponent() noexcept
{
}

}
