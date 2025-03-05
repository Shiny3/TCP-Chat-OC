 
#pragma once

/* ThreadPool Class(Manages worker threads)*/
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <functional>
#include <mutex>
#include <deque>
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <functional>
#include <deque>

using boost::asio::ip::tcp;
using namespace std;

// ThreadPool class to manage worker threads
class ThreadPool777 {

public:

    ThreadPool777(size_t num_threads);

    void run();

    void join();

    void stop();

    ~ThreadPool777();


//public:
 
    void submit(std::function<void()> task);

private:
    void worker();

    //std::vector<std::thread> workers_;
    std::deque<std::function<void()>> task_queue_;
    std::mutex queue_mutex_;
    bool stop_ = false;

    boost::asio::io_context io_context_;

public:
    std::vector<std::thread> threads_;  

    size_t num_threads_;

    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;

    boost::asio::io_context& get_io_context() {
        return io_context_;
    };

    // Post a task to the thread pool
    template <typename F>
    void  post(F&& f) {
        boost::asio::post(io_context_, std::forward<F>(f));
    };

};