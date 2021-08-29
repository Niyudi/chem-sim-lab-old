/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "threadpool.h"

#include <QThreadPool>

Threadpool::Threadpool(QObject* parent) : QObject(parent) {
    this->threadpool = new QThreadPool(this);
}

Threadpool::~Threadpool(){
    delete this->threadpool;
}
