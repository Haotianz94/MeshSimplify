#include "heap.h"
#include<iostream>
#include<fstream>

using namespace std;

void BinaryHeap::swap_(int i, int j)
{
    if(i==j)
        return;
 
	_pos[_key[i]] = j;
	_pos[_key[j]] = i;
	swap(_key[i], _key[j]);
}

int BinaryHeap::pop()//与删除最大元素操作相对应 
{
    if(_n==0)
        return 0;
    int eid = _key[0];
    swap_(0,_n-1);
	_pos[_key[_n-1]] = -1;
	_n--;

    percolatedown(0);
	return eid;
}

void BinaryHeap::push(int id, double error)
{
	_key[_n] = id; 
	_pos[id] = _n;
	_error[id] = error;
	_n++;
}

void BinaryHeap::heaplify()
{
	int i = parent(_n-1);
	for( int j = i; j >= 0; j--)
		percolatedown(j);
}

void BinaryHeap::percolateup(int i)
{
    while(parentVaild(i))//还有父节点 
    {
        int j=parent(i);
        if(cmp(i,j))//若已经满足堆序，则上滤结束 
            break;
        else
        {
            swap_(i,j);//否则父子交换 
            i=j;
        }
    }
}

void BinaryHeap::percolatedown(int i)
{
	int j;
    while(i!=(j=properParent(i)))//若满足堆序，则结束下滤 
    {
        swap_(i,j);
        i=j;
    }
}

int BinaryHeap::properParent(int i)
{
        int l=lChild(i);
        int r=rChild(i);
        if(l==-1&&r==-1)//没有子节点 
            return i;
        if( (l==-1&&r!=-1) )//只有右儿子 
        {
            if(cmp(r,i))
                return i;
            else
                return r;
        }
        if( (l!=-1&&r==-1) )//只有左儿子 
        {
            if(cmp(l,i))
                return i;
            else
                return l;
        }
		//两个儿子都有 
        if( cmp(l,i) )
			if( cmp(r,i) )
				return i;
			else
				return r;
		else
			if( cmp(r,l) )
				return l;
			else
				return r;
}

void BinaryHeap::change( int id, double error)
{
	int pos = _pos[id];
	_error[id] = error;
	percolateup(pos);
	percolatedown(pos);
}