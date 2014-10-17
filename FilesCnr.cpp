// GTakPM
// (c) Benjamin Stein 1994
// All Rights Reserved
// FilesCnr.cpp


#include <GTakPM.hpp>

#define SPLITBAR_OFFSET  150  // Pixel offset of details splitbar


FilesCnr::FilesCnr(pGTPMWin Parent)
  : OContainer<FileInfo, MINIRECORDCORE>(GTPM_FILECNR, Parent, 
               CCS_AUTOPOSITION | CCS_MINIRECORDCORE | CCS_READONLY | CCS_MINIICONS,
               CA_CONTAINERTITLE | CA_TITLESEPARATOR | 
               CV_DETAIL | CA_DETAILSVIEWTITLES | CV_MINI),
    populate(this, FilesCnr::getFiles, 65536),
    file_ico(FILE_ICO, 
           GTPMApp::GTakPM->resLibrary->getLibHandle()),
    fileS_ico(FILE_INC_ICO, 
           GTPMApp::GTakPM->resLibrary->getLibHandle())
  {}

FilesCnr::~FilesCnr()
  {}


BOOL FilesCnr::setupDetails()
{
 PFIELDINFO      pfi, pfiFirst, pfiLastLeftCol;
 FIELDINFOINSERT fii;

 // Allocate storage for container column data

 pfi = allocDetailFields(5);

 if(!pfi)
   return(FALSE);

 pfiFirst = pfi;

 pfi->flData     = CFA_BITMAPORICON | CFA_LEFT;
 pfi->flTitle    = CFA_LEFT | CFA_FITITLEREADONLY;
 pfi->pTitleData = "";
 pfi->offStruct  = (ULONG)offsetof(FileInfo, rc.hptrIcon);
 pfi             = pfi->pNextFieldInfo;

 pfi->flData     = CFA_STRING | CFA_LEFT | CFA_HORZSEPARATOR;
 pfi->flTitle    = CFA_LEFT | CFA_FITITLEREADONLY;
 pfi->pTitleData = "File";
 pfi->offStruct  = (ULONG)offsetof(FileInfo, rc.pszIcon);
 pfiLastLeftCol  = pfi;                 // set splitbar after this column
 pfi             = pfi->pNextFieldInfo;

 pfi->flData     = CFA_ULONG | CFA_RIGHT | CFA_HORZSEPARATOR | CFA_SEPARATOR;
 pfi->flTitle    = CFA_LEFT | CFA_FITITLEREADONLY;
 pfi->pTitleData = "Bytes";
 pfi->offStruct  = (ULONG)offsetof(FileInfo, cbFile);
 pfi                = pfi->pNextFieldInfo;

 pfi->flData     = CFA_DATE | CFA_RIGHT | CFA_HORZSEPARATOR | CFA_SEPARATOR;
 pfi->flTitle    = CFA_LEFT | CFA_FITITLEREADONLY;
 pfi->pTitleData = "Date";
 pfi->offStruct  = (ULONG)offsetof(FileInfo, date);
 pfi             = pfi->pNextFieldInfo;

 pfi->flData     = CFA_TIME | CFA_RIGHT | CFA_HORZSEPARATOR | CFA_SEPARATOR;
 pfi->flTitle    = CFA_LEFT | CFA_FITITLEREADONLY;
 pfi->pTitleData = "Time";
 pfi->offStruct  = (ULONG)offsetof(FileInfo, time);

 memset(&fii, 0, sizeof(FIELDINFOINSERT));
 fii.cb                   = (ULONG)(sizeof(FIELDINFOINSERT));    
 fii.pFieldInfoOrder      = (PFIELDINFO)CMA_END;
 fii.cFieldInfoInsert     = 5;
 fii.fInvalidateFieldInfo = TRUE;              

 cnri.pFieldInfoLast = pfiLastLeftCol;
 cnri.xVertSplitbar  = SPLITBAR_OFFSET;

 return((insertDetailInfo(pfiFirst, &fii)) &&
        (setCnrInfo(CMA_PFIELDINFOLAST | CMA_XVERTSPLITBAR)));
}



BOOL FilesCnr::setupItems()
{
 return(TRUE);
}


void FilesCnr::getFiles()
{
 PFILEFINDBUF3 pffb = NULL;
 OString       FileSpec(scanDirectory);
 ULONG         bufSize = sizeof(FILEFINDBUF3) * 200;
 HDIR          hdir = HDIR_CREATE;
 ULONG         ulMaxFiles = 200;
 APIRET        rc;

 freeItems();
 FileList.reset();

 DosAllocMem((PPVOID)&pffb, bufSize, PAG_READ | PAG_WRITE | PAG_COMMIT);

 if (!pffb)
   return;

 FileSpec + "\\*";

 DosError(FERR_DISABLEHARDERR);
 rc = DosFindFirst(FileSpec.text, &hdir, 
                   FILE_NORMAL | FILE_HIDDEN | FILE_SYSTEM | FILE_READONLY,
                   pffb, bufSize, &ulMaxFiles, FIL_STANDARD);

 while(!rc) {
   insertFiles(pffb, ulMaxFiles);
   rc = DosFindNext(hdir, pffb, bufSize, &ulMaxFiles); }

 DosFindClose(hdir);
 DosError(FERR_ENABLEHARDERR);
 DosFreeMem(pffb);
}


void FilesCnr::insertFiles(PFILEFINDBUF3 pffb, ULONG cFiles)
{
 PBYTE          pbBuf = (PBYTE) pffb;
 pFileInfo      pfiFirst, pfi;
 ULONG          i;
 PFILEFINDBUF3  pffbFile;
 RECORDINSERT   ri;

 if (!cFiles)
   return;

 pfi = allocRecords(cFiles);
 pfiFirst = pfi;

 for(i = 0; i < cFiles; i++)
  {
   pffbFile = (PFILEFINDBUF3) pbBuf;

   pFileItem item = new FileItem;

   item->fullName << scanDirectory;
   item->fullName + "\\" + pffbFile->achName;
   item->name     << pffbFile->achName;

   pfi->rc.pszIcon   = item->name.text;
   pfi->fileItem     = item;
   pfi->rc.hptrIcon  = file_ico.hptr;
   pfi->date.day     = pffbFile->fdateLastWrite.day;
   pfi->date.month   = pffbFile->fdateLastWrite.month;
   pfi->date.year    = pffbFile->fdateLastWrite.year + 80;
   pfi->time.seconds = pffbFile->ftimeLastWrite.twosecs;
   pfi->time.minutes = pffbFile->ftimeLastWrite.minutes;
   pfi->time.hours   = pffbFile->ftimeLastWrite.hours;
   pfi->cbFile       = pffbFile->cbFile;

   FileList.add(item);
   pfi = (pFileInfo) pfi->rc.preccNextRecord;   // Get the next container record
   pbBuf += pffbFile->oNextEntryOffset;         // Point to the next file in the buffer
  }

 memset(&ri, 0, sizeof( RECORDINSERT));
 ri.cb                 = sizeof(RECORDINSERT);
 ri.pRecordOrder       = (PRECORDCORE) CMA_END;
 ri.pRecordParent      = (PRECORDCORE) NULL;
 ri.zOrder             = (USHORT) CMA_TOP;
 ri.cRecordsInsert     = cFiles;
 ri.fInvalidateRecord  = TRUE;

 insertRecord(pfiFirst, &ri);
}


// end of source
