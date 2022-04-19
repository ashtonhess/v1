// SERVER
// Created by Ashton Hess on 3/25/22.
// SERVER

#include <iostream>
#include<unistd.h>
#include <pthread.h>
#include "Singleton.h"
#include "Network.h"

//disabling inf loop inspection for this file in CLion. Perm fix. Comment out to make work with -Werror.
// add -Wno-unknown-pragmas to Makefile args to ignore in compilation.
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

void *gameFunc(void*arg){
    cout<<"Game func"<<endl;
    cout<<"first sock: "<<((pair<int,int>*)arg)->first<<endl;
    cout<<"second sock: "<<((pair<int,int>*)arg)->second<<endl;
    int sock1=((pair<int,int>*)arg)->first;
    int sock2=((pair<int,int>*)arg)->second;

    Network network=Network();

    //It is PLAYER1's turn first.
    network.sendMsg(sock1, "You have successfully joined a game.\nWelcome to Cup Pong!\n");
    network.sendMsg(sock1, "INIT PLAYER1");

    //PLAYER2 is waiting to receive game state after player 1's toss.
    network.sendMsg(sock2, "You have successfully joined a game.\nWelcome to Cup Pong!\n");
    network.sendMsg(sock2, "INIT PLAYER2");

    bool playing;
    while(playing){
        string rmsg=network.recvMsg(sock1);
        cout<<"> rmsg sock "<<sock1<<": "<<rmsg<<endl;
        playing=false;
    }


    return NULL;
}

void *connectListener(void*arg){
    bool boolV=true;
    vector<pthread_t*> threadV;
    int sock1;
    int sock2;
    while (true){
        Network network = Network();
        if(boolV){//if the server is not connected yet (first time calling this), connect to server...
            if(network.connect()){
//                cout<<"Network connected."<<endl;
                boolV=false;
            }else{
                cout<<"> Error connecting to network."<<endl;
            }
        }
        int connections=0;
        while(connections!=2){
            //accept first connection, store in sock1
            if(connections==0){
                sock1=network.acceptConnection();
                if(sock1==-1){
                    cout<<"> Error accepting sock1."<<endl;
                }else{
                    cout<<"> sock1 accepted: "<<sock1<<endl;
                    connections++;
                }
            }
            //accept second connection, store in sock2
            if(connections==1) {
                sock2 = network.acceptConnection();
                if(sock2==-1){
                    cout<<"> Error accepting sock2"<<endl;
                }else{
                    cout<<"> sock2 accepted: "<<sock2<<endl;
                    connections++;
                }
            }
        }
        cout<<"> Creating game with socks: "<<sock1<<" and "<<sock2<<"."<<endl;
        //store both accepted socks in pair to pass to gameThread.
        pair<int,int> *sockPair = (pair<int,int>*)malloc(sizeof(pair<int,int>*));
        sockPair->first = sock1;
        sockPair->second = sock2;
        pthread_t *threadPtr = (pthread_t*)malloc(sizeof(pthread_t));
        int res;
        //create game thread with sock1 and sock2.
        res=pthread_create(threadPtr, NULL, &gameFunc, (void*)sockPair);
        if (res!=0){
            cout<<"> Error creating thread."<<endl;
        }else{
            threadV.push_back(threadPtr);
        }
        connections=0;
    }
    pthread_exit(NULL);
}

int main(int argc, char*argv[]){
    FileIO *f = new FileIO();
    f->setFilename("users.txt");
    Singleton::getInstance()->setFile(*f);
    Singleton*singler = Singleton::getInstance();
    //cout<<"Filename: " <<singler->getFile().getFilename()<<endl;
    singler->getFile().readUsers();


    vector<pthread_t> threads;
    pthread_t t1;
    int res;
    //Network networkObj = Network();
    //if (networkObj.connect()) {
        res=pthread_create(&t1, NULL, &connectListener, NULL);
        if (res!=0){
            cout<<"> Error creating thread."<<endl;
        }
        pthread_exit(NULL);
}

#pragma clang diagnostic pop




//}else{
    //    cout<<"> Error connecting to network."<<endl;
    //}

//    Network networkObj = Network();
//    if (networkObj.connect()) {
//
//    }


    /*
    bool isActive=false;
    if(networkObj.connect()){
        isActive=true;
        int sock;
        int numOfConnections=0;
        bool acceptingPair=true;
        while(isActive){
            //store socket connection values for pair
            int s1;
            int s2;
            while (acceptingPair){
                sock=networkObj.acceptConnection();
                if (sock==-1){
                    cout<<"> Error: Network::acceptConnection().";
                    //return -1;//returning
                }else if (sock==-2){
                    isActive=false;//THIS ELSE IF IS TO MAKE INF LOOP HIGHLIGHT GO AWAY IN CLION. NOT ACTUALLY USED.
                                    //ended up disabling inf loop inspection for this file in CLion. Perm fix. This is not needed anymore.
                }else{
                    //add this socket to list for matchmaking...
                    numOfConnections++;
                    if(numOfConnections==2){//if we have a pair of sockets, stop accepting new connections.
                        s2=sock;//store value of second socket connected.
                        cout<<"s2: "<<s2<<endl;
                        cout<<"Pair of sockets accepted:"<<endl<<"\ts1: "<<s1<<endl<<"\ts2: "<<s2<<endl;
                        acceptingPair=false;
                    }else{
                        s1=sock;//store value of first socket connected.
                        cout<<"s1: "<<s1<<endl;
                    }
                    networkObj.sendMsg("Welcome to Cup Pong!\n");
                    //once match is found, add both users into a thread of a game instance...

                }
            }
            while(true){


                break;
            }

            //isActive=false;
        }

    }else{
        cout<<"Network is not connected"<<endl;
    }
     */





//    bool isConnected = false;
//    while(!isConnected){
//        isConnected=networkObj.connect();
//    }
//    while(isConnected){
//
//
//
//        isConnected=false;
//    }