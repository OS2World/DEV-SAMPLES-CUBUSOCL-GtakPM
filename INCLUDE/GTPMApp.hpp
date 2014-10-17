// GTakPM
// (c) Benjamin Stein 1994
// All Rights Reserved
// GTPMApp.hpp


#ifndef GTPMAPP_HPP_INCLUDED
  #define GTPMAPP_HPP_INCLUDED


#ifndef GTAKPM_HPP_INCLUDED
  #include <GTakPM.hpp>
#endif


class GTPMApp : public OXApp
{
 private:
   OThread<GTPMApp> waitThread;

 protected:

 public:
   ODynamicLib  *resLibrary;


   GTPMApp(const ULONG num, CHAR *parms[]);   
   ~GTPMApp();

   void initArgs(PCSZ argStr);
   void wait();

   BOOL TasklistKill();              // trap task list kills
   BOOL ShutdownPosted();            // trap shutdown message
 
// static

   static pGTPMApp GTakPM;
};



#endif // GTPMAPP_HPP_INCLUDED

// end of source
