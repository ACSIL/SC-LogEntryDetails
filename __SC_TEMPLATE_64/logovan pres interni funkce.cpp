#include "sierrachart.h"
SCDLLName("LOGOVANI")

//loguje detaily po vstupu
/*
vylogovat bych rad nasledujici:
-denni range - beru z podkladu
-volatilitu (atr) - beru z jineho grafu (10 sec)
-polohu ticku - beru z jiheno grafu (2 min)
-iml situaci krz perzist vary (beru z jinych grafu
-prumer volume (beru z jineho grafu nebo z podkladu)
-prumernou velikost kotaci na domu (10levelu) - beru z jineho grafu (1 sec)
-prumernou velikost market orderu - beru z jineho graf (1 mkt)
-wtr referenci - beru z jineho grafu (1 sec)
*/
SCSFExport scsf_LoggingEntryDetails(SCStudyInterfaceRef sc)
{
	SCInputRef atr_ref = sc.Input[0];
	SCInputRef tick_ref = sc.Input[1];
	SCInputRef vol_ref = sc.Input[2];
	SCInputRef wtr_ref = sc.Input[3];
	SCInputRef dom_ref = sc.Input[4];

	if (sc.SetDefaults)
	{
		atr_ref.Name = "ATR ref";
		atr_ref.SetChartStudySubgraphValues(1, 1, 0);
		
		tick_ref.Name = "Chart number of TickNyse";
		tick_ref.SetChartNumber(0);

		wtr_ref.Name = "WTR";
		wtr_ref.SetChartStudySubgraphValues(1, 1, 0);
			
		dom_ref.Name = "DOM";
		dom_ref.SetChartStudySubgraphValues(1, 1, 0);
			

		//inpus to draw sgs
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

		//subgraphs
		sc.Subgraph[0].Name = "ATR";
		sc.Subgraph[1].Name = "Tick Nyse";
		sc.Subgraph[2].Name = "Avg Volume";
		sc.Subgraph[3].Name = "WTR Reference";
		sc.Subgraph[4].Name = "DOM sum quotes";
	
		sc.GraphName = "Log";
		sc.AutoLoop = 1;
		//sc.GraphRegion = 1;
		sc.UpdateAlways = 1;
		sc.FreeDLL = 1;

		return;
	}

	//get daily range
	SCDateTime start_time = sc.GetTradingDayStartDateTimeOfBar(sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar]);
	float daily_open; 
	float daily_high;
	float daily_low; 
	float daily_close;
	sc.GetOHLCForDate(start_time, daily_open, daily_high, daily_low, daily_close);
	float daily_range = (daily_high - daily_low)/sc.TickSize;

	//get atr - referenced from other chart because i need different timeframe.. 
	SCFloatArray atr;
	sc.GetStudyArrayFromChartUsingID(atr_ref.GetChartStudySubgraphValues(), atr);
	if (sc.Input[10].GetYesNo() == 1) { sc.Subgraph[0][sc.Index] = atr[sc.Index]; }

	//get tick nyse 
	SCGraphData base_data_tick_nyse;
	sc.GetChartBaseData(sc.Input[1].GetChartNumber(), base_data_tick_nyse);
	SCFloatArrayRef last_price_tick_nyse = base_data_tick_nyse[SC_LAST]; //jakej je rozdil kdyzbych tady nedal SCFloatArrayREF ale jen SCFloatArray?? bude to delat kopii?
	int reference_index = sc.GetContainingIndexForDateTimeIndex(sc.Input[1].GetChartNumber(), sc.Index);
	float tick_nyse_last_price = last_price_tick_nyse[reference_index];
	if (sc.Input[11].GetYesNo() == 1) { sc.Subgraph[1][sc.Index] = tick_nyse_last_price; }; //vykresli do sg
	
	s_UseTool tick_tool;
	tick_tool.Clear();
	tick_tool.ChartNumber = sc.ChartNumber;
	tick_tool.DrawingType = DRAWING_TEXT;
	tick_tool.FontSize = 11;
	tick_tool.FontBold = false;
	tick_tool.AddMethod = UTAM_ADD_OR_ADJUST;
	tick_tool.UseRelativeVerticalValues = 1; 
	tick_tool.BeginDateTime = 10;
	tick_tool.BeginValue = 90;
	tick_tool.Color = RGB(0, 200, 0);
	tick_tool.Region = 0;
	tick_tool.Text.Format("Hodnota ticku je %f", tick_nyse_last_price);
	tick_tool.LineNumber = 2;
	sc.UseTool(tick_tool);


	
	////get avg volume - beru naprimo z podkladu ale pak to asi predelam
	//SCFloatArray vol;
	//sc.SimpleMovAvg(sc.Volume, vol, 5);
	//if (sc.Input[12].GetYesNo() == 1) { sc.Subgraph[2][sc.Index] = vol[sc.Index]; }

	////get wtr reference
	//SCFloatArray wtr;
	//sc.GetStudyArrayFromChartUsingID(wtr_ref.GetChartStudySubgraphValues(), wtr);
	//if (sc.Input[13].GetYesNo() == 1) { sc.Subgraph[3][sc.Index] = wtr[sc.Index]; }
	//
	//s_UseTool wtr_tool;
	//wtr_tool.Clear();
	//wtr_tool.ChartNumber = sc.ChartNumber;
	//wtr_tool.DrawingType = DRAWING_TEXT;
	//wtr_tool.FontSize = 21;
	//wtr_tool.FontBold = false;
	//wtr_tool.AddMethod = UTAM_ADD_OR_ADJUST;
	//wtr_tool.UseRelativeVerticalValues = 0; //kdyz chci pouzit begin datetime musi byt 0
	//wtr_tool.BeginDateTime = 5;
	//wtr_tool.Color = RGB(0, 200, 0);
	//wtr_tool.Region = 0;
	//wtr_tool.Text.Format("WTR reference %f", wtr[sc.IndexOfLastVisibleBar]);
	//wtr_tool.LineNumber = 1;
	//wtr_tool.BeginValue = 11;
	//sc.UseTool(wtr_tool);

	//get DOM quottes
	SCFloatArray dom;
	sc.GetStudyArrayFromChartUsingID(dom_ref.GetChartStudySubgraphValues(), dom);
	if (sc.Input[14].GetYesNo() == 1) { sc.Subgraph[4][sc.Index] = dom[sc.Index]; }
	
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

	//priprava sctoolu pro vypis do grafu
	//s_UseTool TextTool;
	//TextTool.Clear();
	//TextTool.ChartNumber = sc.ChartNumber;
	//TextTool.DrawingType = DRAWING_TEXT;
	//TextTool.FontSize = 21;
	//TextTool.FontBold = false;
	//TextTool.AddMethod = UTAM_ADD_OR_ADJUST;
	//TextTool.UseRelativeVerticalValues = 1;
	//TextTool.BeginDateTime = 5;
	//TextTool.Color = RGB(0, 200, 0);
	//TextTool.Region = sc.GraphRegion;

	//vypis obsahu vektoru do grafu
	//TextTool.Text.Format("vektor nad midem obsahuje volume velikosti %f", 1);
	//TextTool.LineNumber = 98890;
	//TextTool.BeginValue = 11;
	//sc.UseTool(TextTool);

	//TextTool.Text.Format("vektor pod midem obsahuje volume velikosti %f", 1);
	//TextTool.LineNumber = 98891;
	//TextTool.BeginValue = 10;
	//sc.UseTool(TextTool);


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
		sc.Subgraph[0].Name = "X";
		sc.Subgraph[1].Name = "Y";
	
		sc.Input[1].Name = "vyreslit?";
		sc.Input[1].SetYesNo(0);

		sc.GraphName = "test";
		sc.AutoLoop = 1;
		sc.UpdateAlways = 1;
		sc.FreeDLL = 1;

		return;
	}
	
	sc.ATR(sc.BaseDataIn, sc.Subgraph[0], 10, MOVAVGTYPE_SIMPLE);
	float atr = sc.Subgraph[0][sc.Index];
	
	//vykresli to jen kdyz reknu !!! 
	if (sc.Input[1].GetYesNo() == 1) { sc.Subgraph[1][sc.Index] = atr; }
	
}

