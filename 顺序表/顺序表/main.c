#include<stdio.h>
#include<stdbool.h>
#include<memory.h>
#include<assert.h>
#include<malloc.h>
#define MAX_SIZE 100
#define INIT_SIZE 3
typedef int DataType;

//顺序表的静态存储
typedef struct SeqList_s
{
	DataType array[MAX_SIZE];  //数据段
	size_t size;    //数据的个数
}SeqList_s;

//顺序表的动态存储
typedef struct SeqList_d
{
	DataType* array;  //数据块指针
	size_t size;   //有效数据的个数
	size_t capacity;  //容量
}SeqList_d;

//检查容量，增容
void CheckCapacity(SeqList_d *pSeq)
{
	assert(pSeq);
	DataType* temp;
	if (pSeq->size >= pSeq->capacity)
	{
		pSeq->capacity *= 2;  //容量增至2倍
		temp = (DataType*)realloc(pSeq->array, pSeq->capacity*sizeof(DataType));
		if (NULL == temp)
		{
			return;//如果没有增容成功，返回
		}
		pSeq->array = temp;
	}
	else
	{
		return;
	}
}

//初始化
int InitSeqList_d(SeqList_d* pSeq)
{
	assert(pSeq);
	pSeq->array = (DataType*)malloc(sizeof(DataType)*INIT_SIZE);
	if (NULL == pSeq)
	{
		return 0;   //未成功
	}
	pSeq->size = 0;
	pSeq->capacity = INIT_SIZE;
	return 1;
}

//查找
size_t Find(SeqList_d* pSeq, DataType x)
{
	assert(pSeq);
	size_t pos = pSeq->size;   //用pos记录x所在位置，若大于等于size，则说明未找到
	for (size_t i = 0; i < pSeq->size; i++)
	{
		if (*(pSeq->array + i) == x)
		{
			pos = i;
			break;
		}
	}
	if (pos >= pSeq->size)
	{
		printf("未找到\n");
	}
	return pos;
}

//尾插
void PushBack(SeqList_d* pSeq, DataType x)
{
	assert(pSeq != NULL);
	CheckCapacity(pSeq);
	pSeq->array[pSeq->size++] = x;
}

//插入
void Insert(SeqList_d* pSeq, size_t pos, DataType x)
{
	assert(pSeq);
	CheckCapacity(pSeq);
	if (pos > pSeq->size)
	{
		printf("插入位置有误！\n");
		return;
	}

	DataType* temp = pSeq->array + pSeq->size - 1;
	while (temp >= (pSeq->array + pos))
	{
		*(temp + 1) = temp;
		temp--;
	}

	////也可以这样移动
	//for (size_t i = pSeq->size - 1; i >= pos; i--)
	//{
	//	pSeq->array[i+1] = pSeq->array[i];
	//}

	*(pSeq->array + pos) = x;
	pSeq->size++;
}

//删除pos位置的数据
void Erase(SeqList_d* pSeq, size_t pos)
{
	assert(pSeq);
	if (pos < 0 || pos >= pSeq->size)
	{
		printf("要删除的位置有误！\n");
		return;
	}
	for (size_t i = pos; i < pSeq->size; i++)
	{
		*(pSeq->array + i) = *(pSeq->array + i + 1);
	}
	pSeq->size--;
}

//删除找到的第一个x
void Remove(SeqList_d* pSeq, DataType x)
{
	assert(pSeq);
	size_t pos = Find(pSeq, x);
	if (pos < 0 || pos >= pSeq->size)
	{
		printf("没有x这个数据！\n");
		return;
	}
	else
	{
		for (size_t i = pos; i < pSeq->size; i++)
		{
			*(pSeq->array + i) = *(pSeq->array + i + 1);
		}
		pSeq->size--;
	}
}

//删除所有的x(有优化)
void RemoveAll(SeqList_d* pSeq, DataType x)
{
	assert(pSeq);
	size_t first = 0, second = 0, count = 0;
	while (second < pSeq->size)
	{
		if (*(pSeq->array + second) == x)
		{
			count++;
		}
		else
		{
			*(pSeq->array + first) = *(pSeq->array + second);
			first++;
		}
		second++;
	}
	pSeq->size -= count;
}

//排序(从小到大)
void Sort(SeqList_d* pSeq)
{
	assert(pSeq);
	if (pSeq->size <= 0)
	{
		return;
	}
	DataType temp;
	bool flag = true;
	for (size_t i = 0; (i < pSeq->size - 1) && flag; i++)   //进行size-1趟排序
	{
		flag = false;
		for (size_t j = 0; j < pSeq->size - i - 1; j++)
		{
			if (*(pSeq->array + j) > *(pSeq->array + j + 1))
			{
				temp = *(pSeq->array + j);
				*(pSeq->array + j) = *(pSeq->array + j + 1);
				*(pSeq->array + j + 1) = temp;
				flag = true;
			}
		}
	}
}

//二分查找（非递归）
int BinarySearch(SeqList_d* pSeq, DataType x)
{
	assert(pSeq);
	int left = 0, right = pSeq->size - 1;
	int mid;
	while (left <= right)
	{
		mid = left + (right - left) / 2;   //这样算mid可以避免溢出
		if (*(pSeq->array + mid) < x)   //x在后边部分
		{
			left = mid + 1;
		}
		else if (*(pSeq->array + mid) > x)   //x在前半部分
		{
			right = mid - 1;
		}
		else
		{
			return mid;
			break;
		}
	}
	return -1;
}

//二分查找（递归）
int _BinarySearch_R(SeqList_d* pSeq, int left, int right, DataType x)
{
	assert(pSeq);
	if (left <= right)
	{
		int mid = left + (right - left) / 2;
		if (pSeq->array[mid] < x)
		{
			return _BinarySearch_R(pSeq, mid + 1, right, x);
		}
		else if (pSeq->array[mid] > x)
		{
			return _BinarySearch_R(pSeq, left, mid - 1, x);
		}
		else
		{
			return mid;
		}
	}
	return -1;
}
int Binary_R(SeqList_d* pSeq, DataType x)
{
	return _BinarySearch_R(pSeq, 0, pSeq->size - 1, x);
}
void Test()
{
	SeqList_d s;
	InitSeqList_d(&s);
	PushBack(&s, 1);
	PushBack(&s, 2);
	PushBack(&s, 3);
	PushBack(&s, 6);
	Erase(&s, 3);
	int ret = Find(&s, 6);
	printf("%d\n",ret);
}
int main()
{
	Test();
	return 0;
}