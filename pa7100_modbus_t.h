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
#ifndef PA7100_MODBUS_T_H
#define PA7100_MODBUS_T_H

#include <QObject>
#include <modbus/modbus.h>

#define REGADDR(n) (n-1)

#define PA7100_REG_HEARTBEAT        REGADDR(1)  /**< Heartbeat 1 Sec. */
#define PA7100_REG_FAULT            REGADDR(2)  /**< Fault Code */
#define PA7100_REG_MODEL_NO         REGADDR(3)  /**< Model Number [7100] */
#define PA7100_REG_VERSION          REGADDR(4)  /**< Version [Maj:Min] */
#define PA7100_REG_OPTION           REGADDR(5)  /**< Option Bits */
#define PA7100_REG_SERIAL_NO_0      REGADDR(6)  /**< Serial Number [0:1] */
#define PA7100_REG_SERIAL_NO_1      REGADDR(7)  /**< Serial Number [2:3] */
#define PA7100_REG_SERIAL_NO_2      REGADDR(8)  /**< Serial Number [4:5] */
#define PA7100_REG_SERIAL_NO_3      REGADDR(9)  /**< Serial Number [0:1] */
#define PA7100_REG_SERIAL_NO_4      REGADDR(10) /**< Serial Number [2:3] */
#define PA7100_REG_SERIAL_NO_5      REGADDR(11) /**< Serial Number [4:5] */
#define PA7100_REG_ADDRESS          REGADDR(12) /**< MODBUS Address */
#define PA7100_REG_BAUD             REGADDR(13) /**< BAUD/10 */
#define PA7100_REG_TEMPC_100        REGADDR(14) /**< TEMPC*100 */
#define PA7100_REG_TEMPF_100        REGADDR(15) /**< TEMPF*100 */
#define PA7100_REG_RH_100           REGADDR(16) /**< RH*100 */
#define PA7100_REG_KPA_INTEGER      REGADDR(17) /**< Baro Press in kPa [Integer] */
#define PA7100_REG_KPA_DECIMAL      REGADDR(18) /**< Baro Press in kPa [Decimal] */
#define PA7100_REG_TEMPC_F          REGADDR(19) /**< TEMPC Float */
#define PA7100_REG_TEMPF_F          REGADDR(21) /**< TEMPF Float */
#define PA7100_REG_RH_F             REGADDR(23) /**< Relatie Humidity % Float */
#define PA7100_REG_KPA_F            REGADDR(25) /**< Baro Press in kPa Float */
#define PA7100_REG_HPA_F            REGADDR(27) /**< Baro Press in hPa Float */

class pa7100_modbus_t : public QObject
{
    Q_OBJECT
public:
   
    explicit    pa7100_modbus_t(QObject *parent = nullptr);
    int         open(const QString& port_name,int baud_rate,int unit_id);
    void        close();
    uint16_t    read_model_no()
                {
                    return read_one_register(PA7100_REG_MODEL_NO) ;
                }
    uint16_t    read_one_register(uint16_t addr);
private:
    modbus_t    *ctx;

};

#endif // PA7100_MODBUS_T_H
