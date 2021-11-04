
#include "headers/oaemumainwindow.h"

#include <minizip/unzip.h>
#include <iostream>
#include <fstream>

namespace oa
{
    namespace emu
    {
        
        MainWindow::MainWindow(QWidget *parent)
            : QWidget(parent)
        {
            ui_.setupUi(this);
            systemData_.LoadFile("/home/dmax/projects/oaemu/SystemData.txt");
            
            vcsGameList_ = new QStringListModel(this);
            
            QStringList list;
            for (VcsGame* game : systemData_.GetVcsSystem()->GetVcsGames())
            {
                list << game->GetName();
            }
            vcsGameList_->setStringList(list);
            
            for (QString mapper : systemData_.GetVcsSystem()->GetMappers())
            {
                ui_.cmbMapper->addItem(mapper);
            }

            for (QString controller : systemData_.GetVcsSystem()->GetControllerTypes())
            {
                ui_.cmbController->addItem(controller);
            }

            for (QString screen : systemData_.GetVcsSystem()->GetCompanies())
            {
                ui_.cmbCompany->addItem(screen);
            }

            for (QString screen : systemData_.GetScreenTypes()->GetScreenTypes())
            {
                ui_.cmbScreen->addItem(screen);
            }
            
            EditClicked(false);
            ui_.lstGameView->setModel(vcsGameList_);
            connect(ui_.lstGameView, SIGNAL(clicked(QModelIndex)), this, SLOT(VcsGameSelected(QModelIndex)));
            connect(ui_.btnEdit, SIGNAL(clicked(bool)), this, SLOT(EditClicked(bool)));
            connect(ui_.btnSave, SIGNAL(clicked(bool)), this, SLOT(SaveClicked(bool)));
            connect(ui_.btnAdd, SIGNAL(clicked(bool)), this, SLOT(AddClicked(bool)));
            connect(ui_.btnRun, SIGNAL(clicked(bool)), this, SLOT(RunClicked(bool)));
            connect(ui_.leName, SIGNAL(textEdited(QString)), this, SLOT(VcsGameNameEdit(QString)));
            connect(ui_.cmbMapper, SIGNAL(currentTextChanged(QString)), this, SLOT(VcsGameMapperEdit(QString)));
            connect(ui_.cmbController, SIGNAL(currentTextChanged(QString)), this, SLOT(VcsGameControllerEdit(QString)));
            connect(ui_.cmbCompany, SIGNAL(currentTextChanged(QString)), this, SLOT(VcsGameCompanyEdit(QString)));
            connect(ui_.cmbScreen, SIGNAL(currentTextChanged(QString)), this, SLOT(VcsGameScreenEdit(QString)));
            connect(ui_.leRomFile, SIGNAL(textEdited(QString)), this, SLOT(VcsGameRomFileEdit(QString)));
        }
        
        MainWindow::~MainWindow()
        {
            delete vcsGameList_;
            if (vcs_ != NULL)
            {
                delete vcs_;
            }
        }
        
        void MainWindow::VcsGameSelected(const QModelIndex &index)
        {
            QString gameName = vcsGameList_->stringList().at(index.row());
            for (VcsGame* game : systemData_.GetVcsSystem()->GetVcsGames())
            {
                if (game->GetName() == gameName)
                {
                    vcsGameSelected_= game;
                    
                    ui_.lblNameValue->setText(game->GetName());
                    ui_.lblMapperValue->setText(game->GetMapper());
                    ui_.lblControllerValue->setText(game->GetController());
                    ui_.lblCompanyValue->setText(game->GetCompany());
                    ui_.lblScreenValue->setText(game->GetScreenType());
                    ui_.lblRomValue->setText(game->GetRomFile());
                    
                    ui_.leName->setText(game->GetName());
                    ui_.cmbMapper->setCurrentText(game->GetMapper());
                    ui_.cmbController->setCurrentText(game->GetController());
                    ui_.cmbCompany->setCurrentText(game->GetCompany());
                    ui_.cmbScreen->setCurrentText(game->GetScreenType());
                    ui_.leRomFile->setText(game->GetRomFile());
                    break;
                }
            }            
        }
        
        void MainWindow::EditClicked(bool checked)
        {
            Q_UNUSED(checked);
            isEdit_ = !isEdit_;
            ui_.leName->setVisible(isEdit_);
            ui_.cmbMapper->setVisible(isEdit_);
            ui_.cmbController->setVisible(isEdit_);
            ui_.cmbCompany->setVisible(isEdit_);
            ui_.cmbScreen->setVisible(isEdit_);
            ui_.leRomFile->setVisible(isEdit_);
            ui_.lblNameValue->setVisible(!isEdit_);
            ui_.lblMapperValue->setVisible(!isEdit_);
            ui_.lblControllerValue->setVisible(!isEdit_);
            ui_.lblCompanyValue->setVisible(!isEdit_);
            ui_.lblScreenValue->setVisible(!isEdit_);
            ui_.lblRomValue->setVisible(!isEdit_);
        }
        
        void MainWindow::SaveClicked(bool checked)
        {
            Q_UNUSED(checked);
            systemData_.SaveFile("/home/dmax/projects/oaemu/SystemData.txt");
        }
        
        void MainWindow::AddClicked(bool checked)
        {
            Q_UNUSED(checked);
            VcsGame* game = new VcsGame();
            game->SetName("1234");
            systemData_.GetVcsSystem()->AppendVcsGame(game);
            
            QStringList list;
            for (VcsGame* game : systemData_.GetVcsSystem()->GetVcsGames())
            {
                list << game->GetName();
            }
            vcsGameList_->setStringList(list);
        }

        void MainWindow::VcsGameNameEdit(const QString &text)
        {
            vcsGameSelected_->SetName(text);
            ui_.lblNameValue->setText(vcsGameSelected_->GetName());

            QStringList list;
            for (VcsGame* game : systemData_.GetVcsSystem()->GetVcsGames())
            {
                list << game->GetName();
            }
            vcsGameList_->setStringList(list);
        }
        
        void MainWindow::VcsGameMapperEdit(const QString &text)
        {
            vcsGameSelected_->SetMapper(text);
            ui_.lblMapperValue->setText(vcsGameSelected_->GetMapper());
        }
        
        void MainWindow::VcsGameControllerEdit(const QString &text)
        {
            vcsGameSelected_->SetController(text);
            ui_.lblControllerValue->setText(vcsGameSelected_->GetController());
        }

        void MainWindow::VcsGameCompanyEdit(const QString &text)
        {
            vcsGameSelected_->SetCompany(text);
            ui_.lblCompanyValue->setText(vcsGameSelected_->GetCompany());
        }
        
        void MainWindow::VcsGameScreenEdit(const QString &text)
        {
            vcsGameSelected_->SetScreenType(text);
            ui_.lblScreenValue->setText(vcsGameSelected_->GetScreenType());
        }
        
        void MainWindow::VcsGameRomFileEdit(const QString &text)
        {
            vcsGameSelected_->SetRomFile(text);
            ui_.lblRomValue->setText(vcsGameSelected_->GetRomFile());
        }

        void MainWindow::RunClicked(bool checked)
        {
            Q_UNUSED(checked);
            QString romFile = systemData_.GetVcsSystem()->GetRomZipFile();
            QString gameName = vcsGameList_->stringList().at(ui_.lstGameView->currentIndex().row());
            QString gameRom = "";
            for (VcsGame* game : systemData_.GetVcsSystem()->GetVcsGames())
            {
                if (game->GetName() == gameName)
                {
                    gameRom = game->GetRomFile();
                    break;
                }
            }
            
            int result;
            unz_file_info fileInfo;
            memset(&fileInfo, 0, sizeof(unz_file_info));
            
            unzFile zip = unzOpen(romFile.toUtf8().data());
            char* fileName = QString("ROMS/" + gameRom).toUtf8().data();
            result = unzLocateFile(zip, fileName, 0);
            if (result != 0)
            {
                throw std::runtime_error(QString("Error calling unzLocateFile [%1]").arg(result).toLocal8Bit().data());
            }
            result = unzGetCurrentFileInfo(zip, &fileInfo, fileName, strlen(fileName) + 1, NULL, 0, NULL, 0);
            if (result != 0)
            {
                throw std::runtime_error(QString("Error calling unzGetCurrentFileInfo [%1]").arg(result).toLocal8Bit().data());
            }
            
            int bufferSize = fileInfo.uncompressed_size;            
            uint8_t* buffer = (uint8_t*)malloc(bufferSize);
            result = unzOpenCurrentFile(zip);
            if (result != 0)
            {
                throw std::runtime_error(QString("Error calling unzOpenCurrentFile [%1]").arg(result).toLocal8Bit().data());
            }
            result = unzReadCurrentFile(zip, buffer, bufferSize);
            if (result != bufferSize)
            {
                throw std::runtime_error(QString("Error calling unzReadCurrentFile [%1]").arg(result).toLocal8Bit().data());
            }
            result = unzCloseCurrentFile(zip);
            if (result != 0)
            {
                throw std::runtime_error(QString("Error calling unzCloseCurrentFile [%1]").arg(result).toLocal8Bit().data());
            }

            result = unzClose(zip);
            if (result != 0)
            {
                throw std::runtime_error(QString("Error calling unzClose [%1]").arg(result).toLocal8Bit().data());
            }

            vcs::ConsoleType consoleType;
            if (ui_.lblScreenValue->text() == "NTSC")
            {
                consoleType = vcs::NTSC;
            }
            else if (ui_.lblScreenValue->text() == "PAL")
            {
                consoleType = vcs::PAL;
            }
            else
            {
                consoleType = vcs::SECAM;
            }
            vcsParameters_.SetConsoleType(consoleType);
            vcsParameters_.SetCartData(buffer, bufferSize);
            vcsParameters_.SetMapper(ui_.lblMapperValue->text());
            
            free(buffer);

            if (vcs_ != NULL)
            {
                delete vcs_;
            }
            vcs_ = new vcs::VcsConsole(&vcsParameters_);
            vcs_->show();            
        }

    }
}

