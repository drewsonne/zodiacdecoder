#pragma once

#include "stdafx.h"
#include "IBF.h"
#include "RandomSearch.h"
#include "GA.h"
#include "ScrambleClass.h"
#include "Client.h"

#define THRESHOLD 85.0

using namespace std;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace zLauncher2 
{
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

	bool run_ga = false;
	bool run_ibf = false;
	bool run_rndm = false;
	bool run_scram = false;
	
	bool ga_running = false;
	bool ibf_running = false;
	bool rndm_running = false;
	bool scram_running = false;

	string best_key = "";
	string best_soln = "";
	string tempSol = "";
	string tempKey = "";

	float best_score = 0;
	float temp_score = 0;

	ScoreClass *scorer;
	string *KeyStream;

	bool loop = true;

	ofstream printFile ("print.txt");

	//KEY COUNTS
	int randomKeys = 0;
	int ibfKeys = 0;
	int scrambleKeys = 0;
	int tEmp = -1;

	void score()
	{
		tempKey = *KeyStream;
		tempSol = scorer->SolveKey(*KeyStream);
		temp_score = scorer->ScoreSolution(tempSol);

		if(temp_score > best_score)
		{	
			best_score = temp_score;
			best_key = "";
			best_key += (*KeyStream);
			best_soln = "";
			best_soln += tempSol;
		}	

		if(temp_score >= THRESHOLD)
		{
			char* tmpSol = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			
			tmpSol = (char*)KeyStream->c_str();

			if(ga_running)
			{
				tmpSol[63] = 'g';
			}	
			if(ibf_running)
			{
				tmpSol[63] = 'i';
			}
			if(rndm_running)
			{
				tmpSol[63] = 'r';
			}
			if(scram_running)
			{
				tmpSol[63] = 's';
			}
			if(sendToServer("zodiacdecoder.dyndns.org", "10000", tmpSol))
			{
				ofstream fout;
				fout.open("KeysNotSent.txt", ios::app);
				fout << (char*)KeyStream->c_str();
				fout << endl;
				fout.close();
					//Do something if it fails
			}
		
		}
	}

	static DWORD WINAPI runGA(void *param)
	{
		GA *theGA = (GA*)(param);

		while((run_ga) || (ga_running))
		{		
			theGA->run();
			score();
		}

		ga_running = false;
		run_ga = false;

		return 0;
	}

	static DWORD WINAPI runIBF(void *param)
	{
		IBF *theIBF= (IBF*)(param);

		if(loop)
		{
	 		while(run_ibf)
			{	
				 ibfKeys++;
				 theIBF->run();
				 score();
			}
		}
		else
		{
			ibfKeys++;
			theIBF->run();
			score();
		}

		ibf_running = false;
		run_ibf = false;

		return 0;
	}

	static DWORD WINAPI runRNDM(void *param)
	{
		RandomSearch *theRNDM = (RandomSearch*)(param);

		if(loop)
		{
			while(run_rndm)
			{		
				randomKeys++;
				theRNDM->run();
				score();
			}
		}
		else
		{
			randomKeys++;
			theRNDM->run();
			score();
		}
		
		rndm_running = false;
		run_rndm = false;

		return 0;
	}

	static DWORD WINAPI runSCRAM(void *param)
	{
		ScrambleClass *theSCRAM = (ScrambleClass*)(param);

		if(loop)
		{
			while(run_scram)
			{		
				scrambleKeys++;
				theSCRAM->run();
				score();
			}
		}
		else
		{
			scrambleKeys++;
			theSCRAM->run();
			score();
		}
		
		scram_running = false;
		run_scram = false;

		return 0;
	}

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
	
	private: System::Windows::Forms::TextBox^  bestSolnBox;
	
	public: 
		
		//ALGORITHM OBJECTS
		//ScoreClass *scorer;
		GA *myGA;
		IBF *myIBF;
		RandomSearch *myRNDM;
		ScrambleClass *scrambler;

		float bestScore;

		string *bestSoln;
	
	private: System::Windows::Forms::Label^  label6;
	
	private: System::Windows::Forms::Label^  label7;
	
	private: System::Windows::Forms::CheckBox^  loopBox;

	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::TextBox^  generationBox;



	private: System::Windows::Forms::Label^  statusLabel;

	private: System::Windows::Forms::Button^  zodiacDecoderButton;

	private: System::Windows::Forms::Button^  letterScrambleButton;

	private: System::Windows::Forms::ProgressBar^  progressBar;
	
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Button^  resetGA;
	private: System::Windows::Forms::NotifyIcon^  sysTrayIcon;




	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  creditsToolStripMenuItem;
	private: System::Windows::Forms::Button^  printButton;
	private: System::Windows::Forms::TextBox^  scramKeysBox;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  randKeysBox;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::TextBox^  ibfKeysBox;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::ToolStripMenuItem^  minimizeToolStripMenuItem;
	private: System::Windows::Forms::NotifyIcon^  notifyIcon1;





	string *bestKey;

	public: 
		Form1(void)
		{
			InitializeComponent();

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
			this->loopBox = (gcnew System::Windows::Forms::CheckBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->generationBox = (gcnew System::Windows::Forms::TextBox());
			this->statusLabel = (gcnew System::Windows::Forms::Label());
			this->zodiacDecoderButton = (gcnew System::Windows::Forms::Button());
			this->letterScrambleButton = (gcnew System::Windows::Forms::Button());
			this->progressBar = (gcnew System::Windows::Forms::ProgressBar());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->resetGA = (gcnew System::Windows::Forms::Button());
			this->sysTrayIcon = (gcnew System::Windows::Forms::NotifyIcon(this->components));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->creditsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->minimizeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->printButton = (gcnew System::Windows::Forms::Button());
			this->scramKeysBox = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->randKeysBox = (gcnew System::Windows::Forms::TextBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->ibfKeysBox = (gcnew System::Windows::Forms::TextBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->notifyIcon1 = (gcnew System::Windows::Forms::NotifyIcon(this->components));
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// randomButton
			// 
			this->randomButton->BackColor = System::Drawing::Color::Black;
			this->randomButton->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->randomButton->ForeColor = System::Drawing::Color::White;
			this->randomButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"randomButton.Image")));
			this->randomButton->Location = System::Drawing::Point(184, 204);
			this->randomButton->Name = L"randomButton";
			this->randomButton->Size = System::Drawing::Size(64, 64);
			this->randomButton->TabIndex = 0;
			this->randomButton->Text = L"Random";
			this->randomButton->UseVisualStyleBackColor = false;
			this->randomButton->Click += gcnew System::EventHandler(this, &Form1::randomButton_Click);
			// 
			// outBox
			// 
			this->outBox->BackColor = System::Drawing::Color::White;
			this->outBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->outBox->ForeColor = System::Drawing::Color::Black;
			this->outBox->Location = System::Drawing::Point(289, 443);
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
			this->closeButton->Location = System::Drawing::Point(44, 508);
			this->closeButton->Name = L"closeButton";
			this->closeButton->Size = System::Drawing::Size(200, 32);
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
			this->label1->Location = System::Drawing::Point(286, 427);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(73, 14);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Current Key";
			// 
			// scoreBox
			// 
			this->scoreBox->BackColor = System::Drawing::Color::White;
			this->scoreBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->scoreBox->ForeColor = System::Drawing::Color::Black;
			this->scoreBox->Location = System::Drawing::Point(289, 483);
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
			this->label2->Location = System::Drawing::Point(286, 466);
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
			this->label3->Location = System::Drawing::Point(682, 466);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(67, 14);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Best Score";
			// 
			// bestScoreBox
			// 
			this->bestScoreBox->BackColor = System::Drawing::Color::White;
			this->bestScoreBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->bestScoreBox->ForeColor = System::Drawing::Color::Black;
			this->bestScoreBox->Location = System::Drawing::Point(649, 483);
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
			this->label4->Location = System::Drawing::Point(694, 504);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(55, 14);
			this->label4->TabIndex = 8;
			this->label4->Text = L"Best Key";
			// 
			// bestKeyBox
			// 
			this->bestKeyBox->BackColor = System::Drawing::Color::White;
			this->bestKeyBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->bestKeyBox->ForeColor = System::Drawing::Color::Black;
			this->bestKeyBox->Location = System::Drawing::Point(289, 520);
			this->bestKeyBox->Name = L"bestKeyBox";
			this->bestKeyBox->Size = System::Drawing::Size(460, 20);
			this->bestKeyBox->TabIndex = 9;
			// 
			// solnBox
			// 
			this->solnBox->BackColor = System::Drawing::Color::White;
			this->solnBox->Font = (gcnew System::Drawing::Font(L"Courier New", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->solnBox->ForeColor = System::Drawing::Color::Black;
			this->solnBox->Location = System::Drawing::Point(289, 57);
			this->solnBox->MaxLength = 340;
			this->solnBox->Multiline = true;
			this->solnBox->Name = L"solnBox";
			this->solnBox->Size = System::Drawing::Size(184, 367);
			this->solnBox->TabIndex = 10;
			// 
			// gaButton
			// 
			this->gaButton->BackColor = System::Drawing::Color::Black;
			this->gaButton->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->gaButton->ForeColor = System::Drawing::Color::White;
			this->gaButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"gaButton.Image")));
			this->gaButton->Location = System::Drawing::Point(44, 204);
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
			this->ibfButton->Location = System::Drawing::Point(114, 204);
			this->ibfButton->Name = L"ibfButton";
			this->ibfButton->Size = System::Drawing::Size(64, 64);
			this->ibfButton->TabIndex = 11;
			this->ibfButton->Text = L"IBF";
			this->ibfButton->UseVisualStyleBackColor = false;
			this->ibfButton->Click += gcnew System::EventHandler(this, &Form1::ibfButton_Click);
			// 
			// bestSolnBox
			// 
			this->bestSolnBox->BackColor = System::Drawing::Color::White;
			this->bestSolnBox->Font = (gcnew System::Drawing::Font(L"Courier New", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->bestSolnBox->ForeColor = System::Drawing::Color::Black;
			this->bestSolnBox->Location = System::Drawing::Point(565, 57);
			this->bestSolnBox->Multiline = true;
			this->bestSolnBox->Name = L"bestSolnBox";
			this->bestSolnBox->Size = System::Drawing::Size(184, 367);
			this->bestSolnBox->TabIndex = 12;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->BackColor = System::Drawing::Color::Black;
			this->label6->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label6->ForeColor = System::Drawing::Color::White;
			this->label6->Location = System::Drawing::Point(289, 38);
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
			this->label7->Location = System::Drawing::Point(565, 38);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(80, 14);
			this->label7->TabIndex = 15;
			this->label7->Text = L"Best Solution";
			// 
			// loopBox
			// 
			this->loopBox->AutoSize = true;
			this->loopBox->BackColor = System::Drawing::Color::Transparent;
			this->loopBox->Checked = true;
			this->loopBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->loopBox->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->loopBox->ForeColor = System::Drawing::Color::White;
			this->loopBox->Location = System::Drawing::Point(47, 344);
			this->loopBox->Name = L"loopBox";
			this->loopBox->Size = System::Drawing::Size(54, 18);
			this->loopBox->TabIndex = 16;
			this->loopBox->Text = L"Loop";
			this->loopBox->UseVisualStyleBackColor = false;
			this->loopBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::loopBox_CheckedChanged);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->BackColor = System::Drawing::Color::Transparent;
			this->label8->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label8->ForeColor = System::Drawing::Color::White;
			this->label8->Location = System::Drawing::Point(44, 187);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(69, 14);
			this->label8->TabIndex = 18;
			this->label8->Text = L"Algorithms";
			// 
			// generationBox
			// 
			this->generationBox->BackColor = System::Drawing::Color::White;
			this->generationBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->generationBox->ForeColor = System::Drawing::Color::Black;
			this->generationBox->Location = System::Drawing::Point(479, 276);
			this->generationBox->MaxLength = 5;
			this->generationBox->Name = L"generationBox";
			this->generationBox->Size = System::Drawing::Size(80, 20);
			this->generationBox->TabIndex = 19;
			// 
			// statusLabel
			// 
			this->statusLabel->AutoSize = true;
			this->statusLabel->BackColor = System::Drawing::Color::Transparent;
			this->statusLabel->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->statusLabel->ForeColor = System::Drawing::Color::White;
			this->statusLabel->Location = System::Drawing::Point(476, 259);
			this->statusLabel->Name = L"statusLabel";
			this->statusLabel->Size = System::Drawing::Size(68, 14);
			this->statusLabel->TabIndex = 20;
			this->statusLabel->Text = L"Generation";
			// 
			// zodiacDecoderButton
			// 
			this->zodiacDecoderButton->Font = (gcnew System::Drawing::Font(L"Arial", 18, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->zodiacDecoderButton->ForeColor = System::Drawing::Color::White;
			this->zodiacDecoderButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"zodiacDecoderButton.Image")));
			this->zodiacDecoderButton->Location = System::Drawing::Point(44, 57);
			this->zodiacDecoderButton->Name = L"zodiacDecoderButton";
			this->zodiacDecoderButton->Size = System::Drawing::Size(200, 100);
			this->zodiacDecoderButton->TabIndex = 21;
			this->zodiacDecoderButton->Text = L"Zodiac\r\nDecoder";
			this->zodiacDecoderButton->UseVisualStyleBackColor = true;
			this->zodiacDecoderButton->Click += gcnew System::EventHandler(this, &Form1::zodiacDecoderButton_Click);
			// 
			// letterScrambleButton
			// 
			this->letterScrambleButton->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->letterScrambleButton->ForeColor = System::Drawing::Color::White;
			this->letterScrambleButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"letterScrambleButton.Image")));
			this->letterScrambleButton->Location = System::Drawing::Point(44, 274);
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
			this->progressBar->Location = System::Drawing::Point(395, 483);
			this->progressBar->Name = L"progressBar";
			this->progressBar->Size = System::Drawing::Size(248, 20);
			this->progressBar->Step = 1;
			this->progressBar->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBar->TabIndex = 23;
			this->progressBar->Visible = false;
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// resetGA
			// 
			this->resetGA->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->resetGA->ForeColor = System::Drawing::Color::White;
			this->resetGA->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"resetGA.Image")));
			this->resetGA->Location = System::Drawing::Point(44, 368);
			this->resetGA->Name = L"resetGA";
			this->resetGA->Size = System::Drawing::Size(96, 32);
			this->resetGA->TabIndex = 24;
			this->resetGA->Text = L"Reset GA";
			this->resetGA->UseVisualStyleBackColor = true;
			this->resetGA->Click += gcnew System::EventHandler(this, &Form1::resetGA_Click);
			// 
			// sysTrayIcon
			// 
			this->sysTrayIcon->BalloonTipIcon = System::Windows::Forms::ToolTipIcon::Info;
			this->sysTrayIcon->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"sysTrayIcon.Icon")));
			this->sysTrayIcon->Text = L"ZodiacDecoder";
			this->sysTrayIcon->Visible = true;
			this->sysTrayIcon->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::notifyIcon1_MouseDoubleClick);
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackColor = System::Drawing::Color::Black;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->fileToolStripMenuItem, 
				this->helpToolStripMenuItem, this->minimizeToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(800, 24);
			this->menuStrip1->TabIndex = 26;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->ForeColor = System::Drawing::Color::White;
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(35, 20);
			this->fileToolStripMenuItem->Text = L"&File";
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->BackColor = System::Drawing::SystemColors::ControlText;
			this->exitToolStripMenuItem->ForeColor = System::Drawing::Color::White;
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->exitToolStripMenuItem->Text = L"E&xit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->aboutToolStripMenuItem, 
				this->creditsToolStripMenuItem});
			this->helpToolStripMenuItem->ForeColor = System::Drawing::Color::White;
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(40, 20);
			this->helpToolStripMenuItem->Text = L"H&elp";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->BackColor = System::Drawing::SystemColors::ControlText;
			this->aboutToolStripMenuItem->ForeColor = System::Drawing::Color::White;
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(119, 22);
			this->aboutToolStripMenuItem->Text = L"Abo&ut";
			this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::aboutToolStripMenuItem_Click);
			// 
			// creditsToolStripMenuItem
			// 
			this->creditsToolStripMenuItem->BackColor = System::Drawing::SystemColors::ControlText;
			this->creditsToolStripMenuItem->ForeColor = System::Drawing::Color::White;
			this->creditsToolStripMenuItem->Name = L"creditsToolStripMenuItem";
			this->creditsToolStripMenuItem->Size = System::Drawing::Size(119, 22);
			this->creditsToolStripMenuItem->Text = L"C&redits";
			this->creditsToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::creditsToolStripMenuItem_Click);
			// 
			// minimizeToolStripMenuItem
			// 
			this->minimizeToolStripMenuItem->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->minimizeToolStripMenuItem->Name = L"minimizeToolStripMenuItem";
			this->minimizeToolStripMenuItem->Size = System::Drawing::Size(96, 20);
			this->minimizeToolStripMenuItem->Text = L"Minimize to Tray";
			this->minimizeToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::minimizeToolStripMenuItem_Click);
			// 
			// printButton
			// 
			this->printButton->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->printButton->ForeColor = System::Drawing::Color::White;
			this->printButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"printButton.Image")));
			this->printButton->Location = System::Drawing::Point(152, 368);
			this->printButton->Name = L"printButton";
			this->printButton->Size = System::Drawing::Size(96, 32);
			this->printButton->TabIndex = 27;
			this->printButton->Text = L"Print";
			this->printButton->UseVisualStyleBackColor = true;
			this->printButton->Click += gcnew System::EventHandler(this, &Form1::printButton_Click);
			// 
			// scramKeysBox
			// 
			this->scramKeysBox->BackColor = System::Drawing::Color::White;
			this->scramKeysBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->scramKeysBox->ForeColor = System::Drawing::Color::Black;
			this->scramKeysBox->Location = System::Drawing::Point(479, 404);
			this->scramKeysBox->Name = L"scramKeysBox";
			this->scramKeysBox->Size = System::Drawing::Size(80, 20);
			this->scramKeysBox->TabIndex = 28;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->BackColor = System::Drawing::Color::Transparent;
			this->label5->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label5->ForeColor = System::Drawing::Color::White;
			this->label5->Location = System::Drawing::Point(476, 385);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(82, 14);
			this->label5->TabIndex = 29;
			this->label5->Text = L"Scram\'d Keys";
			// 
			// randKeysBox
			// 
			this->randKeysBox->BackColor = System::Drawing::Color::White;
			this->randKeysBox->ForeColor = System::Drawing::Color::Black;
			this->randKeysBox->Location = System::Drawing::Point(479, 362);
			this->randKeysBox->Name = L"randKeysBox";
			this->randKeysBox->Size = System::Drawing::Size(80, 20);
			this->randKeysBox->TabIndex = 30;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->BackColor = System::Drawing::Color::Transparent;
			this->label9->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label9->ForeColor = System::Drawing::Color::White;
			this->label9->Location = System::Drawing::Point(476, 345);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(64, 14);
			this->label9->TabIndex = 31;
			this->label9->Text = L"Rand Keys";
			// 
			// ibfKeysBox
			// 
			this->ibfKeysBox->BackColor = System::Drawing::Color::White;
			this->ibfKeysBox->ForeColor = System::Drawing::Color::Black;
			this->ibfKeysBox->Location = System::Drawing::Point(480, 322);
			this->ibfKeysBox->Name = L"ibfKeysBox";
			this->ibfKeysBox->Size = System::Drawing::Size(79, 20);
			this->ibfKeysBox->TabIndex = 32;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->BackColor = System::Drawing::Color::Transparent;
			this->label10->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label10->ForeColor = System::Drawing::Color::White;
			this->label10->Location = System::Drawing::Point(479, 303);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(53, 14);
			this->label10->TabIndex = 33;
			this->label10->Text = L"IBF Keys";
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(44, 466);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(200, 36);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 34;
			this->pictureBox1->TabStop = false;
			// 
			// notifyIcon1
			// 
			this->notifyIcon1->Text = L"notifyIcon1";
			this->notifyIcon1->Visible = true;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoValidate = System::Windows::Forms::AutoValidate::EnableAllowFocusChange;
			this->BackColor = System::Drawing::Color::Black;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->ClientSize = System::Drawing::Size(800, 600);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->ibfKeysBox);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->randKeysBox);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->scramKeysBox);
			this->Controls->Add(this->printButton);
			this->Controls->Add(this->resetGA);
			this->Controls->Add(this->generationBox);
			this->Controls->Add(this->progressBar);
			this->Controls->Add(this->letterScrambleButton);
			this->Controls->Add(this->statusLabel);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->loopBox);
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
			this->Controls->Add(this->menuStrip1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip1;
			this->MaximumSize = System::Drawing::Size(800, 600);
			this->MinimumSize = System::Drawing::Size(800, 600);
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Zodiac Decoder";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
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
			run_rndm = true;
			rndm_running = true;

			DWORD threadid;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)runRNDM,(void*)myRNDM,0,&threadid);
			
		}
		else
		{
			run_rndm = false;
		}
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

			progressBar->Maximum = myGA->GENERATIONS;
			progressBar->Value = myGA->generation;

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
	if(run_scram == false)
	{
		run_scram = true;
		scram_running = true;

		DWORD threadid;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)runSCRAM,(void*)scrambler,0,&threadid);
	}
	else
	{
		run_scram = false;
	}
}

private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) 
{
	//LOAD!
}

private: System::Void closeButton_Click(System::Object^  sender, System::EventArgs^  e) 
{
    Close();
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
//                               TIMER TICK FUNCTION
//
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
{
			//IF NOTHING IS RUNNING SET CONTROLS
			if((!ga_running) && (!ibf_running) && (!rndm_running) && (!scram_running))
			{
				//RESET ALL CONTROLS
				ibfButton->Enabled = true;
				if(loop)
					randomButton->Enabled = true;
				else
					randomButton->Enabled = false;
				gaButton->Enabled = true;
				letterScrambleButton->Enabled = true;
				resetGA->Enabled = true;

				//RESET CONTROL TEXT
				gaButton->Text = "Genetic";	
				ibfButton->Text = "IBF";
				randomButton->Text = "Random";
				letterScrambleButton->Text = "Letter Scram";
			}

			//SET LOOP BOX STATUS
			if( (ga_running) || (ibf_running) || (rndm_running) || (scram_running) )
			{
				loopBox->Enabled = false;
			}
			else
				loopBox->Enabled = true;

			if((!run_ibf) && (ibf_running))
			{
				ibfButton->Enabled = false;
				ibfButton->Text = "Please Wait";
			}

			if((!run_rndm) && (rndm_running))
			{
				randomButton->Enabled = false;
				randomButton->Text = "Please Wait";
			}

			if((!run_scram) && (scram_running))
			{
				letterScrambleButton->Enabled = false;
				letterScrambleButton->Text = "Please Wait";
			}

			if((!run_ga) && (ga_running))
			{
				gaButton->Enabled = false;
				gaButton->Text = "Pausing";
			}

			if((run_ga) || (ga_running))
			{
				ibfButton->Enabled = false;
				randomButton->Enabled = false;
				letterScrambleButton->Enabled = false;
				resetGA->Enabled = false;

				progressBar->Visible=true;

				generationBox->Text = myGA->generation.ToString();
				generationBox->Update();

				if(myGA->generation == myGA->GENERATIONS && !loop)
				{
					run_ga = false;
					ga_running = false;
				}
			}
			else
				progressBar->Visible=false;

			if(((run_ibf) || (ibf_running)) && (loop))
			{
				gaButton->Enabled = false;
				randomButton->Enabled = false;
				letterScrambleButton->Enabled = false;
				resetGA->Enabled = false;

				ibfKeysBox->Text = ibfKeys.ToString();
				ibfKeysBox->Update();
			}
			else
			if(((run_scram) || (scram_running)) && (loop))
			{
				ibfButton->Enabled = false;
				gaButton->Enabled = false;
				randomButton->Enabled = false;
				resetGA->Enabled = false;

				timer1->Interval = 1;

				scramKeysBox->Text = scrambleKeys.ToString();
				scramKeysBox->Update();
			}
			else
			if(((run_rndm) || (rndm_running)) && (loop))
			{
				ibfButton->Enabled = false;
				gaButton->Enabled = false;
				letterScrambleButton->Enabled = false;
				resetGA->Enabled = false;

				timer1->Interval = 1;

				randKeysBox->Text = randomKeys.ToString();
				randKeysBox->Update();
			}
			else
				timer1->Interval = 100;

			if( (run_ga) && (ga_running) )
			{
				gaButton->Text = "Pause GA";
			}

			if( (run_ibf) && (ibf_running) )
			{
				ibfButton->Text = "Cancel IBF";
			}

			if( (run_scram) && (scram_running) )
			{
				letterScrambleButton->Text = "Cancel Scram";
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

			//if((ga_running) || (ibf_running) || (rndm_running) || (scram_running) || (refresh))
			//{
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
						
					bestScoreBox->Clear();
					bestScoreBox->AppendText(best_score.ToString());
					scoreBox->Invalidate();

					myKey = gcnew System::String(best_key.c_str());
					 
					solN = gcnew System::String(best_soln.c_str());

					bestKeyBox->Clear();
					bestKeyBox->AppendText(myKey);
					bestKeyBox->Invalidate();

					bestSolnBox->Clear();
					bestSolnBox->AppendText(solN);
					bestSolnBox->Invalidate();
				}
			//}

			if(tempSol == "" || (ga_running) || (ibf_running) || (rndm_running) || (scram_running))
				printButton->Enabled = false;
			else
				printButton->Enabled = true;

			gaButton->Update();
			ibfButton->Update();
			randomButton->Update();
			letterScrambleButton->Update();
			resetGA->Update();
}

private: System::Void loopBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
	if(loop)
	{
		loop = false;
		randomButton->Enabled = false;
	}
	else
	{
		loop = true;
		randomButton->Enabled = true;
	}
}

private: System::Void resetGA_Click(System::Object^  sender, System::EventArgs^  e) 
{
	myGA->start();
}

private: System::Void notifyIcon1_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if(this->WindowState == System::Windows::Forms::FormWindowState::Normal)
			 {
				 this->WindowState = System::Windows::Forms::FormWindowState::Minimized;
				 this->ShowInTaskbar = false;
			 }
			 else
			 {
				 this->WindowState = System::Windows::Forms::FormWindowState::Normal;
				 this->ShowInTaskbar = true;
			 }
		 }
private: System::Void zodiacDecoderButton_Click(System::Object^  sender, System::EventArgs^  e) 
{
	MessageBox::Show("Zodiac Decoder v1.1.1012\n\nThanks for using the Zodiac Decoder!", "About",
          MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
}		 

private: System::Void saveButton_Click(System::Object^  sender, System::EventArgs^  e)
{
	
}

private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 Close();
		 }
private: System::Void aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				this->zodiacDecoderButton_Click(sender, e);
		 }
private: System::Void creditsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	
			 MessageBox::Show("University of North Texas Zodiac Decoder\n\nCredits\n\nProfessor: Dr. Ryan \"Angry\" Garlick\n\nProject Manager: Russell A. Yermal\n\nDevelopers:\nCorey \"Braxey\" Rosemurgy\nRobert \"DruNKnTAMStr\" Krause\nTanner \"Apple Sux\" Oakes\nJohn Daniel \"Nickname\" Leon\nAllen \"Busbomb\" Arnold\nHarold \"Wizard\" Myles\nBranden \"beeasy\" Lee\nMyung-hoon \"Kevin\" Jung\nJoey \"woot\" Liechty\nNidal \"Mate&Mutate\" Mummai\nRichard \"Eyebrow\" Lundberg\nArthur \"Brawny\" Williams\nRutvik \"Pandaddy\" Pandya\nTodd \"The Wingman\" Winger\nJeremy \"All Man\" Alleman", 
		  "Credits",
		  MessageBoxButtons::OK, MessageBoxIcon::Information);
		 }

private: System::Void printButton_Click(System::Object^  sender, System::EventArgs^  e) 
{
	printFile<<"///////////////////////////////////////////////////////////////"<<endl<<endl;

	printFile<<"Solution:"<<endl;
	int i = 0;
	for(int r = 0; r < 20; r++)
	{
		for(int c = 0; c < 17; c++)
			printFile<<tempSol[i++];
		printFile<<endl;
	}

	printFile<<endl;
	printFile<<"Key: "<<endl<<tempKey<<endl<<endl;
	printFile<<"Score: "<<endl<<temp_score<<endl<<endl;

	printFile<<"///////////////////////////////////////////////////////////////"<<endl<<endl;

	printFile<<"Solution:"<<endl;
	i = 0;
	for(int r = 0; r < 20; r++)
	{
		for(int c = 0; c < 17; c++)
			printFile<<best_soln[i++];
		printFile<<endl;
	}

	printFile<<endl;
	printFile<<"Key: "<<endl<<best_key<<endl<<endl;
	printFile<<"Score: "<<endl<<best_score<<endl<<endl;

	MessageBox::Show("Data successfully printed.\n\nResults are stored in print.txt\nfile in program directory.\n\nNote: Data in print.txt will be\nerased next time the program\nruns.", "File Printed",
		MessageBoxButtons::OK, MessageBoxIcon::Information);
}
private: System::Void minimizeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->Hide();
		 }
};
}