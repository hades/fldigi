//########################################################################
//
//  rigsupport.cxx
//
//  rig control - support functions file
//
//  copywrite David Freese, w1hkj@w1hkj.com
//
//########################################################################

#include <config.h>

#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

#include "rigdialog.h"
#include "rigsupport.h"
#include "rigxml.h"
#include "rigio.h"
#include "threads.h"
#include "main.h"

#include "configuration.h"

using namespace std;

Fl_Double_Window *rigcontrol = (Fl_Double_Window *)0;
string windowTitle;

vector<qrg_mode_t> freqlist;

#if USE_HAMLIB
struct rmode_name_t {
	rmode_t mode;
	const char *name;
} modes[] = {
	{ RIG_MODE_NONE, "NONE" },
	{ RIG_MODE_AM, "AM" },
	{ RIG_MODE_CW, "CW" },
	{ RIG_MODE_USB, "USB" },
	{ RIG_MODE_LSB, "LSB" },
	{ RIG_MODE_RTTY, "RTTY" },
	{ RIG_MODE_FM, "FM" },
	{ RIG_MODE_WFM, "WFM" },
	{ RIG_MODE_CWR, "CWR" },
	{ RIG_MODE_RTTYR, "RTTYR" },
	{ RIG_MODE_AMS, "AMS" },
	{ RIG_MODE_PKTLSB, "PKTLSB" },
	{ RIG_MODE_PKTUSB, "PKTUSB" },
	{ RIG_MODE_PKTFM, "PKTFM" },
	{ RIG_MODE_ECSSUSB, "ECSSUSB" },
	{ RIG_MODE_ECSSLSB, "ECSSLSB" },
	{ RIG_MODE_FAX, "FAX" },
	{ RIG_MODE_SAM, "SAM" },
	{ RIG_MODE_SAL, "SAL" },
	{ RIG_MODE_SAH, "SAH" }
};

map<string, rmode_t> mode_nums;
map<rmode_t, string> mode_names;

void selMode(rmode_t m)
{
	opMODE->value(mode_names[m].c_str());
}

void selFreq(long int f)
{
	if (FreqDisp)
		FreqDisp->value(f);
}
#endif // USE_HAMLIB

void initOptionMenus()
{
	opMODE->clear();
	list<MODE>::iterator MD;
	list<MODE> *pMD = 0;
	if (lmodes.empty() == false)
		pMD = &lmodes;
	else if (lmodeCMD.empty() == false)
		pMD = &lmodeCMD;

	if (pMD) {
		MD = pMD->begin();
		while (MD != pMD->end()) {
			opMODE->add( (*MD).SYMBOL.c_str());
			MD++;
		}
		opMODE->show();
		opMODE->index(0);
	}
	else
		opMODE->hide();

	opBW->clear();
	list<BW>::iterator bw;
	list<BW> *pBW = 0;
	if (lbws.empty() == false)
		pBW = &lbws;
	else if (lbwCMD.empty() == false)
		pBW = &lbwCMD;

	if (pBW) {
		bw = pBW->begin();
		while (bw != pBW->end()) {
			opBW->add( (*bw).SYMBOL.c_str());
			bw++;
		}
		opBW->show();
		opBW->index(0);
	}
	else
		opBW->hide();
}

void clearList()
{
	freqlist.clear();
	FreqSelect->clear();
}

void updateSelect()
{
	FreqSelect->clear();
	if (freqlist.size() == 0)
		return;

	char freq[20];
	for (size_t i = 0; i < freqlist.size(); i++) {
		snprintf(freq, sizeof(freq), "%9.3f", freqlist[i].rfcarrier / 1000.0);
		FreqSelect->add(freq);
	}
}

void addtoList(long val)
{
	qrg_mode_t m;

	m.rfcarrier = val;
	m.rmode = opMODE->value();
	if (active_modem) {
		m.carrier = active_modem->get_freq();
		m.mode = active_modem->get_mode();
	}
	freqlist.push_back(m);
	sort(freqlist.begin(), freqlist.end());
}

bool readFreqList()
{
	ifstream freqfile((HomeDir + "frequencies2.txt").c_str());
	if (!freqfile)
		return false;

	string line;
	qrg_mode_t m;
	while (!getline(freqfile, line).eof()) {
		if (line[0] == '#')
			continue;
		istringstream is(line);
		is >> m;
		freqlist.push_back(m);
	}
	updateSelect();

	return freqlist.size();
}

void saveFreqList()
{
	if (freqlist.empty())
		return;

	ofstream freqfile((HomeDir + "frequencies2.txt").c_str());
	if (!freqfile)
		return;
	freqfile << "# rfcarrier rig_mode carrier mode\n";
	copy(freqlist.begin(), freqlist.end(),
	     ostream_iterator<qrg_mode_t>(freqfile, "\n"));
}

void buildlist()
{
	if (readFreqList() == true)
		return;
	Fl::lock();
	addtoList (1807000L);
	addtoList (10135000L);
	addtoList (21070000L);
	addtoList (24920000);
	addtoList (28120000);
	addtoList (50290000);
	addtoList (3580000L);
	addtoList (14070000L);
	addtoList (21000000L);
	addtoList (7070000L);
	addtoList (14000000L);
	addtoList (28000000L);
	addtoList (7000000L);
	addtoList (3500000L);
	addtoList (3662000L);
	addtoList (7030000L);
	updateSelect();
	FreqDisp->value(freqlist[0].rfcarrier);
	Fl::unlock();
}

void setMode()
{
#if USE_HAMLIB
	if (progdefaults.chkUSEHAMLIBis)
		hamlib_setmode(mode_nums[opMODE->value()]);
	else
#endif
		rigCAT_setmode(opMODE->value());
}

void setBW()
{
	rigCAT_setwidth (opBW->value());
}

int movFreq()
{
	long int f;
	f = FreqDisp->value();
	adjFreq->value(f);
#if USE_HAMLIB
	if (progdefaults.chkUSEHAMLIBis)
		hamlib_setfreq(f);
	else
#endif
		rigCAT_setfreq(f);
	return 0;
}

void selectFreq()
{
	int n = FreqSelect->value() - 1;

	if (freqlist[n].rmode != "NONE") {
#if USE_HAMLIB
		if (progdefaults.chkUSEHAMLIBis)
			hamlib_setmode(mode_nums[freqlist[n].rmode]);
		else
#endif
			rigCAT_setmode(freqlist[n].rmode);
		opMODE->value(freqlist[n].rmode.c_str());
	}

	if (freqlist[n].mode != NUM_MODES) {
		if (freqlist[n].mode != active_modem->get_mode())
			init_modem(freqlist[n].mode);
		active_modem->set_freq(freqlist[n].carrier);
	}

	FreqDisp->value(freqlist[n].rfcarrier);
	movFreq();
}

void delFreq()
{
	int v = FreqSelect->value() - 1;

	if (v >= 0) {
		freqlist.erase(freqlist.begin() + v);
		updateSelect();
	}
}

void addFreq()
{
	long freq = FreqDisp->value();
	if (freq) {
		addtoList(freq);
		updateSelect();
	}
}


bool init_Xml_RigDialog()
{
	initOptionMenus();
	clearList();
	buildlist();
	if (windowTitle.length() > 0)
		rigcontrol->label(windowTitle.c_str());
	return true;
}

#if USE_HAMLIB
bool init_Hamlib_RigDialog()
{
	opBW->hide();
	opMODE->clear();
	for (size_t i = 0; i < sizeof(modes)/sizeof(modes[0]); i++) {
		mode_nums[modes[i].name] = modes[i].mode;
		mode_names[modes[i].mode] = modes[i].name;
		opMODE->add(modes[i].name);
	}
	clearList();
	buildlist();
	rigcontrol->label("Hamlib Controller");
	return true;
}
#endif

Fl_Double_Window *createRigDialog()
{
	Fl_Double_Window *w;
	w = rig_dialog();
	w->xclass(PACKAGE_NAME);
	return w;
}
