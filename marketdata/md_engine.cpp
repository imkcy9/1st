/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   md_engine.cpp
 * Author: kecy
 * 
 * Created on 2019年1月1日, 下午7:12
 */

#include "md_engine.h"

md_engine::md_engine(zmq::context_t* ctx)
:md_handler_(ctx, this){
}

md_engine::~md_engine() {
}

bool md_engine::init() {
    bool ret = md_handler_.init();
    if(!ret) {
        return false;
    }
    return true;
}
