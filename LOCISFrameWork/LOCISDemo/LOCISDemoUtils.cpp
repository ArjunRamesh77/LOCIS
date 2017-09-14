#include "LOCISDemo.h"
#include <iomanip>
#include <sstream>

using namespace System;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Callback to display Messages
long LOCISInfoCallBack_Demo(long lInfoType, const char* cMessage)
{
	GlobalObjects::UI_H->printOutputMessage(lInfoType, cMessage, 0);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Callback to display Errors
long LOCISErrorCallBack_Demo(long lErrorType, const char* cMessage)
{
	GlobalObjects::UI_H->setErrorPrintMode(true);
	GlobalObjects::UI_H->printOutputMessage(lErrorType, cMessage, 1);
	GlobalObjects::UI_H->setErrorPrintMode(false);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// helper to color text selectively in RTB
void LOCISDemo::LOCISDemo::AppendRTBText(System::Windows::Forms::RichTextBox^ box, System::Drawing::Color color, System::String^ text)
{
	int start = box->TextLength;
	box->AppendText(text);
	int end = box->TextLength;

	// Textbox may transform chars, so (end-start) != text.Length
	box->Select(start, end - start);
	{
		box->SelectionColor = color;
		// could set box.SelectionBackColor, box.SelectionFont too.
	}
	box->SelectionLength = 0; // clear
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Callback to get Steady State results
long LOCISSteadyStateResultsCallBack_Demo(long lpNum, const char* cNames, double* dpValues)
{	
	//Create columns for steady state
	GlobalObjects::UI_H->CreateSteadyStateOutputHeader();

	std::stringstream ss;
	ss << cNames;
	std::string line;
	int c = 0;
	while (std::getline(ss, line))
	{
		std::stringstream ss;
		ss << std::scientific << std::setprecision(10) << dpValues[c] << "\0";
		array<String^>^row = gcnew array<String^>{gcnew String(line.c_str()), gcnew String(ss.str().c_str())};
		GlobalObjects::UI_H->AddArrayToDataGridSSRes(row);
		c++;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Callback to get Dynamic Results
long LOCISDynamicResultsCallBack_Demo(long colindex, long lpNum, const char* cNames, double* dpTime, double* dpValues)
{
	//Create Header
	if (lpNum == -1)
	{
		GlobalObjects::UI_H->CreateDynamicOutputHeader(lpNum, cNames);
	}
	else
	{
		// Add time slice
		GlobalObjects::UI_H->AddTimeColumnWithData((int)colindex, *dpTime, (int)lpNum, dpValues);
	}
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Loads all the functions from LOCISCore.dll
bool LOCISDemo::LOCISDemo::loadAllCoreFunctions()
{
	if (DLL_FCN == NULL)
	{
		DLL_FCN = new LOCIS_DLL_FCNPTRS;
		if (DLL_FCN == NULL)
			return false;
	}
	
	// Load the LOCISCore DLL
	if (H_LOCIS_CORE_DLL == NULL)
	{
		H_LOCIS_CORE_DLL = LoadLibrary(L"LOCISCore.dll");

		if (H_LOCIS_CORE_DLL)
		{
			//Load all functions
			DLL_FCN->LOCISCreate = (LOCISDLL_LOCISCreate)GetProcAddress(H_LOCIS_CORE_DLL, "LOCISCreate");
			DLL_FCN->LOCISExit = (LOCISDLL_LOCISExit)GetProcAddress(H_LOCIS_CORE_DLL, "LOCISExit");
			DLL_FCN->LOCISSetTraceFile = (LOCISDLL_LOCISSetTraceFile)GetProcAddress(H_LOCIS_CORE_DLL, "LOCISSetTraceFile");
			DLL_FCN->LOCISSetCallBacks = (LOCISDLL_LOCISSetCallBacks)GetProcAddress(H_LOCIS_CORE_DLL, "LOCISSetCallBacks");
			DLL_FCN->LOCISInit = (LOCISDLL_LOCISInit)GetProcAddress(H_LOCIS_CORE_DLL, "LOCISInit");
			DLL_FCN->LOCISSolve = (LOCISDLL_LOCISSolve)GetProcAddress(H_LOCIS_CORE_DLL, "LOCISSolve");

			if (!DLL_FCN->LOCISCreate)
				return false;
			if (!DLL_FCN->LOCISExit)
				return false;
			if (!DLL_FCN->LOCISSetTraceFile)
				return false;
			if (!DLL_FCN->LOCISSetCallBacks)
				return false;
			if (!DLL_FCN->LOCISInit)
				return false;
			if (!DLL_FCN->LOCISSolve)
				return false;

		}
		else
		{
			// Initializes the variables to pass to the MessageBox::Show method.
			String^ message = "LOCISCore.dll is not found or cannot be opened!";
			String^ caption = "dll missing";
			MessageBoxButtons buttons = MessageBoxButtons::OK;
			MessageBoxIcon Icon = MessageBoxIcon::Stop;
			System::Windows::Forms::DialogResult result;

			// Displays the MessageBox.
			result = MessageBox::Show(this, message, caption, buttons, Icon);
			return false;
		}
	}
	return true;
}

