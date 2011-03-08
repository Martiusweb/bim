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

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/Test.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#include "../log.h"

#ifndef LOG_FIXTURE_CPP
#define LOG_FIXTURE_CPP

class LogFixture : public CppUnit::TestFixture
{
  public:
    void setUp()
    {
    }

    void log_basic()
    {
      bim::error_log("plop ?");
      bim::access_log("plop ?");

      struct stat statbuf;
      CPPUNIT_ASSERT(stat("error_log", &statbuf) == 0);
      CPPUNIT_ASSERT(stat("access_log", &statbuf) == 0);

      CPPUNIT_ASSERT(unlink("error_log") == 0);
      CPPUNIT_ASSERT(unlink("access_log") == 0);
    }

    void tearDown()
    {
    }

    CPPUNIT_TEST_SUITE( LogFixture );
    CPPUNIT_TEST(log_basic);
    CPPUNIT_TEST_SUITE_END();
};

#endif
