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

