#ifndef SCENEGRAPH_HPP
#define SCENEGRAPH_HPP

#include <iostream>
#include "Node.hpp"
class SceneGraph 
{

private:
	Node* root;
	std::string name;
//	void setName(std::string);
//	void setRoot(Node*);
	
public:
	std::string getName();
	Node* getRoot();
	void setName(std::string);
	void setRoot(Node*);
	SceneGraph();
	~SceneGraph();
};
	



#endif
