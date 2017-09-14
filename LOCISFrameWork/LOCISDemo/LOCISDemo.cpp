#include "LOCISDemo.h"
#include <windows.h>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;
using namespace System::IO;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// Create the main window and run it
	Application::Run(gcnew LOCISDemo::LOCISDemo());
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// On Load
System::Void LOCISDemo::LOCISDemo::LOCISDemo_Load(System::Object^  sender, System::EventArgs^  e)
{
	// Get the UI pointer
	GlobalObjects::UI_H = this;
	
	// Load the dll
	if (LOCISDemo::loadAllCoreFunctions() == false)
	{
		// Close the application if dll load failed
		this->Close();
		Application::Exit();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prints a message on the Output Box
bool LOCISDemo::LOCISDemo::printOutputMessage(long msgType, const char* cMessage, int type)
{
	//Convert Message to lines
	std::stringstream ss;
	ss << cMessage;
	std::string line;
	String^ text;

	while (std::getline(ss, line))
	{
		this->RTB_OUTPUT->AppendText("\n");
		//this->RTB_OUTPUT->AppendText(Marshal::PtrToStringAnsi((IntPtr)(char *)line.c_str()));

		if(printErrorMode)
			AppendRTBText(this->RTB_OUTPUT, Color::Red, Marshal::PtrToStringAnsi((IntPtr)(char *)line.c_str()));
		else
			AppendRTBText(this->RTB_OUTPUT, Color::Blue, Marshal::PtrToStringAnsi((IntPtr)(char *)line.c_str()));

		this->RTB_OUTPUT->Update();
	}	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sets The error mode to print
void LOCISDemo::LOCISDemo::setErrorPrintMode(bool state)
{
	printErrorMode = state;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adds a row to the SteadyState Data
void LOCISDemo::LOCISDemo::AddArrayToDataGridSSRes(array<String^>^row)
{
	this->DATAGRID_SS_OUT->Rows->Add(row);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event: 
System::Void LOCISDemo::LOCISDemo::BUTTON_SOLVE_Click(System::Object^  sender, System::EventArgs^  e)
{
	//Test1 : (Simply take the source form source RTB and run the system)
	int numLines = 0;
	std::vector<const char*> cLines;
	for each(String ^ line in this->RICH_TEXT_BOX_SRC->Lines)
	{
		numLines++;
		const char* str = (const char*)(Marshal::StringToHGlobalAnsi(line)).ToPointer();
		cLines.push_back(str);
	}

	char* srcBuffer = new char[numLines * 500];
	int srcBufferptr = 0;
	int lineCounter = 0;
	for each(const char* cp in cLines)
	{
		lineCounter++;
		for (int i = 0; cp[i] != '\0'; i++)
		{
			srcBuffer[srcBufferptr] = cp[i];
			srcBufferptr++;
		}
		if(lineCounter < numLines)
			srcBuffer[srcBufferptr++] = '\n';
		else
			srcBuffer[srcBufferptr++] = '\0';
	}

	// Call
	int obnum;
	int ret;

	// Clear Output
	this->RTB_OUTPUT->Clear();
	this->DATAGRID_SS_OUT->Rows->Clear();

	obnum = DLL_FCN->LOCISCreate();

	errorIndices->clear();
	if (DLL_FCN->LOCISInit(obnum, srcBuffer, LOCISErrorCallBack_Demo, LOCISInfoCallBack_Demo, LOCISSteadyStateResultsCallBack_Demo, LOCISDynamicResultsCallBack_Demo) == 0)
	{
		ret = DLL_FCN->LOCISSolve(obnum);
	}

	ret = DLL_FCN->LOCISExit(obnum, false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Creates the header for the steady  state output
void LOCISDemo::LOCISDemo::CreateSteadyStateOutputHeader()
{
	this->DATAGRID_SS_OUT->Columns->Clear();
	this->DATAGRID_SS_OUT->Columns->Add("VariableName", "Variable");
	this->DATAGRID_SS_OUT->Columns->Add("VariableName", "Value");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Creates the header for the dynamic output
void LOCISDemo::LOCISDemo::CreateDynamicOutputHeader(int numVars, const char* Names)
{
	this->DATAGRID_SS_OUT->Columns->Clear();
	std::stringstream ss;
	ss << Names;
	std::string line;
	int c = 0;
	this->DATAGRID_SS_OUT->Columns->Add("time", "time");
	while (std::getline(ss, line))
	{
		this->DATAGRID_SS_OUT->Columns->Add("varName", gcnew String(line.c_str()));
		c++;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adds a time data set
void LOCISDemo::LOCISDemo::AddTimeColumnWithData(int colindex, double dpTime, int lpNum, double* dpValues)
{
	std::stringstream ssTime;
	ssTime << std::scientific << std::setprecision(7) << dpTime << '\0';
	
	int index = this->DATAGRID_SS_OUT->Rows->Add();
	this->DATAGRID_SS_OUT->Rows[index]->Cells[0]->Value = gcnew String(ssTime.str().c_str());
	for (int i = 0; i < lpNum; i++)
	{
		std::stringstream ssVal;
		ssVal << std::scientific << std::setprecision(7) << dpValues[i] << '\0';
		this->DATAGRID_SS_OUT->Rows[index]->Cells[i+1]->Value = gcnew String(ssVal.str().c_str());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Process Menu Items
System::Void LOCISDemo::LOCISDemo::MENU_OPEN_Click(System::Object^  sender, System::EventArgs^  e)
{
	StreamReader^ sr;

	openFileDialog1->InitialDirectory = "c:\\";
	openFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
	openFileDialog1->FilterIndex = 2;
	openFileDialog1->RestoreDirectory = true;

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		if (openFileDialog1->OpenFile() != nullptr)
		{
			this->RICH_TEXT_BOX_SRC->Clear();
			this->LABEL_FILE_NAME->Text = openFileDialog1->FileName;
			sr = gcnew StreamReader(openFileDialog1->FileName);
			String^ line;
			line = sr->ReadLine();
			while (line)
			{
				this->RICH_TEXT_BOX_SRC->AppendText(line);
				this->RICH_TEXT_BOX_SRC->AppendText("\n");
				line = sr->ReadLine();
			} 
		}
		sr->Close();
	}
}

System::Void LOCISDemo::LOCISDemo::MENU_SAVE_Click(System::Object^  sender, System::EventArgs^  e)
{
	StreamWriter^ sw;

	sw = gcnew StreamWriter(this->LABEL_FILE_NAME->Text);
	if (sw != nullptr)
	{
		for each(String ^ line in this->RICH_TEXT_BOX_SRC->Lines)
		{
			sw->WriteLine(line);
		}
	}
	else
	{
		String^ message = "This operation failed to complete";
		String^ caption = "operation failed";
		MessageBoxButtons buttons = MessageBoxButtons::OK;
		MessageBoxIcon Icon = MessageBoxIcon::Stop;
		System::Windows::Forms::DialogResult result;

		// Displays the MessageBox.
		result = MessageBox::Show(this, message, caption, buttons, Icon);
	}
	sw->Close();
}

System::Void LOCISDemo::LOCISDemo::MENU_SAVEAS_Click(System::Object^  sender, System::EventArgs^  e)
{
	StreamWriter^ sw;
	String^ SaveAsName;

	saveFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
	saveFileDialog1->FilterIndex = 2;
	saveFileDialog1->RestoreDirectory = true;
	if (saveFileDialog1->ShowDialog() == ::DialogResult::OK)
	{
		SaveAsName = saveFileDialog1->FileName;
		sw = gcnew StreamWriter(SaveAsName);
		if (sw != nullptr)
		{
			this->LABEL_FILE_NAME->Text = SaveAsName;
			String^ line;
			for each(String ^ line in this->RICH_TEXT_BOX_SRC->Lines)
			{
				sw->WriteLine(line);
			}
			sw->Close();
		}
		else
		{
			// Initializes the variables to pass to the MessageBox::Show method.
			String^ message = "This operation failed to complete";
			String^ caption = "operation failed";
			MessageBoxButtons buttons = MessageBoxButtons::OK;
			MessageBoxIcon Icon = MessageBoxIcon::Stop;
			System::Windows::Forms::DialogResult result;

			// Displays the MessageBox.
			result = MessageBox::Show(this, message, caption, buttons, Icon);
		}
	}
}

System::Void  LOCISDemo::LOCISDemo::MENU_EXIT_Click(System::Object^  sender, System::EventArgs^  e)
{
	this->Close();
	Application::Exit();
}

/*
System::Void LOCISDemo::LOCISDemo::LIST_BOX_OUTPUT_DrawItem(System::Object^  sender, System::Windows::Forms::DrawItemEventArgs^  e)
{
	e->DrawBackground();
	if(errorIndices->find(e->Index) != errorIndices->end())
		e->Graphics->DrawString(this->LIST_BOX_OUTPUT->Items[e->Index]->ToString(), LIST_BOX_OUTPUT->Font, Brushes::Red, e->Bounds, StringFormat::GenericDefault);
	else
		e->Graphics->DrawString(this->LIST_BOX_OUTPUT->Items[e->Index]->ToString(), LIST_BOX_OUTPUT->Font, Brushes::Blue, e->Bounds, StringFormat::GenericDefault);
}
*/