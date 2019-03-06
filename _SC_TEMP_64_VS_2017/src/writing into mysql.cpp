#include "sierrachart.h"
#include <fstream>
#include <string>
#include <mysql.h>
#include <iostream>
#include <iostream>
#include <iomanip>
#include <fstream>

const char *server = "localhost";
const char *user = "root";
const char *password = "--------";
const char *database = "sc";

int query_state;

MYSQL *conn;
MYSQL_RES *res;


void write_entry_details_into_db(SCStudyInterfaceRef sc)
{    
    
    s_SCPositionData PositionData;
    sc.GetTradePosition(PositionData);

    //getting the vars and converting them all to strings to be used in mysql queries
    std::string symbol{ PositionData.Symbol }; //NQ
    t_OrderQuantity32_64 qty_{ PositionData.PositionQuantity }; 
    std::string qty = std::to_string(qty_); //-1 (long/short)
    double price_ = PositionData.AveragePrice;
    std::string price = std::to_string(price_); //6576:50
    int tick_value_ = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[0].GetChartNumber(), sc.Input[0].GetStudyID(), 0));
    std::string tick_value = std::to_string(tick_value_); //-432
    float atr_value_ = sc.GetStudyPersistentFloatFromChart(sc.Input[1].GetChartNumber(), sc.Input[1].GetStudyID(), 0);
    std::string atr_value = std::to_string(atr_value_); //6.77
    int wtr_value_ = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[2].GetChartNumber(), sc.Input[2].GetStudyID(), 0));
    std::string wtr_value = std::to_string(atr_value_); //3445
    int dom_quotes_ab_ = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[3].GetChartNumber(), sc.Input[3].GetStudyID(), 0));
    std::string dom_quotes_ab = std::to_string(dom_quotes_ab_); //320
    int dom_quotes_ba_ = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[4].GetChartNumber(), sc.Input[4].GetStudyID(), 0));
    std::string dom_quotes_ba = std::to_string(dom_quotes_ba_); //537
      
    //combining all the sub-strings into one for mysql query
    // !!! TADY NA TO PAK NAPIS FUNKCI KTERA TO SLOZI JINAK SE Z TOHO POSERES TO POKAZDY PREPISOVAT !!!
    std::string begin = "INSERT INTO intraday (symbol , trade_type , entry_price , tick_value , atr_value , wtr_ref) VALUES ( ";
    std::string first_val = "'" + symbol + "'"; //-> 'NQ'
    std::string second_val = ", " + qty; // -> , -1
    std::string third_val = ", " + price; // -> , 6576.50
    std::string fourth_val = ", " + tick_value; // -> , -432 
    std::string fifth_val = ", " + atr_value; // -> , 6.77
    std::string last_val = ", " + wtr_value + ")"; // -> , 3445)
    std::string complete = begin + first_val + second_val + third_val + fourth_val + fifth_val + last_val;
    const char * query_insert = complete.c_str();

    conn = mysql_init(0);
    conn = mysql_real_connect(conn, server, user, password, database, 0, NULL, 0); 
    res = mysql_store_result(conn);
    if (conn) 
    {              
        SCString connected{ "Success  connected" };
        sc.AddMessageToLog(connected, 1);

        query_state = mysql_query(conn, "CREATE TABLE intraday (symbol varchar(100), trade_type float, entry_price float, tick_value int, atr_value float, wtr_ref float)"); 
        if (!query_state)
        {
            SCString create_table{ "Successful creating a table" };
            sc.AddMessageToLog(create_table, 1);                
        } else
        {
            SCString create_table_err{ "Creating table failed" };
            sc.AddMessageToLog(create_table_err, 1);
        }

        query_state = mysql_query(conn, query_insert); 
        if (!query_state)
        {
            SCString insert_into_table{ "Successful insert into the table" };
            sc.AddMessageToLog(insert_into_table, 1);
        } else
        {
            SCString insert_into_table_err{ "Inserting into table failed" };
            sc.AddMessageToLog(insert_into_table_err, 1);
        }
    }
    else
    {
        SCString connected{ "Error NOT connected" }; //vypis chybu kdyz je to odpojeny
        sc.AddMessageToLog(connected, 1);
    }
  
        s_UseTool con_db;
        con_db.Clear();
        con_db.ChartNumber = sc.ChartNumber;
        con_db.DrawingType = DRAWING_TEXT;
        con_db.FontSize = 10;
        con_db.FontBold = false;
        con_db.AddMethod = UTAM_ADD_OR_ADJUST;
        con_db.UseRelativeVerticalValues = 1;
        con_db.BeginDateTime = 40;
        con_db.BeginValue = 77;
        con_db.Color = RGB(255, 255, 255);
        con_db.Region = 0;
        con_db.Text.Format("query: %s", query_insert);
        con_db.LineNumber = 11;
        sc.UseTool(con_db);

    mysql_free_result(res);
    mysql_close(conn);
}

//
//void write_into_db(SCStudyInterfaceRef sc)
//{
//    int &cpv_already_created_table = sc.GetPersistentInt(103);
//    int &cpv_already_inserted_into_table = sc.GetPersistentInt(104);
//    int &cpv_connected = sc.GetPersistentInt(105);
//
//    if (conn)
//    {
//        cpv_connected = 1;
//
//        SCString connected{ "Connected" };
//        sc.AddMessageToLog(connected, 1);
//        query_state = mysql_query(conn, "CREATE TABLE SC_TEST (cena float, trh varchar(10), volume int)");
//
//        if (!query_state)
//        {
//            SCString create_table{ "Successful creating a table" };
//            sc.AddMessageToLog(create_table, 1);
//            //cpv_already_created_table = 1;
//        } else
//        {
//           SCString create_table_err{ "Creating table failed" };
//           sc.AddMessageToLog(create_table_err, 1);
//        }
//         
//        query_state = mysql_query(conn, "INSERT INTO SC_TEST (cena, trh, volume) VALUES (33332.3, 'NQ', 455)");
//        if (!query_state)
//        {
//            SCString insert_into_table{ "Successful insert into the table" };
//            sc.AddMessageToLog(insert_into_table, 1);
//            //cpv_already_inserted_into_table;
//        } else
//        {
//            SCString insert_into_table_err{ "Inserting into table failed" };
//            sc.AddMessageToLog(insert_into_table_err, 1);
//        }
//    }
//    else
//    {
//        cpv_connected = 0;
//        SCString no_com{ "Not connected" };
//        sc.AddMessageToLog(no_com, 1);
//    }
//    
//    s_UseTool con_db;
//    con_db.Clear();
//    con_db.ChartNumber = sc.ChartNumber;
//    con_db.DrawingType = DRAWING_TEXT;
//    con_db.FontSize = 10;
//    con_db.FontBold = false;
//    con_db.AddMethod = UTAM_ADD_OR_ADJUST;
//    con_db.UseRelativeVerticalValues = 1;
//    con_db.BeginDateTime = 80;
//    con_db.BeginValue = 87;
//    con_db.Color = RGB(255, 255, 255);
//    con_db.Region = 0;
//    con_db.Text.Format("Connected write?: %i", cpv_connected);
//    con_db.LineNumber = 11;
//    sc.UseTool(con_db);
//
//}
//
