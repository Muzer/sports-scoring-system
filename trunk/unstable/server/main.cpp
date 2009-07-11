#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#if defined _WIN32 || defined _WIN64 || defined WIN32 || defined WIN64 || defined WINDOWS
#include <winsock.h>
#define socketclose(x) closesocket(x)
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#define socketclose(x) close(x)
#endif
#include <cstring>

using namespace std;

bool ok[1000];
bool ping_ready[1000];
int no_pings[1000];
int current_socket = 1;
int new_fd[1000];

void *ping(){
    while(1){
        int loopvar;
        for(loopvar=1;loopvar<current_socket;loopvar++){
            if(ok[loopvar] == true){
                if(ping_ready[loopvar] == true){
                    no_pings[loopvar] = 0;
                    ping_ready[loopvar] = false;
                }
                else
                    no_pings[loopvar]++;
                if(no_pings[loopvar]>=120){
                    ok[loopvar] = false;
                    socketclose(new_fd[loopvar]);
                    cout << "Client " << loopvar << " timed out...\n";
                }
            }

        }
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    if(argc != 1){
        cout << "Usage: " << argv[0] << endl;
        exit(1);
    }
    // Win32 socket setup code
#if defined _WIN32 || defined _WIN64 || defined WIN32 || defined WIN64 || defined WINDOWS
    WSADATA wsaData;   // if this doesn't work
    //WSAData wsaData; // then try this instead

    // MAKEWORD(1,1) for Winsock 1.1, MAKEWORD(2,0) for Winsock 2.0:

    if (WSAStartup(MAKEWORD(1,1), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }
#endif
    // Set up struct
    int status;
    int s;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints;
    struct addrinfo *servinfo;  // will point to the results

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if ((status = getaddrinfo(NULL, "8643", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    // servinfo now points to a linked list of 1 or more struct addrinfos
    // Open socket
    if((s = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) < 0){
        fprintf(stderr, "open socket failed\n");
        exit(1);
    }

    int yes=1;
    //char yes='1'; // Solaris people use this

    // lose the pesky "Address already in use" error message
    if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        fprintf(stderr, "allowing kernel to reuse the port failed\n");
        exit(1);
    }

    // Bind to the correct port

    if(bind(s, servinfo->ai_addr, servinfo->ai_addrlen) < 0){
        fprintf(stderr, "bind failed\n");
        exit(1);
    }

    // Start listening

    listen(s, 20);




//    freeaddrinfo(servinfo); // free the linked-list
    pthread_t pingthrd;
    int retval;
    retval = pthread_create((pthread_t*)&pingthrd,NULL,(void* (*)(void*))ping,NULL);
    char recievehole[65535];
    struct timeval tv;
    fd_set readfds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    while(1){
        memset(recievehole,0,65535);
        FD_ZERO(&readfds);
        FD_SET(s, &readfds);
        select(s+1, &readfds, NULL, NULL, &tv);
        if (FD_ISSET(s, &readfds)){
            if(current_socket <= 1000){
                addr_size = sizeof their_addr;
                new_fd[current_socket] = accept(s, (struct sockaddr *)&their_addr, &addr_size);
                ok[current_socket] = true;
                ping_ready[current_socket] = false;
                cout << "Client " << current_socket << " connected sucessfully!\n";
                current_socket++;
            }
        }
//        cout << "Main running\n";
        int fvar;
        for(fvar=1;fvar<current_socket;fvar++){
            if(ok[fvar] == true){
                FD_ZERO(&readfds);
                FD_SET(new_fd[fvar], &readfds);
                select(new_fd[fvar]+1,&readfds,NULL,NULL,&tv);
                if(FD_ISSET(new_fd[fvar],&readfds) && ok[fvar] == true){
                    if(recv(new_fd[fvar],recievehole,65535,0) == 0){
                        cout << "Connection closed on client " << fvar << endl;
                        socketclose(new_fd[fvar]);
                        ok[fvar] = false;
                    } else {
                        if(recievehole != NULL){
                            char parsed[65535];
                            memset(parsed,0,65535);
                            unsigned int number = 0;
//                            cout << recievehole;
                            unsigned int number2 = 0;
                            for(number=0;number != strlen(recievehole) + 1;++number){
                                if(recievehole[number] != 0 && recievehole[number] != '$')
                                    parsed[number - number2] = recievehole[number];
                                else{
                                    if(parsed[number - number2 - 1] == '\n')
                                        parsed[number - number2 - 1] = 0;
                                    if(strncmp(parsed, "KEEPALIVE",9) == 0)
                                        ping_ready[fvar] = true;
                                    else
                                        cout << parsed << endl;
                                    memset(parsed,0,65535);
                                    number2 = number + 1;
                                }

                            }

                        }
                    }
                }
/*                if(ok[fvar] == true){
                    FD_ZERO(&readfds);
                    FD_SET(new_fd[fvar], &readfds);
                    select(new_fd[fvar]+1,NULL,&readfds,NULL,&tv);
                    if(FD_ISSET(new_fd[fvar],&readfds) && ok[fvar] == true){
                        if(send(new_fd[fvar],"Main running\n",strlen("Main running\n"),0) < 0){
                            cout << "Send failed to client " << fvar << "\n";
                        }
                    }
                }*/
            }
        }
//        sleep(2);
    }
    return 0;
}
