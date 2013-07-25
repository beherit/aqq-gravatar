//---------------------------------------------------------------------------
#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#pragma argsused
#include "Aqq.h"
#include "GravatarFrm.h"
#include "FirstRunFrm.h"
#include "fstream.h"
#include <inifiles.hpp>
#include <IdHashMessageDigest.hpp>
#include <boost\scoped_ptr.hpp>
//---------------------------------------------------------------------------

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  return 1;
}
//---------------------------------------------------------------------------

//Uchwyty do form wtyczki
TGravatarForm *hGravatarForm;
TFirstRunForm *hFirstRun;
//Struktury glowne
TPluginLink PluginLink;
TPluginInfo PluginInfo;
//SETTINGS-------------------------------------------------------------------
int GetMode;
UnicodeString StaticEmail;
int DefaultAvatar;
bool InfoSuccess;
bool InfoFail;
int AccountsMode;
UnicodeString Accounts;
//---------------------------------------------------------------------------
bool FirstRun;

//Pobieranie sciezki do skorki kompozycji
UnicodeString GetThemeSkinDir()
{
  return StringReplace((wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETTHEMEDIR,0,0)), "\\", "\\\\", TReplaceFlags() << rfReplaceAll) + "\\\\Skin";
}
//---------------------------------------------------------------------------
//Pobieranie sciezki katalogu prywatnego uzytkownika
UnicodeString GetPluginUserDir()
{
  return StringReplace((wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETPLUGINUSERDIR,0,0)), "\\", "\\\\", TReplaceFlags() << rfReplaceAll);
}
//---------------------------------------------------------------------------
//Pobieranie sciezki katalogu prywatnego uzytkownika
UnicodeString GetPluginUserDirW()
{
  return (wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETPLUGINUSERDIR,0,0));
}
//---------------------------------------------------------------------------
UnicodeString GetContactsUserDir()
{
  return StringReplace((wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETTHEMEDIR,0,0)), "\\", "\\\\", TReplaceFlags() << rfReplaceAll) + "\\\\Data\\\\Contacts\\\\";
}
//---------------------------------------------------------------------------
UnicodeString GetAvatarsUserDir()
{
  return StringReplace((wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETTHEMEDIR,0,0)), "\\", "\\\\", TReplaceFlags() << rfReplaceAll) + "\\\\Data\\\\Avatars\\\\";
}
//---------------------------------------------------------------------------

//Sprawdzanie czy wlaczona jest obsluga stylow obramowania okien
bool ChkSkinEnabled()
{
  TStrings* IniList = new TStringList();
  IniList->SetText((wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_FETCHSETUP,0,0)));
  TMemIniFile *Settings = new TMemIniFile(ChangeFileExt(Application->ExeName, ".INI"));
  Settings->SetStrings(IniList);
  delete IniList;
  UnicodeString AlphaSkinsEnabled = Settings->ReadString("Settings","UseSkin","1");
  delete Settings;
  return StrToBool(AlphaSkinsEnabled);
}
//---------------------------------------------------------------------------

//Sprawdzanie czy wlaczony jest natywny styl Windows
bool ChkNativeEnabled()
{
  TStrings* IniList = new TStringList();
  IniList->SetText((wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_FETCHSETUP,0,0)));
  TMemIniFile *Settings = new TMemIniFile(ChangeFileExt(Application->ExeName, ".INI"));
  Settings->SetStrings(IniList);
  delete IniList;
  UnicodeString NativeEnabled = Settings->ReadString("Settings","Native","0");
  delete Settings;
  return StrToBool(NativeEnabled);
}
//---------------------------------------------------------------------------

//Zmiana skorki wtyczki
void ChangePluginSkin()
{
  if(hGravatarForm)
  {
	UnicodeString ThemeSkinDir = GetThemeSkinDir();
	if((FileExists(ThemeSkinDir + "\\\\Skin.asz"))&&(!ChkNativeEnabled()))
	{
	  ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
	  hGravatarForm->sSkinManager->SkinDirectory = ThemeSkinDir;
	  hGravatarForm->sSkinManager->SkinName = "Skin.asz";
	  hGravatarForm->sSkinProvider->DrawNonClientArea = ChkSkinEnabled();
	  hGravatarForm->sSkinManager->Active = true;
	}
	else
	 hGravatarForm->sSkinManager->Active = false;
  }
  if(hFirstRun)
  {
	UnicodeString ThemeSkinDir = GetThemeSkinDir();
	if((FileExists(ThemeSkinDir + "\\\\Skin.asz"))&&(!ChkNativeEnabled()))
	{
	  ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
	  hFirstRun->sSkinManager->SkinDirectory = ThemeSkinDir;
	  hFirstRun->sSkinManager->SkinName = "Skin.asz";
	  hFirstRun->sSkinProvider->DrawNonClientArea = ChkSkinEnabled();
	  hFirstRun->sSkinManager->Active = true;
	}
	else
	 hFirstRun->sSkinManager->Active = false;
  }
}
//---------------------------------------------------------------------------

//Hook na zmianê kompozycji
int __stdcall OnThemeChanged (WPARAM wParam, LPARAM lParam)
{
  ChangePluginSkin();
  return 0;
}
//---------------------------------------------------------------------------

//Ustalanie typu pliku graficznego (zmienne)
struct TMagicWordInfo
{
  BYTE Len;
  BYTE Off;
  char *cWord;
  wchar_t *wcName;
};
static const int iTypes = 5;
static const TMagicWordInfo cstMagicWords[iTypes] = {
    {6, 0, "\x47\x49\x46\x38\x39\x61", L"GIF89a"},
    {6, 0, "\x47\x49\x46\x38\x37\x61", L"GIF87a"},
    {4, 6, "\x4A\x46\x49\x46", L"JPEG/JFIF"},
    {8, 0, "\211PNG\r\n\032\n", L"PNG"},
    {2, 0, "BM", L"Bitmap"}
};
//---------------------------------------------------------------------------

//Ustalanie typu pliku graficznego
int GetFileType(wchar_t *wcPath)
{
  FILE *fh;
  if(!(fh = _wfopen(wcPath, L"rb")))
  {
	fclose(fh);
	return -1;
  }
  char cBuf[16];
  fseek(fh, 0, 0);
  fread(cBuf, 16, 1, fh);
  TMagicWordInfo mwi;
  bool bRes;
  for(int i=0;i<iTypes;i++)
  {
	mwi = cstMagicWords[i];
	bRes = true;
	for(int j=0;j<mwi.Len;j++)
	{
	  bRes &= (cBuf[mwi.Off+j] == mwi.cWord[j]);
	}
	if(bRes)
	{
	  fclose(fh);
	  return i;
	}
  }
  fclose(fh);
  return -2;
}
//---------------------------------------------------------------------------

//Kodowanie lancuchow znakow do MD5
UnicodeString MD5(const UnicodeString Text)
{
  boost::scoped_ptr<TIdHashMessageDigest5> idmd5(new TIdHashMessageDigest5());
  return idmd5->HashStringAsHex(Text.LowerCase().Trim()).LowerCase();
}
//---------------------------------------------------------------------------

//Oblicznie MD5 pliku
UnicodeString MD5File(const UnicodeString FileName)
{
  if(FileExists(FileName))
  {
    String Result;
    TFileStream *fs;

    fs = new TFileStream(FileName, fmOpenRead | fmShareDenyWrite);
    try
	{
	  TIdHashMessageDigest5 *idmd5= new TIdHashMessageDigest5();
	  try
	  {
	    Result = idmd5->HashStreamAsHex(fs);
	  }
	  __finally
	  {
	    delete idmd5;
	  }
    }
	__finally
    {
	  delete fs;
    }

    return Result;
  }
  else
   return 0;
}
//---------------------------------------------------------------------------

//Info o pomyslnej aktualizacji
void ShowUpdateMessage(UnicodeString AccountJID)
{
  if(AccountJID.Pos("@plugin.gg")) AccountJID = "GG " + AccountJID.Delete(AccountJID.Pos("@plugin.gg"),AccountJID.Length());
  //else AccountJID = "Jabber " + AccountJID;
  TPluginShowInfo PluginShowInfo;
  PluginShowInfo.cbSize = sizeof(TPluginShowInfo);
  PluginShowInfo.Event = tmeInfo;
  PluginShowInfo.Text = ("Zaktualizowano awatar dla konta " + AccountJID).w_str();
  PluginShowInfo.ImagePath = (wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETPNG_FILEPATH,19,0));
  PluginShowInfo.TimeOut = 1000 * 4;
  PluginLink.CallService(AQQ_FUNCTION_SHOWINFO,0,(LPARAM)(&PluginShowInfo));
}
//---------------------------------------------------------------------------

//Info o bledzie podczas aktualizacji
void ShowFailMessage(UnicodeString AccountJID)
{
  if(AccountJID.Pos("@plugin.gg")) AccountJID = "GG " + AccountJID.Delete(AccountJID.Pos("@plugin.gg"),AccountJID.Length());
  //else AccountJID = "Jabber " + AccountJID;
  TPluginShowInfo PluginShowInfo;
  PluginShowInfo.cbSize = sizeof(TPluginShowInfo);
  PluginShowInfo.Event = tmeInfo;
  PluginShowInfo.Text = ("Wyst¹pi³ b³¹d podczas aktualizacji awatara dla konta " + AccountJID).w_str();
  PluginShowInfo.ImagePath = (wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETPNG_FILEPATH,85,0));
  PluginShowInfo.TimeOut = 1000 * 4;
  PluginLink.CallService(AQQ_FUNCTION_SHOWINFO,0,(LPARAM)(&PluginShowInfo));
}
//---------------------------------------------------------------------------

//Pobieranie pliku z danego URL
bool IdHTTPGetFileToMem(TMemoryStream *File, UnicodeString URL)
{
  File->Position = 0;
  try
  {
	hGravatarForm->IdHTTP->Get(URL, File);
  }
  catch(const Exception& e)
  {
	if(e.Message=="Connection Closed Gracefully.")
	{
	  hGravatarForm->IdHTTP->CheckForGracefulDisconnect(false);
	  hGravatarForm->IdHTTP->Disconnect();
	  File->Position = 0;
	  //Zwrocenie bledu
	  return false;
	}
	else
	{
	  hGravatarForm->IdHTTP->Disconnect();
	  File->Position = 0;
	  //Zwrocenie bledu
	  return false;
	}
  }
  File->Position = 0;
  int Response = hGravatarForm->IdHTTP->ResponseCode;
  //Wszystko ok
  if(Response==200)
   return true;
  //Inne bledy
  else
   return false;
}
//---------------------------------------------------------------------------

//Odswiezanie awatarow
void RefreshAvatars()
{
  //Wlaczenie AntiFreeze
  hGravatarForm->IdAntiFreeze->Active = true;
  //Sciezka folderu prywatnego wtyczek
  UnicodeString PluginDir = GetPluginUserDir();
  //Folder ustawieñ
  if(!DirectoryExists(PluginDir + "\\\\Gravatar\\\\Avatars"))
   CreateDir(PluginDir + "\\\\Gravatar\\\\Avatars");
  //Adres e-mail w MD5 / nazwa pliku
  UnicodeString AvatarFile;
  //Struktura zawierajaca informacje o danym koncie
  TPluginStateChange PluginStateChange;
  //Tworzenie uchwytu do formy
  if(!hGravatarForm)
  {
	Application->Handle = (HWND)GravatarForm;
	hGravatarForm = new TGravatarForm(Application);
  }
  //Pobieranie ilosci kont
  int UserIdxCount = PluginLink.CallService(AQQ_FUNCTION_GETUSEREXCOUNT,0,0);
  //Sprawdzanie awatarow dla danych kont
  for(int UserIdx=0;UserIdx<UserIdxCount;UserIdx++)
  {
	PluginLink.CallService(AQQ_FUNCTION_GETNETWORKSTATE,(WPARAM)(&PluginStateChange),UserIdx);
	UnicodeString JID = (wchar_t*)PluginStateChange.JID;
	//Sposob obslugi kont
	if((!AccountsMode)||((AccountsMode)&&(Accounts.Pos(JID))))
	{
	  //Adres e-mail pobierany z wizytowki konta
	  if(!GetMode)
	  {
		TPluginContactSimpleInfo PluginContactSimpleInfo;
		PluginContactSimpleInfo.JID = JID.w_str();
		PluginLink.CallService(AQQ_CONTACTS_FILLSIMPLEINFO,0,(LPARAM)(&PluginContactSimpleInfo));
		AvatarFile = (wchar_t*)(PluginContactSimpleInfo.Mail);
		if(AvatarFile.Pos("\n")) AvatarFile.Delete(AvatarFile.Pos("\n"), AvatarFile.Length());
	  }
	  //Statyczny adres e-mail
	  else AvatarFile = StaticEmail;
	  //Pozytywne pobranie adresu e-mail
	  if(!AvatarFile.IsEmpty())
	  {
		//Zakodowanie adresu e-mail
		AvatarFile = MD5(AvatarFile);
		//Tworzenie nowego pliku w pamieci
		TMemoryStream *MemFile = new TMemoryStream;
		MemFile->Position = 0;
		//Ustalanie domyslnego awataru w adresie URL
		UnicodeString URL;
		switch(DefaultAvatar)
		{
		  case 1:
			URL = "http://www.gravatar.com/avatar/" + AvatarFile + "?d=identicon";
			break;
		  case 2:
			URL = "http://www.gravatar.com/avatar/" + AvatarFile + "?d=wavatar";
			break;
		  case 3:
			URL = "http://www.gravatar.com/avatar/" + AvatarFile + "?d=monsterid";
			break;
		  default:
			URL = "http://www.gravatar.com/avatar/" + AvatarFile;
			break;
		}
		//Pobieranie awatara
		if(IdHTTPGetFileToMem(MemFile,URL))
		{
		  MemFile->Position = 0;
		  //Awatar pobrany prawidlowo
		  if(MemFile->Size!=0)
		  {
			//Zapisywanie pliku na dysku
			AvatarFile = JID + "_" + AvatarFile;
			MemFile->SaveToFile(PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".tmp");
			delete MemFile;
			//Sprawdzanie typu pliku
			int FileFormat = GetFileType((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".tmp").w_str());
			//Zmiana nazwy pliku w stosunku do jego typu
			if((FileFormat==0)||(FileFormat==1))
			{
			  MoveFile((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".tmp").t_str(),(PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".gif.tmp").t_str());
			  AvatarFile = AvatarFile + ".gif";
			}
			else if(FileFormat==2)
			{
			  MoveFile((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".tmp").t_str(),(PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".jpg.tmp").t_str());
			  AvatarFile = AvatarFile + ".jpg";
			}
			else if(FileFormat==3)
			{
			  MoveFile((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".tmp").t_str(),(PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".png.tmp").t_str());
			  AvatarFile = AvatarFile + ".png";
			}
			else if(FileFormat==4)
			{
			  MoveFile((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".tmp").t_str(),(PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".bmp.tmp").t_str());
			  AvatarFile = AvatarFile + ".bmp";
			}
			//Sprawdzanie MD5 pliku
			if((FileFormat!=-1)&&(FileFormat!=-2))
			{
			  if(MD5File((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile))!=MD5File((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".tmp")))
			  {
				//Usuniecie starego awatara
				DeleteFile((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile));
				MoveFile((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".tmp").t_str(),(PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile).t_str());
				//Aktualizacja awatara na serwerze Jabber
				if(!PluginStateChange.FromPlugin)
				{
				  //Uzupelnianie struktury kontaku
				  TPluginContact PluginContact;
				  PluginContact.cbSize = sizeof(PluginContact);
				  PluginContact.JID = JID.w_str();
				  PluginContact.UserIdx = UserIdx;
				  //Uzupelnianie struktury awataru
				  TPluginAvatar PluginAvatar;
				  PluginAvatar.FileName = StringReplace(PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile, "\\\\", "\\", TReplaceFlags() << rfReplaceAll).w_str();//(PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile).w_str();
				  PluginAvatar.XEPEmpty = false;
				  PluginAvatar.SilentMode = true;
				  PluginAvatar.JID = JID.w_str();
				  //Wywolanie aktualizacji
				  int Rtrn = PluginLink.CallService(AQQ_CONTACTS_SET_AVATAR,(WPARAM)&PluginAvatar,(LPARAM)&PluginContact);
				  //Aktualizacja przebiegla prawidlowo
				  if(Rtrn)
				  {
					if(InfoSuccess) ShowUpdateMessage(JID);
                  }
				  //Blad podczas aktualizacji
				  else
				  {
					DeleteFile((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile));
					if(InfoFail) ShowFailMessage(JID);
				  }
				}
				//Aktualizacja awatara we wtyczce
				else
				{
				  //Uzupelnianie struktury awataru
				  TPluginAvatar PluginAvatar;
				  PluginAvatar.FileName = StringReplace(PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile, "\\\\", "\\", TReplaceFlags() << rfReplaceAll).w_str();//(PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile).w_str();
				  PluginAvatar.XEPEmpty = false;
				  PluginAvatar.SilentMode = true;
				  PluginAvatar.JID = JID.w_str();
				  //Uzpelnienie struktury funkcji komunikacyjnej
				  TPluginHook PluginHook;
				  PluginHook.HookName = AQQ_CONTACTS_SET_AVATAR;
				  PluginHook.wParam = (WPARAM)(&PluginAvatar);
				  PluginHook.lParam = 0;
				  //Wywolanie aktualizacji
				  int Rtrn = PluginLink.CallService(AQQ_SYSTEM_SENDHOOK,(WPARAM)(&PluginHook),0);
				  //Aktualizacja przebiegla prawidlowo
				  if(Rtrn==3)
				  {
					if(InfoSuccess) ShowUpdateMessage(JID);
				  }
				  //Blad podczas aktualizacji
				  else
				  {
					DeleteFile((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile));
					if(InfoFail) ShowFailMessage(JID);
				  }
				}
			  }
			  else
			   DeleteFile((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".tmp"));
			}
			else
			{
			  DeleteFile((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".tmp"));
			  if(InfoFail) ShowFailMessage(JID);
			}
		  }
		  //Blad podczas pobierania
		  else
		  {
			delete MemFile;
			if(InfoFail) ShowFailMessage(JID);
		  }
		}
		//Blad podczas pobierania
		else
		{
		  delete MemFile;
		  if(InfoFail) ShowFailMessage(JID);
		}
	  }
	}
  }
  //Wylaczenie AntiFreeze
  hGravatarForm->IdAntiFreeze->Active = false;
}
//---------------------------------------------------------------------------

//Odswiezenie ustawien
void RefreshSettings()
{
  //Odczyt ustawien
  TIniFile *Ini = new TIniFile(GetPluginUserDir() + "\\\\Gravatar\\\\Settings.ini");
  GetMode = Ini->ReadInteger("Settings","GetMode",0);
  StaticEmail = Ini->ReadString("Settings","StaticEmail","");
  DefaultAvatar = Ini->ReadInteger("Settings","DefaultAvatar",0);
  int Interval = Ini->ReadInteger("Settings","Interval",0);
  InfoSuccess = Ini->ReadBool("Settings","InfoSuccess",true);
  InfoFail = Ini->ReadBool("Settings","InfoFail",false);
  AccountsMode = Ini->ReadInteger("Settings","AccountsMode",0);
  Accounts = Ini->ReadString("Settings","Accounts","");
  delete Ini;
  //Zmiana timera
  if(hGravatarForm==NULL)
  {
	Application->Handle = (HWND)GravatarForm;
	hGravatarForm = new TGravatarForm(Application);
  }
  hGravatarForm->Timer->Enabled = false;
  hGravatarForm->Timer->Interval = 3600000 * Interval;
  hGravatarForm->Timer->Enabled = true;
}
//---------------------------------------------------------------------------

//Pobieranie listy kont
void GetAccountList(bool FirstRun)
{
  //Pierwsze uruchomienie
  if(FirstRun)
  {
	if(!hFirstRun)
	{
	  Application->Handle = (HWND)FirstRunForm;
	  hFirstRun = new TFirstRunForm(Application);
	}
	//Usuwanie starej listy kont
	hFirstRun->AccountsCheckListBox->Clear();
	//Informacje na temat kont uzytkownika
	TPluginStateChange PluginStateChange;
	//Pobieranie ilosci kont
	int UserIdxCount = PluginLink.CallService(AQQ_FUNCTION_GETUSEREXCOUNT,0,0);
	//Pobieranie listy kont
	for(int UserIdx=0;UserIdx<UserIdxCount;UserIdx++)
	{
	  PluginLink.CallService(AQQ_FUNCTION_GETNETWORKSTATE,(WPARAM)(&PluginStateChange),UserIdx);
	  hFirstRun->AccountsCheckListBox->Items->Add((wchar_t*)PluginStateChange.JID);
	}
  }
  //Ustawienia wtyczki
  else
  {
	if(!hGravatarForm)
	{
	  Application->Handle = (HWND)GravatarForm;
	  hGravatarForm = new TGravatarForm(Application);
	}
	//Usuwanie starej listy kont
	hGravatarForm->AccountsCheckListBox->Clear();
	//Informacje na temat kont uzytkownika
	TPluginStateChange PluginStateChange;
	//Pobieranie ilosci kont
	int UserIdxCount = PluginLink.CallService(AQQ_FUNCTION_GETUSEREXCOUNT,0,0);
	//Pobieranie listy kont
	for(int UserIdx=0;UserIdx<UserIdxCount;UserIdx++)
	{
	  PluginLink.CallService(AQQ_FUNCTION_GETNETWORKSTATE,(WPARAM)(&PluginStateChange),UserIdx);
	  hGravatarForm->AccountsCheckListBox->Items->Add((wchar_t*)PluginStateChange.JID);
	}
  }
}
//---------------------------------------------------------------------------

int __stdcall OnModulesLoaded (WPARAM wParam, LPARAM lParam)
{
  //Pierwsze uruchomienie
  if(FirstRun)
  {
	if(!hFirstRun)
	{
	  Application->Handle = (HWND)FirstRunForm;
	  hFirstRun = new TFirstRunForm(Application);
	}
	hFirstRun->Show();
  }
  //Normalne uruchomienie
  else
  {
	if(!hGravatarForm)
	{
	  Application->Handle = (HWND)GravatarForm;
	  hGravatarForm = new TGravatarForm(Application);
	}
	hGravatarForm->StartTimer->Enabled = true;
  }

  return 0;
}
//---------------------------------------------------------------------------

extern "C" int __declspec(dllexport) __stdcall Load(PPluginLink Link)
{
  PluginLink = *Link;
  //Sciezka katalogu prywatnego uzytkownika
  UnicodeString Dir = GetPluginUserDir();
  //Tworzenie katalogu dla ustawien
  if(!DirectoryExists(Dir + "\\\\Gravatar"))
   CreateDir(Dir + "\\\\Gravatar");

  if(!FileExists(Dir + "\\\\Gravatar\\\\Settings.ini"))
  {
	FirstRun = true;
  }
  else
  {
	//Odczyt ustawien
	TIniFile *Ini = new TIniFile(Dir + "\\\\Gravatar\\\\Settings.ini");
	GetMode = Ini->ReadInteger("Settings","GetMode",0);
	StaticEmail = Ini->ReadString("Settings","StaticEmail","");
	DefaultAvatar = Ini->ReadInteger("Settings","DefaultAvatar",0);
	int Interval = Ini->ReadInteger("Settings","Interval",0);
	if(hGravatarForm==NULL)
	{
	  Application->Handle = (HWND)GravatarForm;
	  hGravatarForm = new TGravatarForm(Application);
	}
	hGravatarForm->Timer->Interval = 3600000 * Interval;
	InfoSuccess = Ini->ReadBool("Settings","InfoSuccess",true);
	InfoFail = Ini->ReadBool("Settings","InfoFail",false);
	AccountsMode = Ini->ReadInteger("Settings","AccountsMode",0);
	Accounts = Ini->ReadString("Settings","Accounts","");
	delete Ini;
  }
  //Hook na zmianê kompozycji
  PluginLink.HookEvent(AQQ_SYSTEM_THEMECHANGED, OnThemeChanged);
  //Hook na zaladowanie wszystkich modulow w AQQ
  PluginLink.HookEvent(AQQ_SYSTEM_MODULESLOADED,OnModulesLoaded);
  //Wszystkie moduly juz zaladowane
  if(PluginLink.CallService(AQQ_SYSTEM_MODULESLOADED,0,0))
  {
	//Pierwsze uruchomienie
	if(FirstRun)
	{
	  if(!hFirstRun)
	  {
		Application->Handle = (HWND)FirstRunForm;
		hFirstRun = new TFirstRunForm(Application);
	  }
	  hFirstRun->Show();
	}
	//Normalne uruchomienie
	else
	{
	  if(!hGravatarForm)
	  {
		Application->Handle = (HWND)GravatarForm;
		hGravatarForm = new TGravatarForm(Application);
	  }
	  hGravatarForm->StartTimer->Enabled = true;
	}
  }

  return 0;
}
//---------------------------------------------------------------------------

extern "C" int __declspec(dllexport)__stdcall Settings()
{
  if(!hGravatarForm)
  {
	Application->Handle = (HWND)GravatarForm;
	hGravatarForm = new TGravatarForm(Application);
  }
  hGravatarForm->Show();

  return 0;
}
//---------------------------------------------------------------------------

extern "C" int __declspec(dllexport) __stdcall Unload()
{
  //Anty "Abnormal program termination"
  hGravatarForm->IdHTTP->Disconnect();
  //Wyladowanie hookow
  PluginLink.UnhookEvent(OnThemeChanged);
  PluginLink.UnhookEvent(OnModulesLoaded);

  return 0;
}
//---------------------------------------------------------------------------

extern "C" __declspec(dllexport) PPluginInfo __stdcall AQQPluginInfo(DWORD AQQVersion)
{
  PluginInfo.cbSize = sizeof(TPluginInfo);
  PluginInfo.ShortName = (wchar_t*)L"Gravatar";
  PluginInfo.Version = PLUGIN_MAKE_VERSION(1,1,0,0);
  PluginInfo.Description = (wchar_t*)L"Aktualizacja awatarów na podstawie danych w serwisie gravatar.com";
  PluginInfo.Author = (wchar_t*)L"Krzysztof Grochocki (Beherit)";
  PluginInfo.AuthorMail = (wchar_t*)L"kontakt@beherit.pl";
  PluginInfo.Copyright = (wchar_t*)L"Krzysztof Grochocki (Beherit)";
  PluginInfo.Homepage = (wchar_t*)L"http://beherit.pl";

  return &PluginInfo;
}
//---------------------------------------------------------------------------

