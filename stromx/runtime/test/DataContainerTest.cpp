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

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <cppunit/TestAssert.h>
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/test/DataContainerTest.h"
#include "stromx/runtime/test/TestData.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::DataContainerTest);

namespace stromx
{
    namespace runtime
    {
        void DataContainerTest::testDestroy()
        {
            {
                DataContainer copy;
                {
                    DataContainer container(new TestData());
                    copy = container;
                    CPPUNIT_ASSERT(! TestData::wasDestructed);
                }
                
                CPPUNIT_ASSERT(! TestData::wasDestructed);
            }
            
            CPPUNIT_ASSERT(TestData::wasDestructed);
        }
        
        void DataContainerTest::testRelease()
        {
            DataContainer container(new TestData());
            container.release();

            CPPUNIT_ASSERT(TestData::wasDestructed);
            CPPUNIT_ASSERT(container.empty());
        }
                
        void DataContainerTest::testDestroyDelayed()
        {
            {
                DataContainer container(new TestData());
                boost::thread t(boost::bind(&DataContainerTest::destroyDelayed, this, _1), container);
                CPPUNIT_ASSERT(! TestData::wasDestructed);
                
                t.join();
                CPPUNIT_ASSERT(! TestData::wasDestructed);
            }
            CPPUNIT_ASSERT(TestData::wasDestructed);
        }
        
        void DataContainerTest::destroyDelayed(DataContainer&)
        {
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
        }
        
        void DataContainerTest::testCompare()
        {
            Data* data = new TestData();
            
            DataContainer container1(data);
            DataContainer container2 = container1;
            CPPUNIT_ASSERT(container1 == container2);
            
            DataContainer container3;
            DataContainer container4;
            CPPUNIT_ASSERT(container3 == container4);
            CPPUNIT_ASSERT(container1 != container3);
        }
        
        void DataContainerTest::testEmpty()
        {
            DataContainer container;
            CPPUNIT_ASSERT(container.empty());
            
            Data* data = new TestData();
            container = DataContainer(data);
            CPPUNIT_ASSERT(! container.empty());
            
            container = DataContainer();
            CPPUNIT_ASSERT(container.empty());
        }
    }
}