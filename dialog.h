#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QSerialPort>
#include<QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
private slots:
    void readSerial();
    void updatetemp(QString);
    void updatehum(QString);

private:
    Ui::Dialog *ui;
    QSerialPort *esp;
    static const quint16 esp_vid=4292;
    static const quint16 esp_pid=60000;
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_datatemp;
    QString parsed_datahum;
    double temp_val;
    double hum_val;
};
#endif // DIALOG_H
