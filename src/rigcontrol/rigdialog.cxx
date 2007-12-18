// generated by Fast Light User Interface Designer (fluid) version 1.0107

#include "rigdialog.h"
#include <config.h>
#include "rigsupport.h"

cFreqControl *FreqDisp=(cFreqControl *)0;

Fl_Browser *FreqSelect=(Fl_Browser *)0;

static void cb_FreqSelect(Fl_Browser*, void*) {
  if (FreqSelect->value())
(Fl::event_state() & FL_SHIFT) ? delFreq() : selectFreq();
}

Fl_ComboBox *opMODE=(Fl_ComboBox *)0;

static void cb_opMODE(Fl_ComboBox*, void*) {
  setMode();
}

Fl_ComboBox *opBW=(Fl_ComboBox *)0;

static void cb_opBW(Fl_ComboBox*, void*) {
  setBW();
}

Fl_Button *btnAddFreq=(Fl_Button *)0;

static void cb_btnAddFreq(Fl_Button*, void*) {
  addFreq();
}

Fl_Button *btnDelFreq=(Fl_Button *)0;

static void cb_btnDelFreq(Fl_Button*, void*) {
  delFreq();
}

Fl_Button *btnClearList=(Fl_Button *)0;

static void cb_btnClearList(Fl_Button*, void*) {
  clearList();
}

Fl_Button *btnRCclose=(Fl_Button *)0;

static void cb_btnRCclose(Fl_Button*, void*) {
  closeRigDialog();
}

Fl_Adjuster *adjFreq=(Fl_Adjuster *)0;

static void cb_adjFreq(Fl_Adjuster* o, void*) {
    FreqDisp->value(static_cast<long>(o->value()));
FreqDisp->do_callback();
}

Fl_Double_Window* rig_dialog() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = new Fl_Double_Window(390, 100, "Rig Controller");
    w = o;
    o->box(FL_DOWN_BOX);
    o->color((Fl_Color)23);
    { cFreqControl* o = FreqDisp = new cFreqControl(4, 29, 253, 44, "9");
      o->box(FL_DOWN_BOX);
      o->color(FL_BACKGROUND_COLOR);
      o->selection_color(FL_BACKGROUND_COLOR);
      o->labeltype(FL_NORMAL_LABEL);
      o->labelfont(0);
      o->labelsize(14);
      o->labelcolor(FL_FOREGROUND_COLOR);
      o->align(FL_ALIGN_CENTER);
      o->when(FL_WHEN_RELEASE);
      o->setCallBack(movFreq);
      o->SetONOFFCOLOR( FL_RED, FL_BLACK);
    }
    { Fl_Browser* o = FreqSelect = new Fl_Browser(278, 5, 108, 90);
      o->tooltip("Select operating frequency");
      o->type(2);
      o->box(FL_DOWN_BOX);
      o->labelfont(4);
      o->labelsize(12);
      o->textfont(4);
      o->callback((Fl_Callback*)cb_FreqSelect);
    }
    { Fl_ComboBox* o = opMODE = new Fl_ComboBox(69, 5, 96, 20);
      o->box(FL_DOWN_BOX);
      o->color(FL_BACKGROUND2_COLOR);
      o->selection_color(FL_BACKGROUND_COLOR);
      o->labeltype(FL_NORMAL_LABEL);
      o->labelfont(0);
      o->labelsize(14);
      o->labelcolor(FL_FOREGROUND_COLOR);
      o->callback((Fl_Callback*)cb_opMODE);
      o->align(FL_ALIGN_TOP);
      o->when(FL_WHEN_RELEASE);
      o->end();
    }
    { Fl_ComboBox* o = opBW = new Fl_ComboBox(170, 5, 85, 20);
      o->box(FL_DOWN_BOX);
      o->color(FL_BACKGROUND2_COLOR);
      o->selection_color(FL_BACKGROUND_COLOR);
      o->labeltype(FL_NORMAL_LABEL);
      o->labelfont(0);
      o->labelsize(14);
      o->labelcolor(FL_FOREGROUND_COLOR);
      o->callback((Fl_Callback*)cb_opBW);
      o->align(FL_ALIGN_TOP);
      o->when(FL_WHEN_RELEASE);
      o->end();
    }
    { Fl_Button* o = btnAddFreq = new Fl_Button(257, 6, 20, 20, "@|>");
      o->tooltip("Add to list");
      o->labelsize(10);
      o->callback((Fl_Callback*)cb_btnAddFreq);
    }
    { Fl_Button* o = btnDelFreq = new Fl_Button(257, 30, 20, 20, "@-11+");
      o->tooltip("Delete from list");
      o->labelsize(10);
      o->callback((Fl_Callback*)cb_btnDelFreq);
    }
    { Fl_Button* o = btnClearList = new Fl_Button(257, 54, 20, 20, "@-1square");
      o->tooltip("Clear list");
      o->labelsize(10);
      o->callback((Fl_Callback*)cb_btnClearList);
    }
    { Fl_Button* o = btnRCclose = new Fl_Button(4, 5, 50, 20, "Close");
      o->callback((Fl_Callback*)cb_btnRCclose);
    }
    { Fl_Adjuster* o = adjFreq = new Fl_Adjuster(4, 75, 253, 20);
      o->tooltip("Click and drag to adjust frequency (1000, 100, 10 Hz)");
      o->step(10);
      o->callback((Fl_Callback*)cb_adjFreq);
    }
    o->end();
  }
  return w;
}
