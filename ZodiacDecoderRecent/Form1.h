#pragma once
//#pragma warning  (disable : 3867 )
#include "stdafx.h"
#include "IBF.h"
#include "RandomSearch.h"
#include "GA.h"
#include "ScrambleClass.h"
#include "Client.h"

#define THRESHOLD 50.0

using namespace std;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace zLauncher2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>

bool run_ga=false;
bool run_ibf=false;
bool run_rndm=false;
bool ga_running=false;
bool ibf_running=false;
bool rndm_running=false;

//KEY COUNTS
int randomKeys = 0;
int ibfKeys = 0;
int scrambleKeys = 0;


static DWORD WINAPI runGA(void *param)
 {
	  GA *theGA = (GA*)(param);
	 		while( (run_ga) || (ga_running) )
			{		
				 theGA->run();
			}
		ga_running = false;
return 0;
}

static DWORD WINAPI runIBF(void *param)
 {
	  IBF *theIBF= (IBF*)(param);
	 		while( (run_ibf) )
			{		
				 theIBF->run();
			}
		ibf_running = false;
return 0;
}

static DWORD WINAPI runRNDM(void *param)
 {
	  RandomSearch *theRNDM = (RandomSearch*)(param);
	 		while(run_rndm)
			{		
				 theRNDM->run();

				 randomKeys++;
			}
		rndm_running = false;
return 0;
}

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:

		string *KeyStream;
	
	private: System::Windows::Forms::TextBox^  bestSolnBox;
	
	public: 
		
		//ALGORITHM OBJECTS
		ScoreClass *scorer;
		GA *myGA;
		IBF *myIBF;
		RandomSearch *myRNDM;
		ScrambleClass *scrambler;

		float bestScore;
		string *bestSoln;
	
	private: System::Windows::Forms::Label^  label6;
	
	public: 
	
	private: System::Windows::Forms::Label^  label7;
	
	private: System::Windows::Forms::CheckBox^  checkBox1;

	private: System::Windows::Forms::Label^  label8;

	private: System::Windows::Forms::TextBox^  statusBox;

	private: System::Windows::Forms::Label^  statusLabel;

	private: System::Windows::Forms::Button^  zodiacDecoderButton;

	private: System::Windows::Forms::Button^  letterScrambleButton;

	private: System::Windows::Forms::ProgressBar^  progressBar;
	private: System::Windows::Forms::Timer^  timer1;


	string *bestKey;

	public: 
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			bestScore = 0;

			//ALGORITHM OBJECTS
			KeyStream = new string();
			scorer = new ScoreClass;
			myGA = new GA(KeyStream,scorer);
			myIBF = new IBF(KeyStream);
			myRNDM = new RandomSearch(KeyStream);
			scrambler = new ScrambleClass(KeyStream);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  randomButton;
	protected: 
	private: System::Windows::Forms::TextBox^  outBox;
	private: System::Windows::Forms::Button^  closeButton;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  scoreBox;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  bestScoreBox;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  bestKeyBox;

	private: System::Windows::Forms::TextBox^  solnBox;

	private: System::Windows::Forms::Button^  gaButton;
	private: System::Windows::Forms::Button^  ibfButton;
private: System::ComponentModel::IContainer^  components;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 WINDOWS FORM CODE
//
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->randomButton = (gcnew System::Windows::Forms::Button());
			this->outBox = (gcnew System::Windows::Forms::TextBox());
			this->closeButton = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->scoreBox = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->bestScoreBox = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->bestKeyBox = (gcnew System::Windows::Forms::TextBox());
			this->solnBox = (gcnew System::Windows::Forms::TextBox());
			this->gaButton = (gcnew System::Windows::Forms::Button());
			this->ibfButton = (gcnew System::Windows::Forms::Button());
			this->bestSolnBox = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->statusBox = (gcnew System::Windows::Forms::TextBox());
			this->statusLabel = (gcnew System::Windows::Forms::Label());
			this->zodiacDecoderButton = (gcnew System::Windows::Forms::Button());
			this->letterScrambleButton = (gcnew System::Windows::Forms::Button());
			this->progressBar = (gcnew System::Windows::Forms::ProgressBar());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// randomButton
			// 
			this->randomButton->BackColor = System::Drawing::Color::Black;
			this->randomButton->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->randomButton->ForeColor = System::Drawing::Color::White;
			this->randomButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"randomButton.Image")));
			this->randomButton->Location = System::Drawing::Point(184, 194);
			this->randomButton->Name = L"randomButton";
			this->randomButton->Size = System::Drawing::Size(64, 64);
			this->randomButton->TabIndex = 0;
			this->randomButton->Text = L"Random";
			this->randomButton->UseVisualStyleBackColor = false;
			this->randomButton->Click += gcnew System::EventHandler(this, &Form1::randomButton_Click);
			// 
			// outBox
			// 
			this->outBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->outBox->Location = System::Drawing::Point(289, 433);
			this->outBox->Name = L"outBox";
			this->outBox->Size = System::Drawing::Size(460, 20);
			this->outBox->TabIndex = 1;
			// 
			// closeButton
			// 
			this->closeButton->BackColor = System::Drawing::Color::Black;
			this->closeButton->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->closeButton->ForeColor = System::Drawing::Color::White;
			this->closeButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"closeButton.Image")));
			this->closeButton->Location = System::Drawing::Point(44, 466);
			this->closeButton->Name = L"closeButton";
			this->closeButton->Size = System::Drawing::Size(64, 64);
			this->closeButton->TabIndex = 2;
			this->closeButton->Text = L"Close";
			this->closeButton->UseVisualStyleBackColor = false;
			this->closeButton->Click += gcnew System::EventHandler(this, &Form1::closeButton_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::Transparent;
			this->label1->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(286, 417);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(73, 14);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Current Key";
			// 
			// scoreBox
			// 
			this->scoreBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->scoreBox->Location = System::Drawing::Point(289, 473);
			this->scoreBox->Name = L"scoreBox";
			this->scoreBox->Size = System::Drawing::Size(100, 20);
			this->scoreBox->TabIndex = 4;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::Color::Transparent;
			this->label2->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(286, 456);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(85, 14);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Current Score";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->BackColor = System::Drawing::Color::Transparent;
			this->label3->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->ForeColor = System::Drawing::Color::White;
			this->label3->Location = System::Drawing::Point(682, 454);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(67, 14);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Best Score";
			// 
			// bestScoreBox
			// 
			this->bestScoreBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->bestScoreBox->Location = System::Drawing::Point(649, 471);
			this->bestScoreBox->Name = L"bestScoreBox";
			this->bestScoreBox->Size = System::Drawing::Size(100, 20);
			this->bestScoreBox->TabIndex = 7;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->BackColor = System::Drawing::Color::Transparent;
			this->label4->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(694, 494);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(55, 14);
			this->label4->TabIndex = 8;
			this->label4->Text = L"Best Key";
			// 
			// bestKeyBox
			// 
			this->bestKeyBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->bestKeyBox->Location = System::Drawing::Point(289, 510);
			this->bestKeyBox->Name = L"bestKeyBox";
			this->bestKeyBox->Size = System::Drawing::Size(460, 20);
			this->bestKeyBox->TabIndex = 9;
			// 
			// solnBox
			// 
			this->solnBox->Font = (gcnew System::Drawing::Font(L"Courier New", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->solnBox->Location = System::Drawing::Point(289, 47);
			this->solnBox->MaxLength = 340;
			this->solnBox->Multiline = true;
			this->solnBox->Name = L"solnBox";
			this->solnBox->Size = System::Drawing::Size(184, 367);
			this->solnBox->TabIndex = 10;
			this->solnBox->Text = L"12345678901234567\r\n2\r\n3\r\n4\r\n5\r\n6\r\n7\r\n8\r\n9\r\n0\r\n1\r\n2\r\n3\r\n4\r\n5\r\n6\r\n7\r\n8\r\n9\r\n0";
			// 
			// gaButton
			// 
			this->gaButton->BackColor = System::Drawing::Color::Black;
			this->gaButton->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->gaButton->ForeColor = System::Drawing::Color::White;
			this->gaButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"gaButton.Image")));
			this->gaButton->Location = System::Drawing::Point(44, 194);
			this->gaButton->Name = L"gaButton";
			this->gaButton->Size = System::Drawing::Size(64, 64);
			this->gaButton->TabIndex = 11;
			this->gaButton->Text = L"Genetic";
			this->gaButton->UseVisualStyleBackColor = false;
			this->gaButton->Click += gcnew System::EventHandler(this, &Form1::gaButton_Click);
			// 
			// ibfButton
			// 
			this->ibfButton->BackColor = System::Drawing::Color::Black;
			this->ibfButton->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->ibfButton->ForeColor = System::Drawing::Color::White;
			this->ibfButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ibfButton.Image")));
			this->ibfButton->Location = System::Drawing::Point(114, 194);
			this->ibfButton->Name = L"ibfButton";
			this->ibfButton->Size = System::Drawing::Size(64, 64);
			this->ibfButton->TabIndex = 11;
			this->ibfButton->Text = L"Brute Force";
			this->ibfButton->UseVisualStyleBackColor = false;
			this->ibfButton->Click += gcnew System::EventHandler(this, &Form1::ibfButton_Click);
			// 
			// bestSolnBox
			// 
			this->bestSolnBox->Font = (gcnew System::Drawing::Font(L"Courier New", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->bestSolnBox->Location = System::Drawing::Point(565, 47);
			this->bestSolnBox->Multiline = true;
			this->bestSolnBox->Name = L"bestSolnBox";
			this->bestSolnBox->Size = System::Drawing::Size(184, 367);
			this->bestSolnBox->TabIndex = 12;
			this->bestSolnBox->Text = L"12345678901234567\r\n2\r\n3\r\n4\r\n5\r\n6\r\n7\r\n8\r\n9\r\n0\r\n1\r\n2\r\n3\r\n4\r\n5\r\n6\r\n7\r\n8\r\n9\r\n0";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->BackColor = System::Drawing::Color::Black;
			this->label6->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label6->ForeColor = System::Drawing::Color::White;
			this->label6->Location = System::Drawing::Point(289, 28);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(98, 14);
			this->label6->TabIndex = 14;
			this->label6->Text = L"Current Solution";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->BackColor = System::Drawing::Color::Transparent;
			this->label7->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label7->ForeColor = System::Drawing::Color::White;
			this->label7->Location = System::Drawing::Point(565, 28);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(80, 14);
			this->label7->TabIndex = 15;
			this->label7->Text = L"Best Solution";
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->BackColor = System::Drawing::Color::Transparent;
			this->checkBox1->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->checkBox1->ForeColor = System::Drawing::Color::White;
			this->checkBox1->Location = System::Drawing::Point(47, 353);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(54, 18);
			this->checkBox1->TabIndex = 16;
			this->checkBox1->Text = L"Loop";
			this->checkBox1->UseVisualStyleBackColor = false;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->BackColor = System::Drawing::Color::Transparent;
			this->label8->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label8->ForeColor = System::Drawing::Color::White;
			this->label8->Location = System::Drawing::Point(44, 177);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(69, 14);
			this->label8->TabIndex = 18;
			this->label8->Text = L"Algorithms";
			// 
			// statusBox
			// 
			this->statusBox->BackColor = System::Drawing::SystemColors::Window;
			this->statusBox->Font = (gcnew System::Drawing::Font(L"Courier New", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->statusBox->Location = System::Drawing::Point(479, 388);
			this->statusBox->MaxLength = 5;
			this->statusBox->Name = L"statusBox";
			this->statusBox->Size = System::Drawing::Size(80, 26);
			this->statusBox->TabIndex = 19;
			// 
			// statusLabel
			// 
			this->statusLabel->AutoSize = true;
			this->statusLabel->BackColor = System::Drawing::Color::Transparent;
			this->statusLabel->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->statusLabel->ForeColor = System::Drawing::Color::White;
			this->statusLabel->Location = System::Drawing::Point(480, 369);
			this->statusLabel->Name = L"statusLabel";
			this->statusLabel->Size = System::Drawing::Size(42, 14);
			this->statusLabel->TabIndex = 20;
			this->statusLabel->Text = L"Status";
			// 
			// zodiacDecoderButton
			// 
			this->zodiacDecoderButton->Font = (gcnew System::Drawing::Font(L"Arial", 18, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->zodiacDecoderButton->ForeColor = System::Drawing::Color::White;
			this->zodiacDecoderButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"zodiacDecoderButton.Image")));
			this->zodiacDecoderButton->Location = System::Drawing::Point(44, 47);
			this->zodiacDecoderButton->Name = L"zodiacDecoderButton";
			this->zodiacDecoderButton->Size = System::Drawing::Size(200, 100);
			this->zodiacDecoderButton->TabIndex = 21;
			this->zodiacDecoderButton->Text = L"Zodiac\r\nDecoder";
			this->zodiacDecoderButton->UseVisualStyleBackColor = true;
			// 
			// letterScrambleButton
			// 
			this->letterScrambleButton->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->letterScrambleButton->ForeColor = System::Drawing::Color::White;
			this->letterScrambleButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"letterScrambleButton.Image")));
			this->letterScrambleButton->Location = System::Drawing::Point(44, 264);
			this->letterScrambleButton->Name = L"letterScrambleButton";
			this->letterScrambleButton->Size = System::Drawing::Size(64, 64);
			this->letterScrambleButton->TabIndex = 22;
			this->letterScrambleButton->Text = L"Letter Scram";
			this->letterScrambleButton->UseVisualStyleBackColor = true;
			this->letterScrambleButton->Click += gcnew System::EventHandler(this, &Form1::letterScrambleButton_Click);
			// 
			// progressBar
			// 
			this->progressBar->BackColor = System::Drawing::Color::Black;
			this->progressBar->ForeColor = System::Drawing::Color::Red;
			this->progressBar->Location = System::Drawing::Point(470, 470);
			this->progressBar->Name = L"progressBar";
			this->progressBar->Size = System::Drawing::Size(100, 23);
			this->progressBar->Step = 1;
			this->progressBar->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBar->TabIndex = 23;
			this->progressBar->Visible = false;
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoValidate = System::Windows::Forms::AutoValidate::EnableAllowFocusChange;
			this->BackColor = System::Drawing::Color::Black;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->ClientSize = System::Drawing::Size(792, 573);
			this->Controls->Add(this->statusBox);
			this->Controls->Add(this->progressBar);
			this->Controls->Add(this->letterScrambleButton);
			this->Controls->Add(this->statusLabel);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->bestSolnBox);
			this->Controls->Add(this->ibfButton);
			this->Controls->Add(this->gaButton);
			this->Controls->Add(this->solnBox);
			this->Controls->Add(this->bestKeyBox);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->bestScoreBox);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->scoreBox);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->closeButton);
			this->Controls->Add(this->outBox);
			this->Controls->Add(this->randomButton);
			this->Controls->Add(this->zodiacDecoderButton);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximumSize = System::Drawing::Size(800, 600);
			this->MinimumSize = System::Drawing::Size(800, 600);
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Zodiac Decoder";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
//                                  RANDOM ALGORITHM
//
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

private: System::Void randomButton_Click(System::Object^  sender, System::EventArgs^  e) 
{	
		if(run_rndm == false)
		{
			run_rndm=true;
			rndm_running = true;

			DWORD threadid;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)runRNDM,(void*)myRNDM,0,&threadid);
		}
		else
			run_rndm=false;
		
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
//                        INT. BRUTE FORCE ALGORITHM
//
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

private: System::Void ibfButton_Click(System::Object^  sender, System::EventArgs^  e) 
{
		if(run_ibf == false)
		{
			run_ibf=true;
			ibf_running = true;


			DWORD threadid;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)runIBF,(void*)myIBF,0,&threadid);
		}
		else
		{
			run_ibf=false;
		}
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
//                          GENETIC ALGORITHM
//
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

private: System::Void gaButton_Click(System::Object^  sender, System::EventArgs^  e) 
{
		if(run_ga == false)
		{
			run_ga=true;
			ga_running = true;

			progressBar->Value = myGA->generation;

			myGA->start();
			DWORD threadid;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)runGA,(void*)myGA,0,&threadid);
		}
		else
		{
			run_ga=false;
			ga_running=false;
			
		}
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
//                          LETTER SCRAMBLE ALGORITHM
//
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

private: System::Void letterScrambleButton_Click(System::Object^  sender, System::EventArgs^  e) 
{	
	//RUN SCRAMBLER
	scrambler->run();

	//INCREMENT AND DISPLAY KEY NUMBER
	scrambleKeys++;
	statusLabel->Text = "Key";
	statusLabel->Refresh();
	statusBox->Clear();
	statusBox->AppendText(scrambleKeys.ToString());

	if((*KeyStream)!="")
	{
		 System::String^ myKey = gcnew System::String((*KeyStream).c_str());

		 string tempSol = scorer->SolveKey(*KeyStream);
				 
		 System::String^ solN = gcnew System::String(tempSol.c_str());

		 outBox->Clear();
		 outBox->AppendText( myKey );
		 outBox->Invalidate();	

		 solnBox->Clear();
		 solnBox->AppendText(solN);
		 solnBox->Invalidate();

		 float score = scorer->ScoreSolution(tempSol);
				
		 if ( score > bestScore )
		 {
			 bestScoreBox->Clear();
			 bestScoreBox->AppendText( score.ToString() );
			 scoreBox->Invalidate();
			 bestScore = score;

			 bestKeyBox->Clear();
			 bestKeyBox->AppendText( myKey );
			 bestKeyBox->Invalidate();

			 bestSolnBox->Clear();
			 bestSolnBox->AppendText( solN );
			 bestSolnBox->Invalidate();
		  }

		  if(score > THRESHOLD)
		  {
			if(sendToServer("zodiacdecoder.dyndns.org", "10000", ((char*)(*KeyStream).c_str())))
				{
					// Do something if it fails?			
				 }
		   }

		   scoreBox->Clear();
		   scoreBox->AppendText(score.ToString());
		   scoreBox->Invalidate();
	}

	*KeyStream = "";
}

private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) 
{
	//LOAD!
}

private: System::Void closeButton_Click(System::Object^  sender, System::EventArgs^  e) 
{
    Close();
}

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
			 			if( (!ga_running) && (!ibf_running) && (!rndm_running) )
			{
				ibfButton->Enabled = true;
				randomButton->Enabled = true;
				gaButton->Enabled = true;
				gaButton->Text = "Genetic";	
				ibfButton->Text = "I.B.F.";
				randomButton->Text = "Random";
			}

			if( (!run_ibf) && (ibf_running) )
			{
				ibfButton->Enabled = false;
				ibfButton->Text = "Cancelling";
			}

			if( (!run_rndm) && (rndm_running) )
			{
				randomButton->Enabled = false;
				randomButton->Text = "Cancelling";
			}

			if( (!run_ga) && (ga_running) )
			{
				gaButton->Enabled = false;
				gaButton->Text = "Pausing";
			}

			if( (run_ga) || (ga_running) )
			{
				ibfButton->Enabled = false;
				randomButton->Enabled = false;
				progressBar->Visible=true;
			}
			else
				progressBar->Visible=false;

			if( (run_ibf) || (ibf_running) )
			{
				gaButton->Enabled = false;
				randomButton->Enabled = false;
			}

			if( (run_rndm) || (rndm_running) )
			{
				ibfButton->Enabled = false;
				gaButton->Enabled = false;
				timer1->Interval = 1;

				statusBox->Text = randomKeys.ToString();
				statusBox->Update();
			}
			else
				timer1->Interval = 100;

			if( (run_ga) && (ga_running) )
			{
				gaButton->Text = "Cancel GA";
			}

			if( (run_ibf) && (ibf_running) )
			{
				ibfButton->Text = "Cancel IBF";
			}

			if( (run_rndm) && (rndm_running) )
			{
				randomButton->Text = "Cancel Random";
			}

			if(ga_running)
			{
				progressBar->Value = myGA->generation;
				progressBar->Update();
			}

			if( (ga_running) || (ibf_running) || (rndm_running) )
			{
				if(KeyStream->length() == 63)
				{
					 System::String^ myKey = gcnew System::String((*KeyStream).c_str());

					 string tempSol = scorer->SolveKey(*KeyStream);
					 

					 System::String^ solN = gcnew System::String(tempSol.c_str());

					 outBox->Clear();
					 outBox->AppendText( myKey );
					 outBox->Invalidate();	

					 solnBox->Clear();
					 solnBox->AppendText(solN);
					 solnBox->Invalidate();

					 float score = scorer->ScoreSolution(tempSol);

					 scoreBox->Clear();
					 scoreBox->AppendText(score.ToString());
					 scoreBox->Invalidate();

					if ( score > bestScore )
				 {
					 bestScoreBox->Clear();
					 bestScoreBox->AppendText( score.ToString() );
					 scoreBox->Invalidate();
					 bestScore = score;

					 //bestSoln = tempSol;
					 //bestKey = (*Keystream);

					 bestKeyBox->Clear();
					 bestKeyBox->AppendText( myKey );
					 bestKeyBox->Invalidate();

					 bestSolnBox->Clear();
					 bestSolnBox->AppendText( solN );
					 bestSolnBox->Invalidate();
				 }
					if(score >= THRESHOLD)
					{
						if(sendToServer("zodiacdecoder.dyndns.org", "10000", (char*)KeyStream->c_str()))
						{
								//Do something if it fails
						}
					}
				}
			}

			gaButton->Update();
			ibfButton->Update();
			randomButton->Update();
		 }
};
}