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
const char *password = "emeraldincubus";
const char *database = "sc";

int query_state;

MYSQL *conn;
MYSQL_RES *res;

void connect_to_db(SCStudyInterfaceRef sc)
{    
        conn = mysql_init(0);
        conn = mysql_real_connect(conn, server, user, password, database, 0, NULL, 0); //pripoj se
        res = mysql_store_result(conn);
        if (conn) 
        {              
            SCString connected{ "Success  connected" };
            sc.AddMessageToLog(connected, 1);

            query_state = mysql_query(conn, "CREATE TABLE ANOTHER_SC_TEST (cena float, trh varchar(10), volume int)"); //udelej testovaci tabulku
            if (!query_state)
            {
                SCString create_table{ "Successful creating a table" };
                sc.AddMessageToLog(create_table, 1);                
            } else
            {
                SCString create_table_err{ "Creating table failed" };
                sc.AddMessageToLog(create_table_err, 1);
            }

            query_state = mysql_query(conn, "INSERT INTO ANOTHER_SC_TEST (cena, trh, volume) VALUES (33332.3, 'NQ', 455)"); //udelej testovaci zapis
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
