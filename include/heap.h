#ifndef HEAD_H
#define HEAD_H

#include "common.h"

class BinaryHeap//双端优先队列 
{
private:
    int _key[1000000]; 
    int _pos[1000000]; 
	double _error[1000000];
	int _n;

	bool cmp(int i,int j)
	{
		return (_error[_key[j]]<=_error[_key[i]]);
	} 
    void percolateup(int i);//上滤 
    void percolatedown(int i);//下滤 
    void swap_(int i,int j);//交换 
    int properParent(int i);//此节点和两个子节点中更适合做父节点的值 
    int parent(int i)//返回父节点 
    {
        return (i-1)>>1;
    }
    int lChild(int i)//返回左儿子 
    {
        i=((i<<1)+1);
        return (i<_n)? i:-1;
    }
    int rChild(int i)//返回右儿子 
    {
        i=((i+1)<<1);
        return (i<_n)? i:-1;
    }
    bool parentVaild(int i)//是否还有父节点 
    {
        return (i>0)&&(i<_n);
    }
public:
    BinaryHeap():_n(0)
	{
		for( int i = 0; i < 1000000 ;i++)
			_pos[i] = -1;
	}
    int pop();
	void push( int id ,double error);
	void change( int id ,double error);
	void heaplify();
};

#endif