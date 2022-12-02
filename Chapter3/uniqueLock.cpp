//
// Created by 王明龙 on 2022/12/2.
//

#include <chrono>
#include <cstdio>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>
#include <iostream>

using namespace std;


template<typename Iterator>
class swap_class {      //交换两个迭代器的元素(使用unique_lock实现线程安全)
public:
    explicit swap_class(Iterator iter) : iter(iter) {}   //构造函数

    template<typename iterator>
    friend void swap_use_unique_defer(swap_class<iterator> *const x, swap_class<iterator> *const y);

    Iterator getIter() const {
        return iter;
    }

private:
    mutex m;   //互斥锁
    Iterator iter;   //迭代器
};

template<typename iterator>
void swap_use_unique_defer(swap_class<iterator> *const x, swap_class<iterator> *const y) {
    using std::swap;

    //接收两个迭代器,并交换其迭代器指向元素内容
    if (x == y) {    //两个迭代器相等，即重复交换，直接退出
        return;
    }
    unique_lock<mutex> lc(x->m, defer_lock);    //定义unique_lock,使用锁m，且使用defer_lock推迟加锁
    unique_lock<mutex> rc(y->m, defer_lock);    //定义unique_lock,使用锁m，且使用defer_lock推迟加锁
    lock(lc, rc);       //真正开始加锁
    swap(*(x->iter), *(y->iter));   //交换迭代器指向元素内容
}


int main() {
    vector<int> v{1, 2, 3, 4, 5};
    auto x = swap_class<decltype(v.begin())>(v.begin());
    auto y = swap_class<decltype(v.begin())>(v.begin() + 1);

    std::thread t(swap_use_unique_defer<decltype(v.begin())>, &x, &y);
    std::thread t1(swap_use_unique_defer<decltype(v.begin())>, &x, &y);
    t1.join(), t.join();
    cout << *v.begin();


//    swap(v.begin(), v.begin() + 1);


}
