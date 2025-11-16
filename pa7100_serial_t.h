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
#ifndef PA7100_SERIAL_T_H
#define PA7100_SERIAL_T_H

#include <QObject>
#include <QStringList>

class pa7100_serial_t : public QObject
{
    Q_OBJECT
public:
    explicit pa7100_serial_t(QObject *parent = nullptr);

    QStringList enumerate_serial_ports();
    // Baud rate generation function
    QStringList enumberate_baud_rates() {
        QStringList baudRates = {
            "300", "600", "1200", "2400", "4800",
            "9600", "19200", "38400",
            "57600", "115200"
        };
        return baudRates;
    }
    QString proper_device_name(const QString& input_string);


signals:

};

#endif // PA7100_SERIAL_T_H
