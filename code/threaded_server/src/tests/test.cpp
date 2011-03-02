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

#include "request_test.h"

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
