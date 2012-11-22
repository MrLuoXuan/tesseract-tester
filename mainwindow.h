#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QProcess;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_clicked();
    void loadOutput();
    void updatePlainTextEditFontSettings();

    void on_plainTextEdit_textChanged();

private:
    QProcess *process;
    QTimer *timer;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
