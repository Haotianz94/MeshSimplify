#include "common.h"
#include <iostream>

Matrix4::Matrix4(Vertex *A, Vertex* B, Vertex* C)
{
	Vec3 _A = A->getPos();
	Vec3 _B = B->getPos();
	Vec3 _C = C->getPos();
	Vec3 p1 = _B - _A;
	Vec3 p2 = _C - _A;
	Vec3 N = p1.cross(p2);
	N.normalize();
	double a[4];
	a[0] = N.x;
	a[1] = N.y;
	a[2] = N.z;
	a[3] = -N.dot(_A);
	for( int i = 0 ;i < 4 ; i++)
		for( int j = 0; j < 4; j++)
			_M[j][i] = a[i]*a[j];
}
Vec3 Matrix4::getMinmum()
{
	double a = 0, b = 0 ,c = 0, d = 0, e = 0, f = 0, g = 0, h = 0, i = 0;
	for( int k = 0 ;k < 4; k++)
	{
		a += _M[k][0];
		b += _M[k][1];
		c += _M[k][2];
		d += (_M[k][0]+_M[k][1]);
		e += (_M[k][0]+_M[k][2]);
		f += (_M[k][1]+_M[k][2]);
		g += (_M[k][0]+_M[k][3]);
		h += (_M[k][1]+_M[k][3]);
		i += (_M[k][2]+_M[k][3]);
	}
	d *= 0.5;
	e *= 0.5;
	f *= 0.5;
	g *= 0.5;
	h *= 0.5;
	i *= 0.5;
	double A[3][3]; 
	double B[3];
	A[0][0] = a;
	A[0][1] = d;
	A[0][2] = e;
	A[1][0] = d;
	A[1][1] = b;
	A[1][2] = f;
	A[2][0] = e;
	A[2][1] = f;
	A[2][2] = c;
	B[0] = -g;
	B[1] = -h;
	B[2] = -i;

	double r = 0;
	for( int i = 1; i < 3 ; i++)
	{
		r = A[i][0] / A[0][0];
		for ( int j = 0; j < 3; j++)
		{
			A[i][j] = A[i][j] - A[0][j]*r;
		}
		B[i] = B[i] - B[0]*r;
	}
	r = A[2][1] / A[1][1];
	A[2][2] = A[2][2] - A[1][2]*r;
	B[2] = B[2] - B[1]*r;

	double x,y,z;
	z = B[2] / A[2][2];
	y = (B[1] - A[1][2]*z) / A[1][1];
	x = (B[0] - A[0][2]*z - A[0][1]*y) / A[0][0];
	return Vec3(x,y,z);
}

double Matrix4::getError(Vec3& v)
	{
		double vp[4];
		vp[0] = v.x;
		vp[1] = v.y;
		vp[2] = v.z;
		vp[3] = 1;
		double tmp[4];
		for( int i = 0; i < 4;i ++)
		{
			tmp[i] = 0;
			for( int j = 0; j < 4; j++)
				tmp[i] += vp[j]*_M[j][i];
		}
		double res = 0;
		for( int i = 0; i < 4; i++)
			res += tmp[i]*vp[i];
		return res;
	}

void Triangle::sort()
{
	for( int i = 0 ; i < 2 ;i ++)
		for( int j = i+1 ;j < 3 ;j++)
			if( _vertex[j]->id < _vertex[i]->id )
				std::swap(_vertex[i], _vertex[j]);
}


