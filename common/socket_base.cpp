/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   socket_base.cpp
 * Author: kecy
 * 
 * Created on 2019年1月1日, 上午9:25
 */

#include "socket_base.h"

socket_base::socket_base(zmq::context_t &context_, int type_)
: zmq::socket_t(context_, type_) {
}

socket_base::~socket_base() {
}

