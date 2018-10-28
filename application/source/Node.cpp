#include "..\include\Node.hpp"



Node * Node::getParent()
{
	return parent;
}

Node * Node::getChildren(std::string _name)
{
	for (int i = 0;i < children.size(); i++) {
		if (_name == children.at(i)->getName()) {
			return  children.at(i);
		}

	}
	return nullptr;
}
std::string Node::getName()
{
	return name;
}
std::string Node::getPath()
{
std::string path = "/";
	if (parent != nullptr) {
		path = parent->getPath();
	}
	return path + "/" + name;

	//application_solar.cpp:
	//Node* geo= new Node();
	//geo.setName("any Name");
	//geo.getPath();

}
int Node::getDepth()
{
	int res = 0;
	if (parent != nullptr) {
		res = parent->getDepth()+1;
		return res;
		
	}

	return 1;
}
glm::mat4 Node::getLocalTransform()
{
	return glm::mat4();
}
void Node::setWorldTransfrom(glm::fmat4 b)
{
	worldTransform = b;
}
void Node::addChildren(Node* child)
{
	children.push_back(child);

}
void Node::removeChildren(std::string _name)
{
	for (int i = 0;i < children.size(); i++) {
		if (_name == children.at(i)->getName()) {
			children.erase(children.begin() + i);
		}
	}

}
void Node::setlocalTransform(glm::fmat4 a)

{
	localTransform = a;

}
void Node::setParent(Node* newParent)
{
	parent = newParent;

}

Node::Node()
{
}


Node::~Node()
{
}
