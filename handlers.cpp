//
//  handlers.cpp
//  test-task
//
//  Created by Валентина on 04/04/2019.
//  Copyright © 2019 Валентина. All rights reserved.
//


#include <iomanip>
#include "handlers.hpp"
using namespace std;

bool operator <= (const Event&& first, const Event&& second)
{
    return first.getDate() < second.getDate();
}


bool operator > (const Event&& first, const Event&& second)
{
    return first.getDate() > second.getDate();
}

//-----------------------
//-------Event class-----
//-----------------------

Event::Event(string path)
{
    try
    {
        stream.open(path);
        if (!stream.is_open())
        {
            throw "WrongPathError";
        }
    }
    catch (string error)
    {
        cout << error << ": файл не может быть открыт"<< endl;
    }

}

string Event::getDate(void) const
{
    return dateTime;
}

bool Event::checkStream()
{
    return stream.good();
}

//-------------------------
//----QuoteHandler class---
//-------------------------

QuoteHandler::QuoteHandler(string path) : Event(path)
{
    
    getline(stream, t_string);          //Пропуск заголовка
    getline(stream, dateTime, ',');     //Считывание даты
        
    getline(stream, t_string, ',');     //Считывание первой записи
    bid = stod(t_string);
    getline(stream, t_string, ',');
    bidSize = stod(t_string);
    getline(stream, t_string, ',');
    ask = stod(t_string);
    getline(stream, t_string, '\n');
    askSize = stod(t_string);

    this->calcMidWeighted();

}


void QuoteHandler::nextRecord(void)
{
    
    getline(stream, dateTime, ',');
    
    getline(stream, t_string, ',');
    bid = stod(t_string);
    getline(stream, t_string, ',');
    bidSize = stod(t_string);
    getline(stream, t_string, ',');
    ask = stod(t_string);
    getline(stream, t_string, '\n');
    askSize = stod(t_string);
    
    this->calcMidWeighted();
    
}

void QuoteHandler::calcMidWeighted(void)
{
    midWeighted = float((bid * askSize + ask * bidSize)) / (bidSize + askSize);
}

float QuoteHandler::getMidWeighted(void)
{
    return midWeighted;
}

void QuoteHandler::printRec()
{
    cout       << setw(25) << "DateTime"
    << setw(7) << "Bid"
    << setw(9) << "BidSize"
    << setw(7) << "Ask"
    << setw(9) << "AskSize" << endl;
    
    cout        << setw(25) << dateTime
    << setw(7)  << bid
    << setw(9)  << bidSize
    << setw(7)  << ask
    << setw(9)  << askSize  << endl;
}

QuoteHandler::~QuoteHandler()
{
    stream.close();
}

//-------------------------
//----TradeHandler class---
//-------------------------


TradeHandler::TradeHandler(string path) : Event(path)
{
    
    interval_pq = new float[1000];
    interval_q  = new float[1000];
    counter     = 0;

    getline(stream, t_string);          //Пропуск заголовка
    getline(stream, dateTime, ',');     //Считывание даты
    getline(stream, t_string, ',');     //Считывание первой записи
    price = stod(t_string);
    getline(stream, t_string, '\n');
    quantity = stod(t_string);
    interval_pq[counter % 1000] = price * quantity;
    interval_q [counter % 1000] = quantity;
    
    this->calcVWAP();
    
}
void TradeHandler::nextRecord(void)
{
    
    counter++;
    getline(stream, dateTime, ',');
    getline(stream, t_string, ',');
    price = stod(t_string);
    getline(stream, t_string, '\n');
    quantity = stod(t_string);
    this->calcVWAP();

}
void TradeHandler::calcVWAP(void)
{
    
    if (counter < 1000)
    {
        
        sum_pq += price * quantity;
        sum_q  += quantity;
        
    } else
    {
        sum_pq += (price * quantity) - interval_pq[(counter - 1) % 1000];
        sum_q  += quantity - interval_q[(counter - 1) % 1000];
    }
    
    vwap = sum_pq / sum_q;
    
}

float TradeHandler::get_vwap(void)
{
    return vwap;
}

void TradeHandler::printRec()
{
    cout        << setw(25)     << "DateTime"
    << setw(7)  << "Price"
    << setw(9)  << "Quantity"   << endl;
    
    cout        << setw(25)     << dateTime
    << setw(7)  << price
    << setw(9)  << quantity     << endl;
}

TradeHandler::~TradeHandler()
{

    delete interval_pq;
    delete interval_q;
    stream.close();
    
}
