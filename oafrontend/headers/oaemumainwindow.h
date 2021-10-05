
#include <QMainWindow>
#include <QStringListModel>

#include "ui_oaemumainwindow.h"
#include "headers/oaemusystemdata.h"
#include "oavcs/headers/vcsmainwindow.h"
#include "oavcs/headers/vcsconsoletype.h"
#include "oavcs/headers/vcsparameters.h"

namespace oa
{
    namespace emu
    {
        
        class MainWindow : public QWidget
        {
            Q_OBJECT
            
        public:
            MainWindow(QWidget *parent = 0);
            virtual ~MainWindow();
        private slots:
            void VcsGameNameEdit(const QString &text);
            void VcsGameMapperEdit(const QString &text);
            void VcsGameControllerEdit(const QString &text);
            void VcsGameCompanyEdit(const QString &text);
            void VcsGameScreenEdit(const QString &text);
            void VcsGameRomFileEdit(const QString &text);
            void VcsGameSelected(const QModelIndex &index);
            void EditClicked(bool checked);
            void SaveClicked(bool checked);
            void AddClicked(bool checked);
            void RunClicked(bool checked);
        private:
            vcs::VcsMainWindow* vcs_ = NULL;
            vcs::VcsParameters vcsParameters_;
            bool isEdit_ = true;
            Ui::MainWindow ui_;
            SystemData systemData_;
            QStringListModel *vcsGameList_;
            VcsGame* vcsGameSelected_;
        };
        
    }
}
