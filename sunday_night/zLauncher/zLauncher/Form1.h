#pragma once


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <math.h>

using namespace std;

namespace zLauncher {

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
	private: System::Windows::Forms::Label^  outputLabel;
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
			this->randomButton = (gcnew System::Windows::Forms::Button());
			this->outputLabel = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// randomButton
			// 
			this->randomButton->Location = System::Drawing::Point(99, 12);
			this->randomButton->Name = L"randomButton";
			this->randomButton->Size = System::Drawing::Size(75, 23);
			this->randomButton->TabIndex = 0;
			this->randomButton->Text = L"Random";
			this->randomButton->UseVisualStyleBackColor = true;
			this->randomButton->Click += gcnew System::EventHandler(this, &Form1::randomButton_Click);
			// 
			// outputLabel
			// 
			this->outputLabel->AutoSize = true;
			this->outputLabel->Location = System::Drawing::Point(13, 117);
			this->outputLabel->Name = L"outputLabel";
			this->outputLabel->Size = System::Drawing::Size(31, 13);
			this->outputLabel->TabIndex = 1;
			this->outputLabel->Text = L"Key: ";
			this->outputLabel->Click += gcnew System::EventHandler(this, &Form1::outputLabel_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 266);
			this->Controls->Add(this->outputLabel);
			this->Controls->Add(this->randomButton);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void randomButton_Click(System::Object^  sender, System::EventArgs^  e) {
				stringstream KeyStream (stringstream::in | stringstream::out);	
					
				 int SIMULATIONS=10000;
				 // to this from stream
					string key;
					// iterate for n simulations
					for(int s = 1; s <= SIMULATIONS; s++)
					{
						// send random key to stream
						for(int j = 0; j < 63; j++){
							KeyStream<<(char)(65 + (rand() % 26));
						}
						// test reading from keystream
						KeyStream>>key;
						// comment out for release
						outputLabel
						cout<<key<<" "<<(int)(((double)s / (double)SIMULATIONS) * 100)<<"%"<<endl;
						KeyStream.clear();
					}
			 }
	};
}

