#include "sierrachart.h"
#include <fstream>
#include <string>
#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <fstream>

const char *server = "localhost";
const char *user = "root";
const char *password = "emeraldincubus";
const char *database = "trading_discretionary";

MYSQL *conn;
MYSQL_RES *res;

void update_results(SCStudyInterfaceRef sc)
{
    s_SCPositionData PositionData;
    sc.GetTradePosition(PositionData);

    double profit_loss_{ PositionData.LastTradeProfitLoss };
    std::string profit_loss = std::to_string(profit_loss_); // ziska profit_loss

    //  "UPDATE intraday_entry_parameters set profit_loss = .......... where profit_is NULL"
    std::string update = "UPDATE intraday_entry_parameters set profit_loss = " + profit_loss + " where profit_loss is NULL";
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


