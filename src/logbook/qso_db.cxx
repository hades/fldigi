#include <config.h>

#include <fstream>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include "qso_db.h"
#include "field_def.h"

using namespace std;

// class cQsoRec

static int compby = COMPDATE;

bool cQsoDb::reverse = false;

cQsoRec::cQsoRec() {
  for (int i=0;i < NUMFIELDS; i++) {
    qsofield[i] = new char [fields[i].size + 1];
    memset (qsofield[i],0, fields[i].size + 1);
  }
}

cQsoRec::~cQsoRec () {
  for (int i = 0; i < NUMFIELDS; i++)
    delete qsofield[i];
}

void cQsoRec::clearRec () {
  for (int i = 0; i < NUMFIELDS; i++)
    memset(qsofield[i], 0, fields[i].size + 1);
}

int cQsoRec::validRec() {
  return 0;
}

void cQsoRec::putField (int n, const char *s){
  if (n < 0 || n >= NUMFIELDS) return;
  strncpy( qsofield[n], s, fields[n].size);
}

void cQsoRec::addtoField (int n, const char *s){
	if (n < 0 || n >= NUMFIELDS) return;
	char *temp = new char[strlen(qsofield[n]) + strlen(s) + 1];
	strcpy(temp, qsofield[n]);
	strcat(temp, s);
	strncpy(qsofield[n], temp, fields[n].size);
}

void cQsoRec::trimFields () {
char *p;
  for (int i = 0; i < NUMFIELDS; i++) {
//right trim string
    p = qsofield[i] + strlen(qsofield[i]) - 1;
    while (*p == ' ' && p >= qsofield[i]) *p-- = 0;
//left trim string
	p = qsofield[i];
	while (*p == ' ') strcpy(p, p+1);
//make all upper case if Callsign or Mode  
    if (i == CALL || i == MODE){
      p = qsofield[i];
      while (*p) {*p = toupper(*p); p++;}
    }
  }
}

char * cQsoRec::getField (int n) {
  if (n < 0 || n >= NUMFIELDS) return 0;
  return (qsofield[n]);
}

const cQsoRec &cQsoRec::operator=(const cQsoRec &right) {
  if (this != &right) {
    for (int i = 0; i < NUMFIELDS; i++)
      strncpy( this->qsofield[i], right.qsofield[i], fields[i].size);
   }
  return *this;
}

int compareTimes (const cQsoRec &r1, const cQsoRec &r2) {
  return  strcmp (r1.qsofield[TIME_ON], r2.qsofield[TIME_ON]);
}

int compareDates (const cQsoRec &r1, const cQsoRec &r2) {
int cmp = 0;
  cmp = strcmp (r1.qsofield[QSO_DATE], r2.qsofield[QSO_DATE]);
  if (cmp == 0)
    return compareTimes (r1,r2);
  return cmp;
}

int compareCalls (const cQsoRec &r1, const cQsoRec &r2) {
  int cmp = 0;
  char *s1 = new char[strlen(r1.qsofield[CALL]) + 1];
  char *s2 = new char[strlen(r2.qsofield[CALL]) + 1];
  char *p1, *p2;
  strcpy(s1, r1.qsofield[CALL]);
  strcpy(s2, r2.qsofield[CALL]);
  p1 = strpbrk (&s1[1], "0123456789");
  p2 = strpbrk (&s2[1], "0123456789");
  if (p1 && p2) {
    cmp = (*p1 < *p2) ? -1 :(*p1 > *p2) ? 1 : 0;
    if (cmp == 0) {
      *p1 = 0; *p2 = 0;
      cmp = strcmp (s1, s2);
      if (cmp == 0)
        cmp = strcmp(p1+1, p2+1);
    }
  } else
    cmp = strcmp (r1.qsofield[CALL], r2.qsofield[CALL]);
  if (cmp == 0)
    return compareDates (r1,r2);
  return cmp;
}

int compareModes (const cQsoRec &r1, const cQsoRec &r2) {
  int cmp = 0;
  cmp = strcmp (r1.qsofield[MODE], r2.qsofield[MODE]);
  if (cmp == 0)
    return compareDates (r1,r2);
  return cmp;
}

int compareFreqs (const cQsoRec &r1, const cQsoRec &r2) {
  int cmp = 0;
  double f1, f2;
  f1 = atof(r1.qsofield[FREQ]);
  f2 = atof(r2.qsofield[FREQ]);
  if (f1 == f2) cmp = 0;
  else if (f1 < f2) cmp = -1;
  else cmp = 1;
  if (cmp == 0)
    return compareDates (r1,r2);
  return cmp;
}

int compareqsos (const void *p1, const void *p2) {
	cQsoRec *r1, *r2;
	if (cQsoDb::reverse) {
		r2 = (cQsoRec *)p1;
		r1 = (cQsoRec *)p2;
	} else {
		r1 = (cQsoRec *)p1;
		r2 = (cQsoRec *)p2;
	}

	switch (compby) {
		case COMPCALL :
			return compareCalls (*r1, *r2);
		case COMPMODE :
			return compareModes (*r1, *r2);
		case COMPFREQ :
			return compareFreqs (*r1, *r2);
		case COMPDATE :
		default :
			return compareDates (*r1, *r2);
	}
}

bool cQsoRec::operator==(const cQsoRec &right) const {
  if (compareDates (*this, right) != 0) return false;
  if (compareTimes (*this, right) != 0) return false;
  if (compareCalls (*this, right) != 0) return false;
  if (compareFreqs (*this, right) != 0) return false;
  return true;
}

bool cQsoRec::operator<(const cQsoRec &right) const {
  if (compareDates (*this, right) > -1) return false;
  if (compareTimes (*this, right) > -1) return false;
  if (compareCalls (*this, right) > -1) return false;
  if (compareFreqs (*this, right) > -1) return false;
  return true;
}

static char delim_in = '\t';
static char delim_out = '\t';
static bool isVer3 = false;

ostream &operator<< (ostream &output, const cQsoRec &rec) {
  for (int i = 0; i < EXPORT; i++)
    output << rec.qsofield[i] << delim_out;
  return output;
}

istream &operator>> (istream &input, cQsoRec &rec ) {
char c, *fld;
int i, j, max;
  for (i = 0; i < (isVer3 ? EXPORT : IOTA); i++) {
    j = 0;
    fld = rec.qsofield[i];
    max = fields[i].size;
    c = input.get();
    while (c != delim_in && c != EOF) {
      if (j++ < max) *fld++ = c;
      c = input.get();
    }
    *fld = 0;
  }
  return input;
}

//======================================================================
// class cQsoDb

cQsoDb::cQsoDb() {
  nbrrecs = 0;
  maxrecs = 1;
  qsorec = new cQsoRec[1];
  compby = COMPDATE;
  dirty = 0;
}

cQsoDb::~cQsoDb() {
  delete [] qsorec;
} 

void cQsoDb::deleteRecs() {
  delete [] qsorec;
  qsorec = new cQsoRec[1];
  nbrrecs = 0;
  maxrecs = 1;
  dirty = 0;
}

void cQsoDb::clearDatabase() {
  deleteRecs();
}

int cQsoDb::qsoFindRec(cQsoRec *rec) {
  for (int i = 0; i < nbrrecs; i++)
    if (qsorec[i] == *rec)
      return i;
  return -1;
}

void cQsoDb::qsoNewRec (cQsoRec *nurec) {
  nurec->trimFields();
  if (qsoFindRec(nurec) > -1) return;
  if (nbrrecs == maxrecs) {
    maxrecs *= 2;
    cQsoRec *atemp = new cQsoRec[maxrecs];
    for (int i = 0; i < nbrrecs; i++)
      atemp[i] = qsorec[i];
    delete [] qsorec;
    qsorec = atemp;
  }
  qsorec[nbrrecs] = *nurec;
  nbrrecs++;
  dirty = 1;
}

void cQsoDb::qsoDelRec (int rnbr) {
  if (rnbr < 0 || rnbr > (nbrrecs - 1)) 
    return;
  for (int i = rnbr; i < nbrrecs - 1; i++)
    qsorec[i] = qsorec[i+1];
  nbrrecs--;
  qsorec[nbrrecs].clearRec();
}
  
void cQsoDb::qsoUpdRec (int rnbr, cQsoRec *updrec) {
  if (rnbr < 0 || rnbr > (nbrrecs - 1))
    return;
  qsorec[rnbr] = *updrec;
  return;
}

void cQsoDb::SortByDate () {
  compby = COMPDATE;
  qsort (qsorec, nbrrecs, sizeof (cQsoRec), compareqsos);
}

void cQsoDb::SortByCall () {
  compby = COMPCALL;
  qsort (qsorec, nbrrecs, sizeof (cQsoRec), compareqsos);
}

void cQsoDb::SortByMode () {
  compby = COMPMODE;
  qsort (qsorec, nbrrecs, sizeof (cQsoRec), compareqsos);
}

void cQsoDb::SortByFreq () {
	compby = COMPFREQ;
	qsort (qsorec, nbrrecs, sizeof (cQsoRec), compareqsos);
}

bool cQsoDb::qsoIsValidFile(const char *fname) {
  char buff[256];
  ifstream inQsoFile (fname, ios::in);
  if (!inQsoFile)
    return false;
  inQsoFile.getline (buff, 256);
  if (strstr (buff, "_LOGBOOK DB") == 0) {
    inQsoFile.close();
    return false;
  }
  inQsoFile.close();
  return true;
}

int cQsoDb::qsoReadFile (const char *fname) {
char buff[256];
  ifstream inQsoFile (fname, ios::in);
  if (!inQsoFile)
    return 1;
  inQsoFile.getline (buff, 256);
  if (strstr (buff, "_LOGBOOK DB") == 0) {
    inQsoFile.close();
    return 2;
  }
  if (strstr (buff, "_LOGBOOK DBX") == 0) // new file format
    delim_in = '\n';
  if (strstr (buff, "3.0") != 0)
	isVer3 = true;    
  
  cQsoRec inprec;
  while (inQsoFile >> inprec) {
    qsoNewRec (&inprec);
    inprec.clearRec();
  }
  inQsoFile.close();
  SortByDate();
  return 0;
}

int cQsoDb::qsoWriteFile (const char *fname) {
  ofstream outQsoFile (fname, ios::out);
  if (!outQsoFile) {
  	printf("write failure: %s\n", fname);
    return 1;
  }
  outQsoFile << "_LOGBOOK DBX 3.0" << '\n';
  for (int i = 0; i < nbrrecs; i++)
    outQsoFile << qsorec[i];
  outQsoFile.close();
  return 0;
}


