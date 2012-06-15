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

#include <stromx/core/XmlReader.h>
#include <stromx/core/Factory.h>
#include <stromx/core/Stream.h>

#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::core;

namespace
{
    typedef Stream* (XmlReader::*read_stream_t)(const std::string &, const Factory &);
    Stream* const (XmlReader::*readStreamFromFileWrap)(const std::string &, const Factory &) const = &XmlReader::readStream;
    void (XmlReader::*readParametersFromFileWrap)(const std::string &, const Factory&, const std::vector<stromx::core::Operator*> &) const = &XmlReader::readParameters;
}

void exportXmlReader()
{       
    class_<XmlReader>("XmlReader")
        .def("readStream", reinterpret_cast<read_stream_t>(readStreamFromFileWrap), return_value_policy<manage_new_object>())
        .def("readParameters", readParametersFromFileWrap)
    ;
}