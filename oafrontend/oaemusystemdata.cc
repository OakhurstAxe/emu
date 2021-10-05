
#include "headers/oaemusystemdata.h"

#include <iostream>
#include <QtXml>
#include <QFile>

namespace oa
{
    namespace emu
    {

        SystemData::SystemData()
        {
            screenTypes_ = new ScreenTypes();
            vcsSystem_ = new VcsSystem();
        }
        
        SystemData::~SystemData()
        {
            delete screenTypes_;
            delete vcsSystem_;
        }

        void SystemData::LoadFile(QString fileName)
        {
            QDomDocument xmlBOM;
            
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly))
            {
                std::cerr << "Error while loading file" << std::endl;
                return;
            }
            
            QXmlStreamReader xml(&file);
            
            if (xml.readNextStartElement())
            {
                if (xml.name() == "OaEmu")
                {
                    while (xml.readNextStartElement())
                    {
                        
                        if (xml.name() == "ScreenTypes")
                        {
                            while (xml.readNextStartElement())
                            {
                                if (xml.name() == "Name")
                                {
                                    screenTypes_->AddScreenType(xml.readElementText());
                                }
                            }
                        }
                        
                        if (xml.name() == "VCS")
                        {
                            while (xml.readNextStartElement())
                            {
                                if (xml.name() == "RomZipFile")
                                {
                                    vcsSystem_->SetRomZipFile(xml.readElementText());
                                }
                                if (xml.name() == "MapperTypes")
                                {
                                    while (xml.readNextStartElement())
                                    {
                                        if (xml.name() == "Name")
                                        {
                                            vcsSystem_->AppendMapper(xml.readElementText());
                                        }
                                    }
                                }
                                if (xml.name() == "ControllerTypes")
                                {
                                    while (xml.readNextStartElement())
                                    {
                                        if (xml.name() == "Name")
                                        {
                                            vcsSystem_->AppendControllerType(xml.readElementText());
                                        }
                                    }
                                }
                                if (xml.name() == "Companies")
                                {
                                    while (xml.readNextStartElement())
                                    {
                                        if (xml.name() == "Name")
                                        {
                                            vcsSystem_->AppendCompany(xml.readElementText());
                                        }
                                    }
                                }
                                if (xml.name() == "RomFiles")
                                {
                                    while (xml.readNextStartElement())
                                    {
                                        if (xml.name() == "Game")
                                        {
                                            VcsGame* game = new VcsGame();
                                            while (xml.readNextStartElement())
                                            {
                                                if (xml.name() == "Name")
                                                {
                                                    game->SetName(xml.readElementText());
                                                }
                                                if (xml.name() == "Mapper")
                                                {
                                                    game->SetMapper(xml.readElementText());
                                                }
                                                if (xml.name() == "Controller")
                                                {
                                                    game->SetController(xml.readElementText());
                                                }
                                                if (xml.name() == "Company")
                                                {
                                                    game->SetCompany(xml.readElementText());
                                                }
                                                if (xml.name() == "ScreenType")
                                                {
                                                    game->SetScreenType(xml.readElementText());
                                                }
                                                if (xml.name() == "RomFile")
                                                {
                                                    game->SetRomFile(xml.readElementText());
                                                }
                                            }
                                            vcsSystem_->AppendVcsGame(game);
                                        }
                                    }
                                }
                            }
                            
                        }
                        
                    }
                }                
            }
            file.close();
        }
        
        void SystemData::SaveFile(QString fileName)
        {
            QXmlStreamWriter xml;
            
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly))
            {
                std::cerr << "Error while loading file" << std::endl;
                return;
            }
            
            xml.setDevice(&file);
            
            xml.setAutoFormatting(true);
            xml.writeStartDocument();
            xml.writeStartElement("OaEmu");

            // Screen Types
            xml.writeStartElement("ScreenTypes");
            foreach (QString screenType, screenTypes_->GetScreenTypes())
            {
                xml.writeTextElement("Name", screenType);
            }        
            xml.writeEndElement();
            
            // VCS
            xml.writeStartElement("VCS");
            
            // Rom zip file
            xml.writeTextElement("RomZipFile", vcsSystem_->GetRomZipFile());
            
            // Mapper types
            xml.writeStartElement("MapperTypes");
            foreach (QString mapperType, vcsSystem_->GetMappers())
            {
                xml.writeTextElement("Name", mapperType);
            }        
            xml.writeEndElement();

            // Controller types
            xml.writeStartElement("ControllerTypes");
            foreach (QString controllerType, vcsSystem_->GetControllerTypes())
            {
                xml.writeTextElement("Name", controllerType);
            }        
            xml.writeEndElement();

            // Companies
            xml.writeStartElement("Companies");
            foreach (QString company, vcsSystem_->GetCompanies())
            {
                xml.writeTextElement("Name", company);
            }        
            xml.writeEndElement();

            // Rom Files
            xml.writeStartElement("RomFiles");
            foreach (VcsGame* vcsGame, vcsSystem_->GetVcsGames())
            {
                xml.writeStartElement("Game");
                xml.writeTextElement("Name", vcsGame->GetName());
                xml.writeTextElement("Mapper", vcsGame->GetMapper());
                xml.writeTextElement("Controller", vcsGame->GetController());
                xml.writeTextElement("Company", vcsGame->GetCompany());
                xml.writeTextElement("ScreenType", vcsGame->GetScreenType());
                xml.writeTextElement("RomFile", vcsGame->GetRomFile());
                xml.writeEndElement();
            }        
            xml.writeEndElement();

            xml.writeEndElement();

            xml.writeEndElement();
            xml.writeEndDocument();

            file.close();
        }

        ScreenTypes* SystemData::GetScreenTypes()
        {
            return screenTypes_;
        }
        
        VcsSystem* SystemData::GetVcsSystem()
        {
            return vcsSystem_;
        }
        
    }
}
