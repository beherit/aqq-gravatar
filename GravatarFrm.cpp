//---------------------------------------------------------------------------
// Copyright (C) 2010-2014 Krzysztof Grochocki
//
// This file is part of Gravatar
//
// Gravatar is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// Gravatar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Radio; see the file COPYING. If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street,
// Boston, MA 02110-1301, USA.
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <windows.h>
#include <vcl.h>
#pragma hdrstop
#include "GravatarFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#ifdef _WIN64
#pragma link "Vcl.IdAntiFreeze"
#endif
#pragma link "acPNG"
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
#pragma link "sSkinManager"
#pragma link "sSkinProvider"
#pragma resource "*.dfm"
TGravatarForm *GravatarForm;
//---------------------------------------------------------------------------
__declspec(dllimport)UnicodeString GetPluginUserDir();
__declspec(dllimport)UnicodeString GetThemeSkinDir();
__declspec(dllimport)bool ChkSkinEnabled();
__declspec(dllimport)bool ChkThemeAnimateWindows();
__declspec(dllimport)bool ChkThemeGlowing();
__declspec(dllimport)int GetSaturation();
__declspec(dllimport)int GetHUE();
__declspec(dllimport)void GetAccountList(bool FirstRun);
__declspec(dllimport)void RefreshAvatars();
__declspec(dllimport)void LoadSettings(bool OnLoad);
//---------------------------------------------------------------------------
bool SettingsChanged = false;
//---------------------------------------------------------------------------
__fastcall TGravatarForm::TGravatarForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::WMTransparency(TMessage &Message)
{
  Application->ProcessMessages();
  if(sSkinManager->Active) sSkinProvider->BorderForm->UpdateExBordersPos(true,(int)Message.LParam);
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::FormCreate(TObject *Sender)
{
  //Wlaczona zaawansowana stylizacja okien
  if(ChkSkinEnabled())
  {
	UnicodeString ThemeSkinDir = GetThemeSkinDir();
	//Plik zaawansowanej stylizacji okien istnieje
	if(FileExists(ThemeSkinDir + "\\\\Skin.asz"))
	{
	  //Dane pliku zaawansowanej stylizacji okien
	  ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
	  sSkinManager->SkinDirectory = ThemeSkinDir;
	  sSkinManager->SkinName = "Skin.asz";
	  //Ustawianie animacji AlphaControls
	  if(ChkThemeAnimateWindows()) sSkinManager->AnimEffects->FormShow->Time = 200;
	  else sSkinManager->AnimEffects->FormShow->Time = 0;
	  sSkinManager->Effects->AllowGlowing = ChkThemeGlowing();
	  //Zmiana kolorystyki AlphaControls
	  sSkinManager->HueOffset = GetHUE();
	  sSkinManager->Saturation = GetSaturation();
	  //Aktywacja skorkowania AlphaControls
	  sSkinManager->Active = true;
	}
	//Brak pliku zaawansowanej stylizacji okien
	else sSkinManager->Active = false;
  }
  //Zaawansowana stylizacja okien wylaczona
  else sSkinManager->Active = false;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::FormShow(TObject *Sender)
{
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

void __fastcall TGravatarForm::SaveButtonClick(TObject *Sender)
{
  //Wylaczanie przyciskow
  SaveButton->Enabled = false;
  CancelButton->Enabled = false;
  OKButton->Enabled = false;
  //Zapisanie ustawien
  aSaveSettings->Execute();
  //Zmienna sprawdzania dokonania zmian
  SettingsChanged = false;
  //Odczyt ustawien w rdzeniu wtyczki
  LoadSettings(false);
  //Odswiezenie awatarow
  int Response = MessageBox(Application->Handle,
	L"Ustawienia wtyczki Gravatar zosta³y zmienione.\n"
	L"Czy dokonaæ teraz aktualizacji awatarów?",
	L"Gravatar - aktualizacja",
	MB_OKCANCEL | MB_ICONQUESTION);
  //MB_OK
  if(Response==1)
   RefreshAvatars();
  //Wlaczanie przyciskow
  CancelButton->Enabled = true;
  OKButton->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::OKButtonClick(TObject *Sender)
{
  //Jezeli zostaly dokononane zmiany
  if(SettingsChanged)
  {
    //Wylaczanie przyciskow
	SaveButton->Enabled = false;
	CancelButton->Enabled = false;
	OKButton->Enabled = false;
	//Zapisanie ustawien
	aSaveSettings->Execute();
	//Odczyt ustawien w rdzeniu wtyczki
	LoadSettings(false);
    //Odswiezenie awatarow
	int Response = MessageBox(Application->Handle,
	L"Ustawienia wtyczki Gravatar zosta³y zmienione.\n"
	L"Czy dokonaæ teraz aktualizacji awatarów?",
	L"Gravatar - aktualizacja",
	MB_OKCANCEL | MB_ICONQUESTION);
	//MB_OK
	if(Response==1)
	{
	  Close();
	  RefreshAvatars();
	}
	//MB_CANCEL
	else
	 Close();
	//Wlaczanie przyciskow
	CancelButton->Enabled = true;
	OKButton->Enabled = true;
  }
  //Jezelo zmiany nie zostaly dokonane
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
  //Domyslny awatar
  switch(Ini->ReadInteger("Settings","DefaultAvatar",0))
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
  //Sposob pobierania adresu e-mail
  if(!Ini->ReadBool("Settings","GetMode",false))
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
  //Ustalony przez usera adres e-mail
  EmailEdit->Text = Ini->ReadString("Settings","StaticEmail","");
  //Czestotliwosc sprawdzania aktualizacji
  IntervalComboBox->ItemIndex = Ini->ReadInteger("Settings","Interval",0);
  //Wymuszanie aktualizacji
  ForceUpdateCheckBox->Checked = Ini->ReadBool("Settings","ForceUpdate",false);
  InfoSuccessCheckBox->Enabled = !ForceUpdateCheckBox->Checked;
  InfoFailCheckBox->Enabled = !ForceUpdateCheckBox->Checked;
  //Informacja o pomyslnym zaktualizowaniu awatarow
  InfoSuccessCheckBox->Checked = Ini->ReadBool("Settings","InfoSuccess",true);
  //Informacja o bledzie podczas aktualizacji awatarow
  InfoFailCheckBox->Checked = Ini->ReadBool("Settings","InfoFail",false);
  //Tryb obslugi kont
  if(!Ini->ReadBool("Settings","AccountsMode",false)) AccountsMode0RadioButton->Checked = true;
  else AccountsMode1RadioButton->Checked = true;
  //Lista obslugiwanych kont
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
  AccountsCheckListBox->Enabled = true;
  aAllowApply->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::StartTimerTimer(TObject *Sender)
{
  //Wylaczenie timera
  StartTimer->Enabled = false;
  //Odswiezanie awatarow
  RefreshAvatars();
  //Wlaczenie timera cyklicznego
  if(Timer->Interval) Timer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::TimerTimer(TObject *Sender)
{
  //Odswiezanie awatarow
  RefreshAvatars();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::ForceUpdateCheckBoxClick(TObject *Sender)
{
  InfoSuccessCheckBox->Enabled = !ForceUpdateCheckBox->Checked;
  InfoFailCheckBox->Enabled = !ForceUpdateCheckBox->Checked;
  aAllowApply->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::aAllowApplyExecute(TObject *Sender)
{
  SaveButton->Enabled = true;
  SettingsChanged = true;
}
//---------------------------------------------------------------------------

void __fastcall TGravatarForm::sSkinManagerSysDlgInit(TacSysDlgData DlgData, bool &AllowSkinning)
{
  AllowSkinning = false;
}
//---------------------------------------------------------------------------

