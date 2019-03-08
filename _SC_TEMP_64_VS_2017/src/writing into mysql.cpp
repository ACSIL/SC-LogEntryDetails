#include "sierrachart.h"
#include <fstream>
#include <string>
#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <fstream>

const char *server = "localhost";
const char *user = "root";
const char *password = "--------";
const char *database = "trading_discretionary";

MYSQL *conn;
MYSQL_RES *res;

//ok
void write_entry_details_into_db(SCStudyInterfaceRef sc)
{        
    s_SCPositionData PositionData;
    sc.GetTradePosition(PositionData);

    //getting the vars and converting them all to strings to be used in mysql queries
    std::string symbol{ PositionData.Symbol }; //NQ
    t_OrderQuantity32_64 qty_ { PositionData.PositionQuantity };
    std::string qty = std::to_string(qty_); //-1 (long/short)
    double price_ { PositionData.AveragePrice };   
    std::string price = std::to_string(price_); //6576:50
   
    auto last_entry_dt = PositionData.LastEntryDateTime;
    int year_, month_, day_, hour_, minute_, second_;
    last_entry_dt.GetDateTimeYMDHMS(year_, month_, day_, hour_, minute_, second_);

    std::string year = std::to_string(year_);
    std::string month = std::to_string(month_);
    std::string day = std::to_string(day_);
    std::string hour = std::to_string(hour_);
    std::string minute = std::to_string(minute_);
    std::string second = std::to_string(second_);
    
    std::string entry_date = "'" + year + "-" + month + "-" + day + "'";
    std::string entry_time = "'" + hour + ":" + minute + ":" + second +"'";
    std::string entry_datetime = "'" + year + "-" + month + "-" + day + " " + hour + ":" + minute + ":" + second +"'";
        

    int tick_value_ = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[0].GetChartNumber(), sc.Input[0].GetStudyID(), 0));
    std::string tick_value = std::to_string(tick_value_); //-432
    float atr_value_ = sc.GetStudyPersistentFloatFromChart(sc.Input[1].GetChartNumber(), sc.Input[1].GetStudyID(), 0);
    std::string atr_value = std::to_string(atr_value_); //6.77
    int wtr_value_ = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[2].GetChartNumber(), sc.Input[2].GetStudyID(), 0));
    std::string wtr_value = std::to_string(wtr_value_); //3445

    //int dom_quotes_ab_ = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[3].GetChartNumber(), sc.Input[3].GetStudyID(), 0));
    //std::string dom_quotes_ab = std::to_string(dom_quotes_ab_); //320
    //int dom_quotes_ba_ = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[4].GetChartNumber(), sc.Input[4].GetStudyID(), 0));
    //std::string dom_quotes_ba = std::to_string(dom_quotes_ba_); //537
    //int dom{};
    //if (dom_quotes_ab > abs(dom_quotes_ba)) dom = dom_quotes_ab;
    //else dom = dom_quotes_ba;

    
    //combining all the sub-strings into one for mysql query
    std::string s0 = "INSERT INTO intraday (symbol, qty, entry_date, entry_time, entry_datetime, entry_price, tick_value, atr_value, wtr_ref) VALUES ( ";
    std::string s1 = "'" + symbol + "'";
    std::string s2 = ", " + qty; // -> , -1
    std::string s3 = ", " + entry_date; // -> '2019-03-07'
    std::string s4 = ", " + entry_time; // -> '14:54:37'
    std::string s5 = ", " + entry_datetime;// -> '2019-03-07 14:54:37'
    std::string s6 = ", " + price; // -> , 6576.50
    std::string s7 = ", " + tick_value; // -> , -432 
    std::string s8 = ", " + atr_value; // -> , 6.77
    std::string s9 = ", " + wtr_value + ")"; // -> , 3445
    std::string complete = s0 + s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9;
    const char * query_insert = complete.c_str();
    
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, server, user, password, database, 0, NULL, 0); 
    res = mysql_store_result(conn);
    int query_state;

    if (conn) 
    {         
        SCString connected{ "Success connected - writing entry parameters" };
        sc.AddMessageToLog(connected, 1);

        //query_state = mysql_query(conn, 
        //    "CREATE TABLE IF NOT EXISTS intraday"
        //    "(symbol varchar(100), qty int, entry_date date, entry_time time, entry_datetime datetime, exit_datetime datetime, entry_price float, exit_price float, tick_value int, atr_value float, wtr_ref float, profit_loss float)"); 

        //if (!query_state)
        //{
        //    SCString create_table{ "Successful creating a table - writing entry parameters" };
        //    sc.AddMessageToLog(create_table, 1);               
        //} else
        //{
        //    SCString create_table_err{ "Creating table failed - writing entry parameters" };
        //    sc.AddMessageToLog(create_table_err, 1);
        //}

        query_state = mysql_query(conn, query_insert); 
        if (!query_state)
        {
            SCString insert_into_table{ "Successful insert into the table - writing entry parameters" };
            sc.AddMessageToLog(insert_into_table, 1);
        } else
        {
            SCString insert_into_table_err{ "Inserting into table failed - writing entry parameters" };
            sc.AddMessageToLog(insert_into_table_err, 1);
        }
    }
    else
    {
        SCString connected{ "ERROR !! Could not connect to write entry details" }; 
        sc.AddMessageToLog(connected, 1);
    }
    mysql_free_result(res);
    mysql_close(conn);
}

void update_results(SCStudyInterfaceRef sc)
{
    s_SCPositionData PositionData;
    sc.GetTradePosition(PositionData);
   
    std::string symbol{ PositionData.Symbol }; //symbol
    std::string profit_loss = std::to_string(PositionData.LastTradeProfitLoss); //profit_loss
    std::string runup = std::to_string(PositionData.MaximumOpenPositionProfit);  //runup
    std::string drawdown = std::to_string(PositionData.MaximumOpenPositionLoss); //drawdown
        
    auto last_entry_dt{ PositionData.LastExitDateTime };
    int year_, month_, day_, hour_, minute_, second_;
    last_entry_dt.GetDateTimeYMDHMS(year_, month_, day_, hour_, minute_, second_);
    std::string year = std::to_string(year_);
    std::string month = std::to_string(month_);
    std::string day = std::to_string(day_);
    std::string hour = std::to_string(hour_);
    std::string minute = std::to_string(minute_);
    std::string second = std::to_string(second_);
    std::string exit_datetime = "'" + year + "-" + month + "-" + day + " " + hour + ":" + minute + ":" + second + "'"; //exit_datetime

    // Query -> " update intraday set profit_loss = ....., runup = ......, drawdown = ........., exit_datetime where profit_loss is null and symbol = ..... ";

    std::string update = "UPDATE intraday SET profit_loss = " + profit_loss + 
                                                ", runup = " + runup + 
                                                ", drawdown = " + drawdown + 
                                                ", exit_datetime = " + exit_datetime +
                                                "  WHERE profit_loss is NULL AND symbol = " + "'" + symbol + "'" + ";";
    const char * query_update = update.c_str();

    conn = mysql_init(0);
    conn = mysql_real_connect(conn, server, user, password, database, 0, NULL, 0);
    res = mysql_store_result(conn);
    int query_state;
    if (conn)
    {
        SCString connected{ "Success connected - updating results" };
        sc.AddMessageToLog(connected, 1);

        query_state = mysql_query(conn, query_update);
        if (!query_state)
        {
            SCString insert_into_table{ "Successful updating results" };
            sc.AddMessageToLog(insert_into_table, 1);
        }
        else
        {
            SCString insert_into_table_err{ "Updating results failed" };
            sc.AddMessageToLog(insert_into_table_err, 1);
        }
    }
    else
    {
        SCString connected{ "ERROR !! Could not connect to update results" };
        sc.AddMessageToLog(connected, 1);
    }
    mysql_free_result(res);
    mysql_close(conn);
}

