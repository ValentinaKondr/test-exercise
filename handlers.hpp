//
//  handlers.hpp
//  test-task
//
//  Created by Валентина on 04/04/2019.
//  Copyright © 2019 Валентина. All rights reserved.
//

#ifndef handlers_hpp
#define handlers_hpp

#include <iostream>
#include <fstream>
#include <stdio.h>
//#include <boost/filesystem.hpp>
#include <exception>

using namespace std;


//-----------------------

class Event
{
protected:
    string  path;
    fstream stream;
    string  dateTime;
    string  t_string;    //Временная переменная для сохранения значения перед приведением к float
public:
    Event(string path);
    string getDate(void) const;
    virtual void nextRecord() = 0;
    virtual void printRec()   = 0;
    bool checkStream();
};

//-----------------------

bool operator < (const Event& first, const Event& second);


bool operator > (const Event& first, const Event& second);


//-----------------------


class QuoteHandler : public Event
{
private:
    float   bid;
    float   bidSize;
    float   ask;
    float   askSize;
    float   midWeighted;
    void    calcMidWeighted(void);
public:
    QuoteHandler(string path);
    void    nextRecord(void);
    float   getMidWeighted(void);
    void    printRec();
    ~QuoteHandler();
};

//-----------------------

class TradeHandler : public Event
{
private:
    int     counter;
    float   price;
    float   quantity;
    float   sum_pq;
    float   sum_q;
    float   vwap;
    float*  interval_pq;
    float*  interval_q;
    void    calcVWAP(void);
public:
    TradeHandler(string path);
    void    nextRecord(void);
    float   get_vwap(void);
    void    printRec();
    ~TradeHandler();
};



#endif /* handlers_hpp */

