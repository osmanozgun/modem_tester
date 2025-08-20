#include "Kayit_Listele.h"
#include <QSplitter>
#include <QCheckBox>
#include <QMessageBox>
#include <QSqlQuery>
#include <QFont>
#include <QBrush>

TabKayitListele::TabKayitListele(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 🔍 Arama kutusu
    QHBoxLayout *searchLayout = new QHBoxLayout();
    QLabel *searchLabel = new QLabel("Aranacak Seri No:");
    searchEdit = new QLineEdit();
    searchEdit->setFixedWidth(200);
    searchLayout->addStretch();
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchEdit);
    searchLayout->addStretch();
    mainLayout->addLayout(searchLayout);

    // 📊 Tablo
    table = new QTableWidget();
    table->setFont(QFont("Arial", 11));
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Seri No", "EEPROM", "RF", "Network", "Sonuç"});

    // Sabit sütun boyutları ve manuel genişlikler
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);

    table->horizontalHeader()->resizeSection(0, 180); // Seri No
    table->horizontalHeader()->resizeSection(1, 100); // EEPROM
    table->horizontalHeader()->resizeSection(2, 70);  // RF
    table->horizontalHeader()->resizeSection(3, 80);  // Network
    table->horizontalHeader()->resizeSection(4, 150); // Sonuç

    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    // Sol layout (sadece tablo)
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(table);
    leftLayout->setContentsMargins(0,0,0,0);
    leftLayout->setSpacing(5);

    QWidget *leftWidget = new QWidget();
    leftWidget->setLayout(leftLayout);

    // 📝 Açıklama kutusu
    QLabel *aciklamaLabel = new QLabel("Açıklama:");
    aciklamaViewer = new QTextEdit();
    aciklamaViewer->setFont(QFont("Arial", 10));
    aciklamaViewer->setMinimumHeight(100);

    QPushButton *saveAciklamaBtn = new QPushButton("Açıklamayı Kaydet");
    saveAciklamaBtn->setFixedSize(180, 32);

    QVBoxLayout *aciklamaLayout = new QVBoxLayout();
    aciklamaLayout->addWidget(aciklamaLabel);
    aciklamaLayout->addWidget(aciklamaViewer);
    aciklamaLayout->addWidget(saveAciklamaBtn, 0, Qt::AlignHCenter);

    QWidget *aciklamaWidget = new QWidget();
    aciklamaWidget->setLayout(aciklamaLayout);

    // 📜 Log kutusu
    QLabel *logLabel = new QLabel("Log:");
    logViewer = new QPlainTextEdit();
    logViewer->setFont(QFont("Courier New", 10));
    logViewer->setPlaceholderText("Loglar burada gösterilecek...");
    logViewer->setMinimumHeight(120);
    logViewer->setReadOnly(true);

    QPushButton *deleteLogBtn = new QPushButton("Log Kaydını Sil");
    deleteLogBtn->setFixedSize(180, 32);

    QVBoxLayout *logLayout = new QVBoxLayout();
    logLayout->addWidget(logLabel);
    logLayout->addWidget(logViewer);
    logLayout->addWidget(deleteLogBtn, 0, Qt::AlignHCenter);

    QWidget *logWidget = new QWidget();
    logWidget->setLayout(logLayout);

    // Sağ taraf dikey splitter
    QSplitter *rightSplitter = new QSplitter(Qt::Vertical);
    rightSplitter->addWidget(aciklamaWidget);
    rightSplitter->addWidget(logWidget);
    rightSplitter->setStretchFactor(0, 1);
    rightSplitter->setStretchFactor(1, 1);

    // Ana horizontal splitter
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->addWidget(leftWidget);
    mainSplitter->addWidget(rightSplitter);
    mainSplitter->setHandleWidth(0);
    mainSplitter->setChildrenCollapsible(false);
    mainSplitter->setStretchFactor(0, 6);
    mainSplitter->setStretchFactor(1, 5);

    mainLayout->addWidget(mainSplitter);

    // 🔗 Bağlantılar
    connect(searchEdit, &QLineEdit::textChanged, this, &TabKayitListele::ara);
    connect(deleteLogBtn, &QPushButton::clicked, this, &TabKayitListele::seciliLoguSil);
    connect(table, &QTableWidget::itemSelectionChanged, this, &TabKayitListele::verileriGoster);
    connect(table, &QTableWidget::itemDoubleClicked, this, &TabKayitListele::guncellePenceresiAc);

    connect(saveAciklamaBtn, &QPushButton::clicked, this, [=]() {
        int row = table->currentRow();
        if(row < 0) {
            QMessageBox::warning(this, "Uyarı", "Lütfen bir modem seçiniz", QMessageBox::Ok);
            return;
        }
        QString seriNo = table->item(row, 0)->text();
        QString yeniAciklama = aciklamaViewer->toPlainText();
        QSqlQuery query;
        query.prepare("UPDATE modem_testleri SET aciklama = :aciklama WHERE seri_no = :seri_no");
        query.bindValue(":aciklama", yeniAciklama);
        query.bindValue(":seri_no", seriNo);
        if(query.exec()) {
            QMessageBox::information(this, "Başarılı", "Açıklama kaydedildi", QMessageBox::Ok);
        } else {
            QMessageBox::warning(this, "Hata", "Açıklama kaydedilirken bir hata oluştu", QMessageBox::Ok);
        }
    });

    tabloyuDoldur();
}


// Diğer fonksiyonlar (tabloyuDoldur, verileriGoster, seciliLoguSil, guncellePenceresiAc) 
// aynen yukarıdaki gibi kalabilir, sadece layout ve sütunları birleştirdik.



void TabKayitListele::tabloyuDoldur(const QString &filter) {
    QSqlQuery query;
    if (filter.isEmpty()) {
        query.exec("SELECT seri_no, eeprom, rf, network, result FROM modem_testleri");
    } else {
        query.prepare("SELECT seri_no, eeprom, rf, network, result FROM modem_testleri WHERE seri_no LIKE :filter");
        query.bindValue(":filter", "%" + filter + "%");
        query.exec();
    }

    table->setRowCount(0);
    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col = 0; col < 5; ++col) {
            QString text;
            QTableWidgetItem *item;

            if (col >= 1 && col <= 3) {
                bool val = query.value(col).toBool();
                text = val ? QString::fromUtf8("✅") : QString::fromUtf8("❌");
                item = new QTableWidgetItem(text);
                item->setFont(QFont("Segoe UI Emoji", 14));
            } else {
                text = query.value(col).toString();
                item = new QTableWidgetItem(text);
                item->setFont(QFont("Arial", 11));
            }

            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);

            if (col == 4) {
                if (text == "PASS")
                    item->setForeground(QBrush(Qt::darkGreen));
                else if (text == "FAIL")
                    item->setForeground(QBrush(Qt::red));
            }

            table->setItem(row, col, item);
        }
        row++;
    }
}
// Constructor içinde çift tıklama için bağlantı


void TabKayitListele::guncellePenceresiAc(QTableWidgetItem *item) {
    int row = item->row();
    if (row < 0) return;

    QString eskiSeriNo = table->item(row, 0)->text();

    // Yeni pencere
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Modem Güncelle");
    dialog->setModal(true);
    dialog->setMinimumWidth(350);

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);

    // Seri No kutusu
    QLabel *seriNoLabel = new QLabel("Seri No:");
    QLineEdit *seriNoEdit = new QLineEdit(eskiSeriNo);
    seriNoEdit->setMaximumWidth(200);

    QHBoxLayout *seriLayout = new QHBoxLayout();
    seriLayout->addStretch();
    seriLayout->addWidget(seriNoLabel);
    seriLayout->addWidget(seriNoEdit);
    seriLayout->addStretch();
    mainLayout->addLayout(seriLayout);

    // Checkboxlar yatay
    QHBoxLayout *cbLayout = new QHBoxLayout();
    QCheckBox *eepromCB = new QCheckBox("EEPROM");
    QCheckBox *rfCB = new QCheckBox("RF");
    QCheckBox *networkCB = new QCheckBox("NETWORK");
    cbLayout->addStretch();
    cbLayout->addWidget(eepromCB);
    cbLayout->addWidget(rfCB);
    cbLayout->addWidget(networkCB);
    cbLayout->addStretch();

    // Mevcut verileri getir ve checkboxları işaretle
    QSqlQuery query;
    query.prepare("SELECT eeprom, rf, network FROM modem_testleri WHERE seri_no = :seri_no");
    query.bindValue(":seri_no", eskiSeriNo);
    if(query.exec() && query.next()) {
        eepromCB->setChecked(query.value(0).toBool());
        rfCB->setChecked(query.value(1).toBool());
        networkCB->setChecked(query.value(2).toBool());
    }

    mainLayout->addLayout(cbLayout);

    // Butonlar
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *updateBtn = new QPushButton("Güncelle");
    QPushButton *deleteBtn = new QPushButton("Kaydı Sil");
    btnLayout->addStretch();
    btnLayout->addWidget(updateBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    // Güncelle butonu işlemi
    connect(updateBtn, &QPushButton::clicked, [=]() {
        QString yeniSeriNo = seriNoEdit->text().trimmed();
        bool eeprom = eepromCB->isChecked();
        bool rf = rfCB->isChecked();
        bool network = networkCB->isChecked();

        if (yeniSeriNo.isEmpty()) {
            QMessageBox::warning(dialog, "Hata", "Seri No boş olamaz", QMessageBox::Ok);
            return;
        }

        QString result = (eeprom && rf && network) ? "PASS" : "FAIL";

        // Veritabanını güncelle
        QSqlQuery updateQuery;
        updateQuery.prepare(
            "UPDATE modem_testleri SET seri_no=:yeniSeriNo, eeprom=:eeprom, rf=:rf, network=:network, result=:result "
            "WHERE seri_no=:eskiSeriNo"
        );
        updateQuery.bindValue(":yeniSeriNo", yeniSeriNo);
        updateQuery.bindValue(":eeprom", eeprom);
        updateQuery.bindValue(":rf", rf);
        updateQuery.bindValue(":network", network);
        updateQuery.bindValue(":result", result);
        updateQuery.bindValue(":eskiSeriNo", eskiSeriNo);

        if(updateQuery.exec()) {
            QMessageBox::information(dialog, "Başarılı", "Modem bilgileri güncellendi", QMessageBox::Ok);
            tabloyuDoldur(searchEdit->text());
            table->clearSelection(); // Otomatik seçim kaldırıldı
            dialog->close();
        } else {
            QMessageBox::warning(dialog, "Hata", "Güncelleme sırasında hata oluştu", QMessageBox::Ok);
        }
    });

    // Kaydı sil butonu işlemi
    connect(deleteBtn, &QPushButton::clicked, [=]() {
        QMessageBox::StandardButton reply = QMessageBox::question(
            dialog, "Kayıt Sil", "Seçilen kaydı silmek istediğinize emin misiniz?",
            QMessageBox::Yes|QMessageBox::No 
        );
        if (reply == QMessageBox::Yes) {
            QSqlQuery delQuery;
            delQuery.prepare("DELETE FROM modem_testleri WHERE seri_no = :seri_no");
            delQuery.bindValue(":seri_no", eskiSeriNo);
            if (delQuery.exec()) {
                tabloyuDoldur(searchEdit->text());
                table->clearSelection();
                dialog->close();
            } else {
                QMessageBox::warning(dialog, "Hata", "Kayıt silinirken bir hata oluştu", QMessageBox::Ok);
            }
        }
    });

    dialog->exec();
}



void TabKayitListele::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    tabloyuDoldur(searchEdit->text());
    logViewer->clear();
    aciklamaViewer->clear();
}

void TabKayitListele::ara(const QString &filter) {
    tabloyuDoldur(filter);
    logViewer->clear();
    aciklamaViewer->clear();
}


void TabKayitListele::seciliLoguSil() {
    int row = table->currentRow();
    if (row < 0) return;

    QString seriNo = table->item(row, 0)->text();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Log Sil", "Seçilen kaydın log bilgisini silmek istediğinize emin misiniz?",
        QMessageBox::Yes | QMessageBox::No
    );
    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM modem_loglari WHERE seri_no = :seri_no");
        query.bindValue(":seri_no", seriNo);
        if (query.exec()) {
            logViewer->clear();
        }
    }
}

void TabKayitListele::verileriGoster() {
    int row = table->currentRow();
    if (row < 0) return;

    QString seriNo = table->item(row, 0)->text();

    QSqlQuery query1;
    query1.prepare("SELECT aciklama FROM modem_testleri WHERE seri_no = :seri_no");
    query1.bindValue(":seri_no", seriNo);
    if (query1.exec() && query1.next()) {
        aciklamaViewer->setPlainText(query1.value(0).toString());
    } else {
        aciklamaViewer->setPlainText("Açıklama bulunamadı.");
    }

    QSqlQuery query2;
    query2.prepare("SELECT log FROM modem_loglari WHERE seri_no = :seri_no");
    query2.bindValue(":seri_no", seriNo);
    if (query2.exec() && query2.next()) {
        logViewer->setPlainText(query2.value(0).toString());
    } else {
        logViewer->setPlainText("Log bulunamadı.");
    }
}

