#include "sierrachart.h"
SCDLLName("LOGOVANI")

//loguje detaily po vstupu
/*
vylogovat bych rad nasledujici:
-denni range
-volatilitu (atr)
-polohu ticku
-iml situaci krz perzist vary
-prumer volume
-prumernou velikost kotaci na domu (10levelu)
-prumernou velikost market orderu
-wtr referenci
*/
SCSFExport scsf_LoggingEntryDetails(SCStudyInterfaceRef sc)
{
	
	if (sc.SetDefaults)
	{
		sc.Subgraph[0].Name = "ATR";
		sc.Subgraph[1].Name = "Tick nyse";

		sc.Input[1].Name = "Chart number of TickNyse";
		sc.Input[1].SetChartNumber(1);

		sc.Input[2].Name = "Chart number of TickNyse";
		sc.Input[2].SetChartNumber(1);

		//draw sgs
		sc.Input[10].Name = "Draw ATR";
		sc.Input[10].SetYesNo(0);
		
		sc.Input[11].Name = "Draw Tick Nyse";
		sc.Input[11].SetYesNo(0);
		
		sc.Input[12].Name = "Draw Volume Avg";
		sc.Input[12].SetYesNo(0);
		
		sc.Input[13].Name = "Draw WTR ref";
		sc.Input[13].SetYesNo(0);

		sc.Input[14].Name = "Draw DOM sum quotes";
		sc.Input[14].SetYesNo(0);

		sc.GraphName = "Log";
		sc.AutoLoop = 1;
		//sc.GraphRegion = 1;
		sc.UpdateAlways = 1;
		sc.FreeDLL = 1;

		return;
	}
	//calculate daily range
	SCDateTime start_time = sc.GetTradingDayStartDateTimeOfBar(sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar]);
	float daily_open; 
	float daily_high;
	float daily_low; 
	float daily_close;
	sc.GetOHLCForDate(start_time, daily_open, daily_high, daily_low, daily_close);
	float daily_range = (daily_high - daily_low)/sc.TickSize;

	//calcualte atr	
	sc.ATR(sc.BaseDataIn, sc.Subgraph[0], 10, MOVAVGTYPE_SIMPLE);
	float ATR_value = sc.Subgraph[0][sc.Index];
	if (sc.Input[2].GetYesNo() == 1) { sc.Subgraph[0][sc.Index]; }	

	//calculate tick nyse 
	SCGraphData base_data_tick_nyse;
	sc.GetChartBaseData(sc.Input[1].GetChartNumber(), base_data_tick_nyse);
	SCFloatArrayRef last_price_tick_nyse = base_data_tick_nyse[SC_LAST]; //jakej je rozdil kdyzbych tady nedal SCFloatArrayREF ale jen SCFloatArray?? bude to delat kopii?
	int reference_index = sc.GetNearestMatchForDateTimeIndex(sc.Input[1].GetChartNumber(), sc.Index);
	float tick_nyse_last_price = last_price_tick_nyse[reference_index];
	if (sc.Input[3].GetYesNo() == 1) { sc.Subgraph[1][sc.Index] = tick_nyse_last_price; }

	//calculate volume avg
	SCFloatArray vol;
	sc.SimpleMovAvg(sc.Volume, vol, 5);
	if (sc.Input[4].GetYesNo() == 1) { sc.Subgraph[2][sc.Index] = vol[sc.Index]; }

	//calculate wtr reference
	//SCGraphData wtr_ref;
	//sc.GetChartBaseData()











	s_SCPositionData PositionData;
	sc.GetTradePosition(PositionData);

	int &previous_qt_pv = sc.GetPersistentInt(0);
	int &already_logged_pv = sc.GetPersistentInt(1);
	
	SCString log;
	log.Format("Entry price: %3.2f, Daily range: %f, ATR floatArr: %f, ATR floatArr2: %f, ATR subgraph: %f ", PositionData.AveragePrice);

	if (previous_qt_pv == 0 && PositionData.PositionQuantity != 0)
	{		
		if (already_logged_pv == 0)
		{
			sc.AddMessageToLog(log, 0);
			already_logged_pv = 1;
		}
	}
	//reset the persist var for loging to zero again
	if (PositionData.PositionQuantity == 0) { already_logged_pv = 0; } 
	
	







	////----------------------------------------------------------------------------- trading booly ----------------------------------------------------------------------

	//sc.AllowMultipleEntriesInSameDirection = true;
	//sc.MaximumPositionAllowed = 1000;
	//sc.SupportReversals = false;
	//sc.SendOrdersToTradeService = false;
	//sc.AllowOppositeEntryWithOpposingPositionOrOrders = true;
	//sc.SupportAttachedOrdersForTrading = true;
	//sc.CancelAllOrdersOnEntriesAndReversals = false;
	//sc.AllowEntryWithWorkingOrders = false;
	//sc.CancelAllWorkingOrdersOnExit = false;
	//sc.AllowOnlyOneTradePerBar = true;
	//sc.MaintainTradeStatisticsAndTradesData = true;

	////----------------------------------------------------------------------------- priprava obchodnich pokynu ----------------------------------------------------------------------

	//s_SCNewOrder obycejny_pokyn;
	//obycejny_pokyn.OrderType = SCT_ORDERTYPE_MARKET;
	//obycejny_pokyn.OrderQuantity = 1;

	////----------------------------------------------------------------------------- vypis do logu ----------------------------------------------------------------------

	//int& stary_cas_persist = sc.GetPersistentInt(999);

	////log jednou za vterinu
	//if (logovani.GetYesNo() == 1 && sc.IsFullRecalculation == 0 && stary_cas_persist != sc.CurrentSystemDateTime.GetSecond())

	//{
	//	//tady co chci vylogovavat
	//	float volume_front = 0;
	//	float volume_back = 0;
	//	if (!v_asks->empty())
	//	{
	//		volume_front = v_asks->front();
	//		volume_back = v_asks->back();
	//	}

	//	SCString vypis_parametru;
	//	vypis_parametru.Format("pocet prvku: %i, prvni prvek: %3.3f, posledni prvek: %3.3f", v_asks->size(), volume_front, volume_back);
	//	sc.AddMessageToLog(vypis_parametru, 0);

	//	stary_cas_persist = sc.CurrentSystemDateTime.GetSecond();
	//}

	////----------------------------------------------------------------------------- vykreslovani userdrawings ----------------------------------------------------------------------

	

	////----------------------------------------------------------------------------- vystup na konci dne ----------------------------------------------------------------------

	//s_SCPositionData PositionData;
	//sc.GetTradePosition(PositionData);

	//if (PositionData.PositionQuantity != 0 && sc.BaseDateTimeIn[sc.Index].GetTime() == konec_rth.GetTime()) 
	//	sc.FlattenAndCancelAllOrders();
	//
}


SCSFExport scsf_Test(SCStudyInterfaceRef sc)
{
	if (sc.SetDefaults)
	{
		sc.Subgraph[0].Name;
		sc.Subgraph[1].Name;
		sc.Subgraph[1].PrimaryColor - RGB(111, 111, 111);

	
		sc.GraphName = "Test vykreslovani";
		sc.AutoLoop = 1;
		sc.UpdateAlways = 1;
		sc.FreeDLL = 1;

		return;
	}
	
	sc.ATR(sc.BaseDataIn, sc.Subgraph[0], 10, MOVAVGTYPE_SIMPLE);
	float ATR_value = sc.Subgraph[0][sc.Index];
	sc.Subgraph[1][sc.Index] = ATR_value;
	
}