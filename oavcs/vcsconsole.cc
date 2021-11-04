
#include "headers/vcsconsole.h"

#include <QPainter>

namespace oa
{
    namespace vcs
    {
        VcsConsole::VcsConsole(VcsParameters* vcsParameters) :
            vcsRiot_(),
            vcsConsoleType_(vcsParameters->GetConsoleType()),
            vcsTia_(&vcsConsoleType_),
            vcsMemory_(&vcsTia_, &vcsRiot_, vcsParameters),
            vcsAudio_(&vcsTia_),
            cpu_(&vcsMemory_)
        {
            this->resize(vcsConsoleType_.GetXResolution() * 4, vcsConsoleType_.GetYResolution() * 4);
            qImage_ = new QImage(vcsConsoleType_.GetXResolution(),
                                 vcsConsoleType_.GetYResolution(),
                                 QImage::Format_RGB32);
            
            //vcsMainWindow_ = vcsMainWindow;
            ticksPerFrame_ = vcsConsoleType_.TicksPerSecond() / vcsConsoleType_.GetFramesPerSecond();
            StartUp();
        }
        
        VcsConsole::~VcsConsole()
        {
        }
       
        void VcsConsole::StartUp()
        {
            cpu_.Reset();
            vcsRiot_.Reset();
            vcsTia_.Reset();

            connect(&cpuTimer_, SIGNAL(timeout()), SLOT(StartNextFrame()));
            cpuTimer_.setTimerType(Qt::PreciseTimer);
            cpuTimer_.setInterval(16);
            cpuTimer_.start();
        }
        
        void VcsConsole::paintEvent(QPaintEvent* event)
        {
            Q_UNUSED(event);
            
            QRgb* dest = (QRgb*)qImage_->bits();
            memcpy(dest, vcsTia_.GetScreen(), vcsConsoleType_.GetYResolution() * vcsConsoleType_.GetXResolution() * sizeof(QRgb));

            QRect rect(0,0,vcsConsoleType_.GetXResolution()*4, vcsConsoleType_.GetYResolution()*4);
            QPainter painter(this);          
            painter.drawImage(rect, *qImage_);
        }
        
        void VcsConsole::StartNextFrame()
        {
            //qDebug() << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            uint32_t ticks = 0;
            vcsAudio_.ExecuteTick();
            while (ticks < ticksPerFrame_)
            {
                vcsRiot_.ExecuteTick();
                if (vcsTia_.IsCycleZero())
                {
                    while (ticks % 3 != 0)
                    {
                        ticks ++;
                    }
                }
                if ((ticks % 3) == 0 && !vcsTia_.IsCpuBlocked())
                {
                    cpu_.ExecuteTick();
                }
                vcsTia_.ExecuteTick();
                if (vcsTia_.Repaint())
                {
                    this->repaint();
                }
                ticks++;
            }
        }
    }
}
