#include "headers/vcsmainwindow.h"

#include <QPainter>
#include <QDebug>
#include <QtGamepad/QGamepad>

#include "headers/vcsconsole.h"

namespace oa
{
    namespace vcs
    {
        VcsMainWindow::VcsMainWindow()
        {
            qImage_ = new QImage(160, 192, QImage::Format_RGB32);
            
            vcsConsole_ = new VcsConsole(this);
            vcsConsole_->StartUp();
            
            auto gamepads = QGamepadManager::instance()->connectedGamepads();
            if (gamepads.isEmpty()) {
                qDebug() << "Did not find any connected gamepads";
                return;
            }
            
            m_gamepad_ = new QGamepad(*gamepads.begin(), this);
            
            connect(m_gamepad_, SIGNAL(buttonAChanged(bool)), this, SLOT(LeftControllerA(bool)));
            //connect(m_gamepad_, SIGNAL(buttonBChanged(bool)), this, SLOT(leftControllerB(bool)));
            //connect(m_gamepad_, SIGNAL(buttonXChanged(bool)), this, SLOT(leftControllerSelect(bool)));
            //connect(m_gamepad_, SIGNAL(buttonYChanged(bool)), this, SLOT(leftControllerStart(bool)));
            connect(m_gamepad_, SIGNAL(axisLeftXChanged(double)), this, SLOT(LeftControllerLeftRight(double)));
            connect(m_gamepad_, SIGNAL(axisLeftYChanged(double)), this, SLOT(LeftControllerUpDown(double)));
        }

        VcsMainWindow::~VcsMainWindow()
        {
            delete m_gamepad_;
            delete vcsConsole_;
            //delete qImage_;
        }

        void VcsMainWindow::DrawFrame(uint8_t *screen)
        {
            screen_ = screen;
            repaint();
        }
        
        void VcsMainWindow::LeftControllerA(bool value)
        {
            //qDebug() << "A" << value;
            leftController_ &= 0xfe;
            if (value != 0)
            {
                leftController_ |= 0x01;
            }
        }

        void VcsMainWindow::LeftControllerUpDown(double value)
        {
            //qDebug() << "Button Up" << value;
            leftController_ &= 0xcf;
            if (value < 0)
            {
                leftController_ |= 0x10;
            }
            if (value > 0)
            {
                leftController_ |= 0x20;
            }
        }
            
        void VcsMainWindow::LeftControllerLeftRight(double value)
        {
            //qDebug() << "Left X" << value;
            leftController_ &= 0x3f;
            if (value < 0)
            {
                leftController_ |= 0x40;
            }
            if (value > 0)
            {
                leftController_ |= 0x80;
            }
        }

        void VcsMainWindow::paintEvent(QPaintEvent* event)
        {
            Q_UNUSED(event);
            
            QRect rect(0,0,640,768);
            QPainter painter(this);
            
            for (int y=0; y<192; y++)
            {
                QRgb *rowData = (QRgb*)qImage_->scanLine(y);
                for (int x=0; x<160; x++)
                {
                    uint8_t value = screen_[y*160+x];
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

