#pragma once
class Node
{
public:
	int x = -1;
	int y = -1;
	double f, g, h;
	Node* parentNode;
	bool operator == (const Node& rhs) const;
};