
#include "GeometryComponent.h"

#include "../Object.h"

#include <utki/Exc.hpp>

namespace byd
{

GeometryComponent::GeometryComponent() 
{
}
    
GeometryComponent::~GeometryComponent() noexcept
{
    this->releaseBuffers();
}

void GeometryComponent::init()
{
    Component::init();
    
    Object * owner = this->getOwner();
    
    if (!owner)
    {
        throw utki::Exc("GeometryComponent::init(): owner for component must be inside init");
    }
    
    auto mesh = owner->FindComponent<MeshComponent>();

    if (mesh)
    {
        this->setVertices(mesh->getVertices());
	this->setIndices(mesh->getIndices());
        
        this->meshComponent = mesh;
    }
}

void GeometryComponent::GotMessage(Object * sender, const std::string & message)
{
    Component::GotMessage(sender, message);
    
    if (this->getOwner() == sender)
    {
        if (Object::isComponentAddedMessage(message))
        {
            // TODO:: check GeometryComponent supports only one mesh per object
            
            auto holdedMesh = this->meshComponent.lock();
            auto foundMesh = this->getOwner()->FindComponent<MeshComponent>();

            if (holdedMesh != foundMesh)
            {
                this->releaseBuffers();
                
                if (foundMesh)
                {
                    this->setVertices(foundMesh->getVertices());
                    this->setIndices(foundMesh->getIndices());
                    
                    indexCount = foundMesh->getIndices().size();
                }

                this->meshComponent = foundMesh;
            }
        }
    }
}

void GeometryComponent::setVertices(const std::vector<float>& v)
{ 
    ASSERT (this->vertexBufferId == 0)
    ASSERT (!v.empty())
    
    glGenBuffers(1, &this->vertexBufferId);
    
    // just temp. solution to store prev. bounded buf
    GLint currentBuffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBuffer);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferId);
    
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(v[0]), &v[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, currentBuffer);
}

void GeometryComponent::setIndices(const std::vector<unsigned int>& i)
{ 
    ASSERT (this->indexBufferId == 0)
    ASSERT (!i.empty())
    
    glGenBuffers(1, &this->indexBufferId);
    
    // just temp. solution to store prev. bounded buf
    GLint currentBuffer;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentBuffer);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferId);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i.size() * sizeof(i[0]), &i[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentBuffer);
}

void GeometryComponent::releaseBuffers()
{
    if (this->vertexBufferId != 0)
    {
        glDeleteBuffers(1, &this->vertexBufferId);
        this->vertexBufferId = 0;
    }
    
    if (this->indexBufferId != 0)
    {
        glDeleteBuffers(1, &this->indexBufferId);
        this->indexBufferId = 0;
    }
    
    this->indexCount = 0;
}

}
