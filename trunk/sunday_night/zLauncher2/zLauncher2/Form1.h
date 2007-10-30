//#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include "RandomSearch.h"
#include "IBF.h"

using namespace std;

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
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
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
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::Button^  gaButton;
	private: System::Windows::Forms::Button^  ibfButton;
	private: System::Windows::Forms::Button^  emButton;

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
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->gaButton = (gcnew System::Windows::Forms::Button());
			this->ibfButton = (gcnew System::Windows::Forms::Button());
			this->emButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// randomButton
			// 
			this->randomButton->Location = System::Drawing::Point(12, 99);
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
			// textBox1
			// 
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->textBox1->Location = System::Drawing::Point(12, 491);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 20);
			this->textBox1->TabIndex = 4;
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
			// textBox2
			// 
			this->textBox2->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->textBox2->Location = System::Drawing::Point(158, 15);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(100, 20);
			this->textBox2->TabIndex = 7;
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
			// textBox3
			// 
			this->textBox3->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->textBox3->Location = System::Drawing::Point(158, 42);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(460, 20);
			this->textBox3->TabIndex = 9;
			// 
			// textBox4
			// 
			this->textBox4->Font = (gcnew System::Drawing::Font(L"Courier New", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->textBox4->Location = System::Drawing::Point(12, 197);
			this->textBox4->Multiline = true;
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(605, 236);
			this->textBox4->TabIndex = 10;
			// 
			// gaButton
			// 
			this->gaButton->Location = System::Drawing::Point(12, 12);
			this->gaButton->Name = L"gaButton";
			this->gaButton->Size = System::Drawing::Size(75, 23);
			this->gaButton->TabIndex = 11;
			this->gaButton->Text = L"Genetic";
			this->gaButton->UseVisualStyleBackColor = true;
			// 
			// ibfButton
			// 
			this->ibfButton->Location = System::Drawing::Point(12, 70);
			this->ibfButton->Name = L"ibfButton";
			this->ibfButton->Size = System::Drawing::Size(75, 23);
			this->ibfButton->TabIndex = 11;
			this->ibfButton->Text = L"Brute Force";
			this->ibfButton->UseVisualStyleBackColor = true;
			this->ibfButton->Click += gcnew System::EventHandler(this, &Form1::ibfButton_Click);
			// 
			// emButton
			// 
			this->emButton->Location = System::Drawing::Point(12, 41);
			this->emButton->Name = L"emButton";
			this->emButton->Size = System::Drawing::Size(75, 23);
			this->emButton->TabIndex = 11;
			this->emButton->Text = L"EM";
			this->emButton->UseVisualStyleBackColor = true;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(658, 523);
			this->Controls->Add(this->emButton);
			this->Controls->Add(this->ibfButton);
			this->Controls->Add(this->gaButton);
			this->Controls->Add(this->textBox4);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->closeButton);
			this->Controls->Add(this->outBox);
			this->Controls->Add(this->randomButton);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void randomButton_Click(System::Object^  sender, System::EventArgs^  e) {
						
			 RandomSearch ^myRS = gcnew RandomSearch();
			 myRS->run();
			 outBox->Clear();
			 outBox->AppendText( myRS->getKey() );
			 outBox->Invalidate();	

			 }

	private: System::Void closeButton_Click(System::Object^  sender, System::EventArgs^  e) {

		     Close();

			 }

	private: System::Void ibfButton_Click(System::Object^  sender, System::EventArgs^  e) {

			 IBF *myIBF = new IBF();
			 myIBF->run();
			 outBox->Clear();
			 outBox->AppendText( myIBF->getKey() );
			 outBox->Invalidate();

		 }
};
}

