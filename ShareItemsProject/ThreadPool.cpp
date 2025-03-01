#include "ThreadPool.h"
 

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

ThreadPool777::ThreadPool777(size_t num_threads) :num_threads_(num_threads), io_context_(num_threads), work_guard_(boost::asio::make_work_guard(io_context_)) {
    run();
};

void ThreadPool777::run() {
    for (size_t i = 0; i < num_threads_; ++i) {
        threads_.emplace_back([this]() {
            worker();
            //boost::asio::io_context::work work(io_context_);
            io_context_.run();
            });
    }
};

void ThreadPool777::join() {
    for (auto& worker : threads_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
};

void ThreadPool777::stop() {
    stop_ = true;
    io_context_.stop();
    join();
};

ThreadPool777::~ThreadPool777() {
    join(); //stop();
};


void ThreadPool777::submit(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        task_queue_.push_back(task);
    }
};


void ThreadPool777::worker() {
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
};
 