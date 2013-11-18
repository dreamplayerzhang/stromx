/* 
 *  Copyright 2013 Matthias Fuchs
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

#include "Client.h"

#include "stromx/runtime/Data.h"
#include "stromx/runtime/Factory.h"
#include "stromx/runtime/InputProvider.h"
#include "stromx/runtime/Version.h"

using namespace boost::asio;

namespace
{
    class StreamInput : public stromx::runtime::InputProvider
    {            
    public:
        StreamInput(boost::asio::streambuf & textBuffer,
                    boost::asio::streambuf & fileBuffer)
          : m_textStream(&textBuffer),
            m_fileStream(&fileBuffer),
            m_hasFile(fileBuffer.size() != 0)
        {}
        
        std::istream & text()
        {
            return m_textStream;
        }
        
        bool hasFile() const
        {
            return m_hasFile;
        }
        
        std::istream & openFile(const OpenMode /*mode*/)
        {
            return m_fileStream;
        }
        
        std::istream & file()
        {
            return m_fileStream;
        }
        
    private:
        std::istream m_textStream;
        std::istream m_fileStream;
        bool m_hasFile;
    };
}

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            const std::string Client::LINE_DELIMITER("\r\n");
            
            Client::Client(const std::string& url, const std::string& port)
              : m_socket(m_ioService)
            {
                try
                {
                    ip::tcp::resolver resolver(m_ioService);
                    ip::tcp::resolver::query query(url, port);
                    ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

                    boost::asio::connect(m_socket, endpoint_iterator);
                    
                    m_thread = boost::thread(boost::bind(&Client::run, this));
                }
                catch (std::exception& e)
                {
                    throw NoConnection();
                }
            }
            
            const DataContainer Client::receive(const Factory& factory)
            {
                try
                {
                    boost::asio::streambuf buf;
                    for (int i = 0; i < 5; ++i)
                        boost::asio::read_until(m_socket, buf, LINE_DELIMITER);
                    
                    stromx::runtime::Version serverVersion;
                    std::string package;
                    std::string type;
                    stromx::runtime::Version dataVersion;
                    unsigned int textBufferSize = 0;
                    unsigned int fileBufferSize = 0;
                    std::istream stream(&buf);
                    
                    stream >> serverVersion;
                    stream >> package;
                    stream >> type;
                    stream >> dataVersion;
                    stream >> textBufferSize;
                    stream >> fileBufferSize;
                    
                    boost::asio::streambuf textBuffer(textBufferSize);
                    boost::asio::streambuf fileBuffer(fileBufferSize);
                    
                    size_t size = 0;
                    if (textBufferSize)
                    {
                        try
                        {
                            size = boost::asio::read(m_socket, textBuffer);
                        }
                        catch(boost::system::system_error &)
                        {
                        }
                    }
                    
                    if (fileBufferSize)
                    {
                        try
                        {
                            boost::asio::read(m_socket, fileBuffer);
                        }
                        catch(boost::system::system_error &)
                        {
                        }
                    }
                    
                    StreamInput input(textBuffer, fileBuffer);
                    
                    stromx::runtime::Data* outData = factory.newData(package, type);
                    stromx::runtime::DataContainer outContainer(outData);
                    outData->deserialize(input, dataVersion);
                    
                    return outContainer;
                }
                catch (std::exception& e)
                {
                    std::cerr << e.what() << std::endl;
                    throw NoConnection();
                }
            }
            
            void Client::run()
            {

            }

            void Client::stop()
            {
                m_thread.interrupt();
            }

            void Client::join()
            {
                m_thread.join();
            }
        }
    }
}
