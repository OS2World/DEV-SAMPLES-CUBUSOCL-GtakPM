// GTakPM
// (c) Benjamin Stein 1994
// All Rights Reserved
// Backup.cpp


#include <GTakPM.hpp>


void GTPMWin::backup()
{
#ifdef __TK21__
 PSZ   tempEnv;
#else
 PCSZ  tempEnv;
#endif 
 PSZ      pstr;
 ULONG    item, items;
 OString  path;
 OString  includes;
 OString  excludes;
 OString  batch;
 OString  dirfile;
 OString  str;
 ofstream incfile;
 ofstream excfile;
 ofstream batchfile;
 
 if ((DosScanEnv("TEMP", &tempEnv)) &&
     (DosScanEnv("TMP", &tempEnv)))
  {
   path << GTPMApp::GTakPM->callName;
   path - '\\';
  }
 else
   path << (PSZ)tempEnv;

 if (path.text[strlen(path.text) - 1] != '\\')
   path + "\\";

 includes << path;
 includes + "gtmp.inc";
 excludes << path;
 excludes + "gtmp.exc";
 batch << path;
 batch + "gtmp.cmd";
 dirfile << path;
 dirfile + "gtmp.dir";

 path << "Cannot open: ";

 incfile.open(includes.text);
 if (!incfile) {
   path + includes;
   throw OPMException(path.text, 0); }

 excfile.open(excludes.text);
 if (!excfile) {
   path + excludes;
   throw OPMException(path.text, 0); }

 batchfile.open(batch.text);
 if (!batchfile) {
   path + batch;
   throw OPMException(path.text, 0); }

// write includes-file 
 items = Includes->queryItemCount();
 for(item = 0; item < items; item++)
   if (Includes->queryItemText(str, item)) {
     pstr = str.text;
     while((pstr = strchr(pstr, '\\'))!=NULL)
       pstr[0] = '/';
     incfile << ((PSZ)(strchr(str.text, ' ')+1)) << endl; }
 incfile.close();

// write excludes-file 
 items = Excludes->queryItemCount();
 for(item = 0; item < items; item++)
   if (Excludes->queryItemText(str, item)) {
     pstr = str.text;
     while((pstr = strchr(pstr, '\\'))!=NULL)
       pstr[0] = '/';
     excfile << ((PSZ)(strchr(str.text, ' ')+1)) << endl; }
 excfile.close();

// write batchfile
 batchfile << "@ECHO OFF\n"
           << "echo Initializing Tape\n"
           << "tape stat >NUL 2>NUL\n"
           << "tape blocksize 0 stat sel 0 eraseq tell\n"
           << "echo Backup in progress\ntar -cEppP @"
           << includes
           << " --totals --exclude-from "
           << excludes
           << " -D "
           << dirfile
           << "\ntape stat\n"
           << "echo Compare in progress\n"
           << "tape rew >NUL 2>NUL\n"
           << "tar -dEppP\n"
           << "tape stat\n"
           << "echo Operation completed.\n";
 batchfile.close();
 tape->batch(batch.text);
}


// end of source
