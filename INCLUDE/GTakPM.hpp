// GTakPM
// (c) Benjamin Stein 1994
// All Rights Reserved
// GTakPM.hpp

#ifndef GTAKPM_HPP_INCLUDED
  #define GTAKPM_HPP_INCLUDED

// include defines

#define INCL_DOS
#define INCL_WIN
#define INCL_PM

#include <GTakPM.h>
#include <Resources.h>
#include <Messages.h>
#include <HelpPanels.h>
#include <Stringtable.h>

// include common headers

#include <ocl.hpp>
#include <pmwp.h>


// include OCL class definitions

#include <OXApp.hpp>
#include <OXcptPM.hpp>
#include <OVioException.hpp>
#include <OPMException.hpp>
#include <OString.hpp>
#include <OPMinit.hpp>
#include <OCommand.hpp>
#include <OCreate.hpp>
#include <OWindow.hpp>
#include <OFrame.hpp>
#include <OBook.hpp>
#include <OThread.hpp>
#include <OProfile.hpp>
#include <OSysInfo.hpp>
#include <OSystem.hpp>
#include <ODialog.hpp>
#include <OMSGS.hpp>
#include <OLogo.hpp>
#include <OHelp.hpp>
#include <OIndicator.hpp>
#include <OListbox.hpp>
#include <OEntryField.hpp>
#include <OContainer.hpp>
#include <OMle.hpp>
#include <ODynamicLib.hpp>
#include <OStatusLine.hpp>
#include <OButtonBar.hpp>
#include <OFcl.hpp>
#include <OPMPipeTool.hpp>

// foreward class declarations

typedef class GTPMException   *pGTPMException;
typedef class SystemHandler   *pSystemHandler;
typedef class GTPMApp         *pGTPMApp;
typedef class GTPMWin         *pGTPMWin;
typedef class DrivesCnr       *pDrivesCnr;
typedef class FilesCnr        *pFilesCnr; 

// foreward struct declarations

typedef struct FileInfo *pFileInfo;
typedef struct DirInfo  *pDirInfo;
typedef struct DirItem  *pDirItem;

// includes class definitions

#include <Tape.hpp>
#include <GTPMException.hpp>    // GTakPM exception class and system exception handler
#include <GTPMApp.hpp>          // GTakPM application class 
#include <GTPMWin.hpp>          // GTakPM main window




#endif // GTAKPM_HPP_INCLUDED

// end of source
