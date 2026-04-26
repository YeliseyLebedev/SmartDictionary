#pragma once

namespace ООП1 {

	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;

	public ref class MyForm : public Form
	{
	public:
		MyForm(void);

	private:
		Button^ button1;
		Label^ label1;
		int clickCount;

		void button1_Click(Object^ sender, EventArgs^ e);
	};
}