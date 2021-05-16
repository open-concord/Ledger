#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <iostream> // For cout
#include <unistd.h> // For read


// error handling is for losers

class Server {
    private:
        int portaddr = 3000; // setting this up to be changeable programmatically        
        int queuelim = 20; // set to 20 connections rn

        // socket
        struct sockaddr_in serv_addr, cli_addr;
        sockaddr_in sockaddr;
        int sockfd;

        void start_socket() {
            sockaddr.sin_family = AF_INET;
            sockaddr.sin_addr.s_addr = INADDR_ANY;
            sockaddr.sin_port = htons(portaddr);
        }

    public:
        int create_socket() {
            this->start_socket();
            sockfd = socket (AF_INET, SOCK_STREAM, 0);
            if (sockfd == -1) {
                std::cout << "Error while creating socket: " << errno << std::endl;
                // exit handling here
                std::cout << EXIT_FAILURE << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        void start_listen() {
            // testing port
            if (bind(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
                std::cout << "Error while binding to port: " << errno << std::endl;
                // exit handling here
                exit(EXIT_FAILURE);
            }
            
            // queue blocking
            if (listen(sockfd, queuelim) < 0) {
                std::cout << "Error on listen: " << errno << std::endl;
                // exit handling here
                exit(EXIT_FAILURE);
            }
        };

        // Block Connection
        class Block {
            private:
                // setting 'global' vars
                int connection;
                char buffer[256];
            public:
                // constructor is just pulling a new connection from queue
                Block () {
                    // pickup the next connection in queue
                    auto addrlen = sizeof(sockaddr);

                    // accept new connection
                    connection = accept(sockfd, (struct sockaddr *) &sockaddr, (socklen_t*)&addrlen);

                    if (connection < 0) {
                        std::cout << "Error while grabbing connection: " << errno << std::endl;
                        // exit handling here
                        exit(EXIT_FAILURE);
                    }

                    // read connection
                    auto bytesread = read(connection, buffer, 255);
                    
                    // debugging
                    std::cout << "Received: " << buffer;
                }

                // sanitization + timestamping + chain update + receipt confirmation
                void handle() {



                    // confirm receipt
                    std::string resp = "We don't have status codes but we got ur message";
                    send(connection, resp.c_str(), resp.size(), 0);

                    // close the buffer connection
                    close(connection);
                }

        };

        void shutdown() {
            close(sockfd);
        };
};