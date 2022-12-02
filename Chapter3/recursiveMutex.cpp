//
// Created by 王明龙 on 2022/12/1.
//
#include <chrono>
#include <cstdio>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

using namespace std;

//template<typename T>
//class vector {
//public:
//    unsigned long get_size() {
//        lock_guard<mutex> lk(m);
//        return size_f();
//    }
//
//    bool is_empty() {
//        lock_guard<mutex> lk(m);
//        return size_f() == 0;
//    }
//
//private:
//    int size_f() {
//        return size;
//    }
//
//    mutex m;
//    unsigned long size;
//    T *m_data;
//};

#include <vector>

using namespace std::chrono_literals;

std::shared_mutex smtx;

int main(int, char**) {

    std::vector<std::thread> threads;

    auto read_task = [&]() {
        std::shared_lock l{ smtx };
        printf("read\n");
        std::this_thread::sleep_for(1s);
    };

    auto write_task = [&]() {
        std::unique_lock l{ smtx };
        printf("write\n");
        std::this_thread::sleep_for(1s);
    };

    // Create a few reader tasks.
    threads.emplace_back(read_task);
    threads.emplace_back(read_task);
    threads.emplace_back(read_task);


    // Try to lock a unique_lock before read tasks are done.
    std::this_thread::sleep_for(1ms);
    threads.emplace_back(write_task);

    // Then, enque a gazillion read tasks.
    // Will the unique_lock be locked? [drum roll]

    // Would be while(true), 120 should be enough for demo
    for (size_t i = 0; i < 120; ++i) {
        std::this_thread::sleep_for(1ms);
        threads.emplace_back(read_task);
    }

    for (auto& t : threads) {
        t.join();
    }
}

//int main() {
//    vector<int> v;
//    cout << boolalpha << v.is_empty();
//}

