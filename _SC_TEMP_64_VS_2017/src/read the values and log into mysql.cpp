#include "sierrachart.h"
#include "database.h"
#include <fstream>
#include <string>
#include <mysql.h>

//dat do grafu kde vsechno vypisu 
SCSFExport scsf_ReadValuesFromPerzistVarsAndLogIntoMySQL(SCStudyInterfaceRef sc)
{
    if (sc.SetDefaults)
    {
        sc.Input[0].Name = "Chart for Tick";
        sc.Input[0].SetChartStudyValues(1, 0);
        sc.Input[1].Name = "Chart for ATR";
        sc.Input[1].SetChartStudyValues(2, 0);
        sc.Input[2].Name = "Chart for WTR";
        sc.Input[2].SetChartStudyValues(3, 0);
        sc.Input[3].Name = "Chart for DOM Ask-Bid";
        sc.Input[3].SetChartStudyValues(4, 0);
        sc.Input[4].Name = "Chart for DOM Bid-Ask";
        sc.Input[4].SetChartStudyValues(5, 0);
        sc.GraphRegion = 0;//same as main
        sc.GraphName = "TEST Read the perzists and log into mysql";
        sc.GlobalDisplayStudyNameSubgraphNamesAndValues = 0;
        sc.FreeDLL = 1;
        sc.UpdateAlways = 1;
        return;
    }

    //get the perzist data
    int tick_value = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[0].GetChartNumber(), sc.Input[0].GetStudyID(), 0));
    float atr_value = sc.GetStudyPersistentFloatFromChart(sc.Input[1].GetChartNumber(), sc.Input[1].GetStudyID(), 0);
    int wtr_value = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[2].GetChartNumber(), sc.Input[2].GetStudyID(), 0));
    int dom_quotes_ab = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[3].GetChartNumber(), sc.Input[3].GetStudyID(), 0));
    int dom_quotes_ba = static_cast<int>(sc.GetStudyPersistentFloatFromChart(sc.Input[4].GetChartNumber(), sc.Input[4].GetStudyID(), 0));

    //set the internal sierra logs message details
    int &previous_qt_perzist = sc.GetPersistentInt(0);
    int &already_logged_perzist = sc.GetPersistentInt(1);
    SCString log;
    s_SCPositionData PositionData;
    sc.GetTradePosition(PositionData);
    std::string symbol{ PositionData.Symbol };
    t_OrderQuantity32_64 qty{ PositionData.PositionQuantity };

    //writing into database
    if (previous_qt_perzist == 0 && PositionData.PositionQuantity != 0) // after opening the possition write entry details into mysql
    {
        write_entry_details_into_db(sc);
        previous_qt_perzist = 1;
    }
    if (previous_qt_perzist == 1 && PositionData.PositionQuantity == 0) // after closing the possition write results into mysql
    { 
        update_results(sc);
        previous_qt_perzist = 0;
    }	

    double PriceHighDuringPosition = 0;
    PriceHighDuringPosition = PositionData.PriceHighDuringPosition;
    double PriceLowDuringPosition = 0;
    PriceLowDuringPosition = PositionData.PriceLowDuringPosition;

    double MaximumOpenPositionProfit = 0;
    MaximumOpenPositionProfit = PositionData.MaximumOpenPositionProfit;
    double MaximumOpenPositionLoss = 0;
    MaximumOpenPositionLoss = PositionData.MaximumOpenPositionLoss;

    s_UseTool tick;
    tick.Clear();
    tick.ChartNumber = sc.ChartNumber;
    tick.DrawingType = DRAWING_TEXT;
    tick.FontSize = 10;
    tick.FontBold = false;
    tick.AddMethod = UTAM_ADD_OR_ADJUST;
    tick.UseRelativeVerticalValues = 1;
    tick.BeginDateTime = 7;
    tick.BeginValue = 80;
    tick.Color = RGB(255, 255, 255);
    tick.Region = 0;
    tick.Text.Format("PriceHighDuringPosition: %f", PriceHighDuringPosition);
    tick.LineNumber = 11;
    sc.UseTool(tick);

    s_UseTool atr;
    atr.Clear();
    atr.ChartNumber = sc.ChartNumber;
    atr.DrawingType = DRAWING_TEXT;
    atr.FontSize = 10;
    atr.FontBold = false;
    atr.AddMethod = UTAM_ADD_OR_ADJUST;
    atr.UseRelativeVerticalValues = 1;
    atr.BeginDateTime = 7;
    atr.BeginValue = 77;
    atr.Color = RGB(255, 255, 255);
    atr.Region = 0;
    atr.Text.Format("PriceLowDuringPosition: %f", PriceLowDuringPosition);
    atr.LineNumber = 12;
    sc.UseTool(atr);

    s_UseTool a;
   a.Clear();
   a.ChartNumber = sc.ChartNumber;
   a.DrawingType = DRAWING_TEXT;
   a.FontSize = 10;
   a.FontBold = false;
   a.AddMethod = UTAM_ADD_OR_ADJUST;
   a.UseRelativeVerticalValues = 1;
   a.BeginDateTime = 7;
   a.BeginValue = 74;
   a.Color = RGB(255, 255, 255);
   a.Region = 0;
   a.Text.Format("MaximumOpenPositionProfit: %f", MaximumOpenPositionProfit);
   a.LineNumber = 13;
    sc.UseTool(a);

    s_UseTool b;
    b.Clear();
    b.ChartNumber = sc.ChartNumber;
    b.DrawingType = DRAWING_TEXT;
    b.FontSize = 10;
    b.FontBold = false;
    b.AddMethod = UTAM_ADD_OR_ADJUST;
    b.UseRelativeVerticalValues = 1;
    b.BeginDateTime = 7;
    b.BeginValue = 71;
    b.Color = RGB(255, 255, 255);
    b.Region = 0;
    b.Text.Format("MaximumOpenPositionLoss: %f", MaximumOpenPositionLoss);
    b.LineNumber = 14;
    sc.UseTool(b);


    //draw the data into chart																		
    {
        s_UseTool tick;
        tick.Clear();
        tick.ChartNumber = sc.ChartNumber;
        tick.DrawingType = DRAWING_TEXT;
        tick.FontSize = 10;
        tick.FontBold = false;
        tick.AddMethod = UTAM_ADD_OR_ADJUST;
        tick.UseRelativeVerticalValues = 1;
        tick.BeginDateTime = 7;
        tick.BeginValue = 90;
        tick.Color = RGB(255, 255, 255);
        tick.Region = 0;
        tick.Text.Format("Tick: %i", tick_value);
        tick.LineNumber = 1;
        sc.UseTool(tick);

        s_UseTool atr;
        atr.Clear();
        atr.ChartNumber = sc.ChartNumber;
        atr.DrawingType = DRAWING_TEXT;
        atr.FontSize = 10;
        atr.FontBold = false;
        atr.AddMethod = UTAM_ADD_OR_ADJUST;
        atr.UseRelativeVerticalValues = 1;
        atr.BeginDateTime = 7;
        atr.BeginValue = 87;
        atr.Color = RGB(255, 255, 255);
        atr.Region = 0;
        atr.Text.Format("ATR: %3.2f", atr_value);
        atr.LineNumber = 2;
        sc.UseTool(atr);

        s_UseTool wtr;
        wtr.Clear();
        wtr.ChartNumber = sc.ChartNumber;
        wtr.DrawingType = DRAWING_TEXT;
        wtr.FontSize = 10;
        wtr.FontBold = false;
        wtr.AddMethod = UTAM_ADD_OR_ADJUST;
        wtr.UseRelativeVerticalValues = 1;
        wtr.BeginDateTime = 7;
        wtr.BeginValue = 84;
        wtr.Color = RGB(255, 255, 255);
        wtr.Region = 0;
        wtr.Text.Format("WTR: %i", wtr_value);
        wtr.LineNumber = 3;
        sc.UseTool(wtr);
    }
}