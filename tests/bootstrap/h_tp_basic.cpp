//
// Automated Testing Framework (atf)
//
// Copyright (c) 2007 The NetBSD Foundation, Inc.
// All rights reserved.
//
// This code is derived from software contributed to The NetBSD Foundation
// by Julio M. Merino Vidal, developed as part of Google's Summer of Code
// 2007 program.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. All advertising materials mentioning features or use of this
//    software must display the following acknowledgement:
//        This product includes software developed by the NetBSD
//        Foundation, Inc. and its contributors.
// 4. Neither the name of The NetBSD Foundation nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND
// CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include <libatf.hpp>
#include <libatfmain.hpp>

ATF_TEST_CASE(tc_pass);

ATF_TEST_CASE_HEAD(tc_pass)
{
    set("descr", "An empty test case that always passes");
}

ATF_TEST_CASE_BODY(tc_pass)
{
    ATF_PASS();
}

ATF_TEST_CASE(tc_fail);

ATF_TEST_CASE_HEAD(tc_fail)
{
    set("descr", "An empty test case that always fails");
}

ATF_TEST_CASE_BODY(tc_fail)
{
    ATF_FAIL("On purpose");
}

ATF_TEST_CASE(tc_skip);

ATF_TEST_CASE_HEAD(tc_skip)
{
    set("descr", "An empty test case that is always skipped");
}

ATF_TEST_CASE_BODY(tc_skip)
{
    ATF_SKIP("By design");
}

ATF_TEST_CASE(tc_bogus);

ATF_TEST_CASE_HEAD(tc_bogus)
{
    set("descr", "A bogus test case that always fails by being "
                 "incorrectly defined");
}

ATF_TEST_CASE_BODY(tc_bogus)
{
    // Fails because it does not return any result explicitly.
}

ATF_INIT_TEST_CASES(tcs)
{
    tcs.push_back(&tc_pass);
    tcs.push_back(&tc_fail);
    tcs.push_back(&tc_skip);
    tcs.push_back(&tc_bogus);
}