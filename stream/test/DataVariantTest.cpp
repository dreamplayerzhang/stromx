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

#ifndef STREAM_DATAVARIANTTEST_H
#define STREAM_DATAVARIANTTEST_H

#include "DataVariantTest.h"

#include <stream/DataVariant.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::DataTypeTest);

namespace stream
{
    void DataTypeTest::testImageTypes()
    {
        CPPUNIT_ASSERT(DataVariant::RGB_IMAGE.is(DataVariant::IMAGE));
        CPPUNIT_ASSERT(DataVariant::RGB_24_IMAGE.is(DataVariant::IMAGE));
        CPPUNIT_ASSERT(DataVariant::RGB_24_IMAGE.is(DataVariant::RGB_IMAGE));
        
        CPPUNIT_ASSERT(! DataVariant::IMAGE.is(DataVariant::RGB_IMAGE));
        CPPUNIT_ASSERT(! DataVariant::IMAGE.is(DataVariant::RGB_24_IMAGE));
        CPPUNIT_ASSERT(! DataVariant::RGB_IMAGE.is(DataVariant::RGB_24_IMAGE));
        
        CPPUNIT_ASSERT(DataVariant::MONO_IMAGE.is(DataVariant::IMAGE));
        CPPUNIT_ASSERT(DataVariant::MONO_8_IMAGE.is(DataVariant::IMAGE));
        CPPUNIT_ASSERT(DataVariant::MONO_8_IMAGE.is(DataVariant::MONO_IMAGE));
    }

    void DataTypeTest::testIntTypes()
    {
        CPPUNIT_ASSERT(DataVariant::UINT.is(DataVariant::INT));
        CPPUNIT_ASSERT(DataVariant::UINT_8.is(DataVariant::UINT));
        CPPUNIT_ASSERT(DataVariant::UINT_16.is(DataVariant::UINT));
        CPPUNIT_ASSERT(DataVariant::UINT_32.is(DataVariant::UINT));
        
        CPPUNIT_ASSERT(! DataVariant::INT.is(DataVariant::UINT));
        CPPUNIT_ASSERT(! DataVariant::UINT.is(DataVariant::UINT_8));
        CPPUNIT_ASSERT(! DataVariant::UINT.is(DataVariant::UINT_16));
        CPPUNIT_ASSERT(! DataVariant::UINT.is(DataVariant::UINT_32));
    }
}

#endif // STREAM_DATAVARIANTTEST_H