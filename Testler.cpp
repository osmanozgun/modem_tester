#include "Testler.h"
#include <QDir>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QComboBox>
#include <QCheckBox>
#include <QDebug>
#include <QProcess>
#include <QTimer>
#include <QSerialPortInfo>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextStream>
#include <QApplication>
#include <QPushButton>

// ============================================================================
// ANA KONTEYNER WIDGET
// ============================================================================

TestlerWidget::TestlerWidget(QWidget *parent) : QWidget(parent) {
    setupLayout();
}

void TestlerWidget::setupLayout() {
    mainLayout = new QHBoxLayout(this);
    splitter = new QSplitter(Qt::Horizontal, this);
    
    // Sol panel - Test kısmı
    leftPanel = new TabTestBaslat(this);
    leftPanel->setMinimumWidth(600);
    
    // Sağ panel - Sanal Sayaç kısmı
    rightPanel = new VirtualMeterPanel(this);
    rightPanel->setMinimumWidth(600);
    
    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    splitter->setSizes({1, 1}); // Eşit bölme
    
    mainLayout->addWidget(splitter);
    setLayout(mainLayout);
    
    setWindowTitle("Test ve Sanal Sayaç Paneli");
    resize(1400, 800);
}

// ============================================================================
// SOL PANEL - TEST KISMI (2. koddan tamamen alındı)
// ============================================================================

TabTestBaslat::TabTestBaslat(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // PORT COMBO + TARA BUTONU
    QHBoxLayout *portLayout = new QHBoxLayout();
    QLabel *portLabel = new QLabel("Seri Port:");
    portCombo = new QComboBox();
    portCombo->setFixedWidth(200);
    QPushButton *btnPortTara = new QPushButton("Port Tara");
    btnPortTara->setFixedWidth(100);

    auto portlariListele = [this]() {
        QStringList portList;
        QDir devDir("/dev");
        QStringList filters = {"ttyUSB*", "ttyACM*"};
        for (const QString &filter : filters) {
            QStringList found = devDir.entryList(QStringList(filter), QDir::System | QDir::NoSymLinks);
            for (const QString &dev : found)
                portList << "/dev/" + dev;
        }
        portCombo->clear();
        portCombo->addItems(portList);
    };
    portlariListele();
    connect(btnPortTara, &QPushButton::clicked, this, portlariListele);

    portLayout->addStretch();
    portLayout->addWidget(portLabel);
    portLayout->addWidget(portCombo);
    portLayout->addWidget(btnPortTara);
    portLayout->addStretch();
    layout->addLayout(portLayout);

    // SERİ NO GİRİŞİ
    QHBoxLayout *seriKaydetLayout = new QHBoxLayout();
    QLabel *seriLabel = new QLabel("Seri No:");
    seriNoEdit = new QLineEdit();
    seriNoEdit->setFixedWidth(200);
    seriKaydetLayout->addStretch();
    seriKaydetLayout->addWidget(seriLabel);
    seriKaydetLayout->addWidget(seriNoEdit);
    seriKaydetLayout->addStretch();
    layout->addLayout(seriKaydetLayout);

    // RSSI Aralık Ayarı
    QHBoxLayout *rssiRangeLayout = new QHBoxLayout();
    QLabel *maxLabel = new QLabel("RSSI Max:");
    maxRssiCombo = new QComboBox();
    QLabel *minLabel = new QLabel("RSSI Min:");
    minRssiCombo = new QComboBox();
    
    for (int i = -120; i <= -50; i += 5)
        maxRssiCombo->addItem(QString::number(i));
    for (int i = -45; i <= 0; i += 5)
        minRssiCombo->addItem(QString::number(i));
    
    minRssiCombo->setCurrentText("-20");
    maxRssiCombo->setCurrentText("-50");

    rssiRangeLayout->addStretch();
    rssiRangeLayout->addWidget(minLabel);
    rssiRangeLayout->addWidget(minRssiCombo);
    rssiRangeLayout->addSpacing(20);
    rssiRangeLayout->addWidget(maxLabel);
    rssiRangeLayout->addWidget(maxRssiCombo);
    rssiRangeLayout->addStretch();
    layout->addLayout(rssiRangeLayout);

    // BUTONLAR
    QHBoxLayout *testBtnLayout = new QHBoxLayout();
    btnBaslat = new QPushButton("Testleri Başlat");
    btnDurdur = new QPushButton("Testleri Durdur");
    btnTestEkraniEkle = new QPushButton("Test Ekranı Ekle");
    
    testBtnLayout->addStretch();
    testBtnLayout->addWidget(btnBaslat);
    testBtnLayout->addSpacing(20);
    testBtnLayout->addWidget(btnDurdur);
    testBtnLayout->addSpacing(20);
    testBtnLayout->addWidget(btnTestEkraniEkle);
    testBtnLayout->addStretch();
    layout->addLayout(testBtnLayout);

    connect(btnBaslat, &QPushButton::clicked, this, &TabTestBaslat::baslatTest);
    connect(btnDurdur, &QPushButton::clicked, this, &TabTestBaslat::durdurTest);
    connect(btnTestEkraniEkle, &QPushButton::clicked, this, &TabTestBaslat::testEkraniBaslat);

    // DURUM ETİKETLERİ
    statusLabel = new QLabel("Durum: Bekleniyor...");
    statusLabel->setFont(QFont("Arial", 12));
    statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(statusLabel);

    eepromLabel = new QLabel("EEPROM Testi: ❌");
    rfLabel = new QLabel("RF Testi: ❌");
    networkLabel = new QLabel("Ağ Kalitesi Testi: ❌");
    rssiLabel = new QLabel("RSSI: -");
    genelSonucLabel = new QLabel("GENEL SONUÇ: FAIL");

    QFont labelFont("Arial", 13);
    for (QLabel *lbl : {eepromLabel, rfLabel, networkLabel, rssiLabel}) {
        lbl->setFont(labelFont);
        lbl->setAlignment(Qt::AlignCenter);
    }

    genelSonucLabel->setFont(QFont("Segoe UI Emoji", 14, QFont::Bold));
    genelSonucLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(eepromLabel);
    layout->addWidget(rfLabel);
    layout->addWidget(networkLabel);
    layout->addWidget(rssiLabel);
    layout->addWidget(genelSonucLabel);

    logEdit = new QPlainTextEdit();
    logEdit->setFont(QFont("Arial", 10));
    logEdit->setReadOnly(true);
    layout->addWidget(logEdit);

    port = new QSerialPort(this);
    connect(port, &QSerialPort::readyRead, this, &TabTestBaslat::seriVeriOku);

    // QProcess nesnelerini başlat
    veritabaniProcess = new QProcess(this);
    emuProcess = new QProcess(this);
    testProcess = new QProcess(this);
    
    // Test değişkenlerini başlat
    eeprom_ok = rf_ok = network_ok = false;
    blinkTimer = nullptr;
    blinkCount = 0;
    blinkVisible = true;
}

// DÜZELTME: Sadece yeni bir uygulama başlatacak şekilde düzenlendi
void TabTestBaslat::testEkraniBaslat() {
    // Mevcut çalışma dizinini al
    QString currentDir = QDir::currentPath();
    QString appPath = currentDir + "/tester";

    // Dosya varlığını kontrol et
    if (!QFile::exists(appPath)) {
        logEdit->appendPlainText(">> HATA: veritabani uygulaması bulunamadı!");
        logEdit->appendPlainText(">> Aranan dosya: " + appPath);
        return;
    }

    // Uygulamayı BAĞIMSIZ (DETACHED) olarak başlat
    // Bu metot, işlem başarılı bir şekilde başladıysa true, değilse false döner.
    // Artık bir QProcess nesnesi oluşturmamıza ve onu yönetmemize gerek yok.
    if (QProcess::startDetached(appPath, QStringList())) {
        logEdit->appendPlainText(">> Yeni uygulama örneği bağımsız olarak başlatıldı.");
        qDebug() << "Bağımsız uygulama başlatıldı:" << appPath;
    } else {
        logEdit->appendPlainText(">> HATA: Uygulama başlatılamadı!");
        // startDetached daha az hata detayı verir, bu yüzden genel bir mesaj gösteriyoruz.
        qDebug() << "Bağımsız uygulama başlatma hatası oluştu.";
    }
}

void TabTestBaslat::baslatTest() {
    QString seri_no = seriNoEdit->text().trimmed();
    QString selectedPort = portCombo->currentText();

    if (selectedPort.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen bir seri port seçin!");
        return;
    }
    

    // RSSI alanlarını devre dışı bırak
    minRssiCombo->setEnabled(false);
    maxRssiCombo->setEnabled(false);

    statusLabel->setText("Durum: Port açılıyor...");

    port->setPortName(selectedPort);
    port->setBaudRate(921600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);

    if (!port->open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Hata", "Seri port açılamadı:\n" + port->errorString());

        // Açılmadıysa geri aktif et
        minRssiCombo->setEnabled(true);
        maxRssiCombo->setEnabled(true);
        return;
    }

    statusLabel->setText("Durum: Dinleniyor...");
    logEdit->appendPlainText(">> Port açıldı, veri bekleniyor...");

    eeprom_ok = rf_ok = network_ok = false;
    rssiLabel->setText("RSSI: -");
    rssiLabel->setStyleSheet("");
    logLines.clear();

    durBlink();
    blinkCount = 0;
    guncelleGorunum();
}

void TabTestBaslat::durdurTest() {
    if (port && port->isOpen()) {
        port->close();
        logEdit->appendPlainText(">> Port kapatıldı.");
        statusLabel->setText("Durum: Test durduruldu.");
    }

    // RSSI comboBox'larını tekrar aktif et
    minRssiCombo->setEnabled(true);
    maxRssiCombo->setEnabled(true);
}

void TabTestBaslat::seriVeriOku() {
    QString seri_no = seriNoEdit->text().trimmed();
    

    // Yeni veri geldiğinde buffer'a ekle
    readBuffer.append(port->readAll());

    // Satır satır ayır
    while (readBuffer.contains('\n')) {
        int endOfLine = readBuffer.indexOf('\n');
        QByteArray lineData = readBuffer.left(endOfLine + 1);  // '\n' dahil
        readBuffer.remove(0, endOfLine + 1); // Okunan kısmı buffer'dan çıkar

        QString text = QString::fromUtf8(lineData).trimmed();
        logEdit->appendPlainText(">> " + text);
        logLines.append(text);

        
        if (text.contains("Link-layer address:", Qt::CaseInsensitive)) {
        QString serialfound = text.right(9); 
        QString cleanedSerial = serialfound.remove(".");

        seriNoEdit->setText(cleanedSerial);

        // Seri numarası gelince veritabanına INSERT yap
        QSqlQuery insert;
        insert.prepare("INSERT INTO modem_testleri (seri_no, eeprom, rf, network, result, aciklama) "
                       "VALUES (:seri_no, 0, 0, 0, :result, '') "
                       "ON CONFLICT(seri_no) DO UPDATE SET "
                       "eeprom = 0, rf = 0, network = 0, result = :result2, aciklama = ''");
        insert.bindValue(":seri_no", cleanedSerial);
        insert.bindValue(":result", "FAIL");
        insert.bindValue(":result2", "FAIL");
        insert.exec();
        }
        QSqlQuery update;  
        if (text.contains("Flash open success", Qt::CaseInsensitive)) {
            eeprom_ok = true;
            update.prepare("UPDATE modem_testleri SET eeprom = 1 WHERE seri_no = :seri_no");
        } else if (text.contains("Flash open fail", Qt::CaseInsensitive)) {
            eeprom_ok = false;
            update.prepare("UPDATE modem_testleri SET eeprom = 0 WHERE seri_no = :seri_no");
        }

        if (!update.lastQuery().isEmpty()) {
            update.bindValue(":seri_no", seri_no);
            update.exec();
        }

        if (text.contains("rssip", Qt::CaseInsensitive)) {
            QRegularExpression re("rssip\\s*(-?\\d+)");
            QRegularExpressionMatch match = re.match(text);
            if (match.hasMatch()) {
                int rssi = match.captured(1).toInt();
                guncelleRssi(rssi);

                int minRssi = minRssiCombo->currentText().toInt();
                int maxRssi = maxRssiCombo->currentText().toInt();

                if (rssi >= maxRssi && rssi <= minRssi) {
                    network_ok = true;
                    QSqlQuery q;
                    q.prepare("UPDATE modem_testleri SET network = 1 WHERE seri_no = :seri_no");
                    q.bindValue(":seri_no", seri_no);
                    q.exec();
                }
            }
        }

        guncelleGorunum();

        QSqlQuery q;
        q.prepare("UPDATE modem_testleri SET result = :res WHERE seri_no = :seri_no");
        q.bindValue(":res", (eeprom_ok && rf_ok && network_ok) ? "PASS" : "FAIL");
        q.bindValue(":seri_no", seri_no);
        q.exec();
    }
}

void TabTestBaslat::guncelleRssi(int rssi) {
    rssiLabel->setText(QString("RSSI: %1 dBm").arg(rssi));
    rssiLabel->setStyleSheet(rssi >= -50
        ? "QLabel { background-color: lightgreen; font-weight: bold; }"
        : "QLabel { background-color: orange; font-weight: bold; }");
}

void TabTestBaslat::guncelleGorunum() {
    QFont tickFont("Segoe UI Emoji", 13);
    rf_ok = RFTesti(logLines);

    eepromLabel->setText(QString("EEPROM Testi: %1").arg(eeprom_ok ? "✅" : "❌"));
    rfLabel->setText(QString("RF Testi: %1").arg(rf_ok ? "✅" : "❌"));
    networkLabel->setText(QString("Ağ Kalitesi Testi: %1").arg(network_ok ? "✅" : "❌"));

    for (QLabel *lbl : {eepromLabel, rfLabel, networkLabel})
        lbl->setFont(tickFont);

    bool pass = eeprom_ok && rf_ok && network_ok;
    genelSonucLabel->setText(pass ? "GENEL SONUÇ: PASS" : "GENEL SONUÇ: FAIL");
    genelSonucLabel->setStyleSheet(pass
        ? "QLabel { color: green; font-weight: bold; }"
        : "QLabel { color: red; font-weight: bold; }");
    
    if (rf_ok) {
        QString seri_no = seriNoEdit->text().trimmed();
        QSqlQuery q;
        q.prepare("UPDATE modem_testleri SET rf = 1 WHERE seri_no = :seri_no");
        q.bindValue(":seri_no", seri_no);
        q.exec();
    }
    
    baslatBlink();
}

void TabTestBaslat::baslatBlink() {
    durBlink();
    blinkTimer = new QTimer(this);
    blinkCount = 0;
    blinkVisible = true;

    connect(blinkTimer, &QTimer::timeout, this, [=]() mutable {
        blinkCount++;
        if (blinkCount >= 6) {
            durBlink();
            eepromLabel->setStyleSheet(eeprom_ok ? "background-color: lightgreen;" : "background-color: red; color: white;");
            rfLabel->setStyleSheet(rf_ok ? "background-color: lightgreen;" : "background-color: red; color: white;");
            networkLabel->setStyleSheet(network_ok ? "background-color: lightgreen;" : "background-color: red; color: white;");
            return;
        }

        QString yesil = "background-color: lightgreen; font-weight: bold;";
        QString kirmizi = "background-color: red; color: white; font-weight: bold;";
        eepromLabel->setStyleSheet(blinkVisible ? (eeprom_ok ? yesil : kirmizi) : "");
        rfLabel->setStyleSheet(blinkVisible ? (rf_ok ? yesil : kirmizi) : "");
        networkLabel->setStyleSheet(blinkVisible ? (network_ok ? yesil : kirmizi) : "");
        blinkVisible = !blinkVisible;
    });

    blinkTimer->start(300);
}

void TabTestBaslat::durBlink() {
    if (blinkTimer) {
        blinkTimer->stop();
        delete blinkTimer;
        blinkTimer = nullptr;
    }
}

bool TabTestBaslat::RFTesti(const QStringList &logs) {
    int txOkCount = 0, macCreatedCount = 0, rssiCount = 0;
    for (const QString &line : logs) {
        QString lower = line.toLower();
        if (lower.contains("tx ok")) txOkCount++;
        if (lower.contains("4emac:ka is created")) macCreatedCount++;
        if (lower.contains("rssip")) rssiCount++;
    }
    return txOkCount >= 3 && macCreatedCount >= 2 && rssiCount >= 2;
}

// ============================================================================
// SAĞ PANEL - SANAL SAYAÇ KISMI (TOML yolları sayac klasörü için düzeltildi)
// ============================================================================

VirtualMeterPanel::VirtualMeterPanel(QWidget *parent) : QWidget(parent) {
    serialPort = new QSerialPort(this);
    connectionTimer = new QTimer(this);
    switchedTo9600 = false;
    activeMeterID = 0;
    stopThread = false;
    
    meter1Flag = meter2Flag = meter3Flag = meter4Flag = false;
    
    setupUI();
    loadMeterConfigs(); // TOML dosyaları sayac klasöründen yüklenecek
    refreshPorts();
    
    connect(serialPort, &QSerialPort::readyRead, this, &VirtualMeterPanel::onSerialDataReceived);
    connect(connectionTimer, &QTimer::timeout, this, &VirtualMeterPanel::updateTimer);
}

VirtualMeterPanel::~VirtualMeterPanel() {
    if (serialPort->isOpen()) {
        serialPort->close();
    }
}

void VirtualMeterPanel::setupUI() {
    mainLayout = new QVBoxLayout(this);
    
    // Port Control Group
    portControlGroup = new QGroupBox("Port Kontrolü", this);
    QHBoxLayout *portLayout = new QHBoxLayout(portControlGroup);
    
    portLayout->addWidget(new QLabel("Port:"));
    portComboBox = new QComboBox();
    portLayout->addWidget(portComboBox);
    
    connectBtn = new QPushButton("Bağlan");
    disconnectBtn = new QPushButton("Kes");
    refreshBtn = new QPushButton("Yenile");
    
    disconnectBtn->setEnabled(false);
    
    portLayout->addWidget(connectBtn);
    portLayout->addWidget(disconnectBtn);
    portLayout->addWidget(refreshBtn);
    
    mainLayout->addWidget(portControlGroup);
    
    // Timer Display
    timerLabel = new QLabel("00:00.000");
    timerLabel->setFont(QFont("Arial", 20, QFont::Bold));
    timerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(timerLabel);
    
    // Log Frame
    QGroupBox *logGroup = new QGroupBox("İletişim Kayıtları", this);
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);
    
    logTextEdit = new QTextEdit();
    logTextEdit->setReadOnly(true);
    logTextEdit->setFont(QFont("Arial", 9));
    logLayout->addWidget(logTextEdit);
    
    mainLayout->addWidget(logGroup);
    
    // Connect signals
    connect(connectBtn, &QPushButton::clicked, this, &VirtualMeterPanel::connectPort);
    connect(disconnectBtn, &QPushButton::clicked, this, &VirtualMeterPanel::disconnectPort);
    connect(refreshBtn, &QPushButton::clicked, this, &VirtualMeterPanel::refreshPorts);
}

// DÜZELTME: TOML dosyalarını sayac klasöründen yükleyecek şekilde değiştirildi
void VirtualMeterPanel::loadMeterConfigs() {
    // Mevcut çalışma dizinini al ve sayac klasörü yolunu oluştur
    QString currentDir = QDir::currentPath();
    QString configDir = currentDir;
    
    // sayac klasörünün varlığını kontrol et
    QDir sayacDir(configDir);
    if (!sayacDir.exists()) {
        logMessage(QString("❌ sayac klasörü bulunamadı: %1").arg(configDir), "red");
        logMessage("⚠️ TOML dosyaları yüklenemeyecek!", "red");
        return;
    }
    
    logMessage(QString("📁 TOML dosyaları aranıyor: %1").arg(configDir), "blue");
    
    // Meter konfigürasyonları - sayac klasöründen yükle
    meters[1] = {configDir + "/meter1.toml", "", "", {}};
    meters[2] = {configDir + "/meter2.toml", "", "", {}};
    meters[3] = {configDir + "/meter3.toml", "", "", {}};
    meters[4] = {configDir + "/meter4.toml", "", "", {}};
    
    // TOML dosyalarını oku
    for (auto it = meters.begin(); it != meters.end(); ++it) {
        int meterNum = it.key();
        QString configFile = it.value().configFile;
        
        QFile file(configFile);
        if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            QString content = stream.readAll();
            file.close();
            
            // TOML dosyasını parse et
            parseTOMLFile(content, meterNum);
        } else {
            logMessage(QString("⚠️ %1 dosyası bulunamadı").arg(QFileInfo(configFile).fileName()), "red");
        }
    }
}

void VirtualMeterPanel::refreshPorts() {
    QStringList portList;
    QDir devDir("/dev");

    // Sadece USB ve ACM portlarını ara
    QStringList filters = {"ttyUSB*", "ttyACM*"};
    for (const QString &filter : filters) {
        QStringList found = devDir.entryList(QStringList(filter), QDir::System | QDir::NoSymLinks);
        for (const QString &dev : found)
            portList << "/dev/" + dev;
    }

    portComboBox->clear();

    if (!portList.isEmpty()) {
        portComboBox->addItems(portList);
        logMessage(QString("Aktif portlar: %1").arg(portList.join(", ")), "blue");
    } else {
        logMessage("Kullanılabilir USB/ACM port bulunamadı", "red");
    }
}


void VirtualMeterPanel::connectPort() {
    QString portName = portComboBox->currentText();
    if (portName.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen bir port seçin.");
        return;
    }
    
    serialPort->setPortName(portName);
    serialPort->setBaudRate(300);
    serialPort->setDataBits(QSerialPort::Data7);
    serialPort->setParity(QSerialPort::EvenParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    
    if (serialPort->open(QIODevice::ReadWrite)) {
        connectBtn->setEnabled(false);
        disconnectBtn->setEnabled(true);
        
        buffer.clear();
        switchedTo9600 = false;
        activeMeterID = 0;
        lastAddressedSerialNumber.clear();
        startTime = QDateTime::currentDateTime();
        connectionTimer->start(100);
        
        logMessage(QString("Bağlantı kuruldu: %1 @ 300bps - Sürekli dinleme aktif").arg(portName), "green");
    } else {
        QMessageBox::critical(this, "Hata", "Porta bağlanılamadı: " + serialPort->errorString());
    }
}

void VirtualMeterPanel::disconnectPort() {
    if (serialPort->isOpen()) {
        serialPort->close();
        logMessage("Bağlantı kesildi.", "red");
    }
    
    connectBtn->setEnabled(true);
    disconnectBtn->setEnabled(false);
    switchedTo9600 = false;
    activeMeterID = 0;
    connectionTimer->stop();
    timerLabel->setText("00:00.000");
}

void VirtualMeterPanel::updateTimer() {
    if (!startTime.isValid()) return;
    
    qint64 elapsed = startTime.msecsTo(QDateTime::currentDateTime());
    int minutes = (elapsed / 60000) % 60;
    int seconds = (elapsed / 1000) % 60;
    int msec = elapsed % 1000;
    
    timerLabel->setText(QString("%1:%2.%3")
                       .arg(minutes, 2, 10, QChar('0'))
                       .arg(seconds, 2, 10, QChar('0'))
                       .arg(msec, 3, 10, QChar('0')));
}

void VirtualMeterPanel::onSerialDataReceived() {
    buffer.append(serialPort->readAll());
    processBuffer();
}

void VirtualMeterPanel::processBuffer() {
    while (buffer.contains('\n')) {
        int lineEnd = buffer.indexOf('\n');
        QByteArray completeLine = buffer.left(lineEnd + 1);
        buffer.remove(0, lineEnd + 1);
        handleIncomingData(completeLine);
    }
}

void VirtualMeterPanel::handleIncomingData(const QByteArray &data) {
    if (data.isEmpty()) return;  // Boş veri kontrolü
    
    QString hexStr = data.toHex(' ').toUpper();
    QString asciiStr;
    
    // Güvenli karakter dönüşümü
    for (int i = 0; i < data.size(); i++) {
        char c = data.at(i);
        asciiStr += (c >= 32 && c <= 126) ? QString(QChar(c)) : ".";
    }

    logMessage(QString("Gelen (HEX): %1").arg(hexStr), "black");
    logMessage(QString("Gelen (ASCII): %1").arg(asciiStr), "black");

    if (!switchedTo9600) {
        // GENEL SORGU
        if (data.trimmed() == "/?!") {
            logMessage("Genel sorgu (/?!) alındı.", "green");
            lastAddressedSerialNumber.clear(); // Genel sorgu gelince adresli beklentisini iptal et
            activeMeterID = 0;

            QByteArray response = "/MSY5MAVIALPV2\r\n";
            if (serialPort && serialPort->isOpen()) {
                serialPort->write(response);
                logMessage(QString("Kimlik cevabı gönderildi: %1")
                               .arg(QString::fromLatin1(response.trimmed())),
                           "green");
            }
            return;
        }

        // ADRESLİ SORGU
        if (data.startsWith("/?") && data.endsWith("!\r\n") && data.length() > 5) {
            QString serialNumber = QString::fromLatin1(data.mid(2, data.length() - 5));
            logMessage(QString("Adresli sorgu: %1").arg(serialNumber), "green");

            // DEĞİŞİKLİK 1: Sayacı bul ama henüz tam olarak aktif etme. Sadece kimlik gönder.
            if (findMeterBySerialNumber(serialNumber)) {
                logMessage(QString("Sayaç %1 bulundu, kimlik gönderiliyor...").arg(activeMeterID), "blue");
                sendIdentResponse(); // Kimlik cevabını gönder
                lastAddressedSerialNumber = serialNumber; // Seri numarasını ACK için sakla
                activeMeterID = 0; // activeMeterID'yi sıfırla ki genel sorgu ACK ile karışmasın
            } else {
                logMessage(QString("Tanımsız sayaç ID'si: %1").arg(serialNumber), "red");
                lastAddressedSerialNumber.clear();
            }
            return;
        }

        // GENEL SORGU ACK056 (Bu blok değişmedi)
        else if (data.contains('\x06') && (data.contains("056") || data.contains("050")) && activeMeterID == 0 && lastAddressedSerialNumber.isEmpty()) {
            logMessage("Genel sorgu ACK056 alındı, hız değiştiriliyor...", "green");
            
            if (serialPort && serialPort->isOpen()) {
                serialPort->close();
                QThread::msleep(200); // Not: Bu hala arayüzü kısa süreliğine bloklar.

                serialPort->setBaudRate(9600);
                if (serialPort->open(QIODevice::ReadWrite)) {
                    switchedTo9600 = true;
                    // sırayla sayaç cevabı
                    if (!meter1Flag) {
                        activeMeterID = 1; meter1Flag = true;
                        sendDataList();
                    } else if (!meter2Flag) {
                        activeMeterID = 2; meter2Flag = true;
                        sendDataList();
                    } else if (!meter3Flag) {
                        activeMeterID = 3; meter3Flag = true;
                        sendDataList();
                    } else if (!meter4Flag) {
                        activeMeterID = 4; meter4Flag = true;
                        sendDataList();
                    } else {
                        logMessage("Tüm sayaçlar cevap verdi, artık cevap yok.", "red");
                    }
                } else {
                    logMessage("Baudrate değiştirilemedi", "red");
                }
            }
            return;
        }

        // DEĞİŞİKLİK 2: ADRESLİ SORGU İÇİN ACK KONTROLÜ
        else if (data.contains('\x06') && (data.contains("056") || data.contains("050")) && !lastAddressedSerialNumber.isEmpty()) {
            // Saklanan seri numarasından sayacı bul ve ŞİMDİ aktif et
            findMeterBySerialNumber(lastAddressedSerialNumber);
            
            logMessage(QString("Adresli ACK056 alındı (Sayaç %1), hız değiştiriliyor...").arg(activeMeterID), "green");
            
            if (serialPort && serialPort->isOpen()) {
                serialPort->close();
                QThread::msleep(200); // Not: Bu hala arayüzü kısa süreliğine bloklar.

                serialPort->setBaudRate(9600);
                if (serialPort->open(QIODevice::ReadWrite)) {
                    switchedTo9600 = true;
                    logMessage(QString("Aktif sayaç %1 için veri gönderilecek.").arg(activeMeterID), "blue");
                    sendDataList();
                } else {
                    logMessage("Baudrate değiştirilemedi", "red");
                }
            }
            lastAddressedSerialNumber.clear(); // İşlem bitti, saklanan seri numarasını temizle
            return;
        }
    } else {
        // 9600 baud modunda gelen diğer veriler
        logMessage("9600 baud'da veri alındı - Dinleme devam ediyor", "blue");
    }
}

bool VirtualMeterPanel::findMeterBySerialNumber(const QString &serialNumber) {
    for (auto it = meters.begin(); it != meters.end(); ++it) {
        if (it.value().id == serialNumber) {
            activeMeterID = it.key();
            return true;
        }
    }
    activeMeterID = 0;
    return false;
}

void VirtualMeterPanel::sendIdentResponse() {
    if (activeMeterID == 0) return;
    
    try {
        QString response = meters[activeMeterID].response;
        if (response.isEmpty()) response = "/UNKNOWN";
        
        QByteArray responseBytes = (response + "\r\n").toLatin1();
        serialPort->write(responseBytes);
        logMessage(QString("📤 Kimlik cevabı gönderildi: %1").arg(response), "green");
        logMessage("🔄 Kimlik gönderildi - Dinleme devam ediyor...", "blue");
    } catch (...) {
        logMessage("❌ Kimlik cevabı gönderme hatası", "red");
    }
}

QByteArray VirtualMeterPanel::calculateBCC(const QByteArray &data) {
    char bcc = 0;
    for (char byte : data) {
        bcc ^= byte;
    }
    return QByteArray(1, bcc);
}

void VirtualMeterPanel::sendDataList() {
    if (activeMeterID == 0) {
        logMessage("❌ Veri gönderilecek aktif sayaç yok!", "red");
        return;
    }
    
    QThread::msleep(100);
    
    try {
        QStringList dataList = meters[activeMeterID].dataList;
        
        if (dataList.isEmpty()) {
            logMessage(QString("⚠️ Sayaç %1 için data_list bulunamadı!").arg(activeMeterID), "red");
            return;
        }
        
        QString dataString = dataList.join("");
        QByteArray dataBlock = dataString.toLatin1();

        QByteArray payload = dataBlock + "!\r\n\x03";
        QByteArray bcc = calculateBCC(payload);
        QByteArray fullMessage = "\x02" + payload + bcc;
        serialPort->write(fullMessage);
        serialPort->flush();
        
        logMessage(QString("✅ Sayaç %1 veri listesi başarıyla gönderildi").arg(activeMeterID), "blue");
        logMessage(QString("  📤 Gönderilen HEX: %1").arg(QString::fromLatin1(fullMessage.toHex(' '))), "green");
        
        QString asciiStr;
        for (char c : fullMessage) {
            asciiStr += (c >= 32 && c <= 126) ? QString(c) : ".";
        }
        logMessage(QString("  📤 Gönderilen ASCII: %1").arg(asciiStr), "green");
        
        logMessage("🔄 Veri gönderildi - Yeni sorgular için dinleme devam ediyor...", "blue");
        
    } catch (...) {
        logMessage("❌ Veri gönderilemedi!", "red");
    }
    
    // Port'u tekrar 300 baud'a döndür
     QTimer::singleShot(2000, this, &VirtualMeterPanel::resetToDefaultBaud);
}

// TOML Dosya Parse Fonksiyonu
// TOML Dosya Parse Fonksiyonu - DÜZELTİLMİŞ VERSİYON
void VirtualMeterPanel::parseTOMLFile(const QString &content, int meterNum) {
    QStringList lines = content.split('\n');
    bool inDataSection = false;
    QString currentSection = "";
    
    for (const QString &line : lines) {
        QString trimmedLine = line.trimmed();
        
        // Boş satırları ve yorumları atla
        if (trimmedLine.isEmpty() || trimmedLine.startsWith('#')) {
            continue;
        }
        
        // Section başlıklarını kontrol et
        if (trimmedLine.startsWith('[') && trimmedLine.endsWith(']')) {
            currentSection = trimmedLine.mid(1, trimmedLine.length() - 2);
            inDataSection = (currentSection == "data");
            logMessage(QString("📂 Section bulundu: %1").arg(currentSection), "blue");
            continue;
        }
        
        // Key-value çiftlerini parse et
        if (trimmedLine.contains('=')) {
            QStringList parts = trimmedLine.split('=', Qt::KeepEmptyParts);
            if (parts.size() >= 2) {
                QString key = parts[0].trimmed();
                QString value = parts[1].trimmed();
                
                // Meter bilgilerini ata ([meter] section'ında)
                if (currentSection == "meter" || currentSection.isEmpty()) {
                    // Tırnak işaretlerini kaldır
                    if (value.startsWith('"') && value.endsWith('"')) {
                        value = value.mid(1, value.length() - 2);
                    }
                    
                    if (key == "id") {
                        meters[meterNum].id = value;
                        logMessage(QString("📋 Sayaç %1 ID: %2").arg(meterNum).arg(value), "blue");
                    } 
                    else if (key == "response") {
                        meters[meterNum].response = value;
                        logMessage(QString("📋 Sayaç %1 Response: %2").arg(meterNum).arg(value), "blue");
                    }
                }
                // Data list ([data] section'ında)
                else if (inDataSection && key == "data_list") {
                    logMessage(QString("🔍 data_list anahtarı bulundu, parse ediliyor..."), "blue");
                    
                    // Çok satırlı array kontrolü
                    if (value.startsWith('[')) {
                        QString fullArrayContent = value;
                        
                        // Eğer aynı satırda kapanmazsa, devam eden satırları topla
                        if (!value.endsWith(']')) {
                            // Sonraki satırları okumaya devam et
                            int currentLineIndex = lines.indexOf(line);
                            for (int i = currentLineIndex + 1; i < lines.size(); i++) {
                                QString nextLine = lines[i].trimmed();
                                fullArrayContent += " " + nextLine;
                                if (nextLine.contains(']')) {
                                    break;
                                }
                            }
                        }
                        
                        // Array içeriğini çıkar
                        int startBracket = fullArrayContent.indexOf('[');
                        int endBracket = fullArrayContent.lastIndexOf(']');
                        
                        if (startBracket != -1 && endBracket != -1 && endBracket > startBracket) {
                            QString arrayContent = fullArrayContent.mid(startBracket + 1, endBracket - startBracket - 1);
                            logMessage(QString("📝 Array içeriği: %1").arg(arrayContent.left(100) + "..."), "blue");
                            
                            QStringList dataItems;
                            QString currentItem;
                            bool insideQuotes = false;
                            
                            for (int i = 0; i < arrayContent.length(); i++) {
                                QChar ch = arrayContent.at(i);
                                
                                if (ch == '"' && (i == 0 || arrayContent.at(i-1) != '\\')) {
                                    insideQuotes = !insideQuotes;
                                    currentItem.append(ch);
                                } else if (ch == ',' && !insideQuotes) {
                                    // Virgül bulundu ve tırnak içinde değiliz
                                    QString item = currentItem.trimmed();
                                    if (item.startsWith('"') && item.endsWith('"')) {
                                        item = item.mid(1, item.length() - 2);
                                    }
                                    if (!item.isEmpty()) {
                                        dataItems.append(item);
                                    }
                                    currentItem.clear();
                                } else {
                                    currentItem.append(ch);
                                }
                            }
                            
                            // Son öğeyi ekle
                            if (!currentItem.isEmpty()) {
                                QString item = currentItem.trimmed();
                                if (item.startsWith('"') && item.endsWith('"')) {
                                    item = item.mid(1, item.length() - 2);
                                }
                                if (!item.isEmpty()) {
                                    dataItems.append(item);
                                }
                            }
                            
                            meters[meterNum].dataList = dataItems;
                            logMessage(QString("✅ Sayaç %1 Data List yüklendi (%2 öğe)")
                                      .arg(meterNum)
                                      .arg(dataItems.size()), "green");
                            
                            // İlk birkaç öğeyi göster
                            for (int i = 0; i < qMin(3, dataItems.size()); i++) {
                                logMessage(QString("  [%1]: %2").arg(i+1).arg(dataItems[i]), "blue");
                            }
                            if (dataItems.size() > 3) {
                                logMessage(QString("  ... ve %1 öğe daha").arg(dataItems.size() - 3), "blue");
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Yükleme sonucu kontrolü
    if (meters[meterNum].id.isEmpty()) {
        logMessage(QString("⚠️ %1 - ID bulunamadı!")
                  .arg(QFileInfo(meters[meterNum].configFile).fileName()), "red");
    }
    
    if (meters[meterNum].dataList.isEmpty()) {
        logMessage(QString("⚠️ %1 - data_list bulunamadı!")
                  .arg(QFileInfo(meters[meterNum].configFile).fileName()), "red");
    }
    
    logMessage(QString("✅ %1 yapılandırması yüklendi - ID: %2, Data List: %3 öğe")
               .arg(QFileInfo(meters[meterNum].configFile).fileName())
               .arg(meters[meterNum].id)
               .arg(meters[meterNum].dataList.size()), "green");
}

void VirtualMeterPanel::resetToDefaultBaud() {
    if (serialPort && serialPort->isOpen()) {
        serialPort->close();
        // msleep kaldırıldı, bekleme QTimer ile yapıldı
        serialPort->setBaudRate(300);
        if (!serialPort->open(QIODevice::ReadWrite)) {
            logMessage("Port 300 baud'da açılamadı", "red");
        }
    }
    switchedTo9600 = false;
}
void VirtualMeterPanel::logMessage(const QString &message, const QString &color) {
    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString("hh:mm:ss.zzz");
    
    QString formattedMessage = QString("[%1] %2").arg(timestamp, message);
    
    // HTML formatında renk ekleme
    QString coloredMessage;
    if (color == "green") {
        coloredMessage = QString("<font color='green'>%1</font>").arg(formattedMessage);
    } else if (color == "red") {
        coloredMessage = QString("<font color='red'>%1</font>").arg(formattedMessage);
    } else if (color == "blue") {
        coloredMessage = QString("<font color='blue'>%1</font>").arg(formattedMessage);
    } else if (color == "black") {
        coloredMessage = QString("<font color='black'>%1</font>").arg(formattedMessage);
    } else {
        coloredMessage = formattedMessage;
    }
    
    logTextEdit->append(coloredMessage);
    
    // Scroll to bottom
    QTextCursor cursor = logTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    logTextEdit->setTextCursor(cursor);
}

// ============================================================================
// SERIAL READER THREAD
// ============================================================================

SerialReaderThread::SerialReaderThread(QSerialPort *port, QObject *parent)
    : QThread(parent), serialPort(port), stopRequested(false) {
}

void SerialReaderThread::stop() {
    QMutexLocker locker(&mutex);
    stopRequested = true;
}

// Düzeltilmiş ve doğru arabelleklemeyi yapan run() metodu
void SerialReaderThread::run() {
    // Bu arabelleğin (m_buffer), okumalar arasında veriyi saklayabilmesi için
    // sınıfın bir üye değişkeni olması gerekir. Kod bu varsayıma göre çalışır.
    
    while (true) {
        {
            QMutexLocker locker(&mutex);
            if (stopRequested) {
                break; // Durdurma isteği geldiyse döngüden çık
            }
        }

        // Yeni veri gelmesini bekle
        if (serialPort && serialPort->isOpen() && serialPort->waitForReadyRead(100)) {
            // Gelen yeni veriyi kalıcı arabelleğimize ekle
            m_buffer.append(serialPort->readAll());

            // Arabellekte tam bir satır (yani '\n' karakteri) olduğu sürece işlem yap
            while (m_buffer.contains('\n')) {
                int endOfLine = m_buffer.indexOf('\n');
                
                // Yeni satır karakteri dahil olmak üzere tam satırı çıkar
                QByteArray completeLine = m_buffer.left(endOfLine + 1);
                
                // İşlenen bu satırı arabelleğin başından kaldır
                m_buffer.remove(0, endOfLine + 1);

                // Sadece tam ve işlenmiş olan bu satırı yayınla (emit)
                emit dataReceived(completeLine);
            }
        } else {
            // Veri gelmediyse veya port kapalıysa CPU'yu %100 kullanmamak için kısa bir süre bekle.
            msleep(10);
        }
    }
}