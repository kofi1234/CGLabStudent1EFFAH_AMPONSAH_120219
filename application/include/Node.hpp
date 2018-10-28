#ifndef NODE_HPP
#define NODE_HPP
#include <vector>
#include <glm/matrix.hpp>
class Node
{
private:
	Node* parent;
	std::string name;
	std::vector<Node*> children;
	std::string path;
	int depth;
	glm::fmat4 localTransform;
	glm::fmat4 worldTransform;


	


public:
	std::string getName();
	Node* getParent();
	Node* getChildren(std::string);
	void setParent(Node*);
	std::string getPath();
	int getDepth();
	glm::mat4 getLocalTransform();
	void setlocalTransform(glm::fmat4);
	glm::mat4 getWorldTransform();
	void setWorldTransfrom(glm::fmat4);
	void addChildren(Node*);
	void removeChildren(std::string);
	Node();
	~Node();
};

#endif