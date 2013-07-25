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

TGravatarForm *hGravatarForm;
TFirstRunForm *hFirstRun;

TPluginLink PluginLink;
TPluginInfo PluginInfo;

int GetMode;
UnicodeString StaticEmail;
int DefaultAvatar;
int Interval;
bool InfoSuccess;
bool InfoFail;
int AccountsMode;
UnicodeString Accounts;

bool FirstRun;

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

extern "C" __declspec(dllexport) PPluginInfo __stdcall AQQPluginInfo(DWORD AQQVersion)
{
  //Sprawdzanie wersji AQQ
  /*if(PLUGIN_COMPARE_VERSION(AQQVersion,PLUGIN_MAKE_VERSION(2,2,0,61))<0)
  {
	MessageBox(Application->Handle,
	  "Wymagana wesja AQQ przez wtyczkê to minimum 2.2.0.61!\n"
	  "Wtyczka Gravatar nie bêdzie dzia³aæ poprawnie!",
	  "Nieprawid³owa wersja AQQ",
	  MB_OK | MB_ICONEXCLAMATION);
  }*/
  PluginInfo.cbSize = sizeof(TPluginInfo);
  PluginInfo.ShortName = (wchar_t*)L"Gravatar";
  PluginInfo.Version = PLUGIN_MAKE_VERSION(1,0,0,0);
  PluginInfo.Description = (wchar_t*)L"Aktualizacja awatarów na podstawie danych w serwisie gravatar.com";
  PluginInfo.Author = (wchar_t*)L"Krzysztof Grochocki (Beherit)";
  PluginInfo.AuthorMail = (wchar_t*)L"email@beherit.pl";
  PluginInfo.Copyright = (wchar_t*)L"Krzysztof Grochocki (Beherit)";
  PluginInfo.Homepage = (wchar_t*)L"http://beherit.pl";

  return &PluginInfo;
}
//---------------------------------------------------------------------------

String __fastcall MD5(const String Text)
{
  boost::scoped_ptr<TIdHashMessageDigest5> idmd5(new TIdHashMessageDigest5());

  return idmd5->HashStringAsHex(Text.LowerCase().Trim()).LowerCase().Trim();
}
//---------------------------------------------------------------------------

String __fastcall MD5File(const String FileName)
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

void ShowUpdateMessage(UnicodeString AccountJID)
{
  UnicodeString IconPath = (wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETPNG_FILEPATH,19,0));

  TPluginShowInfo PluginShowInfo;
  PluginShowInfo.cbSize = sizeof(TPluginShowInfo);
  PluginShowInfo.Event = tmeInfo;
  PluginShowInfo.Text = ("Zaktualizowano awatar dla konta " + AccountJID).w_str();
  PluginShowInfo.ImagePath = IconPath.w_str();
  PluginShowInfo.TimeOut = 1000 * 6;
  PluginLink.CallService(AQQ_FUNCTION_SHOWINFO,0,(LPARAM)(&PluginShowInfo));
}
//---------------------------------------------------------------------------

void ShowFailMessage(UnicodeString AccountJID)
{
  UnicodeString IconPath = (wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETPNG_FILEPATH,85,0));

  TPluginShowInfo PluginShowInfo;
  PluginShowInfo.cbSize = sizeof(TPluginShowInfo);
  PluginShowInfo.Event = tmeInfo;
  PluginShowInfo.Text = ("Wyst¹pi³ b³¹d podczas aktualizacji awatara dla konta " + AccountJID).w_str();
  PluginShowInfo.ImagePath = IconPath.w_str();
  PluginShowInfo.TimeOut = 1000 * 6;
  PluginLink.CallService(AQQ_FUNCTION_SHOWINFO,0,(LPARAM)(&PluginShowInfo));
}
//---------------------------------------------------------------------------

void RefreshAvatars()
{
  TPluginContact PluginContact;
  TPluginAvatar PluginAvatar;
  TPluginContactSimpleInfo PluginContactSimpleInfo;
  TPluginStateChange PluginStateChange;

  //Czy konto pochodzi z wtyczki
  bool FromPlugin;
  //Czy poprawno ustawiono awatar
  bool Rtrn;
  //Email w MD5
  UnicodeString AvatarFile;
  //JID konta AQQ
  UnicodeString JID;
  //Ilosc kont w AQQ
  int UserExCount;
  //Indeks konta
  int UserEx;
  //Sprawdzanie formatu pliku
  int FileFormat;

  //Sciezka folderu prywatnego wtyczek
  UnicodeString PluginDir = (wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETPLUGINUSERDIR,(WPARAM)(HInstance),0));
  PluginDir = StringReplace(PluginDir, "\\", "\\\\", TReplaceFlags() << rfReplaceAll);

  //Folder ustawieñ
  if(!DirectoryExists(PluginDir + "\\\\Gravatar\\\\Avatars"))
   CreateDir(PluginDir + "\\\\Gravatar\\\\Avatars");

  if(hGravatarForm==NULL)
  {
	Application->Handle = (HWND)GravatarForm;
	hGravatarForm = new TGravatarForm(Application);
  }

  UserExCount = PluginLink.CallService(AQQ_FUNCTION_GETUSEREXCOUNT,0,0);

  for(UserEx=0;UserEx<UserExCount;UserEx++)
  {
	PluginLink.CallService(AQQ_FUNCTION_GETNETWORKSTATE,(WPARAM)(&PluginStateChange),UserEx);
	FromPlugin = PluginStateChange.FromPlugin;

	if(FromPlugin==false)
	{
	  JID = (wchar_t*)PluginStateChange.JID;

	  if((AccountsMode==0)||((AccountsMode==1)&&(AnsiPos(JID,Accounts)>0)))
	  {
		if(GetMode==0)
		{
		  PluginContactSimpleInfo.JID=JID.w_str();
		  PluginLink.CallService(AQQ_CONTACTS_FILLSIMPLEINFO,0,(LPARAM)(&PluginContactSimpleInfo));
		  AvatarFile = (wchar_t*)(PluginContactSimpleInfo.Mail);

		  if(AnsiPos("\n",AvatarFile)>0)
		   AvatarFile.Delete(AnsiPos("\n", AvatarFile), AvatarFile.Length());
		}
		else if(GetMode==1)
		 AvatarFile = StaticEmail;

		if(AvatarFile!="")
		{
		  AvatarFile = MD5(AvatarFile);

		  TFileStream *F = new TFileStream((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + JID + "_" + AvatarFile + ".tmp") , fmCreate);  try
		  {
			switch(DefaultAvatar)
			{
			  case 1:
				hGravatarForm->IdHTTP->Get("http://www.gravatar.com/avatar/" + AvatarFile + "?d=identicon", F);
				break;
			  case 2:
				hGravatarForm->IdHTTP->Get("http://www.gravatar.com/avatar/" + AvatarFile + "?d=wavatar", F);
				break;
			  case 3:
				hGravatarForm->IdHTTP->Get("http://www.gravatar.com/avatar/" + AvatarFile + "?d=monsterid", F);
				break;
			  default:
				hGravatarForm->IdHTTP->Get("http://www.gravatar.com/avatar/" + AvatarFile, F);
				break;
			}
		  }
		  catch (...)
		  {
			//Blad odczytu
		  }
		  delete F;

		  AvatarFile = JID + "_" + AvatarFile;

		  FileFormat = GetFileType((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".tmp").w_str());

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

		  if((FileFormat!=-1)&&(FileFormat!=-2))
		  {
			if(MD5File((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile))!=MD5File((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".tmp")))
			{
			  DeleteFile((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile));
			  MoveFile((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".tmp").t_str(),(PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile).t_str());

			  PluginContact.cbSize = sizeof(PluginContact);
			  PluginContact.JID = JID.w_str();
			  PluginContact.UserIdx = 0;

			  PluginAvatar.FileName = (PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile).w_str();
			  PluginAvatar.XEPEmpty = false;
			  PluginAvatar.SilentMode = true;

			  Rtrn = PluginLink.CallService(AQQ_CONTACTS_SET_AVATAR,(WPARAM)&PluginAvatar,(LPARAM)&PluginContact);

			  if(Rtrn==false)
			  {
				DeleteFile((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile));
				if(InfoFail)
				 ShowFailMessage(JID);
			  }
			  else
			  {
				if(InfoSuccess)
				 ShowUpdateMessage(JID);
			  }
			}
			else
			 DeleteFile((PluginDir + "\\\\Gravatar\\\\Avatars\\\\" + AvatarFile + ".tmp"));
		  }
		}
	  }
	}
  }
}
//---------------------------------------------------------------------------

int __stdcall OnModulesLoaded (WPARAM wParam, LPARAM lParam)
{
  if(!FirstRun)
  {
	if(hGravatarForm==NULL)
	{
	  Application->Handle = (HWND)GravatarForm;
	  hGravatarForm = new TGravatarForm(Application);
	}
	hGravatarForm->StartTimer->Enabled=true;
  }
  else
  {
	if(hFirstRun==NULL)
	{
	  Application->Handle = (HWND)FirstRunForm;
	  hFirstRun = new TFirstRunForm(Application);
	}
	hFirstRun->Show();
  }

  return 0;
}
//---------------------------------------------------------------------------

extern "C" int __declspec(dllexport) __stdcall Load(PPluginLink Link)
{
  PluginLink = *Link;

  //Sciezka katalogu prywatnego uzytkownika
  UnicodeString Dir = (wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETPLUGINUSERDIR,(WPARAM)(HInstance),0));
  Dir = StringReplace(Dir, "\\", "\\\\", TReplaceFlags() << rfReplaceAll);

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
	Interval = Ini->ReadInteger("Settings","Interval",0);
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

  //Hook na zaladowanie wszystkich modulow w AQQ
  PluginLink.HookEvent(AQQ_SYSTEM_MODULESLOADED,OnModulesLoaded);

  //Jak AQQ jest juz zaladowane
  if(PluginLink.CallService(AQQ_SYSTEM_MODULESLOADED,0,0)==true)
  {
	if(!FirstRun)
	{
	  RefreshAvatars();

	  if(hGravatarForm==NULL)
	  {
		Application->Handle = (HWND)GravatarForm;
		hGravatarForm = new TGravatarForm(Application);
	  }
	  hGravatarForm->Timer->Enabled=true;
	}
	else
	{
	  if(hFirstRun==NULL)
	  {
		Application->Handle = (HWND)FirstRunForm;
		hFirstRun = new TFirstRunForm(Application);
	  }
	  hFirstRun->Show();
	}
  }

  return 0;
}
//---------------------------------------------------------------------------

extern "C" int __declspec(dllexport)__stdcall Settings()
{
  if(hGravatarForm==NULL)
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
  PluginLink.UnhookEvent(OnModulesLoaded);

  return 0;
}
//---------------------------------------------------------------------------

//Pobieranie sciezki katalogu prywatnego uzytkownika
UnicodeString GetPluginUserDir()
{
  UnicodeString Dir = (wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETPLUGINUSERDIR,(WPARAM)(HInstance),0));
  Dir = StringReplace(Dir, "\\", "\\\\", TReplaceFlags() << rfReplaceAll);
  return Dir;
}
//---------------------------------------------------------------------------

//Pobieranie sciezki katalogu prywatnego uzytkownika
UnicodeString GetPluginUserDirW()
{
  UnicodeString Dir = (wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETPLUGINUSERDIR,(WPARAM)(HInstance),0));
  //Dir = StringReplace(Dir, "\\", "\\\\", TReplaceFlags() << rfReplaceAll);
  return Dir;
}
//---------------------------------------------------------------------------


void RefreshSettings()
{
  //Sciezka katalogu prywatnego uzytkownika
  UnicodeString Dir = (wchar_t*)(PluginLink.CallService(AQQ_FUNCTION_GETPLUGINUSERDIR,(WPARAM)(HInstance),0));
  Dir = StringReplace(Dir, "\\", "\\\\", TReplaceFlags() << rfReplaceAll);

  //Odczyt ustawien
  TIniFile *Ini = new TIniFile(Dir + "\\\\Gravatar\\\\Settings.ini");
  GetMode = Ini->ReadInteger("Settings","GetMode",0);
  StaticEmail = Ini->ReadString("Settings","StaticEmail","");
  DefaultAvatar = Ini->ReadInteger("Settings","DefaultAvatar",0);
  Interval = Ini->ReadInteger("Settings","Interval",0);
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
  hGravatarForm->Timer->Enabled=false;
  hGravatarForm->Timer->Interval = 3600000 * Interval;
  hGravatarForm->Timer->Enabled=true;

}
//---------------------------------------------------------------------------

void GetAccountList()
{
  if(hGravatarForm==NULL)
  {
	Application->Handle = (HWND)GravatarForm;
	hGravatarForm = new TGravatarForm(Application);
  }
  hGravatarForm->AccountsCheckListBox->Clear();

  TPluginStateChange PluginStateChange;

  //Ilosc kont w AQQ
  int UserExCount;
  //Indeks konta
  int UserEx;
  //Nazwa konta (JID)
  UnicodeString JID;
  //Czy to konto z wtyczki?
  bool FromPlugin;

  UserExCount = PluginLink.CallService(AQQ_FUNCTION_GETUSEREXCOUNT,0,0);

  for(UserEx=0;UserEx<UserExCount;UserEx++)
  {
	PluginLink.CallService(AQQ_FUNCTION_GETNETWORKSTATE,(WPARAM)(&PluginStateChange),UserEx);
	FromPlugin = PluginStateChange.FromPlugin;

	if(FromPlugin==false)
	{
	  JID = (wchar_t*)PluginStateChange.JID;
	  hGravatarForm->AccountsCheckListBox->Items->Add(JID);
	}
  }
}
//---------------------------------------------------------------------------

void GetAccountList_FR()
{
  if(hFirstRun==NULL)
  {
	Application->Handle = (HWND)FirstRunForm;
	hFirstRun = new TFirstRunForm(Application);
  }

  TPluginStateChange PluginStateChange;

  //Ilosc kont w AQQ
  int UserExCount;
  //Indeks konta
  int UserEx;
  //Nazwa konta (JID)
  UnicodeString JID;
  //Czy to konto z wtyczki?
  bool FromPlugin;

  UserExCount = PluginLink.CallService(AQQ_FUNCTION_GETUSEREXCOUNT,0,0);

  for(UserEx=0;UserEx<UserExCount;UserEx++)
  {
	PluginLink.CallService(AQQ_FUNCTION_GETNETWORKSTATE,(WPARAM)(&PluginStateChange),UserEx);
	FromPlugin = PluginStateChange.FromPlugin;

	if(FromPlugin==false)
	{
	  JID = (wchar_t*)PluginStateChange.JID;
      hFirstRun->AccountsCheckListBox->Items->Add(JID);
	}
  }
}
//---------------------------------------------------------------------------


