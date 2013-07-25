//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "FirstRunFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "acPNG"
#pragma link "sBevel"
#pragma link "sButton"
#pragma link "sCheckBox"
#pragma link "sCheckListBox"
#pragma link "sComboBox"
#pragma link "sEdit"
#pragma link "sLabel"
#pragma link "sListBox"
#pragma link "sPanel"
#pragma link "sRadioButton"
#pragma link "sSkinManager"
#pragma link "sSkinProvider"
#pragma resource "*.dfm"
TFirstRunForm *FirstRunForm;
//---------------------------------------------------------------------------
__declspec(dllimport)UnicodeString GetPluginUserDir();
__declspec(dllimport)UnicodeString GetThemeSkinDir();
__declspec(dllimport)bool ChkSkinEnabled();
__declspec(dllimport)bool ChkNativeEnabled();
__declspec(dllimport)void GetAccountList(bool FirstRun);
__declspec(dllimport)void RefreshAvatars();
__declspec(dllimport)void LoadSettings(bool OnLoad);
//---------------------------------------------------------------------------
__fastcall TFirstRunForm::TFirstRunForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::FormShow(TObject *Sender)
{
  //Skorkowanie okna
  if(!ChkSkinEnabled())
  {
	UnicodeString ThemeSkinDir = GetThemeSkinDir();
	//Wlaczenie skorkowania
	if((FileExists(ThemeSkinDir + "\\\\Skin.asz"))&&(!ChkNativeEnabled()))
	{
	  ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
	  sSkinManager->SkinDirectory = ThemeSkinDir;
	  sSkinManager->SkinName = "Skin.asz";
	  sSkinProvider->DrawNonClientArea = false;
	  sSkinManager->Active = true;
	}
	//Wylaczenie skorkowania
	else
	 sSkinManager->Active = false;
  }
  //Pobieranie listy kont
  GetAccountList(true);
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::FormCreate(TObject *Sender)
{
  ///Skorkowanie okna
  if(ChkSkinEnabled())
  {
	UnicodeString ThemeSkinDir = GetThemeSkinDir();
	//Wlaczenie skorkowania
	if((FileExists(ThemeSkinDir + "\\\\Skin.asz"))&&(!ChkNativeEnabled()))
	{
	  ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
	  sSkinManager->SkinDirectory = ThemeSkinDir;
	  sSkinManager->SkinName = "Skin.asz";
	  sSkinProvider->DrawNonClientArea = true;
	  sSkinManager->Active = true;
	}
	//Wylaczenie skorkowania
	else
	 sSkinManager->Active = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::aSaveSettingsExecute(TObject *Sender)
{
  TIniFile *Ini = new TIniFile(GetPluginUserDir() + "\\\\Gravatar\\\\Settings.ini");
  //Domyslny awatar
  if(DefaultAvatarRadioButton0->Checked)
   Ini->WriteInteger("Settings","DefaultAvatar",0);
  else if(DefaultAvatarRadioButton1->Checked)
   Ini->WriteInteger("Settings","DefaultAvatar",1);
  else if(DefaultAvatarRadioButton2->Checked)
   Ini->WriteInteger("Settings","DefaultAvatar",2);
  else
   Ini->WriteInteger("Settings","DefaultAvatar",3);
  //Sposob pobierania adresu e-mail
  Ini->WriteBool("Settings","GetMode",!GetMode0RadioButton->Checked);
  //Ustalony przez usera adres e-mail
  if(EmailEdit->Text=="Wpisz tutaj adres e-mail") EmailEdit->Text = "";
  Ini->WriteString("Settings","StaticEmail",EmailEdit->Text);
   //Czestotliwosc sprawdzania aktualizacji
  Ini->WriteInteger("Settings","Interval",IntervalComboBox->ItemIndex);
  //Wymuszanie aktualizacji
  Ini->WriteBool("Settings","ForceUpdate",ForceUpdateCheckBox->Checked);
  //Informacja o pomyslnym zaktualizowaniu awatarow
  Ini->WriteBool("Settings","InfoSuccess",InfoSuccessCheckBox->Checked);
  //Informacja o bledzie podczas aktualizacji awatarow
  Ini->WriteBool("Settings","InfoFail",InfoFailCheckBox->Checked);
  //Tryb obslugi kont
  Ini->WriteBool("Settings","AccountsMode",!AccountsMode0RadioButton->Checked);
  //Lista obslugiwanych kont
  UnicodeString pAccounts;
  for(int Count=0;Count<AccountsCheckListBox->Items->Count;Count++)
  {
	if(AccountsCheckListBox->Checked[Count])
	 pAccounts = pAccounts + AccountsCheckListBox->Items->Strings[Count] + ";";
  }
  Ini->WriteString("Settings","Accounts",pAccounts);
  delete Ini;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::aNextButtonExecute(TObject *Sender)
{
  if(Panel1->Visible)
  {
	Panel1->Visible = false;
	Panel2->Visible = true;
	aPrevButton->Enabled = true;
	NextButton->SetFocus();
  }
  else if(Panel2->Visible)
  {
	if((GetMode0RadioButton->Checked)||((GetMode1RadioButton->Checked)&&(!EmailEdit->Text.IsEmpty())))
	{
	  Panel2->Visible = false;
	  Panel3->Visible = true;
	  NextButton->SetFocus();
	}
  }
  else if(Panel3->Visible)
  {
	Panel3->Visible = false;
	Panel4->Visible = true;
	NextButton->SetFocus();
  }
  else if(Panel4->Visible)
  {
	Panel4->Visible = false;
	Panel5->Visible = true;
	NextButton->SetFocus();
  }
  else if(Panel5->Visible)
  {
	Panel5->Visible = false;
	Panel6->Visible = true;
	aNextButton->Visible = false;
	SaveButton->Visible = true;
	SaveButton->SetFocus();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::aPrevButtonExecute(TObject *Sender)
{
  if(Panel2->Visible)
  {
	Panel2->Visible = false;
	Panel1->Visible = true;
	aPrevButton->Enabled = false;
	NextButton->SetFocus();
  }
  else if(Panel3->Visible)
  {
	Panel3->Visible = false;
	Panel2->Visible = true;
	PrevButton->SetFocus();
  }
  else if(Panel4->Visible)
  {
	Panel4->Visible = false;
	Panel3->Visible = true;
	PrevButton->SetFocus();
  }
  else if(Panel5->Visible)
  {
	Panel5->Visible = false;
	Panel4->Visible = true;
	PrevButton->SetFocus();
  }
  else if(Panel6->Visible)
  {
	Panel6->Visible = false;
	Panel5->Visible = true;
	aNextButton->Visible = true;
	SaveButton->Visible = false;
	PrevButton->SetFocus();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::GetMode0RadioButtonClick(TObject *Sender)
{
  if(EmailEdit->Text.IsEmpty()) EmailEdit->Text = "Wpisz tutaj adres e-mail";
  EmailEdit->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::GetMode1RadioButtonClick(TObject *Sender)
{
  if(EmailEdit->Text=="Wpisz tutaj adres e-mail") EmailEdit->Text = "";
  EmailEdit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::DefaultAvatarImage0Click(TObject *Sender)
{
  DefaultAvatarRadioButton0->Checked = true;
  aDefaultAvatar0->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::DefaultAvatarImage1Click(TObject *Sender)
{
  DefaultAvatarRadioButton1->Checked = true;
  aDefaultAvatar1->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::DefaultAvatarImage2Click(TObject *Sender)
{
  DefaultAvatarRadioButton2->Checked = true;
  aDefaultAvatar2->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::DefaultAvatarImage3Click(TObject *Sender)
{
  DefaultAvatarRadioButton3->Checked = true;
  aDefaultAvatar3->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::AccountsMode0RadioButtonClick(TObject *Sender)
{
  AccountsCheckListBox->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::AccountsMode1RadioButtonClick(TObject *Sender)
{
  AccountsCheckListBox->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::aDefaultAvatar0Execute(TObject *Sender)
{
  DefaultAvatarBevel0->Visible = true;
  DefaultAvatarBevel1->Visible = false;
  DefaultAvatarBevel2->Visible = false;
  DefaultAvatarBevel3->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::aDefaultAvatar1Execute(TObject *Sender)
{
  DefaultAvatarBevel0->Visible = false;
  DefaultAvatarBevel1->Visible = true;
  DefaultAvatarBevel2->Visible = false;
  DefaultAvatarBevel3->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::aDefaultAvatar2Execute(TObject *Sender)
{
  DefaultAvatarBevel0->Visible = false;
  DefaultAvatarBevel1->Visible = false;
  DefaultAvatarBevel2->Visible = true;
  DefaultAvatarBevel3->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::aDefaultAvatar3Execute(TObject *Sender)
{
  DefaultAvatarBevel0->Visible = false;
  DefaultAvatarBevel1->Visible = false;
  DefaultAvatarBevel2->Visible = false;
  DefaultAvatarBevel3->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::ForceUpdateCheckBoxClick(TObject *Sender)
{
  InfoSuccessCheckBox->Enabled = !ForceUpdateCheckBox->Checked;
  InfoFailCheckBox->Enabled = !ForceUpdateCheckBox->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::SaveButtonClick(TObject *Sender)
{
  //Zapisanie ustawien
  aSaveSettings->Execute();
  //Odczyt ustawien w rdzeniu wtyczki
  LoadSettings(false);
  //Zamkniecie formy
  Close();
  //Odswiezanie awatarow
  RefreshAvatars();
}
//---------------------------------------------------------------------------
