/* 
*  Copyright 2011 Matthias Fuchs
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#ifndef BASE_CAMERATEST_H
#define BASE_CAMERATEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace stream
{
    class SynchronizedOperatorKernel;
    class DataContainer;
}

namespace base
{
    class CameraTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (CameraTest);
        CPPUNIT_TEST (testExecuteSoftwareTrigger);
        CPPUNIT_TEST (testExecuteInternalTrigger);
        CPPUNIT_TEST (testAdjustROI);
        CPPUNIT_TEST (testAdjustPixelType);
        CPPUNIT_TEST_SUITE_END ();

        public:
            CameraTest() : m_operator(0) {}
            
            void setUp();
            void tearDown();

        protected:
            void testExecuteSoftwareTrigger();
            void testExecuteInternalTrigger();
            void testAdjustROI();
            void testAdjustPixelType();
            
        private:
            stream::SynchronizedOperatorKernel* m_operator;
    };
}

#endif // BASE_CAMERATEST_H