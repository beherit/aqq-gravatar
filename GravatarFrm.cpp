//---------------------------------------------------------------------------
#include <windows.h>
#include <vcl.h>
#pragma hdrstop
#include "GravatarFrm.h"
#include <inifiles.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "IdBaseComponent"
#pragma link "IdComponent"
#pragma link "IdHTTP"
#pragma link "IdTCPClient"
#pragma link "IdTCPConnection"
#pragma link "LMDPNGImage"
#pragma resource "*.dfm"
TGravatarForm *GravatarForm;
//---------------------------------------------------------------------------
__declspec(dllimport)UnicodeString GetPluginUserDir();
__declspec(dllimport)UnicodeString GetPluginUserDirW();
__declspec(dllimport)void RefreshSettings();
__declspec(dllimport)void RefreshAvatars();
__declspec(dllimport)void GetAccountList();
//---------------------------------------------------------------------------
UnicodeString eDir;
//---------------------------------------------------------------------------
__fastcall TGravatarForm::TGravatarForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::SaveButtonClick(TObject *Sender)
{
  aSaveSettings->Execute();
  aClearAvatars->Execute();
  RefreshSettings();
  RefreshAvatars();
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::aExitExecute(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::FormShow(TObject *Sender)
{
  aReadSettings->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::aReadSettingsExecute(TObject *Sender)
{
  GetAccountList();

  eDir = GetPluginUserDir();

  TIniFile *Ini = new TIniFile(eDir + "\\\\Gravatar\\\\Settings.ini");
  EmailEdit->Text = Ini->ReadString("Settings","StaticEmail","");
  int pGetMode = Ini->ReadInteger("Settings","GetMode",0);
  if(pGetMode==0)
  {
	GetMode0RadioButton->Checked=true;

	if(EmailEdit->Text=="")
	 EmailEdit->Text="Wpisz tutaj adres e-mail";
	EmailEdit->Enabled=false;
  }
  else if(pGetMode==1)
  {
	GetMode1RadioButton->Checked=true;

	if(EmailEdit->Text=="Wpisz tutaj adres e-mail")
	 EmailEdit->Text="";
	EmailEdit->Enabled=true;
  }
  int pDefaultAvatar = Ini->ReadInteger("Settings","DefaultAvatar",0);
  switch(pDefaultAvatar)
  {
	case 1:
	  DefaultAvatarRadioButton1->Checked=true;
	  DefaultAvatarBevel0->Visible=false;
	  DefaultAvatarBevel1->Visible=true;
	  DefaultAvatarBevel2->Visible=false;
	  DefaultAvatarBevel3->Visible=false;
	  break;
	case 2:
	  DefaultAvatarRadioButton2->Checked=true;
	  DefaultAvatarBevel0->Visible=false;
	  DefaultAvatarBevel1->Visible=false;
	  DefaultAvatarBevel2->Visible=true;
	  DefaultAvatarBevel3->Visible=false;
	  break;
	case 3:
	  DefaultAvatarRadioButton3->Checked=true;
	  DefaultAvatarBevel0->Visible=false;
	  DefaultAvatarBevel1->Visible=false;
	  DefaultAvatarBevel2->Visible=false;
	  DefaultAvatarBevel3->Visible=true;
	  break;
	default:
	  DefaultAvatarRadioButton0->Checked=true;
	  DefaultAvatarBevel0->Visible=true;
	  DefaultAvatarBevel1->Visible=false;
	  DefaultAvatarBevel2->Visible=false;
	  DefaultAvatarBevel3->Visible=false;
	  break;
  }
  IntervalComboBox->ItemIndex = Ini->ReadInteger("Settings","Interval",0);
  InfoSuccessCheckBox->Checked = Ini->ReadBool("Settings","InfoSuccess",true);
  InfoFailCheckBox->Checked = Ini->ReadBool("Settings","InfoFail",false);
  int pAccountsMode = Ini->ReadInteger("Settings","AccountsMode",0);
  if(pAccountsMode==0)
   AccountsMode0RadioButton->Checked=true;
  else if(pAccountsMode==1)
   AccountsMode1RadioButton->Checked=true;
  UnicodeString pAccounts = Ini->ReadString("Settings","Accounts","");
  for(int pCount=0;pCount<AccountsCheckListBox->Items->Count;pCount++)
  {
	if(AnsiPos(AccountsCheckListBox->Items->Strings[pCount],pAccounts)>0)
	 AccountsCheckListBox->Checked[pCount]=true;
  }

  delete Ini;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::aSaveSettingsExecute(TObject *Sender)
{
  eDir = GetPluginUserDir();

  TIniFile *Ini = new TIniFile(eDir + "\\\\Gravatar\\\\Settings.ini");
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

void __fastcall TGravatarForm::DefaultAvatarRadioButton0Click(TObject *Sender)
{
  DefaultAvatarBevel0->Visible=true;
  DefaultAvatarBevel1->Visible=false;
  DefaultAvatarBevel2->Visible=false;
  DefaultAvatarBevel3->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::DefaultAvatarRadioButton1Click(TObject *Sender)
{
  DefaultAvatarBevel0->Visible=false;
  DefaultAvatarBevel1->Visible=true;
  DefaultAvatarBevel2->Visible=false;
  DefaultAvatarBevel3->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::DefaultAvatarRadioButton2Click(TObject *Sender)
{
  DefaultAvatarBevel0->Visible=false;
  DefaultAvatarBevel1->Visible=false;
  DefaultAvatarBevel2->Visible=true;
  DefaultAvatarBevel3->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::DefaultAvatarRadioButton3Click(TObject *Sender)
{
  DefaultAvatarBevel0->Visible=false;
  DefaultAvatarBevel1->Visible=false;
  DefaultAvatarBevel2->Visible=false;
  DefaultAvatarBevel3->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::DefaultAvatarImage0Click(TObject *Sender)
{
  DefaultAvatarRadioButton0->Checked=true;

  DefaultAvatarBevel0->Visible=true;
  DefaultAvatarBevel1->Visible=false;
  DefaultAvatarBevel2->Visible=false;
  DefaultAvatarBevel3->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::DefaultAvatarImage1Click(TObject *Sender)
{
  DefaultAvatarRadioButton1->Checked=true;

  DefaultAvatarBevel0->Visible=false;
  DefaultAvatarBevel1->Visible=true;
  DefaultAvatarBevel2->Visible=false;
  DefaultAvatarBevel3->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::DefaultAvatarImage2Click(TObject *Sender)
{
  DefaultAvatarRadioButton2->Checked=true;

  DefaultAvatarBevel0->Visible=false;
  DefaultAvatarBevel1->Visible=false;
  DefaultAvatarBevel2->Visible=true;
  DefaultAvatarBevel3->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::DefaultAvatarImage3Click(TObject *Sender)
{
  DefaultAvatarRadioButton3->Checked=true;

  DefaultAvatarBevel0->Visible=false;
  DefaultAvatarBevel1->Visible=false;
  DefaultAvatarBevel2->Visible=false;
  DefaultAvatarBevel3->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::GetMode0RadioButtonClick(TObject *Sender)
{
  if(EmailEdit->Text=="")
   EmailEdit->Text="Wpisz tutaj adres e-mail";
  EmailEdit->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::GetMode1RadioButtonClick(TObject *Sender)
{
  if(EmailEdit->Text=="Wpisz tutaj adres e-mail")
   EmailEdit->Text="";
  EmailEdit->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::AccountsMode0RadioButtonClick(TObject *Sender)
{
  AccountsCheckListBox->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::AccountsMode1RadioButtonClick(TObject *Sender)
{
  AccountsCheckListBox->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::StartTimerTimer(TObject *Sender)
{
  RefreshAvatars();
  StartTimer->Enabled=false;
  Timer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::TimerTimer(TObject *Sender)
{
  RefreshAvatars();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::DeleteFiles(UnicodeString DirName, UnicodeString Prefix)
{
  TSearchRec sr;
  int result;
  UnicodeString FileName;

  result = FindFirst(DirName + "*.*", faAnyFile, sr);

  while(result == 0)
  {
	if((sr.Name != ".") && (sr.Name != "..") && (!(sr.Attr & faDirectory) > 0))
	{
	  if(ExtractFileExt(sr.Name).SubString(2, 5) == Prefix)
	  {
		FileName = DirName + sr.Name;
	  }
	  if(Prefix == "*")
	  {
		FileName = DirName + sr.Name;
	  }
	  DeleteFile(FileName);
	}
	result = FindNext(sr);
  }
  FindClose(sr);
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::aClearAvatarsExecute(TObject *Sender)
{
  eDir = GetPluginUserDirW();
  DeleteFiles(eDir + "\\Gravatar\\Avatars\\", "*");
}
//---------------------------------------------------------------------------

