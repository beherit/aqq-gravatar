//---------------------------------------------------------------------------
#ifndef FirstRunFrmH
#define FirstRunFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include "LMDPNGImage.hpp"
#include <CheckLst.hpp>
#include <pngimage.hpp>
//---------------------------------------------------------------------------
class TFirstRunForm : public TForm
{
__published:	// IDE-managed Components
	TBevel *Bevel;
	TPanel *Panel1;
	TButton *NextButton;
	TButton *PrevButton;
	TButton *SaveButton;
	TActionList *ActionList1;
	TAction *aSaveSettings;
	TLabel *WelcomeLabel1;
	TImage *GravatarImage;
	TLabel *WelcomeLabel2;
	TLabel *WelcomeLabel3;
	TPanel *Panel2;
	TLabel *GetModeLabel;
	TRadioButton *GetMode0RadioButton;
	TRadioButton *GetMode1RadioButton;
	TEdit *EmailEdit;
	TPanel *Panel3;
	TLabel *DefaultAvatarLabel;
	TRadioButton *DefaultAvatarRadioButton0;
	TImage *DefaultAvatarImage0;
	TRadioButton *DefaultAvatarRadioButton1;
	TImage *DefaultAvatarImage1;
	TRadioButton *DefaultAvatarRadioButton2;
	TImage *DefaultAvatarImage2;
	TImage *DefaultAvatarImage3;
	TRadioButton *DefaultAvatarRadioButton3;
	TPanel *Panel4;
	TLabel *IntervalLabel;
	TComboBox *IntervalComboBox;
	TCheckBox *InfoSuccessCheckBox;
	TPanel *Panel5;
	TBevel *DefaultAvatarBevel0;
	TBevel *DefaultAvatarBevel2;
	TBevel *DefaultAvatarBevel3;
	TBevel *DefaultAvatarBevel1;
	TLabel *InfoLabel;
	TCheckBox *InfoFailCheckBox;
	TLabel *AccountsLabel;
	TRadioButton *AccountsMode0RadioButton;
	TRadioButton *AccountsMode1RadioButton;
	TCheckListBox *AccountsCheckListBox;
	TPanel *Panel6;
	TLabel *EndLabel1;
	TLabel *EndLabel2;
	TImage *GravatarImage2;
	TLabel *EndLabel3;
	void __fastcall SaveButtonClick(TObject *Sender);
	void __fastcall NextButtonClick(TObject *Sender);
	void __fastcall PrevButtonClick(TObject *Sender);
	void __fastcall GetMode1RadioButtonClick(TObject *Sender);
	void __fastcall GetMode0RadioButtonClick(TObject *Sender);
	void __fastcall DefaultAvatarRadioButton0Click(TObject *Sender);
	void __fastcall DefaultAvatarRadioButton1Click(TObject *Sender);
	void __fastcall DefaultAvatarRadioButton2Click(TObject *Sender);
	void __fastcall DefaultAvatarRadioButton3Click(TObject *Sender);
	void __fastcall DefaultAvatarImage0Click(TObject *Sender);
	void __fastcall DefaultAvatarImage1Click(TObject *Sender);
	void __fastcall DefaultAvatarImage2Click(TObject *Sender);
	void __fastcall DefaultAvatarImage3Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall AccountsMode1RadioButtonClick(TObject *Sender);
	void __fastcall AccountsMode0RadioButtonClick(TObject *Sender);
	void __fastcall aSaveSettingsExecute(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFirstRunForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFirstRunForm *FirstRunForm;
//---------------------------------------------------------------------------
#endif
