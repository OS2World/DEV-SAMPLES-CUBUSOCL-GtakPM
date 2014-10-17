// GTakPM
// (c) Benjamin Stein 1994
// All Rights Reserved
// GTPMException.hpp


#ifndef GTPMEXCEPTION_HPP_INCLUDED
  #define GTPMEXCEPTION_HPP_INCLUDED


#ifndef GTAKPM_HPP_INCLUDED
  #include <GTakPM.hpp>
#endif



class GTPMException : public OPMException
{
 public:
   GTPMException(const ULONG errResource,
                 const ULONG severity = OException::recoverable);
   ~GTPMException();
};

#define throwGTPM(x) (throw GTPMException(x))


class SystemHandler : public OXcptPM
{
 public:
   SystemHandler();
   ~SystemHandler();
   BOOL killTrace();
   BOOL trapTrace();
};

#endif // GTPMEXCEPTION_HPP_INCLUDED

// end of source
