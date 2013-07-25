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
//---------------------------------------------------------------------------
class TGravatarForm : public TForm
{
__published:	// IDE-managed Components
	TIdHTTP *IdHTTP;
	TPageControl *PageControl;
	TTabSheet *DefaultAvatarTabSheet;
	TButton *SaveButton;
	TBevel *Bevel;
	TActionList *ActionList1;
	TAction *aExit;
	TAction *aReadSettings;
	TAction *aSaveSettings;
	TTabSheet *TabSheet1;
	TEdit *EmailEdit;
	TRadioButton *GetMode0RadioButton;
	TRadioButton *GetMode1RadioButton;
	TLabel *GetModeLabel;
	TBevel *DefaultAvatarBevel0;
	TBevel *DefaultAvatarBevel1;
	TBevel *DefaultAvatarBevel2;
	TBevel *DefaultAvatarBevel3;
	TImage *DefaultAvatarImage0;
	TImage *DefaultAvatarImage1;
	TImage *DefaultAvatarImage2;
	TImage *DefaultAvatarImage3;
	TLabel *DefaultAvatarLabel;
	TRadioButton *DefaultAvatarRadioButton0;
	TRadioButton *DefaultAvatarRadioButton1;
	TRadioButton *DefaultAvatarRadioButton2;
	TRadioButton *DefaultAvatarRadioButton3;
	TTabSheet *TabSheet2;
	TCheckBox *InfoFailCheckBox;
	TLabel *InfoLabel;
	TCheckBox *InfoSuccessCheckBox;
	TComboBox *IntervalComboBox;
	TLabel *IntervalLabel;
	TTabSheet *TabSheet3;
	TCheckListBox *AccountsCheckListBox;
	TLabel *AccountsLabel;
	TRadioButton *AccountsMode0RadioButton;
	TRadioButton *AccountsMode1RadioButton;
	TBevel *LineBevel;
	TTimer *Timer;
	TTimer *StartTimer;
	TAction *aClearAvatars;
	void __fastcall SaveButtonClick(TObject *Sender);
	void __fastcall aExitExecute(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall aReadSettingsExecute(TObject *Sender);
	void __fastcall aSaveSettingsExecute(TObject *Sender);
	void __fastcall DefaultAvatarRadioButton0Click(TObject *Sender);
	void __fastcall DefaultAvatarRadioButton1Click(TObject *Sender);
	void __fastcall DefaultAvatarRadioButton2Click(TObject *Sender);
	void __fastcall DefaultAvatarRadioButton3Click(TObject *Sender);
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
	void __fastcall aClearAvatarsExecute(TObject *Sender);
private:	// User declarations
	void __fastcall DeleteFiles(UnicodeString DirName, UnicodeString Prefix);
public:		// User declarations
	__fastcall TGravatarForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGravatarForm *GravatarForm;
//---------------------------------------------------------------------------
#endif
