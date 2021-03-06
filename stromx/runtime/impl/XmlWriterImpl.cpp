/* 
 *  Copyright 2011 Thomas Fidler
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

#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include "stromx/runtime/Config.h"
#include "stromx/runtime/Data.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/FileOutput.h"
#include "stromx/runtime/InputConnector.h"
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/Thread.h"
#include "stromx/runtime/impl/XmlUtilities.h"
#include "stromx/runtime/impl/XmlWriterImpl.h"

using namespace xercesc;

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            
            const unsigned int XmlWriterImpl::XML_FORMAT_VERSION_MAJOR = 0;
            const unsigned int XmlWriterImpl::XML_FORMAT_VERSION_MINOR = 3;
            const unsigned int XmlWriterImpl::XML_FORMAT_VERSION_REVISION = 0;
                
            XmlWriterImpl::XmlWriterImpl() 
                : m_stream(0),
                  m_opList(0),
                  m_output(0), 
                  m_filename(""), 
                  m_impl(0), 
                  m_doc(0), 
                  m_stromxElement(0), 
                  m_strElement(0),
                  m_parsElement(0)
            {
                try
                {
                    XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
                }
                catch (const XMLException& toCatch)
                {
                    char* message = XMLString::transcode(toCatch.getMessage());
                    std::cerr << "Error during initialization! :\n"
                        << message << "\n";
                    XMLString::release(&message);
                    return;
                }
                
                try
                {
                    m_impl = DOMImplementationRegistry::getDOMImplementation(Str2Xml(""));
                }
                catch(DOMException&)
                {
                    throw InternalError("Error in Xerces-C.");
                }
                catch(XMLException&)
                {
                    throw InternalError("Error in Xerces-C.");
                }
            }
            
            void XmlWriterImpl::createDoc()
            {
                m_doc = m_impl->createDocument(0, Str2Xml("Stromx"), 0);
            }
            
            void XmlWriterImpl::createComm()
            {
                DOMComment* comment = m_doc->createComment(Str2Xml("XML generated automatically by XmlWriter of the open source library Stromx"));
                m_doc->appendChild(comment); 
            }

            void XmlWriterImpl::createStromx()
            {
                //Create Stromx branch           
                m_stromxElement = m_doc->getDocumentElement();              
            
                //Create attribute version of Stromx
                DOMAttr* verAttr = m_doc->createAttribute(Str2Xml("version"));
                Version version(XML_FORMAT_VERSION_MAJOR, XML_FORMAT_VERSION_MINOR, XML_FORMAT_VERSION_REVISION);
                std::string str = boost::lexical_cast<std::string>(version);
                verAttr->setValue(Str2Xml(str.c_str()));
                m_stromxElement->setAttributeNode(verAttr);
            }

            
            unsigned int XmlWriterImpl::translateOperatorPointerToID(const Operator* const op) const
            {
                if (m_opList.empty())
                {
                    throw InternalError("No operator list available");
                }
                
                unsigned int count_op = 0;
                for(std::vector<const Operator*>::const_iterator iter_op = m_opList.begin();
                        iter_op != m_opList.end();
                        ++iter_op, ++count_op)
                {
                    if ((*iter_op) == op)
                    {
                        return count_op;
                    }
                }
                
                throw InternalError("Operator does not exist.");
            }
            
            void XmlWriterImpl::createInputConnectors(const Thread* const currThr, DOMElement* const thrElement)
            {
                //Add InputConnector branches (tree structure: Stream:Thread:InputConnector)
                //Processed for each InputConnector belonging to Thread (multiple entries for each Thread possible)
                for(std::vector<InputConnector>::const_iterator iter_inConnectors = currThr->inputSequence().begin();
                    iter_inConnectors != currThr->inputSequence().end();
                    ++iter_inConnectors)
                {
                    //Create current InputNode being child of Thread 
                    DOMElement* inConnectorElement = m_doc->createElement(Str2Xml("InputConnector"));
                    thrElement->appendChild(inConnectorElement);
                    
                    //Create attribute operator of current InputNode (one for each InputNode possible)
                    DOMAttr* opAttr = m_doc->createAttribute(Str2Xml("operator"));
                    unsigned int opId = translateOperatorPointerToID((*iter_inConnectors).op());
                    opAttr->setValue(Str2Xml(boost::lexical_cast<std::string>(opId).c_str()));
                    inConnectorElement->setAttributeNode(opAttr);
                    
                    //Create attribute input of current InputNode (one for each InputNode possible)
                    DOMAttr* inputAttr = m_doc->createAttribute(Str2Xml("input"));
                    inputAttr->setValue(Str2Xml(boost::lexical_cast<std::string>((*iter_inConnectors).id()).c_str()));
                    inConnectorElement->setAttributeNode(inputAttr);
                    
                    DOMAttr* inputTitleAttr = m_doc->createAttribute(Str2Xml("inputTitle"));
                    const Input& desc = (*iter_inConnectors).op()->info().input((*iter_inConnectors).id());
                    inputTitleAttr->setValue(Str2Xml(desc.title().c_str()));
                    inConnectorElement->setAttributeNode(inputTitleAttr);
                }
            }
            
            void XmlWriterImpl::createThreads(const std::vector<Thread*> & threads)
            {
                //Add Thread branches (tree structure: Stream:Thread)
                //Processed for each thread belonging to the strom object (multiple entries for strom possible)
                for(std::vector<Thread*>::const_iterator iter_thr = threads.begin();
                        iter_thr != threads.end();
                        ++iter_thr)
                {
                    //Create current Thread being child of Stream (one for each Thread possible)
                    DOMElement* thrElement = m_doc->createElement(Str2Xml("Thread"));
                    m_strElement->appendChild(thrElement);
                    
                    //Create attribute name of Thread (one for each Thread possible)
                    DOMAttr* nameAttr = m_doc->createAttribute(Str2Xml("name"));
                    nameAttr->setValue(Str2Xml((*iter_thr)->name().c_str()));
                    thrElement->setAttributeNode(nameAttr);
                    
                    //Create attribute color of Thread (one for each Thread possible)
                    DOMAttr* colorAttr = m_doc->createAttribute(Str2Xml("color"));
                    std::string colorStr = boost::lexical_cast<std::string>((*iter_thr)->color());
                    colorAttr->setValue(Str2Xml(colorStr.c_str()));
                    thrElement->setAttributeNode(colorAttr);
                    
                    //Create InputNodes of Thread (multiple entries for each Thread possible)
                    createInputConnectors((*iter_thr), thrElement);
                }
            }
            
            void XmlWriterImpl::createParameters(const Operator* const currOp, DOMElement* const opElement)
            {
                //Add parameter branches (tree structure: strom:operator:parameter)
                //Processed for each parameter belonging to current operator currOp (multiple entries for each operator possible)
                for(std::vector<const Parameter*>::const_iterator iter_par = currOp->info().parameters().begin();
                            iter_par != currOp->info().parameters().end();
                            ++iter_par)
                {     
                    //Create current parameter entry param being child of current operator op (one for each parameter possible)
                    DOMElement* parElement = m_doc->createElement(Str2Xml("Parameter"));
                    opElement->appendChild(parElement);
                    
                    //Create attribute id of current parameter param (one for each parameter possible)
                    DOMAttr* id = m_doc->createAttribute(Str2Xml("id"));
                    id->setValue(Str2Xml(boost::lexical_cast<std::string>((*iter_par)->id()).c_str()));
                    parElement->setAttributeNode(id);
                    
                    DOMAttr* titleAttr = m_doc->createAttribute(Str2Xml("title"));
                    const Parameter& desc = currOp->info().parameter((*iter_par)->id());
                    titleAttr->setValue(Str2Xml(desc.title().c_str()));
                    parElement->setAttributeNode(titleAttr);
                    
                    DOMAttr* behaviorAttr = m_doc->createAttribute(Str2Xml("behavior"));
                    switch (desc.updateBehavior())
                    {
                    case Description::PERSISTENT:
                        behaviorAttr->setValue(Str2Xml("persistent"));
                        break;
                    case Description::PULL:
                        behaviorAttr->setValue(Str2Xml("pull"));
                        break;
                    case Description::PUSH:
                        behaviorAttr->setValue(Str2Xml("push"));
                        break;
                    case Description::INTERNAL:
                        behaviorAttr->setValue(Str2Xml("internal"));
                        break;
                    }
                    parElement->setAttributeNode(behaviorAttr);
                                   
                    // Do not persist push operators
                    if ((*iter_par)->updateBehavior() == Parameter::PUSH)
                        continue;
                    
                    // Do not persist pull operators
                    if ((*iter_par)->updateBehavior() == Parameter::PULL)
                        continue;
                    
                    try
                    {
                        // Try to access the parameter in question. 
                        currOp->getParameter((*iter_par)->id());
                    }
                    catch(ParameterError&)
                    {
                        // If the access fails continue with the next parameter.
                        continue;
                    }
                    
                    createData(*iter_par, currOp, parElement);
                    
                }
            }
            
            void XmlWriterImpl::createData(const Parameter*const currPar, const Operator*const currOp, DOMElement*const parElement)
            {
                //Add Data branch
                //Create Data entry data being child of current param (one for each parameter possible)
                DOMElement* dataElement = m_doc->createElement(Str2Xml("Data"));
                parElement->appendChild(dataElement);
                
                //Create attribute type of data (one for each data possible)
                DOMAttr* typeAttr = m_doc->createAttribute(Str2Xml("type"));
                typeAttr->setValue(Str2Xml(currOp->getParameter(currPar->id()).type().c_str()));
                dataElement->setAttributeNode(typeAttr);
                
                //Create attribute package of data (one for each data possible)
                DOMAttr* packageAttr = m_doc->createAttribute(Str2Xml("package"));
                packageAttr->setValue(Str2Xml(currOp->getParameter(currPar->id()).package().c_str()));
                dataElement->setAttributeNode(packageAttr);
                
                //Create attribute version of data (one for each data possible)
                DOMAttr* verAttr = m_doc->createAttribute(Str2Xml("version"));
                std::string str1 = boost::lexical_cast<std::string>(currOp->getParameter(currPar->id()).version());
                verAttr->setValue(Str2Xml(str1.c_str()));
                dataElement->setAttributeNode(verAttr);
                
                //Create value of current parameter param (one for each parameter possible)
                //First, create unique input parameter name for function Data::serialize()
                std::string filename = m_filename +
                                        "_op" + boost::lexical_cast<std::string>(translateOperatorPointerToID(currOp)) + 
                                        "_parameter" + boost::lexical_cast<std::string>(currPar->id());
                                    
                m_output->initialize(filename);
                DataRef data = currOp->getParameter(currPar->id());
                
                try
                {
                    data.serialize(*m_output);
                }
                catch(FileException &)
                {
                    throw;
                }
                catch(std::exception & e)
                {
                    throw SerializationError(data.package(), data.type(), e.what());
                }
                
                //Create attribute for file
                if(! m_output->getFilename().empty())
                { 
                    DOMAttr* fileAttr = m_doc->createAttribute(Str2Xml("file"));
                    fileAttr->setValue(Str2Xml(m_output->getFilename().c_str()));
                    dataElement->setAttributeNode(fileAttr);
                }
                
                DOMText* value = m_doc->createTextNode(Str2Xml(m_output->getText().c_str()));
                dataElement->appendChild(value);
            }
            
            void XmlWriterImpl::createInputs(const Operator*const currOp, DOMElement*const opElement)
            {
                for(std::vector<const Input*>::const_iterator iter_in = currOp->info().inputs().begin();
                    iter_in != currOp->info().inputs().end();
                    ++iter_in)
                {
                    //Get the source node
                    OutputConnector node = m_stream->connectionSource(currOp, (*iter_in)->id());
                    
                    //Create Input only for connected operators
                    if (node.valid())
                    {
                        //Create Input entry in being child of current operator op (one for each parameter possible)
                        DOMElement* inElement = m_doc->createElement(Str2Xml("Input"));
                        opElement->appendChild(inElement);
                        
                        //Create attribute id of current input in (one for each input possible)
                        DOMAttr* idAttr = m_doc->createAttribute(Str2Xml("id"));
                        idAttr->setValue(Str2Xml(boost::lexical_cast<std::string>((*iter_in)->id()).c_str()));
                        inElement->setAttributeNode(idAttr);
                        
                        DOMAttr* titleAttr = m_doc->createAttribute(Str2Xml("title"));
                        const Input& inputDesc = currOp->info().input((*iter_in)->id());
                        titleAttr->setValue(Str2Xml(inputDesc.title().c_str()));
                        inElement->setAttributeNode(titleAttr);
                        
                        //Get the id of the source operator
                        unsigned sourceOp = translateOperatorPointerToID(node.op());
                        
                        //Write the id of the source operator
                        DOMAttr* opAttr = m_doc->createAttribute(Str2Xml("operator"));
                        opAttr->setValue(Str2Xml(boost::lexical_cast<std::string>(sourceOp).c_str()));
                        inElement->setAttributeNode(opAttr);
                        
                        //Write the id of the output
                        DOMAttr* outAttr = m_doc->createAttribute(Str2Xml("output"));
                        outAttr->setValue(Str2Xml(boost::lexical_cast<std::string>(node.id()).c_str()));
                        inElement->setAttributeNode(outAttr);
                        
                        DOMAttr* outputTitleAttr = m_doc->createAttribute(Str2Xml("outputTitle"));
                        const Output& outputDesc = node.op()->info().output(node.id());
                        outputTitleAttr->setValue(Str2Xml(outputDesc.title().c_str()));
                        inElement->setAttributeNode(outputTitleAttr);
                    }
                }
            }
            
            void XmlWriterImpl::createOperators(const std::vector<const Operator*> & operators, xercesc::DOMElement* const parentElement)
            {
                //Add operator branches (tree structure: strom:operator)
                //Processed for each operator belonging to the strom object (multiple entries for strom possible)
                for(std::vector<const Operator*>::const_iterator iter_op = operators.begin();
                    iter_op != operators.end();
                    ++iter_op)
                {
                    const Operator* op = *iter_op;
                    
                    //Create current operator entry op being child of strom (one for each operator possible)
                    DOMElement* opElement = m_doc->createElement(Str2Xml("Operator"));
                    parentElement->appendChild(opElement);
                    
                    //Create attribute id of current operator op (one for each operator possible)
                    DOMAttr* idAttr = m_doc->createAttribute(Str2Xml("id"));
                    idAttr->setValue(Str2Xml(boost::lexical_cast<std::string>(translateOperatorPointerToID(op)).c_str()));
                    opElement->setAttributeNode(idAttr);
                    
                    //Create attribute package of current operator op (one for each operator possible)
                    DOMAttr* packAttr = m_doc->createAttribute(Str2Xml("package"));
                    packAttr->setValue(Str2Xml(op->info().package().c_str()));
                    opElement->setAttributeNode(packAttr);
                    
                    //Create attribute type of current operator op (one for each operator possible)
                    DOMAttr* typeAttr = m_doc->createAttribute(Str2Xml("type"));
                    typeAttr->setValue(Str2Xml(op->info().type().c_str()));
                    opElement->setAttributeNode(typeAttr);
                    
                    //Create attribute name of current operator op (one for each operator possible)
                    DOMAttr* nameAttr = m_doc->createAttribute(Str2Xml("name"));
                    nameAttr->setValue(Str2Xml(op->name().c_str()));
                    opElement->setAttributeNode(nameAttr);
                    
                    //Create attribute version of current operator op (one for each operator possible)
                    DOMAttr* verAttr = m_doc->createAttribute(Str2Xml("version"));
                    std::string str = boost::lexical_cast<std::string>(op->info().version());
                    verAttr->setValue(Str2Xml(str.c_str()));
                    opElement->setAttributeNode(verAttr);
                    
                    DOMAttr* initAttr = m_doc->createAttribute(Str2Xml("isInitialized"));
                    initAttr->setValue(Str2Xml(op->status() == Operator::NONE ? "false" : "true"));
                    opElement->setAttributeNode(initAttr);
                    
                    DOMAttr* xAttr = m_doc->createAttribute(Str2Xml("x"));
                    xAttr->setValue(Str2Xml(boost::lexical_cast<std::string>(op->position().x()).c_str()));
                    opElement->setAttributeNode(xAttr);
                    
                    DOMAttr* yAttr = m_doc->createAttribute(Str2Xml("y"));
                    yAttr->setValue(Str2Xml(boost::lexical_cast<std::string>(op->position().y()).c_str()));
                    opElement->setAttributeNode(yAttr);
                    
                    createParameters(op, opElement);
                    
                    if (m_stream != 0 && op->status() != Operator::NONE)
                    {
                        createInputs(op, opElement);
                    }
                }
            }
        
            void XmlWriterImpl::writeStream(FileOutput & output, const std::string & basename, const Stream& stream)
            {
                if (basename.empty())
                {
                    throw FileAccessFailed(basename, "Invalid file name.");
                }
                
                m_stream = &stream;
                m_output = &output;
                m_filename = basename;
                
                std::vector<const Operator*> operators(m_stream->operators().begin(), m_stream->operators().end());
                m_opList = operators;
                
                try
                {
                    createDoc();
                    createComm();                    
                    createStromx();
                                        
                    //Create Stream branch
                    m_strElement = m_doc->createElement(Str2Xml("Stream"));
                    m_stromxElement->appendChild(m_strElement);
                    
                    //Create attribute name of Stream
                    DOMAttr* nameAttr = m_doc->createAttribute(Str2Xml("name"));
                    nameAttr->setValue(Str2Xml(m_stream->name().c_str()));
                    m_strElement->setAttributeNode(nameAttr);

                    createOperators(m_opList,m_strElement);
                    //Create Threads of Stream (multiple entries for Stream possible)
                    createThreads(m_stream->threads());
                    
                    DOMLSOutput* output = m_impl->createLSOutput();
                    MemBufFormatTarget formatTarget;
                    output->setByteStream(&formatTarget);
                    DOMLSSerializer* serializer = m_impl->createLSSerializer();
                    serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
                    
                    serializer->write(m_doc, output);
                    
                    try
                    {
                        m_output->initialize(basename);
                        m_output->openFile(".xml");
                        m_output->file().write((const char*)(formatTarget.getRawBuffer()),
                                               formatTarget.getLen());
                    }
                    catch(Exception&)
                    {
                        output->release();
                        serializer->release();
                        throw; 
                    }
                    
                    output->release();
                    serializer->release();

                    // done with the document, must call release() to release the entire document resources
                    m_doc->release();
                }
                catch(DOMException&)
                {
                    throw InternalError("Error in Xerces-C.");
                }
                catch(XMLException&)
                {
                    throw InternalError("Error in Xerces-C.");
                }
                            
                try
                {
                    XMLPlatformUtils::Terminate();  // Terminate after release of memory
                }
                catch(XMLException&)
                {
                }
            }
            
            void XmlWriterImpl::writeParameters(FileOutput& output, 
                                                const std::string& basename, 
                                                const std::vector< const stromx::runtime::Operator* >& operators)
            {
                if (basename.empty())
                {
                    throw FileAccessFailed(basename, "Invalid base name.");
                }
                
                m_output = &output;
                m_filename = basename;
                m_opList = operators;
                
                try
                {
                    createDoc();
                    createComm();
                    createStromx();
                    
                    //Create Parameters branch
                    m_parsElement = m_doc->createElement(Str2Xml("Parameters"));
                    m_stromxElement->appendChild(m_parsElement);
                    
                    //Create Operator branches
                    createOperators(m_opList, m_parsElement);
                    
                    DOMLSOutput* output = m_impl->createLSOutput();
                    MemBufFormatTarget formatTarget;
                    output->setByteStream(&formatTarget);
                    DOMLSSerializer* serializer = m_impl->createLSSerializer();
                    serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
                    
                    serializer->write(m_doc, output);
                    
                    try
                    {
                        m_output->initialize(basename);
                        m_output->openFile(".xml");
                        m_output->file().write((const char*)(formatTarget.getRawBuffer()),
                                               formatTarget.getLen());
                    }
                    catch(Exception&)
                    {
                        output->release();
                        serializer->release();

                        // done with the document, must call release() to release the entire document resources
                        m_doc->release();
                    
                        throw; 
                    }
                    
                    output->release();
                    serializer->release();

                    // done with the document, must call release() to release the entire document resources
                    m_doc->release();
                }
                catch(DOMException&)
                {
                    throw InternalError("Error in Xerces-C.");
                }
                catch(XMLException&)
                {
                    throw InternalError("Error in Xerces-C.");
                }
                            
                try
                {
                    XMLPlatformUtils::Terminate();  // Terminate after release of memory
                }
                catch(XMLException&)
                {
                }
            }
        }
    } 
}
