#include <iostream>
#include <list>

int main() {
    std::list<int> mylist(5);
    for (auto it = mylist.begin(); it != mylist.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::max_size()
    return 0;
}