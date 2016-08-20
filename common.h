#ifndef COMMON_H
#define COMMON_H

#include<cmath>
#include<vector>
#include<algorithm>  

class Vec3
{
public:
	union
	{
		struct { double x, y, z; };
		struct { double cell[3]; };
	};
	
	Vec3() : x(0), y(0), z(0) {};
	Vec3( double X_, double Y_, double Z_ ) : x(X_), y(Y_), z(Z_) {};

	void normalize() { double l = 1.0f / length(); x *= l; y *= l; z *= l; }
	double length() { return (double)sqrt( x * x + y * y + z * z ); }
	double sqrLength() { return x * x + y * y + z * z; }
	Vec3 cross( Vec3 b ) { return Vec3( y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x ); }
	double det(Vec3 b, Vec3 c)
	{
		return x*b.y*c.z + b.x*c.y*z + y*b.z*c.x - c.x*b.y*z - y*b.x*c.z - b.z*c.y*x ;
	}
	double dot(Vec3 b)
	{
		return x*b.x + y*b.y + z*b.z;
	}

	void operator += ( Vec3& a_V ) { x += a_V.x; y += a_V.y; z += a_V.z; }
	void operator += ( Vec3* a_V ) { x += a_V->x; y += a_V->y; z += a_V->z; }
	void operator -= ( Vec3& a_V ) { x -= a_V.x; y -= a_V.y; z -= a_V.z; }
	void operator -= ( Vec3* a_V ) { x -= a_V->x; y -= a_V->y; z -= a_V->z; }
	void operator *= ( double f ) { x *= f; y *= f; z *= f; }
	Vec3 operator- () const { return Vec3( -x, -y, -z ); }
	friend Vec3 operator + ( const Vec3& v1, const Vec3& v2 ) { return Vec3( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z ); }
	friend Vec3 operator - ( const Vec3& v1, const Vec3& v2 ) { return Vec3( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ); }
	friend Vec3 operator + ( const Vec3& v1, Vec3* v2 ) { return Vec3( v1.x + v2->x, v1.y + v2->y, v1.z + v2->z ); }
	friend Vec3 operator - ( const Vec3& v1, Vec3* v2 ) { return Vec3( v1.x - v2->x, v1.y - v2->y, v1.z - v2->z ); }
	friend Vec3 operator * ( const Vec3& v, double f ) { return Vec3( v.x * f, v.y * f, v.z * f ); }
	friend Vec3 operator * ( const Vec3& v1, Vec3& v2 ) { return Vec3( v1.x * v2.x, v1.y * v2.y, v1.z * v2.z ); }
	friend Vec3 operator * ( double f, const Vec3& v ) { return Vec3( v.x * f, v.y * f, v.z * f ); }
	
};

class Vertex;
class Matrix4
{
public:
	double _M[4][4];

	Matrix4()
	{
		for( int i = 0 ; i < 4 ; i ++)
			for( int j = 0 ; j < 4 ; j++)
				_M[i][j] = 0;
	}
	Matrix4( Vertex* A, Vertex* B, Vertex* C);
	void add( Matrix4* m)
	{
		for( int i = 0 ; i < 4 ; i ++)
			for( int j = 0 ; j < 4 ; j++)
				_M[i][j] += m->_M[i][j];
	}
	Vec3 getMinmum();
	double getError(Vec3& v);
};

class Vertex
{
private:
	Vec3 _pos;
	Matrix4* _matrix;
public:
	int id;
	int _nei;
	int _tri;
public:
	Vertex(Vec3 pos):_pos(pos), id(0), _nei(-1), _tri(-1)
	{
		_matrix = new Matrix4;
	}
	Vec3& getPos() { return _pos;}
	Matrix4* getMatrix() { return _matrix; }
};

class Triangle
{
public:
	Vertex* _vertex[3];
	Triangle() {}
	Triangle( Vertex* a, Vertex* b, Vertex* c)
	{
		_vertex[0] = a;
		_vertex[1] = b;
		_vertex[2] = c;
	}
	void sort();
};

class Edge
{
public:
	Vertex* _p;
	Vertex* _q;
	double error;
	Vec3 _v;
	Edge(Vertex* p, Vertex* q):_p(p), _q(q){}
};

#endif
