/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   threadpool.h
 * Author: niyudi
 *
 * Created on 29 de agosto de 2021, 16:18
 */

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <QObject>
#include <QThreadPool>

class Threadpool : public QObject {
    Q_OBJECT
public:
    Threadpool(QObject* parent = nullptr);
    ~Threadpool();
private:
    QThreadPool* threadpool;
};

#endif /* THREADPOOL_H */

