// generated by Fast Light User Interface Designer (fluid) version 1.0300

#ifndef arqdialogs_h
#define arqdialogs_h
#include <FL/Fl.H>
#include "flinput2.h"
#include "flslider2.h"
#include "combo.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
extern Fl_Menu_Bar *mnu;
extern void ComposeMail();
extern void cbMenuConfig();
extern void cbMenuAbout();
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
extern Fl_Button *btnCONNECT;
extern Fl_Input2 *txtURCALL;
#include <FL/Fl_Light_Button.H>
extern Fl_Light_Button *btnBEACON;
extern Fl_Input2 *txtBeaconing;
#include <FL/Fl_Box.H>
extern Fl_Box *indCONNECT;
extern Fl_Input2 *txtState;
#include <FL/Fl_Text_Display.H>
extern Fl_Text_Display *txtARQ;
extern Fl_Input2 *txtStatus;
extern Fl_Input2 *txtStatus2;
#include <FL/Fl_Progress.H>
extern Fl_Progress *prgStatus;
extern Fl_Button *btnClearText;
extern Fl_Text_Display *txtRX;
extern Fl_Input2 *txtTX;
extern Fl_Button *btnSendTalk;
Fl_Double_Window* arq_dialog();
extern Fl_Menu_Item menu_mnu[];
#define mnuExit (menu_mnu+1)
#define mnuSend (menu_mnu+3)
#define mnuSendEmail (menu_mnu+4)
#define mnuSendText (menu_mnu+5)
#define mnuSendImage (menu_mnu+6)
#define mnuSendBinary (menu_mnu+7)
#define mnuCompose (menu_mnu+9)
#define mnuConfig (menu_mnu+10)
#define mnuHelp (menu_mnu+11)
#define mnuHowTo (menu_mnu+12)
#define mnuAbout (menu_mnu+13)
extern Fl_Input2 *txtMyCall;
extern Fl_Input2 *txtBEACONTXT;
extern Fl_Spinner2 *spnRetries;
extern Fl_Spinner2 *spnWaitTime;
extern Fl_Spinner2 *spnTimeout;
extern Fl_Spinner2 *spnTxDelay;
extern Fl_Spinner2 *spnBcnInterval;
extern Fl_Button *btnOK;
extern Fl_ComboBox *choiceBlockSize;
Fl_Double_Window* arq_configure();
#include "table.h"
extern Table *tblOutgoing;
extern void sendCancel();
extern Fl_Button *send_Cancel;
#include <FL/Fl_Return_Button.H>
extern void sendOK();
extern Fl_Return_Button *send_OK;
Fl_Double_Window* arq_SendSelect();
extern Fl_Input2 *inpMailTo;
extern Fl_Input2 *inpMailSubj;
#include <FL/Fl_Text_Editor.H>
extern Fl_Text_Editor *txtMailText;
#include <FL/Fl_Pack.H>
extern void cb_OpenComposeMail();
extern Fl_Button *btnOpenComposedMail;
extern void cb_ClearComposer();
extern Fl_Button *btnClearComposer;
extern void cb_UseTemplate();
extern Fl_Button *btnUseTemplate;
extern void cb_CancelComposeMail();
extern Fl_Button *btnCancelComposedMail;
extern void cb_SaveComposeMail();
extern Fl_Button *btnSaveComposedMail;
Fl_Double_Window* arq_composer();
#endif
