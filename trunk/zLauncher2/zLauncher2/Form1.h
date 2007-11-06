#pragma once
//#pragma warning  (disable : 3867 )
#include "stdafx.h"
#include "IBF.h"
#include "RandomSearch.h"
#include "GA.h"

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

	/*class wrapper
	{
		public:
			stringstream *keyStream;
			System::Windows::Forms::TextBox outBox;

			wrapper(stringstream *KeyStream, System::Windows::Forms::TextBox^ theOutBox)
			{
				keyStream = KeyStream;
				outBox = &theOutBox;
			}

			void WINAPI drawKeys( LPVOID pParam )
			{
				string key;
				(*keyStream) >> key;
				if(key!="")
				{
					 outBox->Clear();
					 System::String ^myKey = gcnew System::String(key.c_str());
					 outBox->AppendText( myKey );
					 outBox->Invalidate();	
				}
			}

	}*/



	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:

		string *KeyStream;
	private: System::Windows::Forms::TextBox^  bestSolnBox;
	public: 

			 System::Windows::Forms::Label^  label5;
			 ScoreClass *scorer;
			 GA *myGA;
			 float bestScore;
			 string *bestSoln;
			 string *bestKey;


		/*void WINAPI drawKeys( LPVOID pParam )
		{
			string key;
			(*KeyStream) >> key;
			if(key!="")
			{
				 outBox->Clear();
				 System::String ^myKey = gcnew System::String(key.c_str());
				 outBox->AppendText( myKey );
				 outBox->Invalidate();	
			}
		}*/

		/*static void WINAPI updateKeys(LPVOID lpParam)
		{
			string key;
			KeyStream >> key;
			if(key!="")
			{
				 outbox.Clear();
				 System::String^ myKey = gcnew System::String(key.c_str());
				 outbox.AppendText( myKey );
				 outbox.Invalidate();	
				 lpParam = &myKey;
			}		
			//return (0);
		}*/

	public: 
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
//			wrapper *drawWrapper = new wrapper(KeyStream, this->outBox);

			/*int Data = 0;
			HANDLE DrawThread;
			
			DrawThread = CreateThread(NULL,0,updateKeys,this->outBox->Text,0,NULL);*/
			bestScore = 0;
			KeyStream = new string();
			scorer = new ScoreClass;
			myGA = new GA(KeyStream,scorer);
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
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// randomButton
			// 
			this->randomButton->Location = System::Drawing::Point(12, 81);
			this->randomButton->Name = L"randomButton";
			this->randomButton->Size = System::Drawing::Size(75, 23);
			this->randomButton->TabIndex = 0;
			this->randomButton->Text = L"Random";
			this->randomButton->UseVisualStyleBackColor = true;
			this->randomButton->Click += gcnew System::EventHandler(this, &Form1::randomButton_Click);
			// 
			// outBox
			// 
			this->outBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->outBox->Location = System::Drawing::Point(12, 452);
			this->outBox->Name = L"outBox";
			this->outBox->Size = System::Drawing::Size(460, 20);
			this->outBox->TabIndex = 1;
			// 
			// closeButton
			// 
			this->closeButton->Location = System::Drawing::Point(12, 168);
			this->closeButton->Name = L"closeButton";
			this->closeButton->Size = System::Drawing::Size(75, 23);
			this->closeButton->TabIndex = 2;
			this->closeButton->Text = L"Close";
			this->closeButton->UseVisualStyleBackColor = true;
			this->closeButton->Click += gcnew System::EventHandler(this, &Form1::closeButton_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 436);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(25, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Key";
			// 
			// scoreBox
			// 
			this->scoreBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->scoreBox->Location = System::Drawing::Point(12, 491);
			this->scoreBox->Name = L"scoreBox";
			this->scoreBox->Size = System::Drawing::Size(100, 20);
			this->scoreBox->TabIndex = 4;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 475);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(35, 13);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Score";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(93, 15);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(59, 13);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Best Score";
			// 
			// bestScoreBox
			// 
			this->bestScoreBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->bestScoreBox->Location = System::Drawing::Point(158, 15);
			this->bestScoreBox->Name = L"bestScoreBox";
			this->bestScoreBox->Size = System::Drawing::Size(100, 20);
			this->bestScoreBox->TabIndex = 7;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(93, 42);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(49, 13);
			this->label4->TabIndex = 8;
			this->label4->Text = L"Best Key";
			// 
			// bestKeyBox
			// 
			this->bestKeyBox->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->bestKeyBox->Location = System::Drawing::Point(158, 42);
			this->bestKeyBox->Name = L"bestKeyBox";
			this->bestKeyBox->Size = System::Drawing::Size(460, 20);
			this->bestKeyBox->TabIndex = 9;
			// 
			// solnBox
			// 
			this->solnBox->Font = (gcnew System::Drawing::Font(L"Courier New", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->solnBox->Location = System::Drawing::Point(12, 197);
			this->solnBox->MaxLength = 340;
			this->solnBox->Multiline = true;
			this->solnBox->Name = L"solnBox";
			this->solnBox->Size = System::Drawing::Size(605, 236);
			this->solnBox->TabIndex = 10;
			// 
			// gaButton
			// 
			this->gaButton->Location = System::Drawing::Point(12, 12);
			this->gaButton->Name = L"gaButton";
			this->gaButton->Size = System::Drawing::Size(75, 23);
			this->gaButton->TabIndex = 11;
			this->gaButton->Text = L"Genetic";
			this->gaButton->UseVisualStyleBackColor = true;
			this->gaButton->Click += gcnew System::EventHandler(this, &Form1::gaButton_Click);
			// 
			// ibfButton
			// 
			this->ibfButton->Location = System::Drawing::Point(12, 47);
			this->ibfButton->Name = L"ibfButton";
			this->ibfButton->Size = System::Drawing::Size(75, 23);
			this->ibfButton->TabIndex = 11;
			this->ibfButton->Text = L"Brute Force";
			this->ibfButton->UseVisualStyleBackColor = true;
			this->ibfButton->Click += gcnew System::EventHandler(this, &Form1::ibfButton_Click);
			// 
			// bestSolnBox
			// 
			this->bestSolnBox->Location = System::Drawing::Point(158, 69);
			this->bestSolnBox->Multiline = true;
			this->bestSolnBox->Name = L"bestSolnBox";
			this->bestSolnBox->Size = System::Drawing::Size(459, 122);
			this->bestSolnBox->TabIndex = 12;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(83, 120);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(69, 13);
			this->label5->TabIndex = 13;
			this->label5->Text = L"Best Solution";
			this->label5->Click += gcnew System::EventHandler(this, &Form1::label5_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(658, 523);
			this->Controls->Add(this->label5);
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
			this->Name = L"Form1";
			this->Text = L"Yoo r cracking zodiac";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void randomButton_Click(System::Object^  sender, System::EventArgs^  e) {
						
			RandomSearch *myRS = new RandomSearch(KeyStream);

			myRS->run();
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

					 //bestSoln = tempSol;
					 //bestKey = (*Keystream);

					 bestKeyBox->Clear();
					 bestKeyBox->AppendText( myKey );
					 bestKeyBox->Invalidate();

					 bestSolnBox->Clear();
					 bestSolnBox->AppendText( solN );
					 bestSolnBox->Invalidate();
				 }

				 scoreBox->Clear();
				 scoreBox->AppendText(score.ToString());
				 scoreBox->Invalidate();
			}
			*KeyStream = "";

	}

	private: System::Void closeButton_Click(System::Object^  sender, System::EventArgs^  e) {

		     Close();

			 }

	private: System::Void ibfButton_Click(System::Object^  sender, System::EventArgs^  e) 
	{
			IBF *myIBF = new IBF(KeyStream);
			myIBF->run();
			if(*KeyStream!="")
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

					 //bestSoln = tempSol;
					 //bestKey = (*Keystream);

					 bestKeyBox->Clear();
					 bestKeyBox->AppendText( myKey );
					 bestKeyBox->Invalidate();

					 bestSolnBox->Clear();
					 bestSolnBox->AppendText( solN );
					 bestSolnBox->Invalidate();
				 }
				 scoreBox->Clear();
				 scoreBox->AppendText(score.ToString());
				 scoreBox->Invalidate();
			}		
			*KeyStream = "";
	}
private: System::Void emButton_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label5_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
		 }
	private: System::Void gaButton_Click(System::Object^  sender, System::EventArgs^  e) 
	{
			string key;
			myGA->start();
			while(myGA->generation < myGA->GENERATIONS)
			{
				myGA->run();
//				textBox1->Clear();
//				textBox1->AppendText(myGA->generation.ToString());
//				textBox1->Invalidate();
				if(*KeyStream!="")
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

					 //bestSoln = tempSol;
					 //bestKey = (*Keystream);

					 bestKeyBox->Clear();
					 bestKeyBox->AppendText( myKey );
					 bestKeyBox->Invalidate();

					 bestSolnBox->Clear();
					 bestSolnBox->AppendText( solN );
					 bestSolnBox->Invalidate();
				 }

					 scoreBox->Clear();
					 scoreBox->AppendText(score.ToString());
					 scoreBox->Invalidate();
				}
				
				*KeyStream = "";
			}
	}
};
}

