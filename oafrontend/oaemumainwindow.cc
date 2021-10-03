
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
            
            EditClicked(false);
            ui_.lstGameView->setModel(vcsGameList_);
            connect(ui_.lstGameView, SIGNAL(clicked(QModelIndex)), this, SLOT(VcsGameSelected(QModelIndex)));
            connect(ui_.btnEdit, SIGNAL(clicked(bool)), this, SLOT(EditClicked(bool)));
            connect(ui_.btnSave, SIGNAL(clicked(bool)), this, SLOT(SaveClicked(bool)));
            connect(ui_.btnAdd, SIGNAL(clicked(bool)), this, SLOT(AddClicked(bool)));
            connect(ui_.btnRun, SIGNAL(clicked(bool)), this, SLOT(RunClicked(bool)));
            connect(ui_.leName, SIGNAL(textEdited(QString)), this, SLOT(VcsGameNameEdit(QString)));
            connect(ui_.leController, SIGNAL(textEdited(QString)), this, SLOT(VcsGameControllerEdit(QString)));
            connect(ui_.leCompany, SIGNAL(textEdited(QString)), this, SLOT(VcsGameCompanyEdit(QString)));
            connect(ui_.leScreen, SIGNAL(textEdited(QString)), this, SLOT(VcsGameScreenEdit(QString)));
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
                    ui_.lblNameValue->setText(game->GetName());
                    ui_.lblControllerValue->setText(game->GetController());
                    ui_.lblCompanyValue->setText(game->GetCompany());
                    ui_.lblScreenValue->setText(game->GetScreenType());
                    ui_.lblRomValue->setText(game->GetRomFile());
                    
                    ui_.leName->setText(game->GetName());
                    ui_.leController->setText(game->GetController());
                    ui_.leCompany->setText(game->GetCompany());
                    ui_.leScreen->setText(game->GetScreenType());
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
            ui_.leController->setVisible(isEdit_);
            ui_.leCompany->setVisible(isEdit_);
            ui_.leScreen->setVisible(isEdit_);
            ui_.leRomFile->setVisible(isEdit_);
            ui_.lblNameValue->setVisible(!isEdit_);
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
            QString gameName = vcsGameList_->stringList().at(ui_.lstGameView->currentIndex().row());
            for (VcsGame* game : systemData_.GetVcsSystem()->GetVcsGames())
            {
                if (game->GetName() == gameName)
                {
                    game->SetName(text);
                    ui_.lblNameValue->setText(game->GetName());
                    break;
                }
            }
            QStringList list;
            for (VcsGame* game : systemData_.GetVcsSystem()->GetVcsGames())
            {
                list << game->GetName();
            }
            vcsGameList_->setStringList(list);
        }
        
        void MainWindow::VcsGameControllerEdit(const QString &text)
        {
            QString gameName = vcsGameList_->stringList().at(ui_.lstGameView->currentIndex().row());
            for (VcsGame* game : systemData_.GetVcsSystem()->GetVcsGames())
            {
                if (game->GetName() == gameName)
                {
                    game->SetController(text);
                    ui_.lblController->setText(game->GetController());
                    break;
                }
            }
        }
        
        void MainWindow::VcsGameCompanyEdit(const QString &text)
        {
            QString gameName = vcsGameList_->stringList().at(ui_.lstGameView->currentIndex().row());
            for (VcsGame* game : systemData_.GetVcsSystem()->GetVcsGames())
            {
                if (game->GetName() == gameName)
                {
                    game->SetCompany(text);
                    ui_.lblCompany->setText(game->GetCompany());
                    break;
                }
            }
        }
        
        void MainWindow::VcsGameScreenEdit(const QString &text)
        {
            QString gameName = vcsGameList_->stringList().at(ui_.lstGameView->currentIndex().row());
            for (VcsGame* game : systemData_.GetVcsSystem()->GetVcsGames())
            {
                if (game->GetName() == gameName)
                {
                    game->SetScreenType(text);
                    ui_.lblScreen->setText(game->GetScreenType());
                    break;
                }
            }
        }
        
        void MainWindow::VcsGameRomFileEdit(const QString &text)
        {
            QString gameName = vcsGameList_->stringList().at(ui_.lstGameView->currentIndex().row());
            for (VcsGame* game : systemData_.GetVcsSystem()->GetVcsGames())
            {
                if (game->GetName() == gameName)
                {
                    game->SetRomFile(text);
                    ui_.lblRomValue->setText(game->GetRomFile());
                    break;
                }
            }
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
            
            char* zipFile = romFile.toUtf8().data();
            char* fileName = QString("ROMS/" + gameRom).toUtf8().data();
            unzFile zip = unzOpen(zipFile);
            result = unzLocateFile(zip, fileName, 0);
            result = unzGetCurrentFileInfo(zip, &fileInfo, fileName, strlen(fileName) + 1, NULL, 0, NULL, 0);
            
            int bufferSize = fileInfo.uncompressed_size;            
            uint8_t* buffer = (uint8_t*)malloc(bufferSize);
            result = unzOpenCurrentFile(zip);
            result = unzReadCurrentFile(zip, buffer, bufferSize);
            result = unzCloseCurrentFile(zip);

            result = unzClose(zip);

            vcs::ConsoleType consoleType;
            if (ui_.lblScreenValue->text() == "NTSC")
            {
                consoleType = vcs::ConsoleType::NTSC;
            }
            else if (ui_.lblScreenValue->text() == "PAL")
            {
                consoleType = vcs::ConsoleType::PAL;
            }
            else
            {
                consoleType = vcs::ConsoleType::SECAM;
            }
            vcsParameters_.SetConsoleType(consoleType);
            vcsParameters_.SetCartData(buffer, bufferSize);
            free(buffer);

            if (vcs_ != NULL)
            {
                delete vcs_;
            }
            vcs_ = new vcs::VcsMainWindow(vcsParameters_);
            vcs_->show();            
        }

    }
}

