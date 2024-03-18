#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

#include <QScroller>
//https://stackoverflow.com/questions/7820961/finger-scrolling-in-qt

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);
    //this->setFixedSize(440,410);
    ui->plainTextEdit->setMinimumHeight(800);
    this->setWidgetStyle();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    auto start_time = std::chrono::high_resolution_clock::now();
    std::uint64_t count = 0;


    int R=0;
    QString errorStr;
    QString latinLower = "abcdefghijklmnopqrstuvwxyz";
    QString cyrillicLower("абвгдеёжзийклмнопрстуфхцчшщъыьэюя");
    QString latinUpper = latinLower.toUpper();
    QString cyrillicUpper = cyrillicLower.toUpper();
    QString resStr = "";

    if(ui->latinCheck->isChecked()){
        if(ui->latinCombo->currentIndex()==1){
            resStr += latinUpper; R +=  latinLower.size();
        }else if(ui->latinCombo->currentIndex()==2){
            resStr += latinLower; R +=  latinLower.size();
        }else{
            resStr += latinLower + latinUpper; R +=  2*latinLower.size();
        }
    }
    if(ui->cyrillicCheck->isChecked()){
        if(ui->cyrillicCombo->currentIndex()==1){
            resStr += cyrillicUpper;    R +=  cyrillicLower.size();
        }else if(ui->cyrillicCombo->currentIndex()==2){
            resStr += cyrillicLower; R +=  cyrillicLower.size();
        }else{
            resStr += cyrillicLower + cyrillicUpper; R +=  2*cyrillicLower.size();
        }
    }

    if(ui->symbolCheck->isChecked()){
           QString symbols = ui->symbolEdit->text();
           symbols.replace(" ","");
           //qDebug() << "Symbols=" << symbols;
           resStr += symbols; R += ui->symbolEdit->text().size();
    }
    if(ui->didgitCheck->isChecked()){
            QString str = ui->didgitEdit->text();
            str.replace(" ","");
            if(this->didgitIsValid(str)==false){
                errorStr = "The Numbers field is filled in incorrectly!";
            }else{
                int start = QString(str.at(0)).toInt();
                int stop = QString(str.at(2)).toInt();
                for (int i=start;i<=stop ;i++ ) {
                    resStr += QString::number(i);
                }
                int temp=0;
                for (int i=start;i<=stop ;i++ ) {
                    temp++;
                }
                R +=temp;
            }
     }

    if(resStr.length()==0){
        errorStr.append("\nYou need to choose at least something...");
    }
    if(errorStr.length()>0){
        QMessageBox::warning(this, "Error", errorStr);
    }else{
        int lettersCount = ui->lettersSpin->value();
        int passCount = ui->passCountSpin->value();
        srand(time(0));
        for(int j=0;j<passCount;j++){
            QString pass = "";
            for(int i=0;i<lettersCount;i++){
                int index = rand() % resStr.length();
                    pass += resStr[index];
            }
            //qDebug() << QString::number(j) + ". " + pass;

// log2(x) = log10(x) / log10(2) https://stackoverflow.com/questions/3064926/how-to-write-log-base2-in-c-c
// E = log2(RL)     https://www.techtarget.com/whatis/definition/password-entropy
            //int R =  2*latinLower.size() +    ui->symbolEdit->text().size() + 10;
            //qDebug() << resStr.length() << " " << 2*latinLower.size() << " " << ui->symbolEdit->text().size() << " " << 10;
            int L = lettersCount;
            double E = log10(pow( R, L)) / log10(2);
            //double EE = log2(pow( R, L));
            //qDebug() << R << " " << L << " " << E << " " << EE;
            //pass = QString::number(j) + ". " + pass;
            pass = QString::number(E) + ": " + pass;
            ui->plainTextEdit->appendPlainText(pass);
        }
    }

    uint64_t final_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start_time).count();
    QString formatLabeText = QString::number(ui->passCountSpin->value()) + QString(" password generation at %1 milliseconds ").arg(final_time);
    ui->label_5->setText(formatLabeText);

}


bool Widget::didgitIsValid(QString str)
{
    if(str.length() != 3){
        return false;
    }else if(str.at(1)!='-'){
        return false;
    }else if((str.at(0).isDigit()==false) or (str.at(2).isDigit()==false)){
        return false;
    }else{
        return true;
    }
}

void Widget::on_pushButton_2_clicked()
{
    ui->plainTextEdit->clear();
    ui->label_5->setText("Attention! 10k psw generation 3-10 sec.");
}


void Widget::setWidgetStyle()
{
    this->setStyleSheet(
    "QWidget{"
        "background-color:#ddd;"
    "}"
    "QLineEdit{"
        "border:1px solid #aaa;"
        "background:#fff;"
        "border-radius:3px;"
        "color:#333;"
    "}"
    "QLineEdit:hover{"
        "border:1px solid #0078d7;"
    "}"
    "QSpinBox{"
        "border:1px solid #aaa;"
        "background:#fff;"
        "border-radius:3px;"
        "color:#333;"
    "}"
    "QSpinBox:hover{"
        "border:1px solid #0078d7;"
    "}"
    "QComboBox{"
        "border:1px solid #aaa;"
        "background-color: qlineargradient(spread:pad, x1:0.585, y1:1, x2:0.506, y2:0, "
                           "stop:0 rgba(180, 180, 180, 255), stop:1 rgba(232, 232, 232, 255));"
        "border-radius:3px;"
        "color:#333;"
    "}"
    "QComboBox::drop-down {"
        "subcontrol-origin: padding;"
        "subcontrol-position: top right;"
        "width: 15px;"
        "border-left-width: 1px;"
        "border-left-color: darkgray;"
        "border-left-style: solid;"
        "border-top-right-radius: 3px;"
        "border-bottom-right-radius: 3px;"
    "}"
    "QComboBox::down-arrow {"
        "image: url(:/images/arrow.png);"
    "}"
    "QComboBox QListView{"
        "background:#fff;"
    "}"
    "QComboBox:hover{"
        "border:1px solid #0078d7;"
    "}"
    "QSpinBox::up-button {"
        "subcontrol-origin: border;"
        "subcontrol-position: top right;"
        "background-color:#eee;"
        "width: 16px;"
        "border-width: 1px;"
        "height:10px;"
        "margin-top:1px;"
        "margin-right:1px;"
        "border-radius:4px;"
    "}"
    "QSpinBox::down-button {"
        "subcontrol-origin: border;"
        "subcontrol-position: bottom right;"
        "background-color:#eee;"
        "width: 16px;"
        "border-width: 1px;"
        "height:10px;"
        "margin-bottom:1px;"
        "margin-right:1px;"
        "border-radius:4px;"
    "}"
    "QSpinBox::down-arrow {"
        "image: url(:/images/down.png);"
        "width: 7px;"
        "height: 7px;"
    "}"
    "QSpinBox::up-arrow {"
        "image: url(:/images/up.png);"
        "width: 7px;"
        "height: 7px;"
    "}"
    "QPlainTextEdit{"
        "border:1px solid #ccc;"
        "background:#fff;"
    "}"
    "QPlainTextEdit:hover{"
        "border:1px solid #0078d7;"
    "}"
    "QPushButton{"
        "background-color: qlineargradient(spread:pad, x1:0.585, y1:1, x2:0.506, y2:0, stop:0 rgba(180, 180, 180, 255), stop:1 rgba(232, 232, 232, 255));"
        "border-radius:5px;"
        "border:1px solid #aaa;"
        "color:#555;"
        "padding:5px;"
    "}"
    "QPushButton:hover{"
        "background-color: qlineargradient(spread:pad, x1:0.585, y1:1, x2:0.506, y2:0, stop:0 rgba(200, 200, 200, 200), stop:1 rgba(232, 232, 232, 255));"
        "border-radius:5px;"
        "border:1px solid #0078d7;"
        "color:#111;"
    "}");
}



void Widget::on_pushButton_3_clicked()
{
    ui->label_5->setText("Attention! 10k psw generation 3-10 sec.");

    QFileDialog dialog(this);
    QString output = dialog.getSaveFileName( this,"Save file export", "/", "Text (*.txt)");

    QFile outputFile(output);
    outputFile.open(QFile::ReadWrite);

    QTextStream out(&outputFile);

    //
    ui->plainTextEdit->clear();


    // незаморачиваясь копипасте )

    QString errorStr;
    QString latinLower = "abcdefghijklmnopqrstuvwxyz";
    QString cyrillicLower("абвгдеёжзийклмнопрстуфхцчшщъыьэюя");
    QString latinUpper = latinLower.toUpper();
    QString cyrillicUpper = cyrillicLower.toUpper();
    QString resStr = "";

    if(ui->latinCheck->isChecked()){
        if(ui->latinCombo->currentIndex()==1){
            resStr += latinUpper;
        }else if(ui->latinCombo->currentIndex()==2){
            resStr += latinLower;
        }else{
            resStr += latinLower + latinUpper;
        }
    }
    if(ui->cyrillicCheck->isChecked()){
        if(ui->cyrillicCombo->currentIndex()==1){
            resStr += cyrillicUpper;
        }else if(ui->cyrillicCombo->currentIndex()==2){
            resStr += cyrillicLower;
        }else{
            resStr += cyrillicLower + cyrillicUpper;
        }
    }

    if(ui->symbolCheck->isChecked()){
           QString symbols = ui->symbolEdit->text();
           symbols.replace(" ","");
           //qDebug() << "Symbols=" << symbols;
           resStr += symbols;
    }
    if(ui->didgitCheck->isChecked()){
            QString str = ui->didgitEdit->text();
            str.replace(" ","");
            if(this->didgitIsValid(str)==false){
                errorStr = "The Numbers field is filled in incorrectly!";
            }else{
                int start = QString(str.at(0)).toInt();
                int stop = QString(str.at(2)).toInt();
                for (int i=start;i<=stop ;i++ ) {
                    resStr += QString::number(i);
                }
            }
     }

    if(resStr.length()==0){
        errorStr.append("\nYou need to choose at least something...");
    }
    if(errorStr.length()>0){
        QMessageBox::warning(this, "Error", errorStr);
    }else{
        int lettersCount = ui->lettersSpin->value();
        int passCount = ui->passCountSpin->value();
        srand(time(0));
        for(int j=0;j<passCount;j++){
            QString pass = "";
            for(int i=0;i<lettersCount;i++){
                int index = rand() % resStr.length();
                    pass += resStr[index];
            }
            //qDebug() << QString::number(j) + ". " + pass;
            pass = QString::number(j) + ". " + pass;
            out << pass << '\n';
            ui->plainTextEdit->appendPlainText(pass);
        }
    }
}

