// Client-OC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ChatServer.h"

int main(int argc, char* argv[]) {

    std::string IPAddress;
    std::string port;
    std::string name;

    try {
        if (argc != 4) throw;
        name = std::string(argv[1]);
        IPAddress = std::string(argv[2]);
        port = std::string(argv[3]);

    }
    catch (...) {
        std::cout << "Incorrect Format" << std::endl;
        std::cout << "Usage: <Server> <ServerName> <IP-address> <Server-Port>" << std::endl;
        return 0;
    }


   try {

       boost::asio::io_context io_context;

      std::size_t num_threads = std::thread::hardware_concurrency();

      // ThreadPool777 thread_pool(num_threads);

      // ChatServer server(thread_pool.get_io_context(), port, num_threads);

       ChatServer server(io_context, port, num_threads);
      // std::cout << "Server is running..." << std::endl;
      // thread_pool.get_io_context().run();
 
   /*      
       std::size_t num_threads = std::thread::hardware_concurrency();

       auto thread_pool = std::make_shared<ThreadPool777>(num_threads);

       ChatServer server(port, thread_pool);
 */
       std::cout << "Server is running..." << std::endl;
       //server.thread_pool_.get_io_context().run();
      

       // Keep the main thread running while the thread pool handles connections
       /*for (std::thread& thread : thread_pool.get_threads()) {
           if (thread.joinable()) {
               thread.join();
           }
       }*/
   }
   catch (const std::exception& e) {
       std::cerr << "Exception: " << e.what() << std::endl;
   }

    return 0;
}
