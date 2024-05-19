#pragma once
#include <iostream>
using namespace std;

template <typename T>
class Queue
{
public:

	Queue()
	{
		size = 0;
		tail = head = nullptr;
	}
	~Queue()
	{
		clear();
	}

	void clear()
	{
		clearHelper(head);
		head = tail = nullptr;
		size = 0;
	}

	bool is_empty() const
	{
		if (head == nullptr)
			return true;
		else
		{
			return false;
		}
	}

	void queue(T data)
	{
		Node* newnode = new Node(data);
		if (is_empty())
		{
			head = tail = newnode;
		}
		else
		{
			tail->next = newnode;
			tail = newnode;
		}
		size++;
	}

	T unqueue()
	{
		if (is_empty())
		{
			throw std::out_of_range("Queue is empty");
		}
		else
		{
			Node* temp = head;
			T data = head->data;
			head = head->next;
			delete temp;
			if (head == nullptr)
			{
				tail = nullptr;
			}
			size--;
			return data;
		}
	}

	int count() const
	{
		return size;
	}
	int count(T data) const
	{
		int count = 0;
		for (Node* temp = head; temp != nullptr; temp = temp->next)
		{
			if (temp->data == data)
			{
				count++;
			}
		}
		return count;
	}

private:
	class Node
	{
	public:
		Node* next;
		T data;
		Node(T data) : data(data), next(nullptr) {}
	};
	Node* tail;
	Node* head;
	int size;

	void clearHelper(Node* node)
	{
		if (node == nullptr)
		{
			return;
		}
		clearHelper(node->next);
		delete node;
	}
};
