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
#pragma link "sSkinManager"
#pragma link "sSkinProvider"
#pragma link "sBevel"
#pragma link "sButton"
#pragma link "sCheckBox"
#pragma link "sCheckListBox"
#pragma link "sComboBox"
#pragma link "sEdit"
#pragma link "sLabel"
#pragma link "sListBox"
#pragma link "sPageControl"
#pragma link "sRadioButton"
#pragma link "IdThreadComponent"
#pragma link "IdCoder"
#pragma link "IdCoder3to4"
#pragma link "IdCoderMIME"
#pragma link "IdAntiFreeze"
#pragma link "IdAntiFreezeBase"
#pragma resource "*.dfm"
TGravatarForm *GravatarForm;
//---------------------------------------------------------------------------
__declspec(dllimport)UnicodeString GetThemeSkinDir();
__declspec(dllimport)bool ChkSkinEnabled();
__declspec(dllimport)bool ChkNativeEnabled();
__declspec(dllimport)UnicodeString GetPluginUserDir();
__declspec(dllimport)UnicodeString GetPluginUserDirW();
__declspec(dllimport)void RefreshSettings();
__declspec(dllimport)void RefreshAvatars();
__declspec(dllimport)void GetAccountList(bool FirstRun);
//---------------------------------------------------------------------------
bool SettingsChanged = false;
//---------------------------------------------------------------------------
__fastcall TGravatarForm::TGravatarForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::FormShow(TObject *Sender)
{
  //AplhaSkins
  if(!ChkSkinEnabled())
  {
	UnicodeString ThemeSkinDir = GetThemeSkinDir();
	if((FileExists(ThemeSkinDir + "\\\\Skin.asz"))&&(!ChkNativeEnabled()))
	{
	  ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
	  sSkinManager->SkinDirectory = ThemeSkinDir;
	  sSkinManager->SkinName = "Skin.asz";
	  sSkinProvider->DrawNonClientArea = false;
	  sSkinManager->Active = true;
	}
	else
	 sSkinManager->Active = false;
  }
  //Odczyt ustawien
  aLoadSettings->Execute();
  //Wylaczenie buttona
  SaveButton->Enabled = false;
  //Ustawienie aktywnej karty
  PageControl->ActivePageIndex = 0;
  //Zmienna sprawdzania dokonania zmian
  SettingsChanged = false;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::FormCreate(TObject *Sender)
{
  //AlphaSkins
  if(ChkSkinEnabled())
  {
	UnicodeString ThemeSkinDir = GetThemeSkinDir();
	if((FileExists(ThemeSkinDir + "\\\\Skin.asz"))&&(!ChkNativeEnabled()))
	{
	  ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
	  sSkinManager->SkinDirectory = ThemeSkinDir;
	  sSkinManager->SkinName = "Skin.asz";
	  sSkinProvider->DrawNonClientArea = true;
	  sSkinManager->Active = true;
	}
	else
	 sSkinManager->Active = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::SaveButtonClick(TObject *Sender)
{
  aSaveSettings->Execute();
  SaveButton->Enabled = false;
  SettingsChanged = false;
  RefreshSettings();
  int Response = MessageBox(Application->Handle,
	"Ustawienia wtyczki Gravatar zosta³y zmienione.\n"
	"Czy dokonaæ teraz aktualizacji awatarów?",
	"Gravatar - aktualizacja",
	MB_OKCANCEL | MB_ICONQUESTION);
  //MB_OK
  if(Response==1)
   RefreshAvatars();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::OKButtonClick(TObject *Sender)
{
  if(SettingsChanged)
  {
	aSaveSettings->Execute();
	RefreshSettings();
	int Response = MessageBox(Application->Handle,
	"Ustawienia wtyczki Gravatar zosta³y zmienione.\n"
	"Czy dokonaæ teraz aktualizacji awatarów?",
	"Gravatar - aktualizacja",
	MB_OKCANCEL | MB_ICONQUESTION);
	//MB_OK
	if(Response==1)
	{
	  Close();
	  RefreshAvatars();
	}
	else
	 Close();
  }
  else
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::aExitExecute(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::aLoadSettingsExecute(TObject *Sender)
{
  //Pobieranie listy kont
  GetAccountList(false);
  //Odczyt ustawien
  TIniFile *Ini = new TIniFile(GetPluginUserDir() + "\\\\Gravatar\\\\Settings.ini");
  EmailEdit->Text = Ini->ReadString("Settings","StaticEmail","");
  int pGetMode = Ini->ReadInteger("Settings","GetMode",0);
  if(!pGetMode)
  {
	GetMode0RadioButton->Checked = true;
	if(EmailEdit->Text.IsEmpty()) EmailEdit->Text = "Wpisz tutaj adres e-mail";
	EmailEdit->Enabled = false;
  }
  else
  {
	GetMode1RadioButton->Checked = true;
	if(EmailEdit->Text=="Wpisz tutaj adres e-mail") EmailEdit->Text = "";
	EmailEdit->Enabled = true;
  }
  int pDefaultAvatar = Ini->ReadInteger("Settings","DefaultAvatar",0);
  switch(pDefaultAvatar)
  {
	case 1:
	  DefaultAvatarRadioButton1->Checked = true;
	  DefaultAvatarBevel0->Visible = false;
	  DefaultAvatarBevel1->Visible = true;
	  DefaultAvatarBevel2->Visible = false;
	  DefaultAvatarBevel3->Visible = false;
	  break;
	case 2:
	  DefaultAvatarRadioButton2->Checked = true;
	  DefaultAvatarBevel0->Visible = false;
	  DefaultAvatarBevel1->Visible = false;
	  DefaultAvatarBevel2->Visible = true;
	  DefaultAvatarBevel3->Visible = false;
	  break;
	case 3:
	  DefaultAvatarRadioButton3->Checked = true;
	  DefaultAvatarBevel0->Visible = false;
	  DefaultAvatarBevel1->Visible = false;
	  DefaultAvatarBevel2->Visible = false;
	  DefaultAvatarBevel3->Visible = true;
	  break;
	default:
	  DefaultAvatarRadioButton0->Checked = true;
	  DefaultAvatarBevel0->Visible = true;
	  DefaultAvatarBevel1->Visible = false;
	  DefaultAvatarBevel2->Visible = false;
	  DefaultAvatarBevel3->Visible = false;
	  break;
  }
  IntervalComboBox->ItemIndex = Ini->ReadInteger("Settings","Interval",0);
  InfoSuccessCheckBox->Checked = Ini->ReadBool("Settings","InfoSuccess",true);
  InfoFailCheckBox->Checked = Ini->ReadBool("Settings","InfoFail",false);
  int pAccountsMode = Ini->ReadInteger("Settings","AccountsMode",0);
  if(!pAccountsMode) AccountsMode0RadioButton->Checked = true;
  else AccountsMode1RadioButton->Checked = true;
  UnicodeString pAccounts = Ini->ReadString("Settings","Accounts","");
  for(int pCount=0;pCount<AccountsCheckListBox->Items->Count;pCount++)
  {
	if(pAccounts.Pos(AccountsCheckListBox->Items->Strings[pCount]))
	 AccountsCheckListBox->Checked[pCount] = true;
  }

  delete Ini;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::aSaveSettingsExecute(TObject *Sender)
{
  TIniFile *Ini = new TIniFile(GetPluginUserDir() + "\\\\Gravatar\\\\Settings.ini");
  if(GetMode0RadioButton->Checked)
   Ini->WriteInteger("Settings","GetMode",0);
  else if(GetMode1RadioButton->Checked)
   Ini->WriteInteger("Settings","GetMode",1);
  if(EmailEdit->Text=="Wpisz tutaj adres e-mail") EmailEdit->Text = "";
  Ini->WriteString("Settings","StaticEmail",EmailEdit->Text);
  if(DefaultAvatarRadioButton0->Checked)
   Ini->WriteInteger("Settings","DefaultAvatar",0);
  else if(DefaultAvatarRadioButton1->Checked)
   Ini->WriteInteger("Settings","DefaultAvatar",1);
  else if(DefaultAvatarRadioButton2->Checked)
   Ini->WriteInteger("Settings","DefaultAvatar",2);
  else if(DefaultAvatarRadioButton3->Checked)
   Ini->WriteInteger("Settings","DefaultAvatar",3);
  Ini->WriteInteger("Settings","Interval",IntervalComboBox->ItemIndex);
  Ini->WriteBool("Settings","InfoSuccess",InfoSuccessCheckBox->Checked);
  Ini->WriteBool("Settings","InfoFail",InfoFailCheckBox->Checked);
  if(AccountsMode0RadioButton->Checked)
   Ini->WriteInteger("Settings","AccountsMode",0);
  else if(AccountsMode1RadioButton->Checked)
   Ini->WriteInteger("Settings","AccountsMode",1);
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

void __fastcall TGravatarForm::DefaultAvatarImage0Click(TObject *Sender)
{
  DefaultAvatarRadioButton0->Checked = true;
  aDefaultAvatar0->Execute();
  aAllowApply->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::DefaultAvatarImage1Click(TObject *Sender)
{
  DefaultAvatarRadioButton1->Checked = true;
  aDefaultAvatar1->Execute();
  aAllowApply->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::DefaultAvatarImage2Click(TObject *Sender)
{
  DefaultAvatarRadioButton2->Checked = true;
  aDefaultAvatar2->Execute();
  aAllowApply->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::DefaultAvatarImage3Click(TObject *Sender)
{
  DefaultAvatarRadioButton3->Checked = true;
  aDefaultAvatar3->Execute();
  aAllowApply->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::GetMode0RadioButtonClick(TObject *Sender)
{
  if(EmailEdit->Text.IsEmpty()) EmailEdit->Text = "Wpisz tutaj adres e-mail";
  EmailEdit->Enabled = false;
  aAllowApply->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::GetMode1RadioButtonClick(TObject *Sender)
{
  if(EmailEdit->Text=="Wpisz tutaj adres e-mail") EmailEdit->Text = "";
  EmailEdit->Enabled = true;
  aAllowApply->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::AccountsMode0RadioButtonClick(TObject *Sender)
{
  AccountsCheckListBox->Enabled = false;
  aAllowApply->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::AccountsMode1RadioButtonClick(TObject *Sender)
{
  aAllowApply->Execute();
  AccountsCheckListBox->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::StartTimerTimer(TObject *Sender)
{
  RefreshAvatars();
  StartTimer->Enabled = false;
  Timer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::TimerTimer(TObject *Sender)
{
  RefreshAvatars();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::aDefaultAvatar0Execute(TObject *Sender)
{
  DefaultAvatarBevel0->Visible = true;
  DefaultAvatarBevel1->Visible = false;
  DefaultAvatarBevel2->Visible = false;
  DefaultAvatarBevel3->Visible = false;
  aAllowApply->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::aDefaultAvatar1Execute(TObject *Sender)
{
  DefaultAvatarBevel0->Visible = false;
  DefaultAvatarBevel1->Visible = true;
  DefaultAvatarBevel2->Visible = false;
  DefaultAvatarBevel3->Visible = false;
  aAllowApply->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::aDefaultAvatar2Execute(TObject *Sender)
{
  DefaultAvatarBevel0->Visible = false;
  DefaultAvatarBevel1->Visible = false;
  DefaultAvatarBevel2->Visible = true;
  DefaultAvatarBevel3->Visible = false;
  aAllowApply->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::aDefaultAvatar3Execute(TObject *Sender)
{
  DefaultAvatarBevel0->Visible = false;
  DefaultAvatarBevel1->Visible = false;
  DefaultAvatarBevel2->Visible = false;
  DefaultAvatarBevel3->Visible = true;
  aAllowApply->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::aPageControlSheetChangeExecute(TObject *Sender)
{
  if(!SaveButton->Focused()&&!CancelButton->Focused()&&!OKButton->Focused())
  {
	if(PageControl->TabIndex!=3)
	 PageControl->TabIndex = PageControl->TabIndex + 1;
	else
	 PageControl->TabIndex = 0;
  }
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::aAllowApplyExecute(TObject *Sender)
{
  SaveButton->Enabled = true;
  SettingsChanged = true;
}
//---------------------------------------------------------------------------
