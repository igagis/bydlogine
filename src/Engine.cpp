#include "Engine.h"
#include "components/MeshRendererComponent.h"

#include <utki/debug.hpp>

namespace byd
{

Engine::Engine() :
		root("root")
{
	auto t0 = std::make_shared<Object>("t0");    
	auto t1 = std::make_shared<Object>("t1");    
	auto t11 = std::make_shared<Object>("t11");    
	auto t2 = std::make_shared<Object>("t2");

	t0->AddChild(t1);
	t0->AddChild(t11);

	root.AddChild(std::move(t0));
	t1->AddChild(t2);

	root.trace();

	ObjectSharedPtr ft11 = root.FindRecursiveChildById("t11");
	if (ft11)
	{
		TRACE (<< "found" << std::endl)
		ft11->RemoveFromParent();
		TRACE (<< "remove child" << std::endl)
	}

	// load scene 
	auto someMesh = std::make_shared<Object>("someMesh");
        auto someMeshGeometryComponent = someMesh->MakeComponent<GeometryComponent>();  
        auto someMeshComponent = someMesh->MakeComponent<MeshComponent>();	              

	root.AddChild(someMesh);

	// like a real game object
	auto someEnemy = std::make_shared<Object>("someEnemy");
	auto someEnemyMeshRendererComponent = someEnemy->MakeComponent<MeshRendererComponent>();

	// what's the mesh?
	dynamic_cast<MeshRendererComponent*>(someEnemyMeshRendererComponent.get())->setMeshObject(someMesh);

	root.AddChild(someEnemy);
/*
	// like camera render
	auto renderers = root.FindRecursiveComponents<MeshRendererComponent>();

	TRACE(<< "renderers size " << renderers.size() << std::endl)

	for(auto it = renderers.begin(); it != renderers.end(); ++it)
	{
		MeshRendererComponent * r = dynamic_cast<MeshRendererComponent*>(it->get());
		r->Render();
	}*/
}

Engine::~Engine()noexcept
{

}

void Engine::check()
{
	TRACE(<< "bla bla bla" << std::endl)
}

}
