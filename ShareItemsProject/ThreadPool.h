 
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

using boost::asio::ip::tcp;
using namespace std;

// ThreadPool class to manage worker threads
class ThreadPool777 {

public:

    ThreadPool777(size_t num_threads) :num_threads_(num_threads) , io_context_(num_threads), work_guard_(boost::asio::make_work_guard(io_context_)) {
        run();
    }

    // Post a task to the thread pool
    template <typename F>
    void post(F&& f) {
        boost::asio::post(io_context_, std::forward<F>(f));
    }

    void run() {
        for (size_t i = 0; i < num_threads_; ++i) {
            workers_.emplace_back([this]() {
                worker(); 
                //boost::asio::io_context::work work(io_context_);
                io_context_.run();
                });
        }
    }

    boost::asio::io_context& get_io_context() {
        return io_context_;
    }

    void join() {
          for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }  
    }

    void stop() {
        stop_ = true;
        io_context_.stop();
        join();
    }

    ~ThreadPool777() {
        join(); //stop();
    };


//public:
 
    void submit(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            task_queue_.push_back(std::move(task));
        }
    }

private:
    void worker() {
        while (!stop_) {
            std::function<void()> task;
            {
                std::lock_guard<std::mutex> lock(queue_mutex_);
                if (!task_queue_.empty()) {
                    task = std::move(task_queue_.front());
                    task_queue_.pop_front();
                }
            }
            if (task) {
                task();
            }
        }
    }

    //std::vector<std::thread> workers_;
    std::deque<std::function<void()>> task_queue_;
    std::mutex queue_mutex_;
    bool stop_;


//private:
    size_t num_threads_;
    boost::asio::io_context io_context_;
    std::vector<std::thread> workers_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
};