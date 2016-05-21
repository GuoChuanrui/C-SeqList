#include<stdio.h>
#include<stdbool.h>
#include<memory.h>
#include<assert.h>
#include<malloc.h>
#define MAX_SIZE 100
#define INIT_SIZE 3
typedef int DataType;

//˳���ľ�̬�洢
typedef struct SeqList_s
{
	DataType array[MAX_SIZE];  //���ݶ�
	size_t size;    //���ݵĸ���
}SeqList_s;

//˳���Ķ�̬�洢
typedef struct SeqList_d
{
	DataType* array;  //���ݿ�ָ��
	size_t size;   //��Ч���ݵĸ���
	size_t capacity;  //����
}SeqList_d;

//�������������
void CheckCapacity(SeqList_d *pSeq)
{
	assert(pSeq);
	DataType* temp;
	if (pSeq->size >= pSeq->capacity)
	{
		pSeq->capacity *= 2;  //��������2��
		temp = (DataType*)realloc(pSeq->array, pSeq->capacity*sizeof(DataType));
		if (NULL == temp)
		{
			return;//���û�����ݳɹ�������
		}
		pSeq->array = temp;
	}
	else
	{
		return;
	}
}

//��ʼ��
int InitSeqList_d(SeqList_d* pSeq)
{
	assert(pSeq);
	pSeq->array = (DataType*)malloc(sizeof(DataType)*INIT_SIZE);
	if (NULL == pSeq)
	{
		return 0;   //δ�ɹ�
	}
	pSeq->size = 0;
	pSeq->capacity = INIT_SIZE;
	return 1;
}

//����
size_t Find(SeqList_d* pSeq, DataType x)
{
	assert(pSeq);
	size_t pos = pSeq->size;   //��pos��¼x����λ�ã������ڵ���size����˵��δ�ҵ�
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
		printf("δ�ҵ�\n");
	}
	return pos;
}

//β��
void PushBack(SeqList_d* pSeq, DataType x)
{
	assert(pSeq != NULL);
	CheckCapacity(pSeq);
	pSeq->array[pSeq->size++] = x;
}

//����
void Insert(SeqList_d* pSeq, size_t pos, DataType x)
{
	assert(pSeq);
	CheckCapacity(pSeq);
	if (pos > pSeq->size)
	{
		printf("����λ������\n");
		return;
	}

	DataType* temp = pSeq->array + pSeq->size - 1;
	while (temp >= (pSeq->array + pos))
	{
		*(temp + 1) = temp;
		temp--;
	}

	////Ҳ���������ƶ�
	//for (size_t i = pSeq->size - 1; i >= pos; i--)
	//{
	//	pSeq->array[i+1] = pSeq->array[i];
	//}

	*(pSeq->array + pos) = x;
	pSeq->size++;
}

//ɾ��posλ�õ�����
void Erase(SeqList_d* pSeq, size_t pos)
{
	assert(pSeq);
	if (pos < 0 || pos >= pSeq->size)
	{
		printf("Ҫɾ����λ������\n");
		return;
	}
	for (size_t i = pos; i < pSeq->size; i++)
	{
		*(pSeq->array + i) = *(pSeq->array + i + 1);
	}
	pSeq->size--;
}

//ɾ���ҵ��ĵ�һ��x
void Remove(SeqList_d* pSeq, DataType x)
{
	assert(pSeq);
	size_t pos = Find(pSeq, x);
	if (pos < 0 || pos >= pSeq->size)
	{
		printf("û��x������ݣ�\n");
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

//ɾ�����е�x(���Ż�)
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

//����(��С����)
void Sort(SeqList_d* pSeq)
{
	assert(pSeq);
	if (pSeq->size <= 0)
	{
		return;
	}
	DataType temp;
	bool flag = true;
	for (size_t i = 0; (i < pSeq->size - 1) && flag; i++)   //����size-1������
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

//���ֲ��ң��ǵݹ飩
int BinarySearch(SeqList_d* pSeq, DataType x)
{
	assert(pSeq);
	int left = 0, right = pSeq->size - 1;
	int mid;
	while (left <= right)
	{
		mid = left + (right - left) / 2;   //������mid���Ա������
		if (*(pSeq->array + mid) < x)   //x�ں�߲���
		{
			left = mid + 1;
		}
		else if (*(pSeq->array + mid) > x)   //x��ǰ�벿��
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

//���ֲ��ң��ݹ飩
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