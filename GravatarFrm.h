//---------------------------------------------------------------------------
#ifndef GravatarFrmH
#define GravatarFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdHTTP.hpp"
#include "IdTCPClient.hpp"
#include "IdTCPConnection.hpp"
#include "LMDPNGImage.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <ActnList.hpp>
#include <pngimage.hpp>
#include <CheckLst.hpp>
#include "sSkinManager.hpp"
#include "sSkinProvider.hpp"
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
#include "IdThreadComponent.hpp"
#include "IdCoder.hpp"
#include "IdCoder3to4.hpp"
#include "IdCoderMIME.hpp"
#include "IdAntiFreeze.hpp"
#include "IdAntiFreezeBase.hpp"
//---------------------------------------------------------------------------
class TGravatarForm : public TForm
{
__published:	// IDE-managed Components
	TIdHTTP *IdHTTP;
	TsPageControl *PageControl;
	TsTabSheet *DefaultAvatarTabSheet;
	TsButton *SaveButton;
	TsBevel *Bevel;
	TActionList *ActionList1;
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
	TAction *aPageControlSheetChange;
	TAction *aAllowApply;
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
	void __fastcall aPageControlSheetChangeExecute(TObject *Sender);
	void __fastcall aAllowApplyExecute(TObject *Sender);
private:	// User declarations
	void __fastcall DeleteFiles(UnicodeString DirName, UnicodeString Prefix);
public:		// User declarations
	__fastcall TGravatarForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGravatarForm *GravatarForm;
//---------------------------------------------------------------------------
#endif
