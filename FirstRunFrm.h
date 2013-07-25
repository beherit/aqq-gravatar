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
#include "sSkinManager.hpp"
#include "sSkinProvider.hpp"
#include "sBevel.hpp"
#include "sButton.hpp"
#include "sCheckBox.hpp"
#include "sComboBox.hpp"
#include "sEdit.hpp"
#include "sLabel.hpp"
#include "sPanel.hpp"
#include "sRadioButton.hpp"
#include "sCheckListBox.hpp"
#include "sListBox.hpp"
#include "IdBaseComponent.hpp"
#include "IdThreadComponent.hpp"
//---------------------------------------------------------------------------
class TFirstRunForm : public TForm
{
__published:	// IDE-managed Components
	TsBevel *Bevel;
	TsPanel *Panel1;
	TsButton *NextButton;
	TsButton *PrevButton;
	TsButton *SaveButton;
	TActionList *ActionList;
	TAction *aSaveSettings;
	TsLabel *WelcomeLabel1;
	TImage *GravatarImage;
	TsLabel *WelcomeLabel2;
	TsLabel *WelcomeLabel3;
	TsPanel *Panel2;
	TsLabel *GetModeLabel;
	TsRadioButton *GetMode0RadioButton;
	TsRadioButton *GetMode1RadioButton;
	TsEdit *EmailEdit;
	TsPanel *Panel3;
	TsLabel *DefaultAvatarLabel;
	TsRadioButton *DefaultAvatarRadioButton0;
	TImage *DefaultAvatarImage0;
	TsRadioButton *DefaultAvatarRadioButton1;
	TImage *DefaultAvatarImage1;
	TsRadioButton *DefaultAvatarRadioButton2;
	TImage *DefaultAvatarImage2;
	TImage *DefaultAvatarImage3;
	TsRadioButton *DefaultAvatarRadioButton3;
	TsPanel *Panel4;
	TsLabel *IntervalLabel;
	TsComboBox *IntervalComboBox;
	TsCheckBox *InfoSuccessCheckBox;
	TsPanel *Panel5;
	TsBevel *DefaultAvatarBevel0;
	TsBevel *DefaultAvatarBevel2;
	TsBevel *DefaultAvatarBevel3;
	TsBevel *DefaultAvatarBevel1;
	TsLabel *InfoLabel;
	TsCheckBox *InfoFailCheckBox;
	TsLabel *AccountsLabel;
	TsRadioButton *AccountsMode0RadioButton;
	TsRadioButton *AccountsMode1RadioButton;
	TsCheckListBox *AccountsCheckListBox;
	TsPanel *Panel6;
	TsLabel *EndLabel1;
	TsLabel *EndLabel2;
	TImage *GravatarImage2;
	TsLabel *EndLabel3;
	TsSkinManager *sSkinManager;
	TsSkinProvider *sSkinProvider;
	TAction *aNextButton;
	TAction *aPrevButton;
	TAction *aDefaultAvatar0;
	TAction *aDefaultAvatar1;
	TAction *aDefaultAvatar2;
	TAction *aDefaultAvatar3;
	void __fastcall SaveButtonClick(TObject *Sender);
	void __fastcall GetMode1RadioButtonClick(TObject *Sender);
	void __fastcall GetMode0RadioButtonClick(TObject *Sender);
	void __fastcall DefaultAvatarImage0Click(TObject *Sender);
	void __fastcall DefaultAvatarImage1Click(TObject *Sender);
	void __fastcall DefaultAvatarImage2Click(TObject *Sender);
	void __fastcall DefaultAvatarImage3Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall AccountsMode1RadioButtonClick(TObject *Sender);
	void __fastcall AccountsMode0RadioButtonClick(TObject *Sender);
	void __fastcall aSaveSettingsExecute(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall aNextButtonExecute(TObject *Sender);
	void __fastcall aPrevButtonExecute(TObject *Sender);
	void __fastcall aDefaultAvatar0Execute(TObject *Sender);
	void __fastcall aDefaultAvatar1Execute(TObject *Sender);
	void __fastcall aDefaultAvatar2Execute(TObject *Sender);
	void __fastcall aDefaultAvatar3Execute(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFirstRunForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFirstRunForm *FirstRunForm;
//---------------------------------------------------------------------------
#endif
