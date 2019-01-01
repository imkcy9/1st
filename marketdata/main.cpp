/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: kecy
 *
 * Created on 2019年1月1日, 下午7:11
 */

#include <cstdlib>
#include "application.h"
#include "proto_message_dispatcher.h"
#include "md_engine.h"
using namespace std;

/*
 * 
 */
void prog_exit(int signo);

application<md_engine> app;
int main(int argc, char** argv) {

    bool ret = app.init_main(argc,argv,prog_exit);
    if(ret) {
        app.start();
        app.join();
    }
    return 0;
}

void prog_exit(int /*signo*/) {
    LOG_WARN("exit");
    app.stop();
}

