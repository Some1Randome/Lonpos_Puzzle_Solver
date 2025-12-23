#ifndef BLOCK_PUZZLE_SOLVER_THREADPOOL_H
#define BLOCK_PUZZLE_SOLVER_THREADPOOL_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>

using namespace std;

class ThreadPool {
public:
    ThreadPool(int num_threads = thread::hardware_concurrency()) {
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock lock(queue_mutex);

                        cv.wait(lock, [this] {
                            return !tasks.empty() || stop_;
                        });

                        if (stop_ && tasks.empty()) {
                            return;
                        }

                        task = move(tasks.front());
                        tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            unique_lock<mutex> lock(queue_mutex);
            stop_ = true;
        }

        cv.notify_all();

        for (auto &thread: threads) {
            thread.join();
        }
    }

    void enqueue(function<void()> task) {
        {
            unique_lock lock(queue_mutex);
            tasks.emplace(move(task));
        }
        cv.notify_one();
    }

private:
    vector<thread> threads;

    queue<function<void()> > tasks;

    mutex queue_mutex;

    condition_variable cv;

    bool stop_ = false;
};

#endif
