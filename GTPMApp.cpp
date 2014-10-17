// GTakPM
// (c) Benjamin Stein 1994
// All Rights Reserved
// GTPMApp.cpp


#include <GTakPM.hpp>


pGTPMApp GTPMApp::GTakPM = NULL;


GTPMApp::GTPMApp(const ULONG num, CHAR *parms[])
  : OXApp(num, parms),
    resLibrary(NULL),
    waitThread(this, GTPMApp::wait, 4096, FALSE)
{
 OLogo       *logo = NULL;
 OString     resLibName;
 GTPMWin     *applicationWindow = NULL;

 GTPMApp::GTakPM = this;

 waitThread.run();              // start logo timer

 resLibName << callName;
 resLibName - '\\';
 resLibName + "\\GTAKPM.RSL";   // full filename of resource library

 try                            // load resource library and show the logo indefinite
  {
   resLibrary = new ODynamicLib(resLibName.text); 
   logo = new OLogo(resLibrary->getLibHandle(), GTPM_LOGO, 300, 225);
   logo->showLogo(0);
   applicationWindow = new GTPMWin;
  }

 catch(OVioException &ex)       // resource library not found!
   {
    OMsgs msg;
   
    msg.error(ERROR_RESOURCELIB_NOT_FOUND);
    throw ex; 
   }
 catch(OPMException &ex)
   {
    ex.viewError();
    resLibrary = NULL;              // do not delete empty object
    throw ex;
   }
 
 if (logo) {
   waitThread.waitFor();        // show logo min. 2 sec. 
   delete logo; }               // destroy logo

 applicationWindow->showFrame();
}   


GTPMApp::~GTPMApp()
{
 if (resLibrary)
   delete resLibrary;
}


void GTPMApp::initArgs(PCSZ argStr)
{

}


void GTPMApp::wait()
{
 DosSleep(2000);
}



// overridden members of OApp


BOOL GTPMApp::TasklistKill()   // deny task list kill and continue
{
 OMsgs msg;

 msg.error(APP_TITLE, DENY_TASK_KILL);
 return(TRUE);   
}



BOOL GTPMApp::ShutdownPosted()
{
 return(FALSE);
}


// end of source
