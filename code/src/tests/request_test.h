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
#include "../request.h"
#include "../context.h"


#ifndef REQUEST_FIXTURE_CPP
#define REQUEST_FIXTURE_CPP

using bim::Request;
using bim::Context;

const char* CRLF = "\r\n";
const char* CRLF2 = "\r\n\r\n";

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
      context->set_document_root(".");

      req_minimal = new Request(2, *context);
      req_firefox4 = new Request(2, *context);

      req_minimal->append_data("GET / HTTP/1.1\r\n");
      req_minimal->append_data(CRLF2);

      req_firefox4->append_data("GET /static/index.html HTTP/1.1\r\n"
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
      std::cout << req_minimal->get_path() << std::endl;
      CPPUNIT_ASSERT(req_minimal->get_path() == "./");
      CPPUNIT_ASSERT(req_minimal->get_path() == "./"); //testing caching
      CPPUNIT_ASSERT(req_firefox4->get_path() == "./static/index.html"); 
    }

    void test_get_request_line()
    {
      std::cerr << "req:" << req_minimal->get_request_line() << std::endl;
      CPPUNIT_ASSERT(req_minimal->get_request_line() == "GET / HTTP/1.1");
      CPPUNIT_ASSERT(req_minimal->get_request_line() == "GET / HTTP/1.1"); //testing caching
      CPPUNIT_ASSERT(req_firefox4->get_request_line() == "GET /static/index.html HTTP/1.1");
    }

    void tearDown()
    {

    }

    CPPUNIT_TEST_SUITE( RequestFixture );
    CPPUNIT_TEST(test_method);
    CPPUNIT_TEST(test_get_url);
    CPPUNIT_TEST(test_get_path);
    CPPUNIT_TEST(test_get_request_line);
    CPPUNIT_TEST_SUITE_END();
};

#endif
