
#pragma once

#include "components/Component.h"

#include <string>
#include <list>
#include <memory>
#include <vector>

namespace byd {

typedef std::shared_ptr<class Object>	ObjectSharedPtr;
typedef std::weak_ptr<class Object>	ObjectWeakPtr;

class Object {
public:

	Object(const std::string & name);

	virtual ~Object() noexcept;

	virtual void AddChild(ObjectSharedPtr child);

	virtual void RemoveChild(Object& child);

	void RemoveFromParent();

	Object * Parent() const
	{
		return parent;
	}

	Object * Root()
	{
		Object * node = this;

		while (node->Parent())
		{
			node = node->Parent();
		}

		return node;
	}

	ObjectSharedPtr FindRecursiveChildById(const std::string & childId);

	template<typename T>
	std::shared_ptr<T> MakeComponent()
	{
		ComponentSharedPtr c(new T());

		components.push_back(c);
		c->setOwner(this);

		c->init();

		SendMessage(ComponentAddedMessage());

		return std::dynamic_pointer_cast<T>(c);
	}

	void AddComponent(ComponentSharedPtr component);

	template<typename T>	
	std::shared_ptr<T> FindComponent()
	{
		for (auto & c : components)
		{
			if (std::shared_ptr<T> p = std::dynamic_pointer_cast<T>(c))
			{
				return p;
			}
		}

		return std::shared_ptr<T>();
	}

	virtual void GotMessage(Object * sender, const std::string & message)
	{

	}

	void SendMessage(const std::string & message)
	{
		Object * root = Root();

		root->SendMessageRecursive(this, message);
	}

	static const std::string & ComponentAddedMessage()
	{
		static std::string m("ComponentAddedMessage");
		return m;
	}

	static bool isComponentAddedMessage(const std::string & m)
	{
		return m == ComponentAddedMessage();
	}

private:
	void SendMessageRecursive(Object * sender,const std::string & message)
	{
		this->GotMessage(sender, message);

		for (auto & c : this->components)
		{
			c->GotMessage(sender, message);
		}

		for (auto & c : this->children)
		{
			c->SendMessageRecursive(sender, message);
		}	    
	}

public:
	template<typename T> std::vector<std::shared_ptr<T>> FindRecursiveComponents(){
		std::vector<std::shared_ptr<T>> ret;
		this->FindRecursiveComponents<T>(ret);
		return std::move(ret);
	}
private:
	template<typename T> void FindRecursiveComponents(std::vector<std::shared_ptr<T>>& ret){
		for (auto& c : this->components){
			if(auto p = std::dynamic_pointer_cast<T>(c)){
				ret.push_back(p);
			}
		}

		for (auto& c : this->children){
			c->FindRecursiveComponents<T>(ret);
		}
	}

public:
	virtual void trace();

private:
	std::string id;

	Object * parent;
	std::list<ObjectSharedPtr>	children;

	std::list<ComponentSharedPtr>	components;
};

}
