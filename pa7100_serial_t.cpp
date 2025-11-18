/********************************************************************************
* ╔═╗┬┬┌─┌─┐  ╔═╗┌─┐┬─┐┌─┐┌─┐┌─┐┌─┐┌─┐┌─┐                                       *
* ╠═╝│├┴┐├┤   ╠═╣├┤ ├┬┘│ │└─┐├─┘├─┤│  ├┤                                        *
* ╩  ┴┴ ┴└─┘  ╩ ╩└─┘┴└─└─┘└─┘┴  ┴ ┴└─┘└─┘                                       *
* ╦═╗┌─┐┌─┐┌─┐┌─┐┬─┐┌─┐┬ ┬  ╔═╗┌─┐                                              *
* ╠╦╝├┤ └─┐├┤ ├─┤├┬┘│  ├─┤  ║  │ │                                              *
* ╩╚═└─┘└─┘└─┘┴ ┴┴└─└─┘┴ ┴  ╚═╝└─┘o                                             *
*                                                                               *
* Copyright © 2025 Pike Aerospace Research Co.                                  *
*                                                                               *
* Permission is hereby granted, free of charge, to any person obtaining a copy  *
* of this software and associated documentation files (the "Software"), to deal *
* in the Software without restriction, including without limitation the rights  *
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     *
* copies of the Software, and to permit persons to whom the Software is         *
* furnished to do so, subject to the following conditions:                      *
*                                                                               *          
* The above copyright notice and this permission notice shall be included in    *
* all copies or substantial portions of the Software.                           *
*                                                                               *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, *
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE *
* SOFTWARE.                                                                     *
*                                                                               *
********************************************************************************/
#include "pa7100_serial_t.h"
#include <QSerialPortInfo>

pa7100_serial_t::pa7100_serial_t(QObject *parent)
    : QObject{parent}
{
}

QString pa7100_serial_t::proper_device_name(const QString& input_string) 
{
    QString return_string;

    // Find the index of the first space
    int idx = input_string.indexOf(' ');

    // If no space is found, return the entire string
    if (idx >= 0 )
    {
        return_string = input_string.left(idx);
    }
    else
    {
        return_string = input_string;
    }

    // Return the substring from the beginning up to the first space
    #if defined(Q_OS_UNIX)
        return "/dev/"+return_string;
    #else
        return return_string+":";   /* Windows ?? */
    #endif
}


QStringList pa7100_serial_t::enumerate_serial_ports()
{
    QStringList availablePorts;

    // Retrieve all available serial ports using QSerialPortInfo
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &portInfo : serialPortInfos) {
        // Construct a descriptive string for each port
        QString portDescription = portInfo.portName();

        // Add additional details if available
        if (!portInfo.description().isEmpty()) {
            portDescription += " (" + portInfo.description() + ")";
        }

        availablePorts.append(portDescription);
    }

    return availablePorts;
}

