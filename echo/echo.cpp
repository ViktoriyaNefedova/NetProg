#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

int main (int argc, char **argv)
{
    sockaddr_in * self_addr = new (sockaddr_in);
    self_addr->sin_family = AF_INET;
    self_addr->sin_port = 0;
    self_addr->sin_addr.s_addr = 0;

    sockaddr_in * srv_addr = new (sockaddr_in);
    srv_addr->sin_family = AF_INET;
    srv_addr->sin_port = htons(7);
    srv_addr->sin_addr.s_addr = inet_addr("172.16.40.1");

    char *buf = new char[1024];
    string str ("hello\n");
    int msg = str.length();
    size_t length = str.copy(buf,msg);
    
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        cerr << "Socket error";
    }
    int rc = bind(s, (const sockaddr *) self_addr, sizeof ( sockaddr_in));
    if (rc == -1) {
        close(s);
        cerr <<"Error bind socket with local address";
    }
    rc = connect(s, ( const sockaddr*) srv_addr, sizeof(sockaddr_in));
    if ( rc == -1) {
        close(s);
        cerr <<"Error connect socket with remote server";
    }
    rc = send(s, buf,msg,0);
    if ( rc == -1) {
        close(s);
        cerr <<"Error send message";
    }
    cout << "We send:  " << buf << endl;
    rc = recv(s, buf, 1024,0);
    if ( rc == -1) {
        close(s);
        cerr <<"Error receive answer";
    }
    buf[rc]='\0';
    cout << "We receive:  " << buf << endl;
    close(s);

    delete self_addr;
    delete srv_addr;
    delete[] buf;
    return 0;
}
