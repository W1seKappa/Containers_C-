#include <iostream>
#include <limits>
#include <initializer_list>

namespace s21 {
    template <typename T>
    class list {
    public:
        // List Member type
        using value_type = T;
        using reference = T&; // ссылка на тип Т
        using const_reference = const T&;
        using size_type = std::size_t;
    private:
        struct Node {
            value_type data;
            Node *previos;
            Node *next;
            Node(const_reference val) : data(val), previos(nullptr), next(nullptr) {}
        };
        Node *head;
        Node *tail;
//        Node *end;
        size_type size_;

    public:
        // List Functions
        list() : head(nullptr), tail(nullptr), size_(0) {} // default constructor
        list(size_type n) : head(nullptr), tail(nullptr), size_(0) { // constructor with parameter
            if (n >= max_size()) {
                throw std::out_of_range("Limit of the list is exceeded");
            }
            for(size_type i = 0; i < n; i++) {
                push_back(value_type());
            }
        }
        list(std::initializer_list<value_type> const &items) : head(nullptr), tail(nullptr), size_(0) { // constructor wiht ining value
            for (const auto& item : items) {
                push_back(item);
            }
        }
        list(const list &l) : head(nullptr), tail(nullptr), size_(0) { // copy constructor
            this->copy(l);
        }
        list(list &&l) : head(nullptr), tail(nullptr), size_(0) { // move constructor
            swap(l);
        }
        ~list() { // destructor
            clear();
//            std::cout << "Вызов деструктора";
        }
        list& operator=(list &&l) {
            if (this != &l) {
                clear();
                swap(l);
            }
            return *this;
        }
        // List Element access
        const_reference front() {
            if (size_ != 0) {
                return head->data;
            } else {
                throw std::out_of_range("List is empty");
            }
        }
        const_reference back() {
            if (size_ != 0) {
                return tail->data;
            } else {
                throw std::out_of_range("List is empty");
            }
        }
        // Support
        void swap(list& other) {
            using std::swap;
            swap(this->head, other.head);
            swap(this->tail, other.tail);
            swap(this->size_, other.size_);
        }
        void copy(const list& l) {
            Node* current = l.head;
            for(size_type i = 0; i != l.size_; i++) {
                push_back(current->data);
                current = current->next;
            }
        }
        // List Modifiers
        void clear() {
            while(empty() == false) {
                pop_back();
            }
        }
        void push_back(const_reference value) {
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
        }
        void pop_back() {
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
        }
        // List Capacity
        bool empty() {
            return size_ == 0;
        }
        size_type size() {
            return size_;
        }
        size_type max_size() {
            return (std::numeric_limits<size_type>::max() / sizeof(Node));
        }

        template<typename value_type>
        class ListIterator {
        public:

        private:
            Node* prt = nullptr;
            friend class list<T>;
        };
    };
}

int main() {
    s21::list<int> spisok; // простой конструктор
    s21::list<int> spisok2(4); // создание определенного размера
    s21::list<int> spisok3 = {1, 2, 3, 4, 5}; // создание с инициализацией
    s21::list<int> spissok4 = spisok3; // копирование
    std::cout << spisok.front() << " " << spisok.back();
    return 0;
}