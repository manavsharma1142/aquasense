#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>

#include <QMainWindow>
#include <QTableWidget>
#include <QHeaderView>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QMessageBox>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void populateDeveloperTable();
    void executePythonScript(const QString& filePath);
    void populateTableFromCSV(const QString& filePath, QTableWidget *tableWidget);

    void initializeVisualizations();
    void showImage();
    void showNextImage();
    void showPrevImage();
    void checkInputsAndPredict();
    void callPredictionModel();
    void sendRequestToCloud(const QVector<double>& inputValues);
    void updateDataFile(const QVector<double>& inputValues, int prediction);

private slots:
    void on_back_to_main_clicked();

    void on_back_to_welcome_clicked();

    void on_back_to_main_contact_clicked();

    void on_back_to_contact_main_clicked();

    void on_back_to_data_clicked();

    void on_back_to_insights_clicked();

    void on_back_to_main_data_clicked();

    void on_back_to_main_insights_clicked();

    void on_back_to_main_visualization_clicked();

    void on_back_to_visualizations_clicked();

    void on_predict_button_clicked();

    void on_reset_entries_button_clicked();

private:
    Ui::MainWindow *ui;

    QStringList images; 	// List of images
    int index = 0; 			// Current index of image
};
#endif // MAINWINDOW_H
