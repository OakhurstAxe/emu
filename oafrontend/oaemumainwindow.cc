
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
            ui.setupUi(this);
            systemData.LoadFile("/home/dmax/projects/oaemu/SystemData.txt");
            
            vcsGameList = new QStringListModel(this);
            
            QStringList list;
            for (VcsGame* game : systemData.GetVcsSystem()->GetVcsGames())
            {
                list << game->GetName();
            }
            vcsGameList->setStringList(list);
            
            EditClicked(false);
            ui.lstGameView->setModel(vcsGameList);
            connect(ui.lstGameView, SIGNAL(clicked(QModelIndex)), this, SLOT(VcsGameSelected(QModelIndex)));
            connect(ui.btnEdit, SIGNAL(clicked(bool)), this, SLOT(EditClicked(bool)));
            connect(ui.btnSave, SIGNAL(clicked(bool)), this, SLOT(SaveClicked(bool)));
            connect(ui.btnAdd, SIGNAL(clicked(bool)), this, SLOT(AddClicked(bool)));
            connect(ui.btnRun, SIGNAL(clicked(bool)), this, SLOT(RunClicked(bool)));
            connect(ui.leName, SIGNAL(textEdited(QString)), this, SLOT(VcsGameNameEdit(QString)));
            connect(ui.leController, SIGNAL(textEdited(QString)), this, SLOT(VcsGameControllerEdit(QString)));
            connect(ui.leCompany, SIGNAL(textEdited(QString)), this, SLOT(VcsGameCompanyEdit(QString)));
            connect(ui.leScreen, SIGNAL(textEdited(QString)), this, SLOT(VcsGameScreenEdit(QString)));
            connect(ui.leRomFile, SIGNAL(textEdited(QString)), this, SLOT(VcsGameRomFileEdit(QString)));
            
            buffer = (unsigned char*)malloc(1);
        }
        
        MainWindow::~MainWindow()
        {
            if (vcs != NULL)
            {
                delete vcs;
            }
            free(buffer);
        }
        
        void MainWindow::VcsGameSelected(const QModelIndex &index)
        {
            QString gameName = vcsGameList->stringList().at(index.row());
            for (VcsGame* game : systemData.GetVcsSystem()->GetVcsGames())
            {
                if (game->GetName() == gameName)
                {
                    ui.lblNameValue->setText(game->GetName());
                    ui.lblControllerValue->setText(game->GetController());
                    ui.lblCompanyValue->setText(game->GetCompany());
                    ui.lblScreenValue->setText(game->GetScreenType());
                    ui.lblRomValue->setText(game->GetRomFile());
                    
                    ui.leName->setText(game->GetName());
                    ui.leController->setText(game->GetController());
                    ui.leCompany->setText(game->GetCompany());
                    ui.leScreen->setText(game->GetScreenType());
                    ui.leRomFile->setText(game->GetRomFile());
                    break;
                }
            }            
        }
        
        void MainWindow::EditClicked(bool checked)
        {
            Q_UNUSED(checked);
            isEdit = !isEdit;
            ui.leName->setVisible(isEdit);
            ui.leController->setVisible(isEdit);
            ui.leCompany->setVisible(isEdit);
            ui.leScreen->setVisible(isEdit);
            ui.leRomFile->setVisible(isEdit);
            ui.lblNameValue->setVisible(!isEdit);
            ui.lblControllerValue->setVisible(!isEdit);
            ui.lblCompanyValue->setVisible(!isEdit);
            ui.lblScreenValue->setVisible(!isEdit);
            ui.lblRomValue->setVisible(!isEdit);
        }
        
        void MainWindow::SaveClicked(bool checked)
        {
            Q_UNUSED(checked);
            systemData.SaveFile("/home/dmax/projects/oaemu/SystemData.txt");
        }
        
        void MainWindow::AddClicked(bool checked)
        {
            Q_UNUSED(checked);
            VcsGame* game = new VcsGame();
            game->SetName("1234");
            systemData.GetVcsSystem()->AppendVcsGame(game);
            
            vcsGameList = new QStringListModel(this);
            
            QStringList list;
            for (VcsGame* game : systemData.GetVcsSystem()->GetVcsGames())
            {
                list << game->GetName();
            }
            vcsGameList->setStringList(list);
        }
            
        void MainWindow::VcsGameNameEdit(const QString &text)
        {
            QString gameName = vcsGameList->stringList().at(ui.lstGameView->currentIndex().row());
            for (VcsGame* game : systemData.GetVcsSystem()->GetVcsGames())
            {
                if (game->GetName() == gameName)
                {
                    game->SetName(text);
                    ui.lblNameValue->setText(game->GetName());
                    break;
                }
            }
        }
        
        void MainWindow::VcsGameControllerEdit(const QString &text)
        {
            QString gameName = vcsGameList->stringList().at(ui.lstGameView->currentIndex().row());
            for (VcsGame* game : systemData.GetVcsSystem()->GetVcsGames())
            {
                if (game->GetName() == gameName)
                {
                    game->SetController(text);
                    ui.lblController->setText(game->GetController());
                    break;
                }
            }
        }
        
        void MainWindow::VcsGameCompanyEdit(const QString &text)
        {
            QString gameName = vcsGameList->stringList().at(ui.lstGameView->currentIndex().row());
            for (VcsGame* game : systemData.GetVcsSystem()->GetVcsGames())
            {
                if (game->GetName() == gameName)
                {
                    game->SetCompany(text);
                    ui.lblCompany->setText(game->GetCompany());
                    break;
                }
            }
        }
        
        void MainWindow::VcsGameScreenEdit(const QString &text)
        {
            QString gameName = vcsGameList->stringList().at(ui.lstGameView->currentIndex().row());
            for (VcsGame* game : systemData.GetVcsSystem()->GetVcsGames())
            {
                if (game->GetName() == gameName)
                {
                    game->SetScreenType(text);
                    ui.lblScreen->setText(game->GetScreenType());
                    break;
                }
            }
        }
        
        void MainWindow::VcsGameRomFileEdit(const QString &text)
        {
            QString gameName = vcsGameList->stringList().at(ui.lstGameView->currentIndex().row());
            for (VcsGame* game : systemData.GetVcsSystem()->GetVcsGames())
            {
                if (game->GetName() == gameName)
                {
                    game->SetRomFile(text);
                    ui.lblRomValue->setText(game->GetRomFile());
                    break;
                }
            }
        }

        void MainWindow::RunClicked(bool checked)
        {
            Q_UNUSED(checked);
            QString romFile = systemData.GetVcsSystem()->GetRomZipFile();
            QString gameName = vcsGameList->stringList().at(ui.lstGameView->currentIndex().row());
            QString gameRom = "";
            for (VcsGame* game : systemData.GetVcsSystem()->GetVcsGames())
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
            
            free(buffer);
            int bufferSize = fileInfo.uncompressed_size;            
            buffer = (unsigned char*)malloc(bufferSize);
            result = unzOpenCurrentFile(zip);
            result = unzReadCurrentFile(zip, buffer, bufferSize);
            result = unzCloseCurrentFile(zip);

            result = unzClose(zip);

            vcs::ConsoleType consoleType;
            if (ui.lblScreenValue->text() == "NTSC")
            {
                consoleType = vcs::ConsoleType::NTSC;
            }
            else if (ui.lblScreenValue->text() == "PAL")
            {
                consoleType = vcs::ConsoleType::PAL;
            }
            else
            {
                consoleType = vcs::ConsoleType::SECAM;
            }
            vcs = new vcs::VcsMainWindow(consoleType, (uint8_t*)buffer, bufferSize);
            vcs->show();            
        }

    }
}

