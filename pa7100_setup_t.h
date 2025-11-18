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
#ifndef PA7100_SETUP_T_H
#define PA7100_SETUP_T_H

#include <QMainWindow>
#include <QStringList>
#include <QComboBox>
#include <QCloseEvent>
#include "pa7100_serial_t.h"
#include "pa7100_modbus_t.h"

QT_BEGIN_NAMESPACE
namespace Ui { class pa7100_setup_t; }
QT_END_NAMESPACE

class pa7100_setup_t : public QMainWindow
{
    Q_OBJECT

public:
    pa7100_setup_t(QWidget *parent = nullptr);
    ~pa7100_setup_t();

protected slots:
    void slot_detect_activated();
    void slot_cancel();
    void slot_save();

protected:
    virtual void closeEvent(QCloseEvent* event);

private:
    Ui::pa7100_setup_t  *ui;
    pa7100_serial_t     serial;
    pa7100_modbus_t     modbus;
    bool                cancel;
    QString             detected_uart;
    int                 detected_baud;
    int                 detected_id;

    void populate_serial_port_combobox(QComboBox* combo_box, const QStringList& ports);
    bool detect();
    bool detect_auto();
    void detected_clear();
    void display_detected_params();
    void save_params_to_detected(int baud, int id, int option_bits);
};
#endif // PA7100_SETUP_T_H
