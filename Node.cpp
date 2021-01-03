#include "Node.h"

bool Node::operator==(const Node& rhs) const
{
	//Two nodes are considered the same if they have X and Y values that match each other
	return this->x == rhs.x && this->y == rhs.y;
}
