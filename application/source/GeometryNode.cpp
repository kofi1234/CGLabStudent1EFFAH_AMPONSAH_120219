#include "GeometryNode.hpp"



model GeometryNode::getGeometry()
{
	return geometry;
}

void GeometryNode::setGeometry( model newModel)
{
	geometry = newModel;
}

GeometryNode::GeometryNode()
{
}


GeometryNode::~GeometryNode()
{
}
