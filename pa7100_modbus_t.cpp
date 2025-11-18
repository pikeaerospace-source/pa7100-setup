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
#include "pa7100_modbus_t.h"
#include <QDebug>
#include <errno.h>

pa7100_modbus_t::pa7100_modbus_t(QObject *parent)
    : QObject{parent}
    , ctx(nullptr)
{

}

int pa7100_modbus_t::open(const QString& port_name,int baud_rate,int unit_id)
{
    int rc = -1;
    close();
    try {
        // Convert QString to standard string for serial port
        std::string port_nameStr = port_name.toStdString();

        // Create Modbus RTU context
        ctx = modbus_new_rtu(
            port_nameStr.c_str(),  // Serial port name
            baud_rate,             // Baud rate
            'N',                  // Parity (None)
            8,                    // Data bits
            1                     // Stop bits
        );

        // Check context creation
        if (ctx == nullptr) {
            qDebug() << "Failed to create Modbus context";
            return 0;
        }

        // Set Modbus unit ID
        modbus_set_slave(ctx, unit_id);

        // Open serial port
        if (modbus_connect(ctx) == -1) {
            qDebug() << "Modbus connection failed: "
                     << modbus_strerror(errno);
            close();
            return 0;
        }

        // Use 1000 ms (1 second) timeout
        uint32_t response_timeout_sec = 0;
        uint32_t response_timeout_usec = (250*1000);
        rc = modbus_set_response_timeout(ctx, response_timeout_sec, response_timeout_usec);
        if (rc == -1) {
            qDebug() << "Failed to set response timeout: "
                     << modbus_strerror(errno);
            close();
            return 0;
        }

    } catch (const std::exception& e) {
        qDebug() << "Exception in Modbus open: " << e.what();
    }
    return rc;
}

void pa7100_modbus_t::close()
{
    // Cleanup
    if (ctx != nullptr) {
        modbus_close(ctx);
        modbus_free(ctx);
        ctx = nullptr;
    }
}

uint16_t pa7100_modbus_t::read_one_register(uint16_t addr)
{
    int rc = -1;
    uint16_t value = 0;
    uint16_t retries=10;

    while ( rc < 0 && retries-- > 0 )
    {
        try {
            // Read holding register 40003
            // Note: Modbus register addressing is 0-based, so 40003 becomes 2
            rc = modbus_read_registers(
                ctx,           // Modbus context
                addr,          // Register address
                1,             // Number of registers to read
                &value         // Buffer to store read value
            );
        } catch (const std::exception& e) {
            qDebug() << "Exception in Modbus read: " << e.what();
            value = 0;
        }

        // Check read operation
        if (rc == -1) {
            qDebug() << "Register read failed: "
                        << modbus_strerror(errno);
            value = 0;
        }
    }

    return value;
}

/********************************************************************************
 * @brief Write one register to the device.
 * @param addr Register address to write.
 * @param value Value to write to the register.
 * @return true if the write was successful, false otherwise.
 ********************************************************************************/
bool pa7100_modbus_t::write_one_register(uint16_t addr, uint16_t value)
{
    int rc = -1;        
    try {
        // Write holding register
        rc = modbus_write_register(
            ctx,      // Modbus context
            addr,     // Register address
            value     // Value to write
        );
    } catch (const std::exception& e) {
        qDebug() << "Exception in Modbus write: " << e.what();
        return false;
    }  
    // Check write operation
    if (rc == -1) {
        qDebug() << "Register write failed: "
                    << modbus_strerror(errno);
        return false;
    }
    return true;
}

