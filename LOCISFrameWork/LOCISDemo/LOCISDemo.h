#pragma once

#include <windows.h>
#include <map>
#include "LOCIS_API_INCLUDE.h"
#include <vcclr.h> 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structure to store all function pointers
struct LOCIS_DLL_FCNPTRS
{
	//pointers
	LOCISDLL_LOCISCreate LOCISCreate;
	LOCISDLL_LOCISExit LOCISExit;
	LOCISDLL_LOCISSetTraceFile LOCISSetTraceFile;
	LOCISDLL_LOCISSetCallBacks LOCISSetCallBacks;
	LOCISDLL_LOCISInit LOCISInit;
	LOCISDLL_LOCISSolve LOCISSolve;


	LOCIS_DLL_FCNPTRS()
	{
		LOCISCreate = NULL;
		LOCISExit = NULL;
		LOCISSetTraceFile = NULL;
		LOCISSetCallBacks = NULL;
		LOCISInit = NULL;
		LOCISSolve = NULL;
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Callback declarations
long LOCISErrorCallBack_Demo(long lErrorType, const char* cMessage);
long LOCISInfoCallBack_Demo(long lInfoType, const char* cMessage);
long LOCISSteadyStateResultsCallBack_Demo(long lpNum, const char* cNames, double* dpValues);
long LOCISDynamicResultsCallBack_Demo(long colindex, long lpNum, const char* cNames, double* dpTime, double* dpValues);

namespace LOCISDemo {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for LOCISDemo
	/// </summary>
	public ref class LOCISDemo : public System::Windows::Forms::Form
	{
	private:
		bool printErrorMode;
		std::map<int,int> *errorIndices;
		LOCIS_DLL_FCNPTRS* DLL_FCN;

	private: System::Windows::Forms::TabPage^  TAB_SS_RES;
	private: System::Windows::Forms::DataGridView^  DATAGRID_SS_OUT;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  VarName;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  VarValue;
	private: System::Windows::Forms::RichTextBox^  RTB_OUTPUT;
	private: System::Windows::Forms::ToolStripMenuItem^  MENU_OPEN;
	private: System::Windows::Forms::ToolStripMenuItem^  MENU_SAVE;
	private: System::Windows::Forms::ToolStripMenuItem^  MENU_SAVEAS;






	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::Label^  LABEL_FILE_NAME;
	private: System::Windows::Forms::ToolStripMenuItem^  MENU_EXIT;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;

			 HINSTANCE H_LOCIS_CORE_DLL;

	public:
		LOCISDemo(void)
		{
			errorIndices = new std::map<int,int>;
			printErrorMode = false;
			H_LOCIS_CORE_DLL = NULL;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~LOCISDemo()
		{
			if (H_LOCIS_CORE_DLL)
			{
				DLL_FCN->LOCISExit(0, true);
				FreeLibrary(H_LOCIS_CORE_DLL);
			}
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  MENU_BAR_MAIN;
	protected:
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;



	private: System::Windows::Forms::Label^  LABEL_SRC;
	private: System::Windows::Forms::RichTextBox^  RICH_TEXT_BOX_SRC;
	private: System::Windows::Forms::TabControl^  TAB_GROUP_OUTPUT;
	private: System::Windows::Forms::TabPage^  TAB_OUPUT;

	private: System::Windows::Forms::GroupBox^  GROUP_BOX_CONTROL;
	private: System::Windows::Forms::Button^  BUTTON_SOLVE;



	protected:



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(LOCISDemo::typeid));
			this->MENU_BAR_MAIN = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MENU_OPEN = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MENU_SAVE = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MENU_SAVEAS = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MENU_EXIT = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->LABEL_SRC = (gcnew System::Windows::Forms::Label());
			this->RICH_TEXT_BOX_SRC = (gcnew System::Windows::Forms::RichTextBox());
			this->TAB_GROUP_OUTPUT = (gcnew System::Windows::Forms::TabControl());
			this->TAB_OUPUT = (gcnew System::Windows::Forms::TabPage());
			this->RTB_OUTPUT = (gcnew System::Windows::Forms::RichTextBox());
			this->TAB_SS_RES = (gcnew System::Windows::Forms::TabPage());
			this->DATAGRID_SS_OUT = (gcnew System::Windows::Forms::DataGridView());
			this->VarName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->VarValue = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->GROUP_BOX_CONTROL = (gcnew System::Windows::Forms::GroupBox());
			this->BUTTON_SOLVE = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->LABEL_FILE_NAME = (gcnew System::Windows::Forms::Label());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->MENU_BAR_MAIN->SuspendLayout();
			this->TAB_GROUP_OUTPUT->SuspendLayout();
			this->TAB_OUPUT->SuspendLayout();
			this->TAB_SS_RES->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->DATAGRID_SS_OUT))->BeginInit();
			this->GROUP_BOX_CONTROL->SuspendLayout();
			this->SuspendLayout();
			// 
			// MENU_BAR_MAIN
			// 
			this->MENU_BAR_MAIN->ImageScalingSize = System::Drawing::Size(24, 24);
			this->MENU_BAR_MAIN->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->MENU_BAR_MAIN->Location = System::Drawing::Point(0, 0);
			this->MENU_BAR_MAIN->Name = L"MENU_BAR_MAIN";
			this->MENU_BAR_MAIN->Size = System::Drawing::Size(2142, 33);
			this->MENU_BAR_MAIN->TabIndex = 0;
			this->MENU_BAR_MAIN->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->MENU_OPEN,
					this->MENU_SAVE, this->MENU_SAVEAS, this->MENU_EXIT
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(47, 29);
			this->fileToolStripMenuItem->Text = L"file";
			// 
			// MENU_OPEN
			// 
			this->MENU_OPEN->Name = L"MENU_OPEN";
			this->MENU_OPEN->Size = System::Drawing::Size(158, 30);
			this->MENU_OPEN->Text = L"Open";
			this->MENU_OPEN->Click += gcnew System::EventHandler(this, &LOCISDemo::MENU_OPEN_Click);
			// 
			// MENU_SAVE
			// 
			this->MENU_SAVE->Name = L"MENU_SAVE";
			this->MENU_SAVE->Size = System::Drawing::Size(158, 30);
			this->MENU_SAVE->Text = L"Save";
			this->MENU_SAVE->Click += gcnew System::EventHandler(this, &LOCISDemo::MENU_SAVE_Click);
			// 
			// MENU_SAVEAS
			// 
			this->MENU_SAVEAS->Name = L"MENU_SAVEAS";
			this->MENU_SAVEAS->Size = System::Drawing::Size(158, 30);
			this->MENU_SAVEAS->Text = L"Save As";
			this->MENU_SAVEAS->Click += gcnew System::EventHandler(this, &LOCISDemo::MENU_SAVEAS_Click);
			// 
			// MENU_EXIT
			// 
			this->MENU_EXIT->Name = L"MENU_EXIT";
			this->MENU_EXIT->Size = System::Drawing::Size(158, 30);
			this->MENU_EXIT->Text = L"Exit";
			this->MENU_EXIT->Click += gcnew System::EventHandler(this, &LOCISDemo::MENU_EXIT_Click);
			// 
			// LABEL_SRC
			// 
			this->LABEL_SRC->AutoSize = true;
			this->LABEL_SRC->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->LABEL_SRC->ForeColor = System::Drawing::SystemColors::HotTrack;
			this->LABEL_SRC->Location = System::Drawing::Point(33, 61);
			this->LABEL_SRC->Name = L"LABEL_SRC";
			this->LABEL_SRC->Size = System::Drawing::Size(157, 26);
			this->LABEL_SRC->TabIndex = 4;
			this->LABEL_SRC->Text = L"Source Editor";
			// 
			// RICH_TEXT_BOX_SRC
			// 
			this->RICH_TEXT_BOX_SRC->AcceptsTab = true;
			this->RICH_TEXT_BOX_SRC->Font = (gcnew System::Drawing::Font(L"Courier New", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->RICH_TEXT_BOX_SRC->Location = System::Drawing::Point(37, 102);
			this->RICH_TEXT_BOX_SRC->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->RICH_TEXT_BOX_SRC->Name = L"RICH_TEXT_BOX_SRC";
			this->RICH_TEXT_BOX_SRC->Size = System::Drawing::Size(1012, 1380);
			this->RICH_TEXT_BOX_SRC->TabIndex = 5;
			this->RICH_TEXT_BOX_SRC->Text = L"";
			this->RICH_TEXT_BOX_SRC->WordWrap = false;
			// 
			// TAB_GROUP_OUTPUT
			// 
			this->TAB_GROUP_OUTPUT->Controls->Add(this->TAB_OUPUT);
			this->TAB_GROUP_OUTPUT->Controls->Add(this->TAB_SS_RES);
			this->TAB_GROUP_OUTPUT->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TAB_GROUP_OUTPUT->Location = System::Drawing::Point(1084, 319);
			this->TAB_GROUP_OUTPUT->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->TAB_GROUP_OUTPUT->Name = L"TAB_GROUP_OUTPUT";
			this->TAB_GROUP_OUTPUT->SelectedIndex = 0;
			this->TAB_GROUP_OUTPUT->Size = System::Drawing::Size(1012, 1165);
			this->TAB_GROUP_OUTPUT->TabIndex = 7;
			// 
			// TAB_OUPUT
			// 
			this->TAB_OUPUT->BackColor = System::Drawing::Color::Silver;
			this->TAB_OUPUT->Controls->Add(this->RTB_OUTPUT);
			this->TAB_OUPUT->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TAB_OUPUT->Location = System::Drawing::Point(4, 29);
			this->TAB_OUPUT->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->TAB_OUPUT->Name = L"TAB_OUPUT";
			this->TAB_OUPUT->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->TAB_OUPUT->Size = System::Drawing::Size(1004, 1132);
			this->TAB_OUPUT->TabIndex = 0;
			this->TAB_OUPUT->Text = L"Output";
			// 
			// RTB_OUTPUT
			// 
			this->RTB_OUTPUT->BackColor = System::Drawing::SystemColors::Window;
			this->RTB_OUTPUT->Font = (gcnew System::Drawing::Font(L"Courier New", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->RTB_OUTPUT->Location = System::Drawing::Point(0, 2);
			this->RTB_OUTPUT->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->RTB_OUTPUT->Name = L"RTB_OUTPUT";
			this->RTB_OUTPUT->ReadOnly = true;
			this->RTB_OUTPUT->Size = System::Drawing::Size(1012, 1122);
			this->RTB_OUTPUT->TabIndex = 0;
			this->RTB_OUTPUT->Text = L"";
			// 
			// TAB_SS_RES
			// 
			this->TAB_SS_RES->Controls->Add(this->DATAGRID_SS_OUT);
			this->TAB_SS_RES->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TAB_SS_RES->Location = System::Drawing::Point(4, 29);
			this->TAB_SS_RES->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->TAB_SS_RES->Name = L"TAB_SS_RES";
			this->TAB_SS_RES->Size = System::Drawing::Size(1004, 1132);
			this->TAB_SS_RES->TabIndex = 2;
			this->TAB_SS_RES->Text = L"Solution";
			this->TAB_SS_RES->UseVisualStyleBackColor = true;
			// 
			// DATAGRID_SS_OUT
			// 
			this->DATAGRID_SS_OUT->AllowUserToAddRows = false;
			this->DATAGRID_SS_OUT->AllowUserToDeleteRows = false;
			this->DATAGRID_SS_OUT->ClipboardCopyMode = System::Windows::Forms::DataGridViewClipboardCopyMode::EnableAlwaysIncludeHeaderText;
			this->DATAGRID_SS_OUT->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->DATAGRID_SS_OUT->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {
				this->VarName,
					this->VarValue
			});
			this->DATAGRID_SS_OUT->Location = System::Drawing::Point(3, 2);
			this->DATAGRID_SS_OUT->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->DATAGRID_SS_OUT->Name = L"DATAGRID_SS_OUT";
			this->DATAGRID_SS_OUT->ReadOnly = true;
			this->DATAGRID_SS_OUT->RowTemplate->Height = 28;
			this->DATAGRID_SS_OUT->Size = System::Drawing::Size(1581, 1480);
			this->DATAGRID_SS_OUT->TabIndex = 3;
			// 
			// VarName
			// 
			this->VarName->HeaderText = L"Name";
			this->VarName->Name = L"VarName";
			this->VarName->ReadOnly = true;
			// 
			// VarValue
			// 
			this->VarValue->HeaderText = L"Value";
			this->VarValue->Name = L"VarValue";
			this->VarValue->ReadOnly = true;
			// 
			// GROUP_BOX_CONTROL
			// 
			this->GROUP_BOX_CONTROL->Controls->Add(this->BUTTON_SOLVE);
			this->GROUP_BOX_CONTROL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->GROUP_BOX_CONTROL->ForeColor = System::Drawing::SystemColors::HotTrack;
			this->GROUP_BOX_CONTROL->Location = System::Drawing::Point(1094, 102);
			this->GROUP_BOX_CONTROL->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->GROUP_BOX_CONTROL->Name = L"GROUP_BOX_CONTROL";
			this->GROUP_BOX_CONTROL->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->GROUP_BOX_CONTROL->Size = System::Drawing::Size(1008, 191);
			this->GROUP_BOX_CONTROL->TabIndex = 8;
			this->GROUP_BOX_CONTROL->TabStop = false;
			this->GROUP_BOX_CONTROL->Text = L"Controls";
			// 
			// BUTTON_SOLVE
			// 
			this->BUTTON_SOLVE->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"BUTTON_SOLVE.BackgroundImage")));
			this->BUTTON_SOLVE->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->BUTTON_SOLVE->Location = System::Drawing::Point(39, 55);
			this->BUTTON_SOLVE->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->BUTTON_SOLVE->Name = L"BUTTON_SOLVE";
			this->BUTTON_SOLVE->Size = System::Drawing::Size(207, 91);
			this->BUTTON_SOLVE->TabIndex = 0;
			this->BUTTON_SOLVE->UseVisualStyleBackColor = true;
			this->BUTTON_SOLVE->Click += gcnew System::EventHandler(this, &LOCISDemo::BUTTON_SOLVE_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// LABEL_FILE_NAME
			// 
			this->LABEL_FILE_NAME->AutoSize = true;
			this->LABEL_FILE_NAME->Location = System::Drawing::Point(48, 1509);
			this->LABEL_FILE_NAME->Name = L"LABEL_FILE_NAME";
			this->LABEL_FILE_NAME->Size = System::Drawing::Size(0, 20);
			this->LABEL_FILE_NAME->TabIndex = 9;
			// 
			// LOCISDemo
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(2142, 1462);
			this->Controls->Add(this->LABEL_FILE_NAME);
			this->Controls->Add(this->GROUP_BOX_CONTROL);
			this->Controls->Add(this->TAB_GROUP_OUTPUT);
			this->Controls->Add(this->RICH_TEXT_BOX_SRC);
			this->Controls->Add(this->LABEL_SRC);
			this->Controls->Add(this->MENU_BAR_MAIN);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->MENU_BAR_MAIN;
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"LOCISDemo";
			this->Text = L"LOCISDemo";
			this->Load += gcnew System::EventHandler(this, &LOCISDemo::LOCISDemo_Load);
			this->MENU_BAR_MAIN->ResumeLayout(false);
			this->MENU_BAR_MAIN->PerformLayout();
			this->TAB_GROUP_OUTPUT->ResumeLayout(false);
			this->TAB_OUPUT->ResumeLayout(false);
			this->TAB_SS_RES->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->DATAGRID_SS_OUT))->EndInit();
			this->GROUP_BOX_CONTROL->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void BUTTON_SOLVE_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void LOCISDemo_Load(System::Object^  sender, System::EventArgs^  e);
		
	//Utilities
	public: void CreateSteadyStateOutputHeader();
	public: void CreateDynamicOutputHeader(int numVars, const char* Names);
	public: void AddTimeColumnWithData(int colindex, double dpTime, int lpNum, double* dpValues);
	void AppendRTBText(RichTextBox^ box, Color color, String^ text);
	bool loadAllCoreFunctions();
	public: bool printOutputMessage(long msgType, const char* cMessage, int type);
	public: void setErrorPrintMode(bool state);
	public: void AddArrayToDataGridSSRes(array<String^>^row);

	// Menu Items
	private: System::Void MENU_OPEN_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MENU_SAVE_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MENU_SAVEAS_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MENU_EXIT_Click(System::Object^  sender, System::EventArgs^  e);
};

}

ref class GlobalObjects
{
public:
	static LOCISDemo::LOCISDemo^ UI_H;
};


