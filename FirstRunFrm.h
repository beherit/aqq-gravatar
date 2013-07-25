//---------------------------------------------------------------------------
#ifndef FirstRunFrmH
#define FirstRunFrmH
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
#include "sPanel.hpp"
#include "sRadioButton.hpp"
#include "sSkinManager.hpp"
#include "sSkinProvider.hpp"
#include <System.Actions.hpp>
#include <System.Classes.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.StdCtrls.hpp>
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
	TsRadioButton *DefaultAvatarRadioButton1;
	TsRadioButton *DefaultAvatarRadioButton2;
	TsRadioButton *DefaultAvatarRadioButton3;
	TsPanel *Panel4;
	TsLabel *IntervalLabel;
	TsComboBox *IntervalComboBox;
	TsCheckBox *InfoSuccessCheckBox;
	TsPanel *Panel5;
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
	TsCheckBox *ForceUpdateCheckBox;
	TsBevel *DefaultAvatarBevel0;
	TsBevel *DefaultAvatarBevel1;
	TsBevel *DefaultAvatarBevel2;
	TsBevel *DefaultAvatarBevel3;
	TImage *DefaultAvatarImage0;
	TImage *DefaultAvatarImage1;
	TImage *DefaultAvatarImage2;
	TImage *DefaultAvatarImage3;
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
	void __fastcall ForceUpdateCheckBoxClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFirstRunForm(TComponent* Owner);
	void __fastcall WMTransparency(TMessage &Message);
	BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_ALPHAWINDOWS,TMessage,WMTransparency);
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TFirstRunForm *FirstRunForm;
//---------------------------------------------------------------------------
#endif
