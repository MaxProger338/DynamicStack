#include <iostream>
#include <initializer_list>
using namespace std;

typedef unsigned int uint;

template<typename T>
class DynamicStack
{
	private:
		class DoubleLinkedList
		{
			private:
				class Node
				{
					public:
						T value;
						Node* next;
						Node* prev;

						Node(T value) : value{ value }, next{ nullptr }, prev{ nullptr } {}
				};

				Node* _first;
				Node* _last;

			public:
				DoubleLinkedList() : _first{ nullptr }, _last{ nullptr } {}

				DoubleLinkedList(initializer_list<T> list) : DoubleLinkedList()
				{
					for (auto i : list)
					{
						addBack(i);
					}
				}

				DoubleLinkedList(const DoubleLinkedList& list) : DoubleLinkedList()
				{
					Node* current = list._first;

					while (current)
					{
						addBack(current->value);
						current = current->next;
					}
				}

				DoubleLinkedList(DoubleLinkedList&& list)
				{
					Node* current = list._first;
					_first = current;
					_last = list._last;

					while (current)
					{
						_first->next = current->next;
						_first->prev = current->prev;
						_first = _first->next;

						current = current->next;
					}
					_first = list._first;

					list._first = nullptr;
					list._last = nullptr;
				}

				~DoubleLinkedList()
				{
					if (!isEmpty())
					{
						Node* current = _first->next;
						while (current)
						{
							delete _first;
							_first = current;
							current = current->next;
						}

						delete _first;

						_first = nullptr;
						_last = nullptr;
					}
				}

				bool isEmpty() const
				{
					return _first == nullptr;
				}

				uint getSize() const
				{
					uint counter = 0;

					Node* current = _first;
					while (current)
					{
						counter++;
						current = current->next;
					}

					return counter;
				}

				DoubleLinkedList& clear()
				{
					this->~DoubleLinkedList();

					return *this;
				}

				T getLast() const
				{
					return _last->value;
				}

				DoubleLinkedList& addBack(T value)
				{
					Node* node = new Node(value);
					if (isEmpty())
					{
						_first = node;
						_last = node;
					}
					else
					{
						_last->next = node;
						node->prev = _last;
						_last = node;
					}

					return *this;
				}

				T deleteBack()
				{
					if (isEmpty())
						throw "Out of range";

					Node* current = _last;
					T value = current->value;

					_last = current->prev;
					_last->next = nullptr;

					delete current;

					return value;
				}

				friend ostream& operator<<(ostream& out, const DoubleLinkedList& list)
				{
					Node* current = list._first;
					while (current)
					{
						out << current->value << '\t';
						current = current->next;
					}
					out << endl;

					return out;
				}
		};

		DoubleLinkedList _list;

	public:
		DynamicStack() {}

		explicit DynamicStack(initializer_list<T> list) : _list{list} {}

		DynamicStack(const DynamicStack& stack) : _list{stack._list} {}

		DynamicStack(DynamicStack&& stack) : _list{ move(stack._list) } {}

		bool isEmpty() const
		{
			return _list.isEmpty();
		}

		uint getSize() const
		{
			return _list.getSize();
		}

		DynamicStack& clear()
		{
			_list.clear();

			return *this;
		}

		DynamicStack& push(T value)
		{
			_list.addBack(value);

			return *this;
		}

		T pop()
		{
			return _list.deleteBack();
		}

		T peek() const
		{
			return _list.getLast();
		}

		friend ostream& operator<<(ostream& out, const DynamicStack& stack)
		{
			out << stack._list << endl;

			return out;
		}
};

int main()
{
	DynamicStack<int> stack;

	stack.push(1);
	stack.push(2);
	stack.push(3);

	cout << stack << endl;

	return 0;
}
