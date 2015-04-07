//---------------------------------------------------------------------------
// Copyright (C) 2010-2015 Krzysztof Grochocki
//
// This file is part of Gravatar
//
// Gravatar is free software: you can redistribute it and/or modify
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
// along with GNU Radio. If not, see <http://www.gnu.org/licenses/>.
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef GravatarFrmH
#define GravatarFrmH
#define WM_ALPHAWINDOWS (WM_USER + 666)
//---------------------------------------------------------------------------
#include "acPNG.hpp"
#include "sBevel.hpp"
#include "sButton.hpp"
#include "sCheckBox.hpp"
#include "sCheckListBox.hpp"
#include "sComboBox.hpp"
#include "sEdit.hpp"
#include "sLabel.hpp"
#include "sListBox.hpp"
#include "sPageControl.hpp"
#include "sRadioButton.hpp"
#include "sSkinManager.hpp"
#include "sSkinProvider.hpp"
#include <IdAntiFreezeBase.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <System.Actions.hpp>
#include <System.Classes.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.IdAntiFreeze.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------
class TGravatarForm : public TForm
{
__published:	// IDE-managed Components
	TIdHTTP *IdHTTP;
	TsPageControl *PageControl;
	TsTabSheet *DefaultAvatarTabSheet;
	TsButton *SaveButton;
	TsBevel *Bevel;
	TActionList *ActionList;
	TAction *aExit;
	TAction *aLoadSettings;
	TAction *aSaveSettings;
	TsTabSheet *EmailTabSheet;
	TsEdit *EmailEdit;
	TsRadioButton *GetMode0RadioButton;
	TsRadioButton *GetMode1RadioButton;
	TsLabel *GetModeLabel;
	TsBevel *DefaultAvatarBevel0;
	TsBevel *DefaultAvatarBevel1;
	TsBevel *DefaultAvatarBevel2;
	TsBevel *DefaultAvatarBevel3;
	TImage *DefaultAvatarImage0;
	TImage *DefaultAvatarImage1;
	TImage *DefaultAvatarImage2;
	TImage *DefaultAvatarImage3;
	TsLabel *DefaultAvatarLabel;
	TsRadioButton *DefaultAvatarRadioButton0;
	TsRadioButton *DefaultAvatarRadioButton1;
	TsRadioButton *DefaultAvatarRadioButton2;
	TsRadioButton *DefaultAvatarRadioButton3;
	TsTabSheet *UpdateTabSheet;
	TsCheckBox *InfoFailCheckBox;
	TsLabel *InfoLabel;
	TsCheckBox *InfoSuccessCheckBox;
	TsComboBox *IntervalComboBox;
	TsLabel *IntervalLabel;
	TsTabSheet *AccountsTabSheet;
	TsCheckListBox *AccountsCheckListBox;
	TsLabel *AccountsLabel;
	TsRadioButton *AccountsMode0RadioButton;
	TsRadioButton *AccountsMode1RadioButton;
	TsBevel *LineBevel;
	TTimer *Timer;
	TTimer *StartTimer;
	TsSkinManager *sSkinManager;
	TsSkinProvider *sSkinProvider;
	TsButton *CancelButton;
	TsButton *OKButton;
	TAction *aDefaultAvatar0;
	TAction *aDefaultAvatar1;
	TAction *aDefaultAvatar2;
	TAction *aDefaultAvatar3;
	TAction *aAllowApply;
	TsCheckBox *ForceUpdateCheckBox;
	TIdAntiFreeze *IdAntiFreeze;
	void __fastcall SaveButtonClick(TObject *Sender);
	void __fastcall aExitExecute(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall aLoadSettingsExecute(TObject *Sender);
	void __fastcall aSaveSettingsExecute(TObject *Sender);
	void __fastcall DefaultAvatarImage0Click(TObject *Sender);
	void __fastcall DefaultAvatarImage1Click(TObject *Sender);
	void __fastcall DefaultAvatarImage2Click(TObject *Sender);
	void __fastcall DefaultAvatarImage3Click(TObject *Sender);
	void __fastcall GetMode0RadioButtonClick(TObject *Sender);
	void __fastcall GetMode1RadioButtonClick(TObject *Sender);
	void __fastcall AccountsMode0RadioButtonClick(TObject *Sender);
	void __fastcall AccountsMode1RadioButtonClick(TObject *Sender);
	void __fastcall StartTimerTimer(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall aDefaultAvatar0Execute(TObject *Sender);
	void __fastcall aDefaultAvatar1Execute(TObject *Sender);
	void __fastcall aDefaultAvatar2Execute(TObject *Sender);
	void __fastcall aDefaultAvatar3Execute(TObject *Sender);
	void __fastcall OKButtonClick(TObject *Sender);
	void __fastcall aAllowApplyExecute(TObject *Sender);
	void __fastcall ForceUpdateCheckBoxClick(TObject *Sender);
	void __fastcall sSkinManagerSysDlgInit(TacSysDlgData DlgData, bool &AllowSkinning);
private:	// User declarations
	void __fastcall DeleteFiles(UnicodeString DirName, UnicodeString Prefix);
public:		// User declarations
	__fastcall TGravatarForm(TComponent* Owner);
	void __fastcall WMTransparency(TMessage &Message);
	BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_ALPHAWINDOWS,TMessage,WMTransparency);
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TGravatarForm *GravatarForm;
//---------------------------------------------------------------------------
#endif
