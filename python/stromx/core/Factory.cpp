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

#include <stromx/core/Operator.h>
#include <stromx/core/OperatorKernel.h>
#include <stromx/core/Factory.h>
#include <stromx/core/Data.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;


namespace
{
    class FactoryWrap : public Factory
    {
    public:
        virtual std::auto_ptr<Operator> newOperatorWrap(const std::string & package, const std::string & name) const
        {
            return std::auto_ptr<Operator>(newOperator(package, name));
        }
        
        virtual std::auto_ptr<Data> newDataWrap(const std::string & package, const std::string & name) const
        {
            return std::auto_ptr<Data>(newData(package, name));
        }
    };  
}


void exportFactory()
{          
    typedef Operator* (Factory::*newOperatorType)(const std::string & package, const std::string & name) const; 
    typedef Data* (Factory::*newDataType)(const std::string & package, const std::string & name) const; 
    
    class_<FactoryWrap, bases<Registry> >("Factory")
        .def("registerOperator", &FactoryWrap::registerOperator)
        .def("registerData", &FactoryWrap::registerData)
        .def("newOperator", &FactoryWrap::newOperatorWrap)
        .def("newData", &FactoryWrap::newDataWrap)
    ;
}