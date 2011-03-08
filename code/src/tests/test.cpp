/**
 * Copyright (c) 2011, Paul ADENOT & Martin Richard
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the bim nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File author : Paul ADENOT <paul@paul.cx>
 *
 **/

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
#include "log_test.h"

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
  runner.addTest(LogFixture::suite());

  runner.run(controller);
  CompilerOutputter outputter( &result, CPPUNIT_NS::stdCOut() );
  outputter.write();

  std::ofstream file( "cppunit-report.xml" );
  XmlOutputter xml( &result, file );
  xml.write();
  file.close();

  return result.wasSuccessful();
}
