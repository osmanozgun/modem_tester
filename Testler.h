#ifndef TESTLER_H
#define TESTLER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QSerialPort>
#include <QTimer>
#include <QDateTime>
#include <QProcess>
#include <QThread>
#include <QMutex>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QDir>

// Forward declarations
class TabTestBaslat;
class VirtualMeterPanel;
class IndependentTestWindow;

// ============================================================================
// ANA KONTEYNER WIDGET
// ============================================================================
class TestlerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestlerWidget(QWidget *parent = nullptr);

private:
    void setupLayout();
    
    QHBoxLayout *mainLayout;
    QSplitter *splitter;
    TabTestBaslat *leftPanel;
    VirtualMeterPanel *rightPanel;
};

// ============================================================================
// SOL PANEL - TEST KISMI
// ============================================================================
class TabTestBaslat : public QWidget
{
    Q_OBJECT

public:
    explicit TabTestBaslat(QWidget *parent = nullptr);

private slots:
    void baslatTest();
    void durdurTest();
    void testEkraniBaslat();
    void seriVeriOku();

private:
    void guncelleRssi(int rssi);
    void guncelleGorunum();
    void baslatBlink();
    void durBlink();
    bool RFTesti(const QStringList &logs);

    // UI Components
    QComboBox *portCombo;
    QLineEdit *seriNoEdit;
    QComboBox *minRssiCombo;
    QComboBox *maxRssiCombo;
    QPushButton *btnBaslat;
    QPushButton *btnDurdur;
    QPushButton *btnTestEkraniEkle;
    QLabel *statusLabel;
    QLabel *eepromLabel;
    QLabel *rfLabel;
    QLabel *networkLabel;
    QLabel *rssiLabel;
    QLabel *genelSonucLabel;
    QPlainTextEdit *logEdit;

    // Serial Communication
    QSerialPort *port;
    QByteArray readBuffer;
    QStringList logLines;

    // Test State
    bool eeprom_ok;
    bool rf_ok;
    bool network_ok;

    // Blink Animation
    QTimer *blinkTimer;
    int blinkCount;
    bool blinkVisible;

    // Process Management
    QProcess *veritabaniProcess;
    QProcess *emuProcess;
    QProcess *testProcess;
};

// ============================================================================
// SANAL SAYAÇ PANEL
// ============================================================================

struct MeterConfig {
    QString configFile;
    QString id;
    QString response;
    QStringList dataList;
};

class VirtualMeterPanel : public QWidget
{
    Q_OBJECT

public:
    explicit VirtualMeterPanel(QWidget *parent = nullptr);
    ~VirtualMeterPanel();

private slots:
    void connectPort();
    void disconnectPort();
    void refreshPorts();
    void onSerialDataReceived();
    void updateTimer();
    void resetToDefaultBaud();
private:
    void setupUI();
    void loadMeterConfigs();
    void parseTOMLFile(const QString &content, int meterNum);
    void processBuffer();
    void handleIncomingData(const QByteArray &data);
    bool findMeterBySerialNumber(const QString &serialNumber);
    void sendIdentResponse();
    void sendDataList();
    
    void logMessage(const QString &message, const QString &color = "black");
    QByteArray calculateBCC(const QByteArray &data);

    // UI Components
    QVBoxLayout *mainLayout;
    QGroupBox *portControlGroup;
    QComboBox *portComboBox;
    QPushButton *connectBtn;
    QPushButton *disconnectBtn;
    QPushButton *refreshBtn;
    QLabel *timerLabel;
    QTextEdit *logTextEdit;
    QString lastAddressedSerialNumber;
    // Serial Communication
    QSerialPort *serialPort;
    QByteArray buffer;
    QTimer *connectionTimer;
    QDateTime startTime;

    // Virtual Meter State
    QMap<int, MeterConfig> meters;
    bool switchedTo9600;
    int activeMeterID;
    bool stopThread;
    bool meter1Flag, meter2Flag, meter3Flag, meter4Flag;
};

// ============================================================================
// SERIAL READER THREAD
// ============================================================================
class SerialReaderThread : public QThread
{
    Q_OBJECT

public:
    explicit SerialReaderThread(QSerialPort *port, QObject *parent = nullptr);
    void stop();

protected:
    void run() override;

signals:
    void dataReceived(const QByteArray &data);

private:
    QSerialPort *serialPort;
    QMutex mutex;
    bool stopRequested;
    QByteArray m_buffer; // <--- HATA VEREN DEĞİŞKEN DOĞRU YERE EKLENDİ
};

#endif // TESTLER_H