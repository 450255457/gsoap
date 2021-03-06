#include "calc.nsmap"        // XML namespace mapping table (only needed once at the global level)
#include "soapcalcService.h" // the service class, also #includes "soapH.h" and "soapStub.h"

int main()
{
  calcService calc(SOAP_XML_INDENT);
  calc.soap->send_timeout = 5; // send timeout is 5s
  calc.soap->recv_timeout = 5; // receive timeout is 5s
  while (calc.run(8090) != SOAP_OK)  
    calc.soap_stream_fault(std::cerr);
  calc.destroy(); // same as: soap_destroy(calc.soap); soap_end(calc.soap);
}
int calcService::add(double a, double b, double &result)
{
  result = a + b;
  return SOAP_OK;
}
int calcService::sub(double a, double b, double &result)
{
  result = a - b;
  return SOAP_OK;
}
int calcService::mul(double a, double b, double &result)
{
  result = a * b;
  return SOAP_OK;
}
int calcService::div(double a, double b, double &result)
{
  if (b == 0.0)
  {
    char *msg = (char*)soap_malloc(this->soap, 1024);
    snprintf(msg, 1024, "Trying to divide %f by zero", a);
    return this->soap_senderfault(msg, NULL);
  }
  result = a / b;
  return SOAP_OK;
}
int calcService::pow(double a, double b, double &result)
{
  result = ::pow(a, b);
  // soap_errno is like errno, but compatible with Win32
  if (soap_errno == EDOM)
  {
    char *msg = (char*)soap_malloc(this->soap, 1024);
    snprintf(msg, 1024, "<error xmlns=\"http://tempuri.org/\">Can't take power of %f to %f</error>", a, b);
    return this->soap_senderfault("Power function domain error", msg);
  }
  return SOAP_OK;
}
