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

#ifndef STROMX_PYTHON_EXPORTOPERATORKERNEL_H
#define STROMX_PYTHON_EXPORTOPERATORKERNEL_H

#include <boost/lambda/lambda.hpp>
#include <boost/python.hpp>

#include <stromx/runtime/OperatorKernel.h>

using namespace stromx::runtime;
using namespace boost::python;

namespace stromx
{
    namespace python
    {
        template <class operator_t>
        boost::shared_ptr<OperatorKernel> allocate()
        {
            return boost::shared_ptr<OperatorKernel>(new operator_t, boost::lambda::_1);
        }
        
        template <class operator_t>
        void exportOperatorKernel(const char* const name)
        {
            class_<operator_t, bases<OperatorKernel>, boost::shared_ptr<operator_t>, boost::noncopyable>(name, no_init)
                .def("__init__", make_constructor(&allocate<operator_t>))
            ;
        }
    }
}

#endif // STROMX_PYTHON_EXPORTOPERATORKERNEL_H