// GTakPM
// (c) Benjamin Stein 1994
// All Rights Reserved
// GTPMWin.hpp


#ifndef GTPMWIN_HPP_INCLUDED
  #define GTPMWIN_HPP_INCLUDED

#ifndef GTAKPM_HPP_INCLUDED
  #include <GTakPM.hpp>
#endif

struct DirItem
{
 OString fullName;
 OString name;
};

typedef DirItem FileItem ;
typedef FileItem *pFileItem;

struct DirInfo
{
 RECORDCORE rc;          // base information
 pDirItem   dirItem;
 BOOL       selected;    // directory is selected
 BOOL       expanded;    // expanded subdirectory?
 BOOL       wasExpanded;
};


class DrivesCnr : public OContainer<DirInfo, RECORDCORE>
{
 protected:
   pGTPMWin ParentWindow;

   lOString           DriveList;
   List<DirItem>      DirList;

 public:
   OThread<DrivesCnr> populate;
   OThread<DrivesCnr> select;

   OIcon   hd_ico;
   OIcon   hdS_ico;
   OIcon   cd_ico;
   OIcon   cdS_ico;
   OIcon   folder_ico;
   OIcon   folderS_ico;
   OIcon   openfolder_ico;
   OIcon   openfolderS_ico;
   OIcon   dontKnow_ico;

   DrivesCnr(pGTPMWin Parent);
   ~DrivesCnr();

   BOOL setupDetails();
   BOOL setupItems();

   void collectDirectories();
   void getDirectories(pDirInfo Parent, PSZ Directory,
                       BOOL recursive = TRUE,
                       BOOL selected = FALSE);

   void insertDirs(pDirInfo Parent, PSZ Directory,
                   PFILEFINDBUF3 pffb, ULONG cFiles,
                   BOOL selected = FALSE);

   void recurse(pDirInfo Parent,
                BOOL recursive = TRUE,
                BOOL selected = FALSE);

   void selectDirRecord(pDirInfo Parent);
   void beginSelect();
};


struct FileInfo
{
 MINIRECORDCORE rc;       // base information
 pFileItem      fileItem;     
 CDATE          date;     // date of file
 CTIME          time;     // time of file
 ULONG          cbFile;   // file size
 BOOL           selected; // file is selected
};


class FilesCnr : public OContainer<FileInfo, MINIRECORDCORE>
{
 protected:
   pGTPMWin          ParentWindow;
   List<FileItem>    FileList;

 public:
   OThread<FilesCnr> populate; 
   OString           scanDirectory;
   OIcon             file_ico;
   OIcon             fileS_ico;

   FilesCnr(pGTPMWin Parent);
   ~FilesCnr();

   BOOL setupDetails();
   BOOL setupItems();
   void getFiles(); 
   void insertFiles(PFILEFINDBUF3 pffb, ULONG cFiles);
};




class GTPMWin 
  : public OFrame, 
    public OPMPipeTool
{
 protected:
   BOOL             wasOutOfContainer;
   BOOL             processing;
   BOOL             showMonitor;
   HWND             frameMenu;
   OThread<GTPMWin> expander;
   OThread<GTPMWin> backupThr;
   OThread<GTPMWin> restoreThr;

 public:
   pTape         tape;

   pOStatusLine  statline;
   pOButtonBar   toolbar;
   pOButtonBar   tapebar;
   pDrivesCnr    Drives;
   pFilesCnr     Files;
   pOListBox     Includes;
   pOListBox     Excludes;
   pOListBox     monitor;

   GTPMWin();
   virtual ~GTPMWin();

   void expandFolder();
   void backup();
   void restore();

   void addExclude(PSZ str);
   void addInclude(PSZ str);
   void updateListbox(PSZ str);

// override printout from OPipeTool
   void printout();

   void resizeClients(MPARAM sizeparms);  // processes WM_SIZE
   BOOL control(MPARAM mp1, MPARAM mp2);  // process WM_CONTROL
   BOOL command(MPARAM mp1, MPARAM mp2);  // process WM_COMMAND

   BOOL OCommand(ULONG msg, MPARAM mp1, MPARAM mp2); 
};

#endif // GTPMWIN_HPP_INCLUDED

// end of source
