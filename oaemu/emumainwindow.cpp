#include "headers/emumainwindow.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <QMessageBox>
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QtGamepad/QGamepad>



using namespace std;

EmuMainWindow::EmuMainWindow()
{
    loadClicked();
    
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
    
    connect(m_gamepad, &QGamepad::buttonSelectChanged, this, [](bool pressed){
    qDebug() << "Button Select" << pressed;
    });
}

void EmuMainWindow::leftControllerA(bool value)
{
    qDebug() << "A" << value;
    leftController &= 0xfe;
    if (value != 0)
    {
        leftController |= 0x01;
    }
}

void EmuMainWindow::leftControllerB(bool value)
{
    qDebug() << "B" << value;
    leftController &= 0xfd;
    if (value != 0)
    {
        leftController |= 0x02;
    }
}

void EmuMainWindow::leftControllerSelect(bool value)
{
    qDebug() << "Button Select" << value;
    leftController &= 0xfb;
    if (value)
    {
        leftController |= 0x04;
    }
}

void EmuMainWindow::leftControllerStart(bool value)
{
    qDebug() << "Button Start" << value;
    leftController &= 0xf7;
    if (value)
    {
        leftController |= 0x08;
    }
}

void EmuMainWindow::leftControllerUpDown(double value)
{
    qDebug() << "Button Up" << value;
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
    
void EmuMainWindow::leftControllerLeftRight(double value)
{
    qDebug() << "Left X" << value;
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

EmuMainWindow::~EmuMainWindow()
{
    delete m_gamepad;
}

void EmuMainWindow::loadClicked()
{
    qImage2 = new QImage(256, 240, QImage::Format_RGB888);
    
    ppu.memory = &cpu.memory;
    ppu.cpu = &cpu;
    
//    apu = new EmuAudio(this);
    apu = new EmuApu(this);
    apu->memory = &cpu.memory;
    
    loadDKong();
    
    unsigned char prgRom[0x4000];
    for (int i=0; i<0x4000; i++)
    {
        prgRom[i] = dKongFile[i+16];
    }
    cpu.memory.loadProgRom(prgRom);
    
    unsigned char charRom[0x2000];
    for (int i=0; i<0x2000; i++)
    {
        charRom[i] = dKongFile[i+16+0x4000];
    }
    cpu.memory.loadCharRom(charRom);
    
    try 
    {
        cpu.reset();
        ppu.reset();        
    //    QMessageBox::information(0, QString("Information"), QString("Finished running!"), QMessageBox::Ok);
    }
    catch (char const* error)
    {
        QMessageBox::information(0, QString("Information"), QString(error), QMessageBox::Ok);
    }
    
    palette[0].setRgb(3*32,3*32,3*32);
    palette[1].setRgb(0*32,1*32,4*32);
    palette[2].setRgb(0*32,0*32,6*32);
    palette[3].setRgb(3*32,2*32,6*32);
    palette[4].setRgb(4*32,0*32,3*32);
    palette[5].setRgb(5*32,0*32,3*32);
    palette[6].setRgb(5*32,1*32,0*32);
    palette[7].setRgb(4*32,2*32,0*32);
    palette[8].setRgb(3*32,2*32,0*32);
    palette[9].setRgb(1*32,2*32,0*32);
    palette[10].setRgb(0*32,3*32,1*32);
    palette[11].setRgb(0*32,4*32,0*32);
    palette[12].setRgb(0*32,2*32,2*32);
    palette[13].setRgb(0*32,0*32,0*32);
    palette[14].setRgb(0*32,0*32,0*32);
    palette[15].setRgb(0*32,0*32,0*32);
    palette[16].setRgb(5*32,5*32,5*32);
    palette[17].setRgb(0*32,3*32,6*32);
    palette[18].setRgb(0*32,2*32,7*32);
    palette[19].setRgb(4*32,0*32,7*32);
    palette[20].setRgb(5*32,0*32,7*32);
    palette[21].setRgb(7*32,0*32,4*32);
    palette[22].setRgb(7*32,0*32,0*32);
    palette[23].setRgb(6*32,3*32,0*32);
    palette[24].setRgb(4*32,3*32,0*32);
    palette[25].setRgb(1*32,4*32,0*32);
    palette[26].setRgb(0*32,4*32,0*32);
    palette[27].setRgb(0*32,5*32,3*32);
    palette[28].setRgb(0*32,4*32,4*32);
    palette[29].setRgb(0,0,0);
    palette[30].setRgb(0,0,0);
    palette[31].setRgb(0,0,0);
    palette[32].setRgb(7*32,7*32,7*32);
    palette[33].setRgb(3*32,5*32,7*32);
    palette[34].setRgb(4*32,4*32,7*32);
    palette[35].setRgb(6*32,3*32,7*32);
    palette[36].setRgb(7*32,0*32,7*32);
    palette[37].setRgb(7*32,3*32,7*32);
    palette[38].setRgb(7*32,4*32,0*32);
    palette[39].setRgb(7*32,5*32,0*32);
    palette[40].setRgb(6*32,6*32,0*32);
    palette[41].setRgb(3*32,6*32,0*32);
    palette[42].setRgb(0*32,7*32,0*32);
    palette[43].setRgb(2*32,7*32,6*32);
    palette[44].setRgb(0*32,7*32,7*32);
    palette[45].setRgb(0,0,0);
    palette[46].setRgb(0,0,0);
    palette[47].setRgb(0,0,0);
    palette[48].setRgb(7*32,7*32,7*32);
    palette[49].setRgb(5*32,6*32,7*32);
    palette[50].setRgb(6*32,5*32,7*32);
    palette[51].setRgb(7*32,5*32,7*32);
    palette[52].setRgb(7*32,4*32,7*32);
    palette[53].setRgb(7*32,5*32,5*32);
    palette[54].setRgb(7*32,6*32,4*32);
    palette[55].setRgb(7*32,7*32,2*32);
    palette[56].setRgb(7*32,7*32,3*32);
    palette[57].setRgb(5*32,7*32,2*32);
    palette[58].setRgb(4*32,7*32,3*32);
    palette[59].setRgb(2*32,7*32,6*32);
    palette[60].setRgb(4*32,6*32,7*32);
    palette[61].setRgb(0,0,0);
    palette[62].setRgb(0,0,0);
    palette[63].setRgb(0,0,0);
    
    cpuTimer = new QTimer(this);
    connect(cpuTimer, SIGNAL(timeout()), SLOT(startNextFrame()));
    cpuTimer->setInterval(16);
    cpuTimer->start();
}

void EmuMainWindow::loadDKong()
{    
    std::ifstream inFile;
    inFile.open("roms/Donkey_kong.nes", ios::in | ios::binary | ios::ate);
    
    if (inFile.is_open())
    {
        inFile.seekg(0, ios::beg);
        inFile.read(dKongFile, SIZE);        
        inFile.close();
    }
    else
    {
        std::cerr << "Can't find Donky Kong file" << std::endl;
    }
}

void EmuMainWindow::paintEvent(QPaintEvent* event)
{
    QRect rect(0,0,1024,768);
    QPainter painter(this);
    
    for (int y=0; y<240; y++)
    {
        for (int x=0; x<256; x++)
        {
            if (ppu.screen[y*256+x] >= 64)
            {
               // throw "invalid palette value";
            }
            int value = ppu.screen[y*256+x];
            QColor color = palette[value];
            qImage2->setPixelColor(x, y, color);
        }
    }
    painter.drawImage(rect, *qImage2);
}

void EmuMainWindow::startNextFrame()
{
    if (inFrame)
    {
        qDebug() << "Frame overrun";
    }
    try 
    {
        inFrame = true;
        int counter = 0;
        while (counter < 16639)
        {
            int ticks = cpu.executeTicks(1);
            ppu.executeTicks(ticks * 3);
            apu->executeTicks();
            counter += ticks;
            readGamepad();
        }
        this->repaint();
        inFrame = false;
    }
    catch (char const* error)
    {
        QMessageBox::information(0, QString("Information"), QString(error), QMessageBox::Ok);
    }    
}

void EmuMainWindow::readGamepad()
{
    if (cpu.memory.loadController == 1)
    {
        cpu.memory.loadController = 0;
        cpu.memory.leftController = leftController;
    }
}

