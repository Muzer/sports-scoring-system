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
#include <vector>


using namespace std;

struct competitor_info{
    int rank;
    int points;
};

struct event_info{
    char *name;
    int competitors;
    char *type;
    vector<competitor_info> these_competitors;
};

struct group_info{
    char *name;
    int no_events;
    vector<event_info> these_events;
};

struct table_info{
    char *filename;
    char *name;
    int no_groups;
    vector<group_info> these_groups;
};

struct user_info{
    char *username;
    char *password;
    bool table_created;
    char *filename;
    bool is_admin;
    table_info *this_table;
};

struct client_info{
    bool ok;
    bool auth;
    bool ping_ready;
    int no_pings;
    int new_fd;
    char *username;
    user_info *this_user;
};

int no_users = 1; // FIXME
user_info all_users[1]; // TODO: FIXME

table_info all_tables[1]; // TODO: FIXME

client_info all_clients[1000];

char *temptablename = (char*)"";
char *temptablefile = (char*)"";

/*bool ok[1000];
bool auth[1000];
bool ping_ready[1000];
int no_pings[1000];*/
int current_socket = 1;
// int new_fd[1000];

char *correct_username = (char*)"test1"; // FIXME: remove
char *correct_password = (char*)"cheese cake"; // FIXME: remove

// bool table_created = false;

int sendall(int s, char *buf, int *len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n;

    while(total < *len) {
        n = send(s, buf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }

    *len = total; // return number actually sent here

    return n==-1?-1:0; // return -1 on failure, 0 on success
}

void set_up_table(int clientnum, char *buf, bool done_replace){ // TODO: Change to struct
    char *tablename;
    char *filename;
    if(done_replace == false){
        int take_number = 9;
        if(buf[strlen(buf)-1] != 13){
            take_number--;
        } else {
            cout << "WARNING: spec violation - doesn't end in $";
        }
        char *nameandfile = (char *)malloc(strlen(buf)-(take_number-1));
        strncpy(nameandfile-1,buf+4,strlen(buf)-take_number);
        tablename = strtok(nameandfile,";");
        filename = strtok(NULL,";");
        if(tablename == NULL || filename == NULL){
            cout << "Client " << clientnum << " failed to create a table, as they didn't specify one (or both) of the fields!\n";
            return;
        }
        if(filename[0] == ' '){
            char *realfilename = (char *)malloc(strlen(filename));
            strncpy(realfilename,filename+1,strlen(filename)-1);
            filename = realfilename;
            cout << "WARNING: Spec violation - space after semicolon" << endl;
        }
        if(all_clients[clientnum].this_user->table_created == true){
            int len = strlen(((string)"TABLEEXISTSREPLACE " + all_clients[clientnum].this_user->this_table->name + "$").c_str());
            temptablename = tablename;
            temptablefile = filename;
            sendall(all_clients[clientnum].new_fd,(char*)((string)"TABLEEXISTSREPLACE " + all_clients[clientnum].this_user->this_table->name + "$").c_str(),&len);
            return;
        }
    } else {
        tablename = temptablename;
        filename = temptablefile;
    }
    temptablename = (char*)"";
    temptablefile = (char*)"";
    all_clients[clientnum].this_user->this_table->name = tablename;
    all_clients[clientnum].this_user->filename = filename;
    all_clients[clientnum].this_user->this_table->filename = filename;
    int len = strlen("GOON$");
    sendall(all_clients[clientnum].new_fd,(char*)"GOON$",&len);
    char receivehole[65535];
    struct timeval tv;
    fd_set readfds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    memset(receivehole,0,65535);
    FD_ZERO(&readfds);
    FD_SET(all_clients[clientnum].new_fd, &readfds);
    select(all_clients[clientnum].new_fd+1, &readfds, NULL, NULL, &tv);
    if (FD_ISSET(all_clients[clientnum].new_fd, &readfds)){
        if(receivehole != NULL){
            char parsed[65535];
            memset(parsed,0,65535);
            unsigned int number = 0;
            //                            cout << receivehole;
            unsigned int number2 = 0;
            for(number=0;number != strlen(receivehole) + 1;++number){
                if(receivehole[number] != 0 && receivehole[number] != '$')
                    parsed[number - number2] = receivehole[number];
                else{
                    if(parsed[number - number2 - 1] == '\n')
                        parsed[number - number2 - 1] = 0;
                    // Start code for checking for each function
                    if(strncmp(parsed,"SETGROUPS",9) != 0 && strncmp(parsed,"\n",1) != 0){
                        cout << "ERROR: Client send bad data (expected SETGROUPS or \\n, got " << parsed << ")" << endl;
                        return;
                    }

                }
            }
        }
    }
}


bool read_config(){
    return false;
}

bool create_config(){
    return true;
}

bool read_database(int clientnum){
    return true;
}

void authenticate(int clientnum, char *buf){
    int take_number = 5;
    if(buf[strlen(buf)-1] != 13){
//        cout << "Ends in $ or no newline\n";
        take_number--;
    } else {
        cout << "WARNING: spec violation - did not end in a $.\n";
    }
    char *userandpassword = (char *)malloc(strlen(buf)-(take_number-1));
    strncpy(userandpassword-1,buf+4,strlen(buf)-take_number);
    char *username = strtok(userandpassword,";");
    char *password = strtok(NULL,";");
    int len;
    if(password == NULL || username == NULL){
        cout << "Client " << clientnum << " Authentication failed, as they didn't specify a username or a password (or both)!\n";
        all_clients[clientnum].auth = false;
        len = strlen("AUTHBAD$");
        if(sendall(all_clients[clientnum].new_fd,(char*)"AUTHBAD$",&len) == -1){
            cout << "Send failed";
            socketclose(all_clients[clientnum].new_fd);
            all_clients[clientnum].ok = false;
        }
        return;
    }
    if(password[0] == ' '){
        char *realpassword = (char *)malloc(strlen(password));
        strncpy(realpassword,password+1,strlen(password)-1);
        password = realpassword;
        cout << "WARNING: Spec violation - space after semicolon" << endl;
    }
    if(strcmp(username,correct_username) == 0 && strcmp(password,correct_password) == 0){ // FIXME
        cout << "Client " << clientnum << "'s Authentication as user " << username << " succesful!\n";
        all_clients[clientnum].auth = true;
        len = strlen("AUTHGOOD$");
        all_clients[clientnum].username = username;
        all_clients[current_socket].this_user = &all_users[0];
        read_database(clientnum);
        if(sendall(all_clients[clientnum].new_fd,(char*)"AUTHGOOD$",&len) == -1){
            cout << "Send failed";
            socketclose(all_clients[clientnum].new_fd);
            all_clients[clientnum].ok = false;
        }
    } else {
        cout << "Client " << clientnum << "'s Authentication as user " << username << " failed!\n";
        all_clients[clientnum].auth = false;
        len = strlen("AUTHBAD$");
        if(sendall(all_clients[clientnum].new_fd,(char*)"AUTHBAD$",&len) == -1){
            cout << "Send failed";
            socketclose(all_clients[clientnum].new_fd);
            all_clients[clientnum].ok = false;
        }
    }
}

void *ping(){
    while(1){
        int loopvar;
        for(loopvar=1;loopvar<current_socket;loopvar++){
            if(all_clients[loopvar].ok == true){
                if(all_clients[loopvar].ping_ready == true){
                    all_clients[loopvar].no_pings = 0;
                    all_clients[loopvar].ping_ready = false;
                    cout << "Received keepalive from " << loopvar << endl;
                }
                else
                    all_clients[loopvar].no_pings++;
                if(all_clients[loopvar].no_pings>=120){
                    all_clients[loopvar].ok = false;
                    socketclose(all_clients[loopvar].new_fd);
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
    if(read_config() == false)
        create_config(); // Read and parse the config and records file
    // FIXME
    all_users[0].filename = (char*)"test.xml";
    all_users[0].is_admin = true;
    all_users[0].password = (char*)"cheese cake";
    all_users[0].table_created = false;
    all_users[0].username = (char*)"test1";
    all_users[0].this_table = &all_tables[0];
    // END FIXME
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
        fprintf(stderr, "bind failed. The server is probably already running; kill it\n");
        exit(1);
    }

    // Start listening

    listen(s, 20);




//    freeaddrinfo(servinfo); // free the linked-list
    pthread_t pingthrd;
    int retval;
    retval = pthread_create((pthread_t*)&pingthrd,NULL,(void* (*)(void*))ping,NULL);
    char receivehole[65535];
    struct timeval tv;
    fd_set readfds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    while(1){
        memset(receivehole,0,65535);
        FD_ZERO(&readfds);
        FD_SET(s, &readfds);
        select(s+1, &readfds, NULL, NULL, &tv);
        if (FD_ISSET(s, &readfds)){
            if(current_socket <= 1000){
                addr_size = sizeof their_addr;
                all_clients[current_socket].new_fd = accept(s, (struct sockaddr *)&their_addr, &addr_size);
                all_clients[current_socket].ok = true;
                all_clients[current_socket].ping_ready = false;
                all_clients[current_socket].auth = false;
                cout << "Client " << current_socket << " connected sucessfully!\nAwaiting authentication...\n";
                current_socket++;
            }
        }
//        cout << "Main running\n";
        int fvar;
        for(fvar=1;fvar<current_socket;fvar++){
            if(all_clients[fvar].ok == true){
                FD_ZERO(&readfds);
                FD_SET(all_clients[fvar].new_fd, &readfds);
                select(all_clients[fvar].new_fd+1,&readfds,NULL,NULL,&tv);
                if(FD_ISSET(all_clients[fvar].new_fd,&readfds) && all_clients[fvar].ok == true){
                    if(recv(all_clients[fvar].new_fd,receivehole,65535,0) == 0){
                        cout << "Connection closed on client " << fvar << endl;
                        socketclose(all_clients[fvar].new_fd);
                        all_clients[fvar].ok = false;
                    } else {
                        if(receivehole != NULL){
                            char parsed[65535];
                            memset(parsed,0,65535);
                            unsigned int number = 0;
//                            cout << receivehole;
                            unsigned int number2 = 0;
                            for(number=0;number != strlen(receivehole) + 1;++number){
                                if(receivehole[number] != 0 && receivehole[number] != '$')
                                    parsed[number - number2] = receivehole[number];
                                else{
                                    if(parsed[number - number2 - 1] == '\n')
                                        parsed[number - number2 - 1] = 0;
                                    // Start code for checking for each function
                                    if(all_clients[fvar].auth == true){
                                        if(strncmp(parsed, "KEEPALIVE",9) == 0)
                                            all_clients[fvar].ping_ready = true;
                                        else if(strncmp(parsed,"NEWTABLE",8) == 0){
                                            cout << "recieved NEWTABLE request from client " << fvar << endl;
                                            if(strcmp(temptablefile, "") != 0 || strcmp(temptablename, "") != 0){
                                                cout << "...but someone else has started to create a table" << endl;
                                                int len = strlen("BUSY$");
                                                sendall(all_clients[fvar].new_fd,(char*)"BUSY$",&len);
                                            } else
                                                set_up_table(fvar,parsed,false);
                                        }
                                        else if(strncmp(parsed,"REPLACE",7) == 0){
                                            cout << "recieved REPLACE request from client " << fvar << endl;
                                            set_up_table(fvar,parsed,false);
                                        }
                                        else
                                            cout << parsed << endl;
                                    }
                                    // Authentication is outside the main function if, since otherwise it'd be impossible
                                    if(strncmp(parsed, "AUTH",4) == 0){
                                        cout << "received AUTH request from client " << fvar << endl;
                                        authenticate(fvar,parsed);
                                    }
                                    memset(parsed,0,65535);
                                    number2 = number + 1;
                                }

                            }

                        }
                    }
                }
            }
        }
//        sleep(2);
    }
    return 0;
}
