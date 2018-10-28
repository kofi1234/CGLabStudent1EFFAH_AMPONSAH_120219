#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP
#include "Node.hpp"
#include "model.hpp"
class GeometryNode : public Node
{
private: model geometry;

public:
	model getGeometry();
	void setGeometry(model);
	GeometryNode();
	~GeometryNode();
};

#endif