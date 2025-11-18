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
    void save_params_to_detected();

};
#endif // PA7100_SETUP_T_H
