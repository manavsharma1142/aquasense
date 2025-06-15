#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Load the icon from the resource file
    QIcon icon(":/icons/assets/icon-128x128.png");

    // Set the new resized pixmap as icon
    this->setWindowIcon(icon);

    // Always start at the welcome page
    ui->stackedWidget->setCurrentIndex(0);

    // Fill the contact page table
    populateDeveloperTable();

    // Connect the prediction button to engine
    connect(ui->predict_button, &QPushButton::clicked, this, &MainWindow::on_predict_button_clicked);

    // Populate the data table
    QString dataFilePath = "E:/GUI/Projects/AquaSense/data/data.csv";
    populateTableFromCSV(dataFilePath, ui->table_for_data);

    // Setup the visualization flow
    initializeVisualizations();
}

MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * ADDITIONAL FUNCTIONS FOR HELPERS
 *
 **/
void MainWindow::sendRequestToCloud(const QVector<double> &inputValues) {
    ui->predictionLabel->setText("Hold your breath, AquaSense is diving deep... 🏊‍♂️");

    // Define the URL of your Azure Function endpoint
    // QUrl url("http://localhost:7071/api/prediction"); // Replace with your actual endpoint URL
    QUrl url("PUT YOUR ENDPOINT URL HERE. I HAVE DELETED MINE.");

    // Create a JSON object with the input values
    QJsonObject jsonObject;
    QJsonArray inputArray;
    for (const auto &value : inputValues) {
        inputArray.append(QJsonValue(value));
    }
    jsonObject["input_values"] = inputArray;

    // Convert the JSON object to a JSON document
    QJsonDocument jsonDocument(jsonObject);
    QByteArray jsonData = jsonDocument.toJson();

    // Create a network access manager to send the request
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);

    // Set up the request headers
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Send the POST request to the Azure Function endpoint
    QNetworkReply *reply = networkManager->post(request, jsonData);

    // Connect signals and slots to handle the response
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Read and process the response data
            QByteArray responseData = reply->readAll();
            QString predictionResult = QString::fromUtf8(responseData);

            // Update the prediction label based on the response
            if (predictionResult == "1") {
                qDebug() << "Prediction turned out to be SAFE WATER\n";
                ui->predictionLabel->setText("<font color='blue'>SAFE TO DRINK</font>");

                updateDataFile(inputValues, 1);
                qDebug() << "Inserting new sample into the data file\n";

                qDebug() << "Also updating the table.\n";

                QString dataFilePath = "E:/GUI/Projects/AquaSense/data/data.csv";
                ui->table_for_data->clearContents();
                ui->table_for_data->setRowCount(0);
                populateTableFromCSV(dataFilePath, ui->table_for_data);
            } else if (predictionResult == "0") {
                ui->predictionLabel->setText("<font color='purple'>NOT SAFE TO DRINK</font>");
                qDebug() << "Prediction turned out to be UNSAFE WATER\n";

                updateDataFile(inputValues, 0);
                qDebug() << "Inserting new sample into the data file\n";
                qDebug() << "Also updating the table.\n";

                QString dataFilePath = "E:/GUI/Projects/AquaSense/data/data.csv";
                ui->table_for_data->clearContents();
                ui->table_for_data->setRowCount(0);
                populateTableFromCSV(dataFilePath, ui->table_for_data);
            } else {
                ui->predictionLabel->setText("<font color='red'>Invalid sample.</font>");
            }
        } else {
            // Handle the error if the request fails
            ui->predictionLabel->setText("Failed to get prediction result: " + reply->errorString());
        }

        // Clean up resources
        reply->deleteLater();
        networkManager->deleteLater();
        ui->predict_button->setEnabled(true);
    });
}
void MainWindow::callPredictionModel() {
    // Disable the predict button so that user doesn't flood it
    ui->predict_button->setEnabled(false);

    // Fetch the input values from the line edits
    double phValue = ui->phLineEdit->text().toDouble();
    double hardnessValue = ui->hardnessLineEdit->text().toDouble();
    double solidsValue = ui->solidsLineEdit->text().toDouble();
    double chloraminesValue = ui->chloraminesLineEdit->text().toDouble();
    double sulfateValue = ui->sulfateLineEdit->text().toDouble();
    double conductivityValue = ui->conductivityLineEdit->text().toDouble();
    double organicCarbonValue = ui->organicCarbonLineEdit->text().toDouble();
    double trihalomethanesValue = ui->trihalomethanesLineEdit->text().toDouble();
    double turbidityValue = ui->turbidityLineEdit->text().toDouble();

    // Make a call to the prediction model with these values
    QVector<double> inputValues = {
        phValue,
        hardnessValue,
        solidsValue,
        chloraminesValue,
        sulfateValue,
        conductivityValue,
        organicCarbonValue,
        trihalomethanesValue,
        turbidityValue
    };

    // Call the cloud prediction model with these input values
    sendRequestToCloud(inputValues);
}

void MainWindow::updateDataFile(const QVector<double> &inputValues, int prediction) {
    // Open the data file in append mode
    QString dataFilePath = "E:/GUI/Projects/AquaSense/data/data.csv";
    QFile file(dataFilePath);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Failed to open data file for writing:" << file.errorString();
        return;
    }

    // Create a text stream to write to the file
    QTextStream out(&file);

    // Write the input values to the file
    for (const auto &value : inputValues) {
        out << value << ",";
    }

    // Write the prediction value to the file
    out << prediction << "\n";

    // Close the file
    file.close();
}


void MainWindow::checkInputsAndPredict() {
    // Check if any line edit is empty
    if (ui->phLineEdit->text().isEmpty() ||
        ui->hardnessLineEdit->text().isEmpty() ||
        ui->solidsLineEdit->text().isEmpty() ||
        ui->chloraminesLineEdit->text().isEmpty() ||
        ui->sulfateLineEdit->text().isEmpty() ||
        ui->conductivityLineEdit->text().isEmpty() ||
        ui->organicCarbonLineEdit->text().isEmpty() ||
        ui->trihalomethanesLineEdit->text().isEmpty() ||
        ui->turbidityLineEdit->text().isEmpty()) {
        // Display a message box asking the user to fill all fields
        QMessageBox::warning(this, "Missing Input", "Please fill all input fields.");
    } else {
        // If all fields are filled, proceed with prediction
        callPredictionModel();
    }
}

void MainWindow::executePythonScript(const QString& filePath)
{
    // wchar_t* python_home = Py_DecodeLocale("E:\\GUI\\Projects\\AquaSense\\venv", NULL);
    // Py_SetPythonHome(python_home);

    Py_Initialize();

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open the Python script.";
        return;
    }

    QTextStream in(&file);
    QString pyScript = in.readAll();
    file.close();

    int result = PyRun_SimpleString(pyScript.toStdString().c_str());
    if (result != 0) {
        qDebug() << "Error executing Python script.";
    }

    // Py_FinalizeEx();
    // PyMem_RawFree(python_home);
    Py_Finalize();
}


void MainWindow::populateDeveloperTable() {
    QTableWidget *table = new QTableWidget;   // Create the table widgets with 4 Rs and 3 Cs
    table->setRowCount(4); 					  // Set the number of developers
    table->setColumnCount(3); 				  // For "Name", "Batch", "Email"
    table->setHorizontalHeaderLabels({"Name", "Batch", "Email"});

    // Set table properties
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    table->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "background-color: #EFEFEF;"
        "padding: 4px;"
        "border: 1px solid #DFDFDF;"
        "font-size: 10pt;"
        "font-weight: bold;"
        "text-align: center;"
        "}"
        );
    table->setStyleSheet(
        "QTableWidget {"
        "font-size: 10pt;"
        "}"
        "QTableWidget::item {"
        "padding: 5px;"
        "}"
        "QHeaderView::section {"
        "background-color: #EFEFEF;"
        "padding: 4px;"
        "border: 1px solid #DFDFDF;"
        "font-size: 10pt;"
        "font-weight: bold;"
        "text-align: center;"
        "}"
        );

    table->setAlternatingRowColors(true);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Add developer data to the table
    const QVector<QVector<QString>> developersData = {
        {"Aman Kediyal", "CSE AI-ML (NH) B2", "500091754@stu.upes.ac.in"},
    };

    for (int row = 0; row < developersData.size(); ++row) {
        for (int col = 0; col < developersData[row].size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(developersData[row][col]);
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, col, item);
        }
    }

    QVBoxLayout *layout = ui->vertical_layout_contact;  // Find the layout and add the table to it
    layout->addWidget(table); 							// Add the table to the layout
    layout->addStretch(1.5);								// Add a stretch to push everything to the top
}

void MainWindow::populateTableFromCSV(const QString& filePath, QTableWidget* tableWidget) {
    // Open the CSV file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open CSV file" << filePath;
        return;
    }

    // Set the necessary properties to disable editing, and other options
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setAlternatingRowColors(true);
    tableWidget->setSortingEnabled(true);


    // Read the contents of the file
    QTextStream in(&file);
    QStringList headers;
    QStringList rowData;
    bool isFirstLine = true;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (isFirstLine) {
            // Save column headers
            headers = fields;
            tableWidget->setColumnCount(fields.size());
            tableWidget->setHorizontalHeaderLabels(headers);
            isFirstLine = false;
        } else {
            // Add row data to the table widget
            tableWidget->insertRow(tableWidget->rowCount());
            for (int i = 0; i < fields.size(); ++i) {
                QTableWidgetItem* item = new QTableWidgetItem(fields[i]);
                tableWidget->setItem(tableWidget->rowCount() - 1, i, item);
            }
        }
    }

    // Close the file
    file.close();
}

void MainWindow::initializeVisualizations()
{
    QDir dir("E:\\GUI\\Projects\\AquaSense\\scripts\\charts");
    QFileInfoList list = dir.entryInfoList(QDir::Files);

    // Check if the directory is empty
    if (list.empty()) {
        QString basePath = "E:/GUI/Projects/AquaSense/scripts/visualize.py";
        // Generate the images
        executePythonScript(basePath);
    }

    // Get the list of image files
    list = dir.entryInfoList(QStringList() << "*.png", QDir::Files);
    foreach (QFileInfo fileInfo, list) {
        images << fileInfo.absoluteFilePath();
    }

    // Connect the buttons to their slots
    connect(ui->next_button_visualization, &QPushButton::clicked, this, &MainWindow::showNextImage);
    connect(ui->previous_button_visualization, &QPushButton::clicked, this, &MainWindow::showPrevImage);

    // Show the first image
    showImage();
}

void MainWindow::showNextImage()
{
    if (!images.isEmpty()) {
        index = (index + 1) % images.size();
        showImage();
    }
}

void MainWindow::showPrevImage()
{
    if (!images.isEmpty()) {
        index = (index - 1 + images.size()) % images.size();
        showImage();
    }
}

void MainWindow::showImage()
{
    QPixmap pixmap(images[index]);
    ui->label_for_graphs->setPixmap(pixmap.scaled(800, 600, Qt::KeepAspectRatio));
    ui->label_for_graphs->setToolTip(QString("This is image number %1").arg(index + 1));
}

/**
 *
 * SLOTS AND SIGNALS BELOW
 *
 **/

/*
 * 1 - main page
 * 2 - contact page
 * 3 - data page
 * 4 - insights page
 * 5 - visualization page
 * 0 - welcome page
 */

void MainWindow::on_back_to_main_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_back_to_welcome_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_back_to_main_contact_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_back_to_contact_main_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_back_to_data_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

}


void MainWindow::on_back_to_insights_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_back_to_main_data_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_back_to_main_insights_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_back_to_main_visualization_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_back_to_visualizations_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_predict_button_clicked()
{
    checkInputsAndPredict();
}


void MainWindow::on_reset_entries_button_clicked()
{
    ui->chloraminesLineEdit->clear();
    ui->conductivityLineEdit->clear();
    ui->trihalomethanesLineEdit->clear();
    ui->turbidityLineEdit->clear();
    ui->solidsLineEdit->clear();
    ui->phLineEdit->clear();
    ui->sulfateLineEdit->clear();
    ui->organicCarbonLineEdit->clear();
    ui->hardnessLineEdit->clear();

    ui->predictionLabel->clear();
}

