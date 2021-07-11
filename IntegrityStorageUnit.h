#ifndef INTEGRITY_STORAGE_UNIT
#define INTEGRITY_STORAGE_UNIT

#include <iostream>
#include "Nodes.h"
using namespace std;

namespace Py {

	template<typename T>
	class IntegrityContainer {
		// we need a pointer to an array on heap
		// we need to maintain size
		// we need a binary node

		using pointer = BinaryNode<T>*;
		using value = BinaryNode<T>;

		// integrity array is the register of the data with which all the 
		T* integrity_array = nullptr;
		int array_size = 0;

		// this will be for the management of linked list
		pointer _top_ = nullptr;
		pointer _bottom_ = nullptr;
		int list_size = 0;

		// the use() function will maintain the record for the element in use
		T element_in_use;

		// the key that tells the member function if a value is in use
		bool is_element_in_use = false;

		// compatible with the iterators that have ++ and * operator overloaded
		template<typename iter>
		void allocate_and_initialize(iter beg, iter end) {
			// making the list first and increasing the list_size
			iter begin = beg;
			this->_top_ = new value{ nullptr };
			this->_top_->data = *begin;
			this->_top_->prev = nullptr;
			this->_top_->next = nullptr;

			this->_bottom_ = this->_top_;
			this->list_size++;
			++begin;

			while (begin != end) {
				pointer temp = new value{nullptr};
				temp->data = *begin;
				temp->prev = nullptr;
				temp->next = nullptr;

				// connecting to the bottom
				this->_bottom_->next = temp;
				temp->prev = this->_bottom_;

				// making the temp the bottom;
				this->_bottom_ = temp;

				this->list_size++;
				++begin;
			}

			this->integrity_array = new T[this->list_size]{};

			for (begin = beg; begin != end; begin++) {
				this->integrity_array[this->array_size++] = *begin;
			}

		}

	public:
		IntegrityContainer() = delete;

		IntegrityContainer(initializer_list<T> init_l) {
			this->allocate_and_initialize(init_l.begin(), init_l.end());
		}

		// compatible with the iterators that have ++ and * operator overloaded
		template<typename iter>
		IntegrityContainer(iter begin, iter end) {
			this->allocate_and_initialize(begin,end);
		}

		IntegrityContainer(const IntegrityContainer& obj) {
			this->integrity_array = new T[obj.list_size]{};
			this->array_size = obj.array_size;
			this->element_in_use = obj.element_in_use;
			this->is_element_in_use = obj.is_element_in_use;

			// filling the integrity array
			for (int i = 0;i<obj.list_size;i++)
				this->integrity_array[i] = obj.integrity_array[i];

			pointer begin = obj._top_;
			this->_top_ = new value{ nullptr };
			this->_top_->data = begin->data;
			this->_top_->prev = nullptr;
			this->_top_->next = nullptr;

			this->_bottom_ = this->_top_;
			this->list_size++;
			begin = begin->next;;

			while (begin) {
				pointer temp = new value{ nullptr };
				temp->data = begin->data;
				temp->prev = nullptr;
				temp->next = nullptr;

				// connecting to the bottom
				this->_bottom_->next = temp;
				temp->prev = this->_bottom_;

				// making the temp the bottom;
				this->_bottom_ = temp;

				this->list_size++;
				begin = begin->next;;
			}
		}

		// once a container is created it cannot be reassigned

		IntegrityContainer& operator=(const IntegrityContainer& obj) = delete;
		IntegrityContainer& operator=(IntegrityContainer&& obj) = delete;

		int size() {return list_size; }

		void show_list() {
			pointer temp = this->_top_;
			cout << "List: ";
			while (temp) {
				cout << temp->data << " ";
				temp = temp->next;
			}cout << endl;
		}

		// shows data's copy stored in the array
		void show_order() {
			cout << "Integrity Array: ";
			for (int i = 0; i < this->array_size; i++) {
				cout << this->integrity_array[i] << " ";
			}cout << endl;
		}

		// the use function returns the top value and maintains a record what it returned
		T use() {
			if (list_size > 0 and !this->is_element_in_use) {
				pointer temp = this->_top_;
				this->_top_ = this->_top_->next;
				this->element_in_use = temp->data;
				delete temp;
				is_element_in_use = true;
				return element_in_use;
			}
			else {
				throw(0);
			}
		}

		// restore function restores the value that was used by earlier
		void restore() {
			if (list_size > 0 and this->is_element_in_use) {
				pointer temp = new value{nullptr};
				temp->data = element_in_use;
				temp->prev = nullptr;
				temp->next = nullptr;

				// putti9ng the restored element at the last
				this->_bottom_->next = temp;
				temp->prev = this->_bottom_;

				this->_bottom_ = temp;

				is_element_in_use = false;
			}
			else {
				throw(0);
			}
		}

		// restores the original order
		void restore_order() {
			if (this->is_element_in_use) { this->restore(); }

			pointer temp = this->_top_;

			for (int i = 0; i < this->array_size; i++) {
				temp->data = this->integrity_array[i];
				temp = temp->next;
			}
		}

	private:
		void clear() {
			// clearing the array
			if (integrity_array)
				delete[] integrity_array;

			// clearing the list
			while (this->_top_) {
				pointer temp = this->_top_;
				this->_top_ = this->_top_->next;
				delete temp;
				temp = nullptr;
			}
		}

	public:
		~IntegrityContainer(){
			this->clear();
		}
	};

}

#endif // !INTEGRITY_STORAGE_UNIT
