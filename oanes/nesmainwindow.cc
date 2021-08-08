#include "headers/nesmainwindow.h"

#include <QPainter>
#include <QDebug>
#include <QtGamepad/QGamepad>

#include "headers/nesconsole.h"

namespace oa
{
    namespace nes
    {
        
        NesMainWindow::NesMainWindow()
        {
            qImage = new QImage(256, 240, QImage::Format_RGB32);
            
            nesConsole_ = new NesConsole(this);
            nesConsole_->StartUp();
            
            auto gamepads = QGamepadManager::instance()->connectedGamepads();
            if (gamepads.isEmpty()) {
                qDebug() << "Did not find any connected gamepads";
                return;
            }
            
            m_gamepad = new QGamepad(*gamepads.begin(), this);
            
            connect(m_gamepad, SIGNAL(buttonAChanged(bool)), this, SLOT(leftControllerA(bool)));
            connect(m_gamepad, SIGNAL(buttonBChanged(bool)), this, SLOT(leftControllerB(bool)));
            connect(m_gamepad, SIGNAL(buttonXChanged(bool)), this, SLOT(leftControllerSelect(bool)));
            connect(m_gamepad, SIGNAL(buttonYChanged(bool)), this, SLOT(leftControllerStart(bool)));
            connect(m_gamepad, SIGNAL(axisLeftXChanged(double)), this, SLOT(leftControllerLeftRight(double)));
            connect(m_gamepad, SIGNAL(axisLeftYChanged(double)), this, SLOT(leftControllerUpDown(double)));
        }

        NesMainWindow::~NesMainWindow()
        {
            delete m_gamepad;
            delete nesConsole_;
            delete qImage;
        }

        void NesMainWindow::DrawFrame(uint8_t *screen)
        {
            screen_ = screen;
            repaint();
        }
        
        void NesMainWindow::leftControllerA(bool value)
        {
            //qDebug() << "A" << value;
            leftController &= 0xfe;
            if (value != 0)
            {
                leftController |= 0x01;
            }
        }

        void NesMainWindow::leftControllerB(bool value)
        {
            //qDebug() << "B" << value;
            leftController &= 0xfd;
            if (value != 0)
            {
                leftController |= 0x02;
            }
        }

        void NesMainWindow::leftControllerSelect(bool value)
        {
            //qDebug() << "Button Select" << value;
            leftController &= 0xfb;
            if (value)
            {
                leftController |= 0x04;
            }
        }

        void NesMainWindow::leftControllerStart(bool value)
        {
            //qDebug() << "Button Start" << value;
            leftController &= 0xf7;
            if (value)
            {
                leftController |= 0x08;
            }
        }

        void NesMainWindow::leftControllerUpDown(double value)
        {
            //qDebug() << "Button Up" << value;
            leftController &= 0xcf;
            if (value < 0)
            {
                leftController |= 0x10;
            }
            if (value > 0)
            {
                leftController |= 0x20;
            }
        }
            
        void NesMainWindow::leftControllerLeftRight(double value)
        {
            //qDebug() << "Left X" << value;
            leftController &= 0x3f;
            if (value < 0)
            {
                leftController |= 0x40;
            }
            if (value > 0)
            {
                leftController |= 0x80;
            }
        }

        void NesMainWindow::paintEvent(QPaintEvent* event)
        {
            Q_UNUSED(event);
            
            QRect rect(0,0,1024,768);
            QPainter painter(this);
            
            for (int y=0; y<240; y++)
            {
                QRgb *rowData = (QRgb*)qImage->scanLine(y);
                for (int x=0; x<256; x++)
                {
                    int value = screen_[y*256+x];
                    QColor color = nesPalette_.GetColor(value);
                    rowData[x] = color.rgb();
                }
            }
            painter.drawImage(rect, *qImage);
        }

    }
}
