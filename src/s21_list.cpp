#include "s21_list.h"


namespace s21 {

    // List Functions
    template <typename value_type>
    list<value_type>::list() : head(nullptr), tail(nullptr), end_(nullptr), size_(0) {
        end_ = new Node(size_);
        change_end();
    } // default constructor


    template <typename value_type>
    list<value_type>::list(size_type n) : head(nullptr), tail(nullptr), end_(nullptr), size_(0) { // constructor with parameter
        if (n >= max_size()) {
            throw std::out_of_range("Limit of the list is exceeded");
        }
        end_ = new Node(size_);
        for(size_type i = 0; i < n; i++) {
            push_back(value_type());
        }
        change_end();
    }


    template <typename value_type>
    list<value_type>::list(std::initializer_list<value_type> const &items) : head(nullptr), tail(nullptr), end_(nullptr), size_(0) { // constructor wiht ining value
        end_ = new Node(size_);
        for (const auto& item : items) {
            push_back(item);
            change_end();
        }
    }


    template <typename value_type>
    list<value_type>::list(const list &l) : head(nullptr), tail(nullptr), end_(nullptr), size_(0) { // copy constructor
        end_ = new Node(size_);
        this->copy(l);
    }

    template <typename value_type>
    list<value_type>::list(list &&l) : head(nullptr), tail(nullptr), end_(nullptr), size_(0) { // move constructor
        swap(l);
    }

    template <typename value_type>
    list<value_type>::~list() { // destructor
        clear();
        delete end_;
    }

    template <typename value_type>
    typename list<value_type>::list& list<value_type>::operator=(list&& l) {
        if (this != &l) {
            clear();
            swap(l);
        }
        return *this;
    }

    // List Element access

    template <typename value_type>
    typename list<value_type>::const_reference list<value_type>::front() {
        return !head ? end_->data : head->data;
    }

    template <typename value_type>
    typename list<value_type>::const_reference list<value_type>::back() {
        return !tail ? end_->data : tail->data;
    }

    // List Iterators
    template <typename value_type>
    typename list<value_type>::iterator list<value_type>::begin() {
        return !head ? iterator(end_) : iterator(head);
    }

    template <typename value_type>
    typename list<value_type>::iterator list<value_type>::end() {
        return iterator(end_);
    }

    template <typename value_type>
    typename list<value_type>::const_iterator list<value_type>::begin() const {
        return !head ? const_iterator(end_) : const_iterator(head);
    }

    template <typename value_type>
    typename list<value_type>::const_iterator list<value_type>::end() const {
        return const_iterator(end_);
    }

    // List Capacity
    template <typename value_type>
    bool list<value_type>::empty() {
        return size_ == 0;
    }

    template <typename value_type>
    typename list<value_type>::size_type list<value_type>::size() {
        return size_;
    }

    template <typename value_type>
    typename list<value_type>::size_type list<value_type>::max_size() {
        return (std::numeric_limits<size_type>::max() / sizeof(Node));
    }

    // List Modifiers

    template <typename value_type>
    void list<value_type>::swap(list& other) {
        using std::swap;
        swap(this->head, other.head);
        swap(this->tail, other.tail);
        swap(this->size_, other.size_);
    }

    template <typename value_type>
    void list<value_type>::pop_front() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        Node* first_node = head;
        if (size_ == 1) {
            head = nullptr;
            tail = nullptr;
        } else {
            head = first_node->next;
            head->previos = nullptr;
        }
        delete first_node;
        size_--;
        change_end();
    }

    template <typename value_type>
    void list<value_type>::push_front(const_reference value) {
        Node* front_new_node = new Node(value);
        if (empty()) {
            head = front_new_node;
            tail = front_new_node;
        } else {
            front_new_node->next = head;
            head->previos = front_new_node;
            head = front_new_node;
        }
        size_++;
        change_end();
    }

    template <typename value_type>
    void list<value_type>::clear() {
        while(empty() == false) {
            pop_back();
        }
    }

    template <typename value_type>
    void list<value_type>::push_back(const_reference value) {
        Node* new_node = new Node(value);
        if (empty()) {
            head = new_node;
            tail = new_node;
        } else {
            new_node->previos = tail;
            tail->next = new_node;
            tail = new_node;
        }
        size_++;
        change_end();
    }

    template <typename value_type>
    void list<value_type>::pop_back() {
        if (empty()) {
            throw std::out_of_range("list is already empty!");
        }
        Node* last_node = tail;
        if (size_ == 1) {
            head = nullptr;
            tail = nullptr;
        } else {
            tail = last_node->previos;
            tail->next = nullptr;
        }
        delete last_node;
        size_--;
        change_end();
    }

    template <typename value_type>
    typename list<value_type>::iterator list<value_type>::insert(iterator pos, const_reference value) {
        Node* current = pos.ptr_;
        Node* add = new Node(value);
        if (empty()) {
            add->next = end_;
            add->previos = end_;
            head = add;
            tail = add;
        } else {
            if (current == head) {
                head = add;
            } else if (current == end_) {
                tail = add;
            }
            add->next = current;
            add->previos = current->previos;
            current->previos->next = add;
            current->previos = add;
        }
        size_++;
        change_end();
        return iterator(add);
    }

    template <typename value_type>
    void list<value_type>::erase(iterator pos) {
        Node* current = pos.ptr_;
        if (!empty() && current != end_) {
            if (current == head) {
                if (current->next && current->next != end_) {
                    head = current->next;
                } else {
                head = end_;
                }
            } else if (current == tail) {
                if (current->previos && current->previos != end_) {
                    tail = current->previos;
                } else {
                    tail = end_;
                }
            }
            current->previos->next = current->next;
            current->next->previos = current->previos;
            delete current;
            this->size_--;
        } else {
            throw std::invalid_argument("Invalid argument");
        }
        change_end();
    }


    template <typename value_type>
    void list<value_type>::merge(list& other) {
        if (!this->empty() && !other.empty()) {
            iterator iterat_this = this->begin();
            iterator iterat_other = other.begin();
            while (iterat_this != this->end()) {
                if (iterat_other != other.end()) {
                    if (iterat_this.ptr_->data >= iterat_other.ptr_->data) {
                        this->insert(iterat_this, iterat_other.ptr_->data);
                        iterat_other++;
                    } else {
                        iterat_this++;
                    }
                } else {
                    break;
                }
            }
            while (iterat_other != other.end()) {
                this->insert(iterat_this, iterat_other.ptr_->data);
                iterat_other++;
            }
        } else if (this->empty() && !other.empty()) {
            this->copy(other);
        }
        other.clear();
    }

    template <typename value_type>
    void list<value_type>::splice(const_iterator pos, list& other) {
        if (!other.empty()) {
            for (iterator i = other.begin(); i != other.end(); i++) {
                this->insert(pos, *i);
            }
            other.clear();
        }
    }

    template <typename value_type>
    void list<value_type>::reverse() {
        if (!this->empty()) {
            size_type step = 0;
            for (iterator i = this->begin(); step <= this->size(); i++) {
                step++;
                std::swap(i.ptr_->previos, i.ptr_->next);
            }
            std::swap(head, tail);
        }
    }

    template <typename value_type>
    void list<value_type>::unique() {
        if (!this->empty()) {
            for (iterator i = this->begin(); i != this->end(); i++) {
                if (i.ptr_->data == i.ptr_->previos->data) {
                    iterator i_del = (i - 1);
                    this->erase(i_del);
                }
            }
        }
    }

    template <typename value_type>
    void list<value_type>::sort() {
        if (size_ > 1) {
            quick_sort_list(begin(), --end());
        }
    }


    // Support

        
    template <typename value_type>   
    void list<value_type>::print_list() {
        std::cout << "[";
        for (iterator i = begin(); i != end(); i++) {
            std::cout << *i;
            if ((i + 1) != end()) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n";
    }

    template <typename value_type>
    void list<value_type>::copy(const list& l) {
        Node* current = l.head;
        for(size_type i = 0; i != l.size_; i++) {
            push_back(current->data);
            current = current->next;
        }
    }

    template <typename value_type>
    void list<value_type>::change_end() {
        if (end_) {
            end_->next = head;
            end_->previos = tail;
            end_->data = size();
            if (head) {
                head->previos = end_;
            }
            if (tail) {
                tail->next = end_;
            }
        }
    }

    template <typename value_type>
    void list<value_type>::quick_sort_list(iterator first, iterator last) {
        if (first == last || first == end_ || last == end_ || first == tail) {
            return;
        }
        iterator pivot = slice(first, last);
        quick_sort_list(first, --pivot);
        quick_sort_list(++pivot, last);
    }

    template <typename value_type>
    typename list<value_type>::iterator list<value_type>::slice(iterator first, iterator last) {
        value_type pivot_value = last.ptr_->data;
        iterator i = first;
        for (iterator j = first; j != last; j++) {
            if (j.ptr_->data <= pivot_value) {
                std::swap(i.ptr_->data, j.ptr_->data);
                i++;
            }
        }
        std::swap(i.ptr_->data, last.ptr_->data);
        return i;
    }
        

};


