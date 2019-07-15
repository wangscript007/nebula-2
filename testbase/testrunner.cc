//------------------------------------------------------------------------------
//  testrunner.cc
//  (C) 2006 Radon Labs GmbH
//  (C) 2013-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "testrunner.h"
#include "stackdebug.h"

namespace Test
{
__ImplementClass(Test::TestRunner, 'TSTR', Core::RefCounted);

using namespace Core;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
TestRunner::AttachTestCase(TestCase* testCase)
{
    n_assert(testCase);
    this->testCases.Append(testCase);
}

//------------------------------------------------------------------------------
/**
*/
bool
TestRunner::Run()
{
    IndexT i;
    int numSucceeded = 0;
    int numFailed = 0;
    char checkpointBuffer[100];
    for (i = 0; i < this->testCases.Size(); i++)
    {
        TestCase* curTestCase = this->testCases[i];
        n_printf("-> Running test: %s\n", curTestCase->GetClassName().AsCharPtr());        
        sprintf(checkpointBuffer, "%s::Run() before", curTestCase->GetClassName().AsCharPtr());
        STACK_CHECKPOINT(checkpointBuffer);
        curTestCase->Run();
        sprintf(checkpointBuffer, "%s::Run() after", curTestCase->GetClassName().AsCharPtr());
        STACK_CHECKPOINT(checkpointBuffer);
        if (curTestCase->GetNumFailed() == 0)
        {
            n_printf("-> SUCCESS: %s runs %d tests ok!\n", curTestCase->GetClassName().AsCharPtr(), curTestCase->GetNumSucceeded());
        }
        else
        {
            n_printf("-> FAILURE: %d of %d tests failed in %s!\n", 
                curTestCase->GetNumFailed(),
                curTestCase->GetNumVerified(),
                curTestCase->GetClassName().AsCharPtr());
        }
        numFailed += curTestCase->GetNumFailed();
        numSucceeded += curTestCase->GetNumSucceeded();
        n_printf("\n");
    }
    n_printf("* TEST RESULT: %d succeeded, %d failed!\n", numSucceeded, numFailed);

    for (auto & c : testCases)
    {
        if (c->GetNumFailed() > 0)
        {
            auto const & failed = c->GetFailed();
            for (auto & f : failed)
            {
                n_printf("%s(%d): failed \"%s\"\n", f.file.AsCharPtr(), f.line, f.compare.AsCharPtr());
            }
        }
    }
    DUMP_STACK_CHECKPOINTS;
    return numFailed == 0;
}

}; // namespace Test
