#pragma once

#include <cstdlib>
#include "asio/include/asio.hpp"


#define BUFF_SIZE_2048  (1<<11)
#define BUFF_SIZE_512   (1<<9)
#define BUFF_SIZE_128   (1<<7)
#define BUFF_SIZE_32    (1<<5)
#define BUFF_SIZE_8     (1<<3)
#define BUFF_SIZE_2     (1<<1)
#define MAX_BUFF_SIZE   BUFF_SIZE_2048


/* 
 * singleton
 */
class Server {
    typedef asio::error_code    AsioErr;
    typedef asio::ip::tcp       AsioTcp;
    typedef std::shared_ptr<AsioTcp::socket>     AsioSocketPtr;
    typedef std::shared_ptr<AsioTcp::acceptor>   AsioAcceptorPtr;

public:
    static Server& GetInstance() {
        // the only instance of Server
        static Server instance;
        return instance;
    }

    void SetListenPort(unsigned short num) {
        _portNumber = num;
    }

    void SetBufferSize(unsigned short size) {
        if(size>MAX_BUFF_SIZE) {
            size = MAX_BUFF_SIZE;
        }
        _bufferSize = size;
    }

    void Run() {
        if(_isRunning) {
            std::cout<<"Server is already running"<<std::endl;
            return;
        }
        // create a endpoint
        AsioTcp::endpoint endpoint(AsioTcp::v4(), _portNumber);
        try {
            // bind endpoint to acceptor
            _acceptorp = std::make_shared<AsioTcp::acceptor>(_service, endpoint);
            // reuse the address
            _acceptorp->set_option(asio::socket_base::reuse_address(true));
            // async accept
            AsyncAccept();
            // run service
            _service.run();
        } catch(std::exception &e) {
            std::cerr<<"Something wrong:"<<e.what()<<std::endl;
            exit(EXIT_SUCCESS);
        }
        _isRunning = true;
    }

private:
    Server(Server const &) = delete; // don't implement
    void operator=(Server const &) = delete; // don't implement

    // constructor: init some data
    Server() {
        _portNumber = 0;
        _isRunning = false;
        _acceptorp = nullptr;
        _bufferSize = BUFF_SIZE_128;
    }
    
    // be ready to accept new connection
    void AsyncAccept() {
        AsioSocketPtr sockp(new AsioTcp::socket(_service));
        // in order to pass sockp to HandleAsyncAccept,
        // the second argument is a lambda.
        _acceptorp->async_accept(*sockp, 
                [this, sockp] (const AsioErr &err) {
                    HandleAsyncAccept(AsioSocketPtr(sockp), err);
                });
    }

    // be ready to read data from connection
    void AsyncRead(AsioSocketPtr &sockp) {
        // tell sockp that call HandleAsyncRead if data comes,
        // and store data in _buffer.
        sockp->async_read_some(asio::buffer(_buffer, _bufferSize), 
                [this, sockp] (const AsioErr &err, size_t bytes) {
                    HandleAsyncRead(sockp, err, bytes);
                });
    }

    // new connection accepted
    void HandleAsyncAccept(AsioSocketPtr &&sockp, const AsioErr &error) {
        if(error) {
            std::cout<<"Something wrong:"<<error.message()<<std::endl;
            sockp->close();
            return ;
        }
        const AsioTcp::endpoint &ep = sockp->remote_endpoint();
        std::cout<<"This is a new connection："<<ep.address()<<":"<<ep.port()<<std::endl;

        // read data
        AsyncRead(sockp);
        // post a new async accept
        AsyncAccept();
    }

    // read and print data received
    void HandleAsyncRead(AsioSocketPtr sockp, const AsioErr &error, size_t bytes) {
        if(error) {
            std::cout<<"Something wrong:"<<error.message()<<std::endl;
            sockp->close();
            return ;
        }
        // print
        std::cout<<bytes<<" bytes received"<<std::endl;
        std::cout<<std::string(_buffer, _buffer+bytes)<<std::endl;
        // read again
        AsyncRead(sockp);
    }

    asio::io_service _service;
    AsioAcceptorPtr _acceptorp;
    char _buffer[MAX_BUFF_SIZE+1];  // shared buffer for all clients
    bool _isRunning;

    unsigned short _portNumber;
    unsigned short _bufferSize;     // the number of bytes at a time
};


