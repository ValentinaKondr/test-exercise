//
//  main.cpp
//  test-task
//
//  Created by Валентина on 03/04/2019.
//  Copyright © 2019 Валентина. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>
#include "handlers.hpp"

using namespace std;


int main(int argc, const char * argv[]) {
    float y;
    string quotePath = "/Users/valentina/Downloads/HSI.Quote.csv";
    string tradePath = "/Users/valentina/Downloads/HSI.Trade.csv";
    string finalPath = "/Users/valentina/Downloads/HSI.Final2.csv";

    
    QuoteHandler    quote(quotePath);
    TradeHandler    trade(tradePath);
    ofstream        write(finalPath);
    
    write << "DateTime" << "," << "MidWeighted" << "," << "VWAP" << "," << "y" << endl;
    auto start_time = clock();

    
    do
    {
        if (trade > quote)
        {
            y = quote.getMidWeighted() - trade.get_vwap();
            
            write   << quote.getDate()  << "," << quote.getMidWeighted() << ","
            << trade.get_vwap() << "," << y << endl;
            quote.nextRecord();
            
        }
        else
        {
            trade.nextRecord();
        }
        
    } while (quote.checkStream() && trade.checkStream());
    auto end_time = clock();

    cout << "Время выполнения программы: " << (end_time - start_time) / CLOCKS_PER_SEC << " секунд(ы)"<< endl;
    return 0;
}
