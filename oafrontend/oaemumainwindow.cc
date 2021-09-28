
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
            
            ui.gbViewVcs->setVisible(true);
            ui.gbEditVcs->setVisible(false);
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
            isEdit = !isEdit;
            ui.gbViewVcs->setVisible(!isEdit);
            ui.gbEditVcs->setVisible(isEdit);
        }
        
        void MainWindow::SaveClicked(bool checked)
        {
            systemData.SaveFile("/home/dmax/projects/oaemu/SystemData.txt");
        }
        
        void MainWindow::AddClicked(bool checked)
        {
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
            
            unz_file_info fileInfo;
            char* zipFile = romFile.toUtf8().data();
            char* fileName = QString("ROMS/" + gameRom).toUtf8().data();
            unzFile zip = unzOpen(zipFile);
            int result = unzLocateFile(zip, fileName, 0);
            result = unzGetCurrentFileInfo(zip, &fileInfo, fileName, strlen(fileName) + 1, NULL, 0, NULL, 0);
            unsigned char buffer[fileInfo.uncompressed_size];
            result = unzOpenCurrentFile(zip);
            result = unzReadCurrentFile(zip, buffer, fileInfo.uncompressed_size);
            result = unzCloseCurrentFile(zip);
            result = unzClose(zip);
        }

    }
}

