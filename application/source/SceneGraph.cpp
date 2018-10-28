#include "SceneGraph.hpp"



std::string SceneGraph::getName()
{
	return name;
}

SceneGraph::SceneGraph()
{
}


SceneGraph::~SceneGraph()
{
}

void SceneGraph::setName(std::string newName)
{
	name = newName;
}

void SceneGraph::setRoot(Node* newRoot)
{
	root = newRoot;
}

Node* SceneGraph::getRoot()
{
	return root;
}
