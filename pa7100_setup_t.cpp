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
#include "pa7100_setup_t.h"
#include "ui_pa7100_setup_t.h"

pa7100_setup_t::pa7100_setup_t(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::pa7100_setup_t)
    , cancel(false)
{
    ui->setupUi(this);
    
    populate_serial_port_combobox(ui->uart_combobox,serial.enumerate_serial_ports());
    
    ui->baud_combobox->addItems(serial.enumberate_baud_rates());
    ui->baud_combobox->setCurrentText("9600");

    ui->id_spinbox->setValue(1);
    
    QObject::connect(ui->detect_button,SIGNAL(clicked()),this,SLOT(slot_detect_activated()));
    QObject::connect(ui->cancel_button,SIGNAL(clicked()),this,SLOT(slot_cancel()));
    QObject::connect(ui->save_button,SIGNAL(clicked()),this,SLOT(slot_save()));
}

pa7100_setup_t::~pa7100_setup_t()
{
    delete ui;
}


void pa7100_setup_t::closeEvent(QCloseEvent* event)
{
    modbus.close();
}

/********************************************************************************
* @brief Populate the serial port combo box accounting for some edge cases.
* @param combo_box The pointer to the combo box to populate
* @param ports The string list of ports to opulate into the combo box
********************************************************************************/
void pa7100_setup_t::populate_serial_port_combobox(QComboBox* combo_box, const QStringList& ports)
{
    // Clear any existing items
    combo_box->clear();

    // Check if ports list is empty
    if (ports.isEmpty())
    {
        combo_box->addItem(tr("No ports found"));
        combo_box->setEnabled(false);
        return;
    }

    // Populate the combo box with available ports
    combo_box->addItems(ports);
    combo_box->setEnabled(true);

    // Optional: Select the first port by default
    if (combo_box->count() > 0)
    {
        combo_box->setCurrentIndex(0);
    }
}

void pa7100_setup_t::slot_cancel()
{
    qDebug() << "cancel pressed";
    cancel = true;
}

void pa7100_setup_t::slot_save()
{
    qDebug() << "save pressed";
    save_params_to_detected(ui->baud_combobox->currentText().toInt(),
                            ui->id_spinbox->value());
}

/********************************************************************************
* @brief Save current parameters to detected device.
********************************************************************************/
void pa7100_setup_t::save_params_to_detected(int baud, int id)
{   
    if ( !detected_uart.isEmpty() && modbus.open(detected_uart,detected_baud,detected_id) == 0 )
    {
        modbus.write_one_register(PA7100_REG_ADDRESS, id);
        modbus.write_one_register(PA7100_REG_BAUD, baud / 10);

        modbus.close();
    }
}

/********************************************************************************
* @brief Clear all of the detected state variables.
********************************************************************************/
void pa7100_setup_t::detected_clear()
{
    detected_baud = 0;
    detected_id = 0;
    detected_uart.clear();

    ui->serialno_display->setText("");
    ui->model_display->setText("");
    ui->version_display->setText("");
}

/********************************************************************************
* @brief Use the detected parameters to populate the on-form params.
********************************************************************************/
void pa7100_setup_t::display_detected_params()
{
    if ( modbus.open(detected_uart,detected_baud,detected_id) == 0 )
    {
        uint16_t version = modbus.read_one_register(PA7100_REG_VERSION);
        QString version_str = QString().asprintf("%d",version>>8)+"."+QString().asprintf("%d",version&0xFF);
        ui->version_display->setText(version_str);
        // for(int n=0; n < 1000; n++ )
            QApplication::processEvents();

        uint16_t model = modbus.read_one_register(PA7100_REG_MODEL_NO);
        QString model_str = QString().asprintf("%d",model);
        ui->model_display->setText(model_str);
        // for(int n=0; n < 1000; n++ )
            QApplication::processEvents();

        QString serial_str;
        for( int serial_idx=0; serial_idx < 6; serial_idx++ )
        {
            uint16_t serial = modbus.read_one_register(PA7100_REG_SERIAL_NO_0+serial_idx);
            serial_str += QString::asprintf("%02X%02X",serial>>8,serial&0xFF);
            ui->serialno_display->setText(serial_str);
            // for(int n=0; n < 1000; n++ )
                QApplication::processEvents();
        }


        modbus.close();
    }
}

/********************************************************************************
* @brief The detect button has been pressed, perform the detection operation..
********************************************************************************/
void pa7100_setup_t::slot_detect_activated()
{
    bool detected;
    detected_clear();
    if ( ui->auto_detect_checkbox->isChecked() )
    {
        detected = detect_auto();
    }
    else
    {
        detected = detect();
    }
    if ( detected )
    {
        detected_uart = serial.proper_device_name(ui->uart_combobox->currentText());
        detected_baud = ui->baud_combobox->currentText().toInt();
        detected_id = ui->id_spinbox->value();
        display_detected_params();
    }
}

/********************************************************************************
* @brief Perform a detection operation given the current parameters.
* @return true if the device was detected.
********************************************************************************/
bool pa7100_setup_t::detect()
{
    bool rc=false;
    QString device_name = serial.proper_device_name(ui->uart_combobox->currentText());

    if ( modbus.open(device_name,ui->baud_combobox->currentText().toInt(),ui->id_spinbox->value()) >= 0 )
    {
        QString msg;
        msg = device_name + QString(" ") +
            ui->baud_combobox->currentText() + QString(" ")
            + QString().asprintf("%d",ui->id_spinbox->value());

        ui->line_status_display->setText("BUSY...");
        ui->line_status_display->repaint();

        if ( modbus.read_model_no() == 7100 )
        {
            msg = "DETECTED " + msg;
            rc = true;
        }
        else
        {
            msg = "!!FAILED " + msg;
        }
        qDebug() << msg;
        ui->line_status_display->setText(msg);

        modbus.close();
    }
    else
    {
        QString msg;
        msg = device_name + QString(" open failed!");
        qDebug() << msg;
        ui->line_status_display->setText(msg);
    }
    return rc;
}

/********************************************************************************
* @brief Perform a scanning detection, cycling through each parameter.
* @return true if the device was detected.
********************************************************************************/
bool pa7100_setup_t::detect_auto()
{
    int uart_count = ui->uart_combobox->count();
    int uart_start = ui->uart_combobox->currentIndex();
    int baud_count = ui->baud_combobox->count();
    int baud_start = ui->baud_combobox->currentIndex(); 
    int id_start = ui->id_spinbox->value();

    if ( uart_start < 0 )   uart_start = 0;
    if ( baud_start < 0 )   baud_start = 0;
    if ( id_start < 1 || id_start > 247 ) id_start = 1;

    int uart_idx = uart_start;
    int baud_idx = baud_start;
    int id_idx = id_start;

    cancel = false;

    do {
        ui->uart_combobox->setCurrentIndex(uart_idx);
    
        do {
            ui->baud_combobox->setCurrentIndex(baud_idx);

            do {
                ui->id_spinbox->setValue(id_idx);

                    QApplication::processEvents(); // Processes pending events

                    if ( detect() )
                        return true;

                if ( ++id_idx > 247 ) id_idx=1;
            } while ( !cancel && id_idx != id_start );

            if ( ++baud_idx >= baud_count ) baud_idx=0;
        } while ( !cancel && baud_idx != baud_start );

        if ( ++uart_idx >= uart_count ) baud_idx=0;
    } while ( !cancel && uart_idx != uart_start );

    cancel = false;

    return false;
}

