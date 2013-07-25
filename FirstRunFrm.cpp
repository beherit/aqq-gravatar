//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "FirstRunFrm.h"
#include <inifiles.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "LMDPNGImage"
#pragma resource "*.dfm"
TFirstRunForm *FirstRunForm;
//---------------------------------------------------------------------------
__declspec(dllimport)UnicodeString GetPluginUserDir();
__declspec(dllimport)void GetAccountList_FR();
__declspec(dllimport)void RefreshAvatars();
__declspec(dllimport)void RefreshSettings();
//---------------------------------------------------------------------------
UnicodeString fDir;
//---------------------------------------------------------------------------
__fastcall TFirstRunForm::TFirstRunForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::SaveButtonClick(TObject *Sender)
{
  aSaveSettings->Execute();
  RefreshSettings();
  RefreshAvatars();
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::NextButtonClick(TObject *Sender)
{
  if(Panel1->Visible==true)
  {
	Panel1->Visible=false;
	Panel2->Visible=true;
	PrevButton->Enabled=true;
  }
  else if(Panel2->Visible==true)
  {
	Panel2->Visible=false;
	Panel3->Visible=true;
  }
  else if(Panel3->Visible==true)
  {
	Panel3->Visible=false;
	Panel4->Visible=true;
  }
  else if(Panel4->Visible==true)
  {
	Panel4->Visible=false;
	Panel5->Visible=true;
  }
  else if(Panel5->Visible==true)
  {
	Panel5->Visible=false;
	Panel6->Visible=true;
	NextButton->Visible=false;
	SaveButton->Visible=true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::PrevButtonClick(TObject *Sender)
{
  if(Panel2->Visible==true)
  {
	Panel2->Visible=false;
	Panel1->Visible=true;
	PrevButton->Enabled=false;
  }
  else if(Panel3->Visible==true)
  {
	Panel3->Visible=false;
	Panel2->Visible=true;
  }
  else if(Panel4->Visible==true)
  {
	Panel4->Visible=false;
	Panel3->Visible=true;
  }
  else if(Panel5->Visible==true)
  {
	Panel5->Visible=false;
	Panel4->Visible=true;
  }
  else if(Panel6->Visible==true)
  {
	Panel6->Visible=false;
	Panel5->Visible=true;
	NextButton->Visible=true;
	SaveButton->Visible=false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::GetMode1RadioButtonClick(TObject *Sender)
{
  if(EmailEdit->Text=="Wpisz tutaj adres e-mail")
   EmailEdit->Text="";
  EmailEdit->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::GetMode0RadioButtonClick(TObject *Sender)
{
  if(EmailEdit->Text=="")
   EmailEdit->Text="Wpisz tutaj adres e-mail";
  EmailEdit->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::DefaultAvatarRadioButton0Click(TObject *Sender)
{
  DefaultAvatarBevel0->Visible=true;
  DefaultAvatarBevel1->Visible=false;
  DefaultAvatarBevel2->Visible=false;
  DefaultAvatarBevel3->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::DefaultAvatarRadioButton1Click(TObject *Sender)
{
  DefaultAvatarBevel0->Visible=false;
  DefaultAvatarBevel1->Visible=true;
  DefaultAvatarBevel2->Visible=false;
  DefaultAvatarBevel3->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::DefaultAvatarRadioButton2Click(TObject *Sender)
{
  DefaultAvatarBevel0->Visible=false;
  DefaultAvatarBevel1->Visible=false;
  DefaultAvatarBevel2->Visible=true;
  DefaultAvatarBevel3->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::DefaultAvatarRadioButton3Click(TObject *Sender)
{
  DefaultAvatarBevel0->Visible=false;
  DefaultAvatarBevel1->Visible=false;
  DefaultAvatarBevel2->Visible=false;
  DefaultAvatarBevel3->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::DefaultAvatarImage0Click(TObject *Sender)
{
  DefaultAvatarRadioButton0->Checked=true;

  DefaultAvatarBevel0->Visible=true;
  DefaultAvatarBevel1->Visible=false;
  DefaultAvatarBevel2->Visible=false;
  DefaultAvatarBevel3->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::DefaultAvatarImage1Click(TObject *Sender)
{
  DefaultAvatarRadioButton1->Checked=true;

  DefaultAvatarBevel0->Visible=false;
  DefaultAvatarBevel1->Visible=true;
  DefaultAvatarBevel2->Visible=false;
  DefaultAvatarBevel3->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::DefaultAvatarImage2Click(TObject *Sender)
{
  DefaultAvatarRadioButton2->Checked=true;

  DefaultAvatarBevel0->Visible=false;
  DefaultAvatarBevel1->Visible=false;
  DefaultAvatarBevel2->Visible=true;
  DefaultAvatarBevel3->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::DefaultAvatarImage3Click(TObject *Sender)
{
  DefaultAvatarRadioButton3->Checked=true;

  DefaultAvatarBevel0->Visible=false;
  DefaultAvatarBevel1->Visible=false;
  DefaultAvatarBevel2->Visible=false;
  DefaultAvatarBevel3->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::FormShow(TObject *Sender)
{
  GetAccountList_FR();
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::AccountsMode1RadioButtonClick(TObject *Sender)
{
  AccountsCheckListBox->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::AccountsMode0RadioButtonClick(TObject *Sender)
{
  AccountsCheckListBox->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::aSaveSettingsExecute(TObject *Sender)
{
  fDir = GetPluginUserDir();

  TIniFile *Ini = new TIniFile(fDir + "\\\\Gravatar\\\\Settings.ini");
  if(GetMode0RadioButton->Checked==true)
   Ini->WriteInteger("Settings","GetMode",0);
  else if(GetMode1RadioButton->Checked==true)
   Ini->WriteInteger("Settings","GetMode",1);
  if(EmailEdit->Text=="Wpisz tutaj adres e-mail") EmailEdit->Text="";
  Ini->WriteString("Settings","StaticEmail",EmailEdit->Text);
  if(DefaultAvatarRadioButton0->Checked==true)
   Ini->WriteInteger("Settings","DefaultAvatar",0);
  else if(DefaultAvatarRadioButton1->Checked==true)
   Ini->WriteInteger("Settings","DefaultAvatar",1);
  else if(DefaultAvatarRadioButton2->Checked==true)
   Ini->WriteInteger("Settings","DefaultAvatar",2);
  else if(DefaultAvatarRadioButton3->Checked==true)
   Ini->WriteInteger("Settings","DefaultAvatar",3);
  Ini->WriteInteger("Settings","Interval",IntervalComboBox->ItemIndex);
  Ini->WriteBool("Settings","InfoSuccess",InfoSuccessCheckBox->Checked);
  Ini->WriteBool("Settings","InfoFail",InfoFailCheckBox->Checked);
  if(AccountsMode0RadioButton->Checked==true)
   Ini->WriteInteger("Settings","AccountsMode",0);
  else if(AccountsMode1RadioButton->Checked==true)
   Ini->WriteInteger("Settings","AccountsMode",1);
  UnicodeString Accounts;
  for(int Count=0;Count<AccountsCheckListBox->Items->Count;Count++)
  {
	if(AccountsCheckListBox->Checked[Count]==true)
	 Accounts = Accounts + AccountsCheckListBox->Items->Strings[Count] + ";";
  }
  Ini->WriteString("Settings","Accounts",Accounts);
  delete Ini;
}
//---------------------------------------------------------------------------

