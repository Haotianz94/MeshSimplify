#ifndef HEAD_H
#define HEAD_H

#include "common.h"

class BinaryHeap//˫�����ȶ��� 
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
    void percolateup(int i);//���� 
    void percolatedown(int i);//���� 
    void swap_(int i,int j);//���� 
    int properParent(int i);//�˽ڵ�������ӽڵ��и��ʺ������ڵ��ֵ 
    int parent(int i)//���ظ��ڵ� 
    {
        return (i-1)>>1;
    }
    int lChild(int i)//��������� 
    {
        i=((i<<1)+1);
        return (i<_n)? i:-1;
    }
    int rChild(int i)//�����Ҷ��� 
    {
        i=((i+1)<<1);
        return (i<_n)? i:-1;
    }
    bool parentVaild(int i)//�Ƿ��и��ڵ� 
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