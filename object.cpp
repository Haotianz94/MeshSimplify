#include"object.h"
#include<iostream>
#include<fstream>
#include<set>
#include<ctime>

using namespace std;

bool operator < (const Triangle& b, const Triangle& a)
{
	if( b._vertex[0]->id != a._vertex[0]->id )
		return ( b._vertex[0]->id < a._vertex[0]->id );
	else
		if( b._vertex[1]->id != a._vertex[1]->id )
			return (b._vertex[1]->id < a._vertex[1]->id );
		else
			return (b._vertex[2]->id < a._vertex[2]->id );
}

void Object::loadObj(char* obj)
{
	clock_t start = clock();

	FILE* fp = fopen(obj,"r");
	char buf[256];
	double x,y,z;
	int v1,v2,v3;
	int eid = 0;
	int tid = 0;

	while(fscanf(fp, "%s", buf) != EOF)
    {
		 switch(buf[0])
		{
			case 'v':			
				if( fscanf(fp, "%lf %lf %lf", &x,&y,&z) == 3)
					_vertex.push_back(new Vertex(Vec3(x*100, y*100, z*100) ) );
				break;
			case 'f':
				if( fscanf(fp, "%d %d %d", &v1,&v2,&v3) == 3)
				{
					v1--;
					v2--;
					v3--;
					_edge.push_back(new Edge(_vertex[v1],_vertex[v2]) );
					addNeighbor(_vertex[v1], eid);
					addNeighbor(_vertex[v2], eid++);
					_edge.push_back(new Edge(_vertex[v2],_vertex[v3]) );
					addNeighbor(_vertex[v2], eid);
					addNeighbor(_vertex[v3], eid++);
					_edge.push_back(new Edge(_vertex[v1],_vertex[v3]) );
					addNeighbor(_vertex[v1], eid);
					addNeighbor(_vertex[v3], eid++);
		
					Matrix4* matrix = new Matrix4(_vertex[v1], _vertex[v2], _vertex[v3]);
					_vertex[v1]->getMatrix()->add(matrix);
					_vertex[v2]->getMatrix()->add(matrix);
					_vertex[v3]->getMatrix()->add(matrix);

					_triangle[_tri++] = Triangle(_vertex[v1], _vertex[v2], _vertex[v3]);
					addTri(_vertex[v1], tid);
					addTri(_vertex[v2], tid);
					addTri(_vertex[v3], tid++);
				}
				break;
			default:
				fgets(buf, sizeof(buf), fp);
				break;
		 }
	}
	_V = _V0 = _vertex.size();

    clock_t end = clock();
	cout<<"load:  "<<1.0*(end - start)/CLOCKS_PER_SEC<<'s'<<endl;
}

void Object::simplify()
{
	clock_t start = clock();

	for( int i = 0;i < _edge.size();i++)
	{
			Matrix4 matrix;
			matrix.add(_edge[i]->_p->getMatrix());
			matrix.add(_edge[i]->_q->getMatrix());
			Vec3  v = (_edge[i]->_p->getPos() + _edge[i]->_q->getPos() ) * 0.5;
			_edge[i]->error = matrix.getError(v);
			_edge[i]->_v = v;
			_heap->push( i, _edge[i]->error);
	}
	_heap->heaplify();

	int t=0;
	while( _V > ratio *_V0 )
	{
		int eid = _heap->pop();
		if( _edge[eid]->_p->id == -1 || _edge[eid]->_q->id == -1)
			continue;
		_V--;
		Edge* e = _edge[eid];
		Vertex* vv[2];
		vv[0] = e->_p;
		vv[1] = e->_q;
		Vertex* newv = new Vertex(e->_v);
		newv->getMatrix()->add( vv[0]->getMatrix() );
		newv->getMatrix()->add( vv[1]->getMatrix() );
		_vertex.push_back(newv);
		vv[0]->id = -1;
		vv[1]->id = -1;

		// 处理临边
		int last = 0;
		for( int i = 0; i < 2; i++)
		{
			int pos = vv[i]->_nei;
			Vertex* neighbor;
			int nedge;
			while( pos != -1)
			{
				nedge = _npos[pos];
				if( i == 0)
					last = pos;
				pos = _nnext[pos];

					Edge* newe = _edge[nedge];
					neighbor = (newe->_p == vv[i])? newe->_q:newe->_p;
					if( neighbor->id == -1)
						continue;

						Matrix4 matrix;
						matrix.add( newv->getMatrix());
						matrix.add( neighbor->getMatrix());
						Vec3  bestv = ( newv->getPos() + neighbor->getPos() ) * 0.5;
						newe->error = matrix.getError(bestv);
						newe->_v = bestv;
						_heap->change( nedge, newe->error);
						(newe->_p == vv[i])? newe->_p = newv : newe->_q = newv;	
					
			}
		}
		_nnext[last] = vv[1]->_nei;
		newv->_nei = vv[0]->_nei;

		//处理临三角形
		last = 0; 
		for( int i = 0; i < 2; i++)
		{
			int pos = vv[i]->_tri;
			int neitri;
			while( pos != -1)
			{
				neitri = _tpos[pos];
				if( i == 0)
					last = pos;
				pos = _tnext[pos];

					Triangle& tri = _triangle[neitri];
					int old;
					for( int j = 0; j < 3; j++)
					{
						if( tri._vertex[j] == vv[i])
						{
							old = j;
							break;
						}
					}
					tri._vertex[old] = newv;		
			}
		}
		_tnext[last] = vv[1]->_tri;
		newv->_tri = vv[0]->_tri;
	}
	
	clock_t end = clock();
	cout<<"simplify:  "<<1.0*(end - start)/CLOCKS_PER_SEC<<'s'<<endl;
}

void Object::outputObj(char* obj)
{
	clock_t start = clock();
	ofstream fout(obj);

	int vid = 1;
	for( int i = 0; i < _vertex.size(); i++)
		if( _vertex[i]->id == 0)
		{
			Vec3 pos = _vertex[i]->getPos();
			_vertex[i]->id = vid++;
			fout<<"v  "<<pos.x<<' '<<pos.y<<' '<<pos.z<<endl;
		}

	for( int i = 0 ;i < _tri; i++)
		_triangle[i].sort();

	std::sort(_triangle, _triangle+_tri);

	int f = 0;
	int id[3];
	id[0] = -1;
	id[1] = -1;
	id[2] = -1;
	for( int i = 0; i < _tri; i++)
	{
		bool vaild = true;
		Triangle& tri = _triangle[i];
		for( int j = 0; j < 3; j++)
			if( tri._vertex[j]->id == -1)
			{
				vaild = false;
				break;
			}
		if( tri._vertex[0]->id == tri._vertex[1]->id || tri._vertex[1]->id == tri._vertex[2]->id )
			vaild = false;

		bool same = true;
		if( vaild)
			for( int j = 0; j < 3; j++)
			{
				if( id[j] != tri._vertex[j]->id )
					same = false;
				id[j] = tri._vertex[j]->id;
			}
		if( !vaild || same )
			continue;

		f++;
		fout<<"f  ";
		for( int j = 0 ;j < 3 ;j++)
			fout<<tri._vertex[j]->id<<' ';
		fout<<endl;
	}
	clock_t end = clock();
	cout<<"output:  "<<1.0*(end - start)/CLOCKS_PER_SEC<<'s'<<endl;
}

void Object::addNeighbor(Vertex* v, int eid)
{
	if( v->_nei == -1)
	{
		v->_nei = _nhead++;
		_npos[v->_nei] = eid;
		_nnext[v->_nei] = -1;
	}
	else
	{
		int npos = _nhead++;
		_npos[npos] = eid;
		_nnext[npos] = v->_nei;
		v->_nei = npos;
	}
}
void Object::addTri(Vertex* v, int tid)
{
	if( v->_tri == -1)
	{
		v->_tri = _thead++;
		_tpos[v->_tri] = tid;
		_tnext[v->_tri] = -1;
	}
	else
	{
		int tpos = _thead++;
		_tpos[tpos] = tid;
		_tnext[tpos] = v->_tri;
		v->_tri = tpos;
	}
}
