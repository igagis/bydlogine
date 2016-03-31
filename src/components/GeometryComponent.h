
#pragma once

#include <utki/util.hpp>

#include "Component.h"
#include "MeshComponent.h"

#include <GL/glew.h>

namespace byd

{

typedef std::shared_ptr<class GeometryComponent>    GeometryComponentSharedPtr;
typedef std::weak_ptr<class GeometryComponent>	    GeometryComponentWeakPtr;
    
class GeometryComponent : public Component
{
public:

    GeometryComponent();
    virtual ~GeometryComponent() noexcept;

    virtual void init() override;

    virtual void GotMessage(Object * sender, const std::string & message) override;

    GLuint VertexBufferId() const
    {
	return this->vertexBufferId;
    }
    
    GLuint IndexBufferId() const
    {
	return this->indexBufferId;
    }
    
    unsigned int IndexCount() const
    {
	return this->indexCount;
    }
    
    void traceGL()
    {
	TRACE (<< "trace GL" << std::endl)
	TRACE (<< "vertices gl id" << vertexBufferId <<  std::endl)
	TRACE (<< "indices gl id " << indexBufferId << std::endl)
    }
    
private:
    
    void setVertices(const std::vector<float>& v);
    void setIndices(const std::vector<unsigned int>& i);
    void releaseBuffers();
        
    MeshComponentWeakPtr meshComponent;
    
    GLuint vertexBufferId   = 0;
    GLuint indexBufferId    = 0;
    
    unsigned int indexCount = 0;
};

}
