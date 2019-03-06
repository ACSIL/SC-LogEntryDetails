#include "sierrachart.h"
#include <fstream>
#include <string>

SCDLLName("LOG USING PERZIST VARS")

//dat do grafu s tickem 
SCSFExport scsf_SaveTheTickValueToPerzistVar(SCStudyInterfaceRef sc)
{
    if (sc.SetDefaults)
    {
        sc.GraphName = "Save TICK value into perzist";
        sc.GraphRegion = 0;//same as main
        sc.FreeDLL = 0;
        sc.UpdateAlways = 1;
        sc.GlobalDisplayStudyNameSubgraphNamesAndValues = 0;
        return;
    }
    float& tick_value = sc.GetPersistentFloat(0);
    tick_value = sc.Close[sc.Index];
}

//dat do grafu s ATR
SCSFExport scsf_SaveTheATRValueToPerzistVar(SCStudyInterfaceRef sc)
{
    if (sc.SetDefaults)
    {
        sc.Input[1].Name = "ATR";
        sc.Input[1].SetStudyID(1);
        sc.GraphName = "Save ATR value into perzist";
        sc.GraphRegion = 0;//same as main
        sc.GlobalDisplayStudyNameSubgraphNamesAndValues = 0;
        sc.FreeDLL = 0;
        sc.UpdateAlways = 1;
        return;
    }
    float& atr_value = sc.GetPersistentFloat(0);
    atr_value = sc.Close[sc.Index];
    SCFloatArray ATR;
    if (sc.GetStudyArrayUsingID(sc.Input[1].GetStudyID(), 0, ATR) > 0 && ATR.GetArraySize() > 0)
    {
        atr_value = ATR[sc.Index]; //uloz to perzistu
    }
}

//dat do grafu s WTR
SCSFExport scsf_SaveTheWTRValueToPerzistVar(SCStudyInterfaceRef sc)
{
    if (sc.SetDefaults)
    {
        sc.GraphName = "Save WTR value into perzist";
        sc.Input[1].Name = "WTR";
        sc.Input[1].SetChartStudySubgraphValues(1, 1, 0);
        sc.GraphRegion = 0;//same as main
        sc.Subgraph[0].Name = " ";
        sc.GlobalDisplayStudyNameSubgraphNamesAndValues = 0;
        sc.FreeDLL = 0;
        sc.UpdateAlways = 1;
        return;
    }
    float& wtr_value = sc.GetPersistentFloat(0);
    SCFloatArray wtr;
    sc.GetStudyArrayFromChartUsingID(sc.Input[1].GetChartStudySubgraphValues(), wtr);
    wtr_value = wtr[sc.Index];
}

//dat do grafu kde je DOM quotes
SCSFExport scsf_SaveTheDOMABQuotesToPerzistVar(SCStudyInterfaceRef sc)
{
    if (sc.SetDefaults)
    {
        sc.GraphName = "Save DOM A-B quotes value into perzist";
        sc.Input[1].Name = "DOM";
        sc.Input[1].SetChartStudySubgraphValues(1, 1, 0);
        sc.GraphRegion = 0;//same as main
        sc.Subgraph[0].Name = " ";
        sc.GlobalDisplayStudyNameSubgraphNamesAndValues = 0;
        sc.FreeDLL = 0;
        sc.UpdateAlways = 1;
        return;
    }
    float& dom_quotes = sc.GetPersistentFloat(0);
    SCFloatArray dom;
    sc.GetStudyArrayFromChartUsingID(sc.Input[1].GetChartStudySubgraphValues(), dom);
    dom_quotes = dom[sc.Index];
}

//dat do grafu kde je DOM quotes
SCSFExport scsf_SaveTheDOMBAQuotesToPerzistVar(SCStudyInterfaceRef sc)
{
    if (sc.SetDefaults)
    {
        sc.GraphName = "Save DOM B-A quotes value into perzist";
        sc.Input[1].Name = "DOM";
        sc.Input[1].SetChartStudySubgraphValues(1, 1, 0);
        sc.GraphRegion = 0;//same as main
        sc.Subgraph[0].Name = " ";
        sc.GlobalDisplayStudyNameSubgraphNamesAndValues = 0;
        sc.FreeDLL = 0;
        sc.UpdateAlways = 1;
        return;
    }
    float& dom_quotes = sc.GetPersistentFloat(0);
    SCFloatArray dom;
    sc.GetStudyArrayFromChartUsingID(sc.Input[1].GetChartStudySubgraphValues(), dom);
    dom_quotes = dom[sc.Index];
}

