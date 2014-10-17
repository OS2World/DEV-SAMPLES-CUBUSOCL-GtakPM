// GTakPM
// (c) Benjamin Stein 1994
// All Rights Reserved
// GTPMWin.cpp


#include <GTakPM.hpp>

static ButtonInfo buttoninfo[] =
{
 {CMD_MONITOR, CMD_MONITOR, "Monitor on/off", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {CMD_RESCAN, CMD_RESCAN, "Rescan Drives", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {CMD_PROCEDURES, CMD_PROCEDURES, "Procedures", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {CMD_TAPE, CMD_TAPE, "Device Information", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {CMD_BACKUP, CMD_BACKUP, "Backup", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {CMD_RESTORE, CMD_RESTORE, "Restore", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {CMD_COMPARE, CMD_COMPARE, "Compare", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {CMD_SETTINGS, CMD_SETTINGS, "Settings", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
// {CMD_INFO, CMD_INFO, "Help", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {CMD_QUIT, CMD_QUIT, "Quit", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL }
};

static ButtonInfo toolinfo[] =
{
 {TAPEBAR_LOAD, TAPEBAR_LOAD, "Load", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {TAPEBAR_UNLOAD, TAPEBAR_UNLOAD, "Unload", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {TAPEBAR_REWIND, TAPEBAR_REWIND, "Rewind", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
// {TAPEBAR_PARTITIONS, TAPEBAR_PARTITIONS, "Make Partitions", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
// {TAPEBAR_FORMAT, TAPEBAR_FORMAT, "Format", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {TAPEBAR_ERASE, TAPEBAR_ERASE, "Erase", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {TAPEBAR_ERASEQ, TAPEBAR_ERASEQ, "Quick Erase", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {TAPEBAR_LOCK, TAPEBAR_LOCK, "Lock Device", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {TAPEBAR_UNLOCK, TAPEBAR_UNLOCK, "Unlock Device", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
 {TAPEBAR_STOP_ASAP, TAPEBAR_STOP_ASAP, "Stop current operation!", NULL, NULLHANDLE, (HBITMAP)NULL, (HPOINTER)NULL },
};


GTPMWin::GTPMWin()
 : OFrame(GTPM_ICO, 
          FCF_SIZEBORDER | 
          FCF_ICON | 
          FCF_TASKLIST | 
          FCF_SYSMENU | 
          FCF_MINMAX | 
          FCF_TITLEBAR | 
          FCF_SHELLPOSITION,
          CS_SIZEREDRAW | CS_HITTEST),
   wasOutOfContainer(TRUE),
   showMonitor(FALSE),
   processing(FALSE),
   toolbar(NULL),
   tapebar(NULL),
   monitor(NULL),
   Drives(NULL),
   Files(NULL),
   statline(NULL),
   frameMenu(NULLHANDLE),
   expander(this, GTPMWin::expandFolder, 65536), 
   backupThr(this, GTPMWin::backup, 65536), 
   restoreThr(this, GTPMWin::restore, 65536), 
   OPMPipeTool((PSZ)NULL, (PSZ)NULL)
{
 ULONG counter;
#ifdef __TK21__
 PSZ   tapeEnv;
#else
 PCSZ  tapeEnv;
#endif 

 DosError(FERR_DISABLEHARDERR);

 DosScanEnv("TAPE", &tapeEnv);
 tape = new Tape(this, (PSZ)tapeEnv, 0);

 winText << "GTakPM Develop";

 if (!createFrame("GTakPM"))
   throwGTPM(ERROR_FRAME_NOT_CREATED);

 frameMenu = WinLoadMenu(frame, 
                         GTPMApp::GTakPM->resLibrary->getLibHandle(), 
                         GTPM_MENU);
 if (!frameMenu)
   throwGTPM(ERROR_LOAD_MENU);

 // setup toolbar
 toolbar = new OButtonBar(GTPM_TOOLBAR, this, OButtonBar::bitmaplike);
 toolbar->setButtonDimensions(40, 40); // set size of buttons
 toolbar->setPicSize(32);              // set size of button pictures
 for (counter = 0; counter < sizeof(buttoninfo)/sizeof(ButtonInfo); counter++)
   toolbar->buttons.addLast(&buttoninfo[counter]);

 // setup tapebar
 tapebar = new OButtonBar(GTPM_TAPEBAR, this, OButtonBar::bitmaplike);
 tapebar->setButtonDimensions(40, 40); // set size of buttons
 tapebar->setPicSize(32);              // set size of button pictures
 for (counter = 0; counter < sizeof(toolinfo)/sizeof(ButtonInfo); counter++)
   tapebar->buttons.addLast(&toolinfo[counter]);

 if ((!toolbar->createButtonBar()) ||
     (!tapebar->createButtonBar()))
   throw GTPMException(ERROR_TOOLBAR_CREATION, OException::fatal);

// setup statuslines
 statline = new OStatusLine(GTPM_STATLINE, this); 
 statline->style |= FCF_BORDER;
 if(!statline->createStatusLine())
   throw GTPMException(ERROR_STATLINE_CREATION, OException::fatal);

// setup listboxes

 Excludes = new OListBox(GTPM_EX, this, LS_NOADJUSTPOS | LS_HORZSCROLL);
 Includes = new OListBox(GTPM_INC, this, LS_NOADJUSTPOS | LS_HORZSCROLL);
 monitor  = new OListBox(GTPM_MON, this, LS_NOADJUSTPOS | LS_HORZSCROLL, TRUE);
 if ((!Excludes->createListBox(0,0,0,0)) || 
     (!Includes->createListBox(0,0,0,0)) ||
     (!monitor->createListBox(0,0,0,0)))
   throw GTPMException(ERROR_LISTBOX_CREATION, OException::fatal);

 Drives = new DrivesCnr(this);
 Drives->winText << ("Drives");
 Files = new FilesCnr(this);
 Files->winText << ("Files");
 if ((!Drives->createContainer()) ||
     (!Files->createContainer()))
   throw GTPMException(ERROR_CONTAINER_CREATION, OException::fatal);

// set presentation parms

 Drives->setBackColor(SYSCLR_WINDOW);
 Drives->setForeColor(SYSCLR_WINDOWTEXT);
 Drives->setFont("8.Helv");

 Files->setBackColor(SYSCLR_WINDOW);
 Files->setForeColor(SYSCLR_WINDOWTEXT);
 Files->setFont("8.Helv");

 Excludes->setBackColor(SYSCLR_WINDOW);
 Excludes->setForeColor(SYSCLR_WINDOWTEXT);
 Excludes->setFont("8.Helv");

 Includes->setBackColor(SYSCLR_WINDOW);
 Includes->setForeColor(SYSCLR_WINDOWTEXT);
 Includes->setFont("8.Helv");

 monitor->setBackColor(SYSCLR_WINDOW);
 monitor->setForeColor(SYSCLR_WINDOWTEXT);
 monitor->setFont("8.Helv");

 setNotify(this);

// ready
 statline->setText("Ready");
}



GTPMWin::~GTPMWin()
{
 if (Drives) delete Drives;
 if (Files) delete Files;
 if (toolbar) delete toolbar;
 if (tapebar) delete tapebar;
 if (statline) delete statline;
 if (tape) delete tape;
 if (monitor) delete monitor;
} 
   

void GTPMWin::resizeClients(MPARAM sizeparms)
{
 ULONG width      = SHORT1FROMMP(sizeparms);
 ULONG height     = SHORT2FROMMP(sizeparms);
 ULONG lineHeight, cnrHeight;

 if ((!statline) || (!toolbar) || 
     (!Drives) || (!Files) || (!monitor) ||
     (!Includes) || (!Excludes) || (!tapebar))
    return;   // please don't trap!

 lineHeight = statline->getHeight();
 cnrHeight = lineHeight + 3*tapebar->getHeight();

// set toolbar
 WinSetWindowPos(toolbar->frame, HWND_TOP,
                 0,
                 height - toolbar->getHeight(),
                 width, 
                 toolbar->getHeight(),
                 SWP_SIZE | SWP_MOVE);

// set tapebar
 WinSetWindowPos(tapebar->frame, HWND_TOP,
                 0,
                 lineHeight,
                 width, 
                 tapebar->getHeight(),
                 SWP_SIZE | SWP_MOVE);

// set includes LB
 WinSetWindowPos(Includes->hwnd, HWND_TOP,
                 0,
                 lineHeight + tapebar->getHeight(),
                 width/2, 
                 2*tapebar->getHeight(),
                 SWP_SIZE | SWP_MOVE);

// set excludes LB
 WinSetWindowPos(Excludes->hwnd, HWND_TOP,
                 width/2,
                 lineHeight + tapebar->getHeight(),
                 width/2, 
                 2*tapebar->getHeight(),
                 SWP_SIZE | SWP_MOVE);

// set statline
 WinSetWindowPos(statline->frame, HWND_TOP, 
                 0, 
                 0,
                 width,
                 lineHeight,
                 SWP_SIZE | SWP_MOVE);
if ((!processing) && (!showMonitor)) {
  // set Drives
   WinSetWindowPos(Drives->hwnd, HWND_TOP,
                   0,
                   cnrHeight,
                   width/2,
                   height - toolbar->getHeight() - cnrHeight,
                   SWP_SIZE | SWP_MOVE);
  // set Files
   WinSetWindowPos(Files->hwnd, HWND_TOP,
                   width/2,
                   cnrHeight,
                   width/2,
                   height - toolbar->getHeight() - cnrHeight,
                   SWP_SIZE | SWP_MOVE);
  // set monitor LB
   WinSetWindowPos(monitor->hwnd, HWND_TOP,
                   0, 0, 0, 0,
                   SWP_SIZE | SWP_MOVE); }
 else {
  // set Drives
   WinSetWindowPos(Drives->hwnd, HWND_TOP,
                   0, 0, 0, 0,
                   SWP_SIZE | SWP_MOVE);
  // set Files
   WinSetWindowPos(Files->hwnd, HWND_TOP,
                   0, 0, 0, 0,
                   SWP_SIZE | SWP_MOVE);
  // set monitor LB
   WinSetWindowPos(monitor->hwnd, HWND_TOP,
                   0,
                   cnrHeight,
                   width, 
                   height - toolbar->getHeight() - cnrHeight,
                   SWP_SIZE | SWP_MOVE); }
}


BOOL GTPMWin::OCommand(ULONG msg, MPARAM mp1, MPARAM mp2)
{
 switch(msg)
  {
   case WM_SIZE:
    resizeClients(mp2);
    break;

   case WM_COMMAND:
     return(command(mp1, mp2));
 
   case WM_CONTROL:
     return(control(mp1, mp2));

   case WM_HITTEST:
     if ((wasOutOfContainer) && (statline) && (tapebar))
      {
       ULONG  topControls, bottomControls;

       WinQueryWindowPos(Files->hwnd, &sizepos);
       bottomControls = statline->getHeight() + tapebar->getHeight();
       topControls = bottomControls + sizepos.cy;
       if ((SHORT2FROMMP(mp1) < topControls) &&
           (SHORT2FROMMP(mp1) > bottomControls))
      {
         statline->setText("File Selection");
         wasOutOfContainer = FALSE; }
      }
     break;

   case WM_APPTERMINATENOTIFY: {
    CHAR    retMsg[25];
    ULONG   rc = SHORT1FROMMP(mp2);
 
    processing = FALSE;
    if (rc != 0)
      sprintf(retMsg, "Error! rc = %ld", rc);
    else
      sprintf(retMsg, "Done.");
    if (!printer.isRunning())
      printer.stop();
    monitor->insertItem(retMsg);
    break; }

   case WM_CLOSE:
    OFrame::OCommand(msg, mp1, mp2);
    GTPMApp::GTakPM->forceQuit();
    delete this;
    break;
 
   default:
    return(OFrame::OCommand(msg, mp1, mp2));
  } 
 return(TRUE); 
} 


void GTPMWin::expandFolder()
{
 pDirInfo record = Drives->queryEmphasis();

 if(record)
   Drives->recurse(record, FALSE, record->selected);
}


void GTPMWin::addExclude(PSZ str)
{
 OString  string("- ");

 string + str;
 Excludes->insertItem(string, LIT_SORTASCENDING);
}

void GTPMWin::addInclude(PSZ str)
{
 OString  string("+ ");

 string + str;
 Includes->insertItem(string, LIT_SORTASCENDING);
}


void GTPMWin::updateListbox(PSZ str)
{
 ULONG     item;
 
 while ((item = Includes->searchString(str, 
                          LIT_FIRST, 
                          LSS_SUBSTRING | LSS_CASESENSITIVE)) != LIT_NONE)
   Includes->deleteItem(item);
 while ((item = Excludes->searchString(str, 
                          LIT_FIRST, 
                          LSS_SUBSTRING | LSS_CASESENSITIVE)) != LIT_NONE)
   Excludes->deleteItem(item);
}


void GTPMWin::printout()
{
 OPMinit     init;

 while(pipe)
  {
   if (pipe.getline(readBuffer.text, bufferSize))
    {
     readBuffer - '\r';
     if (!monitor->insertItem(readBuffer)) {
       monitor->deleteAll();
       monitor->insertItem(readBuffer); }
    }
  }
}


// end of source
