
#pragma once

#include <utki/util.hpp>

#include <vector>

#include "Component.h"

namespace byd

{

typedef std::shared_ptr<class MeshComponent>	MeshComponentSharedPtr;
typedef std::weak_ptr<class MeshComponent>	MeshComponentWeakPtr;

class MeshComponent : public Component 
{
public:

    MeshComponent();    
    virtual ~MeshComponent() noexcept;
    
    inline const std::vector<unsigned int>& getIndices() const
    { 
	return indices;
    }
    
    inline const std::vector<float>& getVertices() const
    { 
	return vertices;
    }
    
private:
    
    //TODO: real mesh data
    std::vector<unsigned int>	indices;
    // like x,y,z,x,y,z....
    std::vector<float>		vertices;
};

}
