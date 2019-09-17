#ifndef OBJECT_H
#define OBJECT_H

#include "common.h"
#include "heap.h"

class Object
{
private:
	std::vector<Vertex*> _vertex;
	std::vector<Edge*> _edge;
	int* _npos;
	int* _nnext;
	int _nhead;
	int* _tpos;
	int* _tnext;
	int _thead;
	Triangle* _triangle;
	BinaryHeap* _heap;
	int _V0;
	int _V;
	double ratio;
	int _tri;
public:
	Object() 
	{
		_heap = new BinaryHeap();
		_triangle = new Triangle[500000];
		_tri = 0;
		_npos = new int[10000000];
		_nnext = new int[10000000];
		_tpos = new int[10000000];
		_tnext = new int[10000000];
		_nhead = 0;
		_thead = 0;
	}
	~Object()
	{
		delete[] _triangle;
		delete[] _npos;
		delete[] _nnext;
		delete[] _tpos;
		delete[] _tnext;
	}
	void setRatio( double r) { ratio = r; }
	void loadObj(char* obj);
	void simplify();
	void outputObj(char* obj);
	void addNeighbor(Vertex* v, int eid);
	void addTri(Vertex* v, int tid);
};
#endif