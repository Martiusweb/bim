/// Author : Paul ADENOT <paul@paul.cx>
/// License : WTFPL <http://sam.zoy.org/wtfpl/>

#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include "../request.h"
#include "../context.h"

const char* CRLF = "\r\n";
const char* CRLF2 = "\r\n\r\n";

using bim::Request;
using bim::Context;

class RequestFixture : public CppUnit::TestFixture
{
  private:
    Context* context;
    Request* req_minimal;
    Request* req_firefox4;
  public:
    void setUp()
    {
      context = new Context();
      context->set_document_root("/home/site");

      req_minimal = new Request(2, *context);
      req_firefox4 = new Request(2, *context);

      req_minimal->append_data("GET / HTTP/1.1");
      req_minimal->append_data(CRLF2);

      req_firefox4->append_data("GET /static/index.html HTTP/1.1"
                                "Host: localhost:8080"
                                "User-Agent: Mozilla/5.0 (X11; Linux i686; rv:2.0b13pre) Gecko/20110226 Firefox/4.0b13pre"
                                "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
                                "Accept-Language: en-us,en;q=0.5"
                                "Accept-Encoding: gzip, deflate"
                                "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7"
                                "Keep-Alive: 115"
                                "Connection: keep-alive");
      req_firefox4->append_data(CRLF2);
    }

    void test_method()
    {
      CPPUNIT_ASSERT(req_minimal->get_method() == "GET");
      CPPUNIT_ASSERT(req_minimal->get_method() == "GET"); // testing caching
      CPPUNIT_ASSERT(req_firefox4->get_method() == "GET");
    }

    void test_get_url()
    {
      CPPUNIT_ASSERT(req_minimal->get_url() == "/");
      CPPUNIT_ASSERT(req_minimal->get_url() == "/"); // testing caching
      CPPUNIT_ASSERT(req_firefox4->get_url() == "/static/index.html");
    }

    void test_get_path()
    {
      CPPUNIT_ASSERT(req_minimal->get_path() == "/home/site/");
      CPPUNIT_ASSERT(req_minimal->get_path() == "/home/site/"); //testing caching
      CPPUNIT_ASSERT(req_firefox4->get_path() == "/home/site/static/index.html"); 
    }

    void tearDown()
    {
    
    }
//    static CppUnit::Test* suite()
//    {
//      CppUnit::TestSuite* suite = new CppUnit::TestSuite("Request test suite");
//      suite->addTest(new CppUnit::TestCaller<RequestFixture>("Method",
//            &RequestFixture::test_method));
//      suite->addTest(new CppUnit::TestCaller<RequestFixture>("get url",
//            &RequestFixture::test_get_url));
//      suite->addTest(new CppUnit::TestCaller<RequestFixture>("get path",
//            &RequestFixture::test_get_path));
//      return suite;
//    }

    CPPUNIT_TEST_SUITE( RequestFixture );
      CPPUNIT_TEST(test_method);
      CPPUNIT_TEST(test_get_url);
      CPPUNIT_TEST(test_get_path);
    CPPUNIT_TEST_SUITE_END();
};

using namespace CppUnit;

int main()
{
  TextUi::TestRunner runner;
  TestResult controller;

  TestResultCollector result;
  controller.addListener( &result );

  BriefTestProgressListener progress;
  controller.addListener( &progress );

  runner.addTest(RequestFixture::suite());

  runner.run(controller);
  CompilerOutputter outputter( &result, CPPUNIT_NS::stdCOut() );
  outputter.write();

  std::ofstream file( "cppunit-report.xml" );
  XmlOutputter xml( &result, file );
  xml.write();
  file.close();

  return result.wasSuccessful();
}
