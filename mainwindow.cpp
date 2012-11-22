#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QtCore>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->checkBoxAntiAliasing, SIGNAL(toggled(bool)), this, SLOT(updatePlainTextEditFontSettings()));
    connect(ui->checkBoxBold, SIGNAL(toggled(bool)), this, SLOT(updatePlainTextEditFontSettings()));
    connect(ui->checkBoxItalic, SIGNAL(toggled(bool)), this, SLOT(updatePlainTextEditFontSettings()));
    connect(ui->checkBoxMonospace, SIGNAL(toggled(bool)), this, SLOT(updatePlainTextEditFontSettings()));
    connect(ui->fontComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePlainTextEditFontSettings()));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePlainTextEditFontSettings()));

    process = new QProcess(this);
    connect(process, SIGNAL(finished(int)), this, SLOT(loadOutput()));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_pushButton_clicked()));
    timer->setSingleShot(true);

    QSettings settings;
    ui->plainTextEdit->setPlainText(settings.value("Last Text").toString());
    ui->plainTextEdit_2->setPlainText(settings.value("Last Text 2").toString());
    ui->lineEditCommand->setText(settings.value("Command").toString());
    ui->spinBox->setValue(settings.value("Font Size", 8).toInt());
    ui->checkBoxBold->setChecked(settings.value("Bold", false).toBool());
    ui->checkBoxItalic->setChecked(settings.value("Italic", false).toBool());
    ui->checkBoxMonospace->setChecked(settings.value("Monospace", false).toBool());
    ui->checkBoxAntiAliasing->setChecked(settings.value("Antialiasing", true).toBool());
    ui->lineEditImageWidth->setText(settings.value("Image Width", 612).toString());
    ui->lineEditImageHeight->setText(settings.value("Image Height", 792).toString());
    ui->lineEditImageDPI->setText(settings.value("Image DPI", 600).toString());
    ui->spinBox_2->setValue(settings.value("Delay", 2).toInt());
    QFont font;
    if (font.fromString(settings.value("Font").toString()))
        ui->fontComboBox->setCurrentFont(font);
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue("Last Text", ui->plainTextEdit->toPlainText());
    settings.setValue("Last Text 2", ui->plainTextEdit_2->toPlainText());
    settings.setValue("Command", ui->lineEditCommand->text());
    settings.setValue("Font Size", ui->spinBox->value());
    settings.setValue("Font", ui->fontComboBox->currentFont().toString());
    settings.setValue("Bold", ui->checkBoxBold->isChecked());
    settings.setValue("Italic", ui->checkBoxItalic->isChecked());
    settings.setValue("Monospace", ui->checkBoxMonospace->isChecked());
    settings.setValue("Antialiasing", ui->checkBoxAntiAliasing->isChecked());
    settings.setValue("Image Width", ui->lineEditImageWidth->text());
    settings.setValue("Image Height", ui->lineEditImageHeight->text());
    settings.setValue("Image DPI", ui->lineEditImageDPI->text());
    settings.setValue("Delay", ui->spinBox_2->value());
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QApplication::setOverrideCursor(Qt::BusyCursor);
    QImage image(ui->lineEditImageWidth->text().toInt(), ui->lineEditImageHeight->text().toInt(), QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.setFont(ui->plainTextEdit->font());
    painter.drawText(image.rect(), 0, ui->plainTextEdit->toPlainText());
    QString input = QDir::cleanPath(QDir::tempPath() + "/TesseractTesterInput.tif");
    QString output = QDir::cleanPath(QDir::tempPath() + "/TesseractTesterOutput");
    image.save(input);
    process->terminate();
    process->waitForFinished();
    process->start(ui->lineEditCommand->text().arg(input).arg(output));
}

void MainWindow::loadOutput()
{
    QString output = QDir::cleanPath(QDir::tempPath() + "/TesseractTesterOutput.txt");
    QFile file(output);
    if (file.open(QIODevice::ReadOnly))
        ui->plainTextEdit_2->setPlainText(file.readAll());
    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::updatePlainTextEditFontSettings()
{
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->spinBox->value());
    font.setBold(ui->checkBoxBold->isChecked());
    font.setItalic(ui->checkBoxItalic->isChecked());
    font.setFixedPitch(ui->checkBoxMonospace->isChecked());
    if (!ui->checkBoxAntiAliasing->isChecked())
        font.setStyleStrategy(QFont::NoAntialias);
    ui->plainTextEdit->setFont(font);
    ui->plainTextEdit_2->setFont(font);
}

void MainWindow::on_plainTextEdit_textChanged()
{
    int timeout = ui->spinBox_2->value();
    if (timeout)
        timer->start(timeout * 1000);
}
