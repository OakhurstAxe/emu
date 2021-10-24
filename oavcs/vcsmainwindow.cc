#include "headers/vcsmainwindow.h"

#include <QPainter>
#include <QDebug>


#include "headers/vcsconsole.h"

namespace oa
{
    namespace vcs
    {
        VcsMainWindow::VcsMainWindow(VcsParameters vcsParameters) :
        vcsConsoleType_(vcsParameters.GetConsoleType()),
        vcsPalette_(&vcsConsoleType_),
        m_gamepad_(*QGamepadManager::instance()->connectedGamepads().begin(), this)
        {
            this->resize(vcsConsoleType_.GetXResolution() * 4, vcsConsoleType_.GetYResolution() * 4);
            
            qImage_ = new QImage(vcsConsoleType_.GetXResolution(),
                                 vcsConsoleType_.GetYResolution(),
                                 QImage::Format_RGB32);

            vcsConsole_ = new VcsConsole(this, &vcsParameters);
            vcsConsole_->StartUp();
            vcsInput = vcsConsole_->GetVcsInput();
            
            connect(&m_gamepad_, SIGNAL(buttonAChanged(bool)), this, SLOT(LeftControllerA(bool)));
            connect(&m_gamepad_, SIGNAL(buttonXChanged(bool)), this, SLOT(LeftControllerReset(bool)));
            connect(&m_gamepad_, SIGNAL(buttonYChanged(bool)), this, SLOT(LeftControllerSelect(bool)));
            connect(&m_gamepad_, SIGNAL(axisLeftXChanged(double)), this, SLOT(LeftControllerLeftRight(double)));
            connect(&m_gamepad_, SIGNAL(axisLeftYChanged(double)), this, SLOT(LeftControllerUpDown(double)));
        }

        VcsMainWindow::~VcsMainWindow()
        {
            delete vcsConsole_;
            delete qImage_;
        }

        void VcsMainWindow::SetScreen(uint8_t *screen)
        {
            screen_ = screen;
        }

        void VcsMainWindow::LeftControllerA(bool value)
        {
            //qDebug() << "A" << value;
            if (value != 0)
            {
                vcsInput->SetInput(JOYSTICK1_FIRE);
            }
            else
            {
                vcsInput->SetInput(JOYSTICK1_FIRE_OFF);
            }
        }

        void VcsMainWindow::LeftControllerReset(bool value)
        {
            //qDebug() << "A" << value;
            if (value != 0)
            {
                vcsInput->SetInput(CONSOLE_RESET);
            }
            else
            {
                vcsInput->SetInput(CONSOLE_RESET_OFF);
            }
        }

        void VcsMainWindow::LeftControllerSelect(bool value)
        {
            //qDebug() << "A" << value;
            if (value != 0)
            {
                vcsInput->SetInput(CONSOLE_SELECT);
            }
            else
            {
                vcsInput->SetInput(CONSOLE_SELECT_OFF);
            }
        }

        void VcsMainWindow::LeftControllerUpDown(double value)
        {
            //qDebug() << "Button Up" << value;
            if (value < 0)
            {
                vcsInput->SetInput(JOYSTICK1_UP);
            }
            else if (value > 0)
            {
                vcsInput->SetInput(JOYSTICK1_DOWN);
            }
            else if (value == 0)
            {
                vcsInput->SetInput(JOYSTICK1_VERT_ZERO);
            }
        }
            
        void VcsMainWindow::LeftControllerLeftRight(double value)
        {
            //qDebug() << "Left X" << value;
            if (value < 0)
            {
                vcsInput->SetInput(JOYSTICK1_LEFT);
            }
            else if (value > 0)
            {
                vcsInput->SetInput(JOYSTICK1_RIGHT);
            }
            else if (value == 0)
            {
                vcsInput->SetInput(JOYSTICK1_HORZ_ZERO);
            }
        }

        void VcsMainWindow::paintEvent(QPaintEvent* event)
        {
            Q_UNUSED(event);
            
            QRect rect(0,0,vcsConsoleType_.GetXResolution()*4, vcsConsoleType_.GetYResolution()*4);
            QPainter painter(this);
            
            for (int y=0; y<vcsConsoleType_.GetYResolution(); y++)
            {
                QRgb *rowData = (QRgb*)qImage_->scanLine(y);
                for (int x=0; x<vcsConsoleType_.GetXResolution(); x++)
                {
                    uint8_t value = screen_[y*vcsConsoleType_.GetXResolution()+x];
                    QColor color = vcsPalette_.GetColor(value);
                    rowData[x] = color.rgb();
                }
            }
            painter.drawImage(rect, *qImage_);
        }

        void VcsMainWindow::ReadGamepad()
        {
        }

    }
}

