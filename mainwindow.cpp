#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidget>

#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CurrentDataTime =QDateTime::currentDateTime();
    QString CurrentDataTimeStr =CurrentDataTime.toString("yyyy年MM月dd日");

    ui->label_todaytime->setText(CurrentDataTimeStr);

    ui->DetailTable->setColumnCount(2);
    ui->DetailTable->setColumnWidth(0,200);
    ui->DetailTable->setColumnWidth(1,130);
    ui->DetailTable->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->DetailTable->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    QStringList sListHeader;
    sListHeader << "调量日期" << "调量";
    ui->DetailTable->setHorizontalHeaderLabels(sListHeader);


    MorningOrNightFlag = 0;
    ui->radioButtonMorning->setChecked(true);
    ui->radioButtonNight->setChecked(false);

    ui->dateEdit_OriDate->setDate(CurrentDataTime.date());
    ui->dateEdit_LastDate->setDate(CurrentDataTime.date());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonAddChange_clicked()
{
    int iRow = ui->DetailTable->rowCount();
    ui->DetailTable->setRowCount(iRow + 1);//总行数增加1

    QDateEdit *DateEditItem = new QDateEdit(ui->dateEdit_OriDate->date());
    DateEditItem->setCalendarPopup(true);
    ui->DetailTable->setCellWidget(iRow,0,DateEditItem);

    QLineEdit *LineEditItem = new QLineEdit();
    ui->DetailTable->setCellWidget(iRow,1,LineEditItem);
}

void MainWindow::on_pushButtonDelChange_clicked()
{
    int i = ui->DetailTable->currentRow();
    if(i >= 0){
        ui->DetailTable->removeRow(i);
    }
}

void MainWindow::on_pushButtonCal_clicked()
{
    QString OriginNumberStr = ui->lineEdit_0Count->text();
    float OriginNumber = OriginNumberStr.toFloat();

    QString OriginDoseStr = ui->lineEdit_0Use->text();
    float OriginDose = OriginDoseStr.toFloat();

    QDate OriginDate = ui->dateEdit_OriDate->date();
    QDate LastMeetDate = ui->dateEdit_LastDate->date();

    float TotalUsed = 0.0;

    int MeetCount = ui->DetailTable->rowCount();
    for (int Tmpi = 0; Tmpi < MeetCount; Tmpi++) {
        if(Tmpi == 0){
            QDateEdit *CurrentDateEditVal = (QDateEdit *)ui->DetailTable->cellWidget(Tmpi,0);
            int DateLength = OriginDate.daysTo(CurrentDateEditVal->date());
            if(MorningOrNightFlag == 0){
                DateLength +=1;
            }
            int SegUse = DateLength * OriginDose;
            TotalUsed += SegUse;
            //qDebug()<<QString("初始到第%1次调量,相差%2天,期间用量%3,期间用药%4").arg(Tmpi+1).arg(DateLength).arg(OriginDose).arg(SegUse);

        }
        else{
            QDateEdit *PreDateEditVal = (QDateEdit *)ui->DetailTable->cellWidget(Tmpi - 1,0);
            QLineEdit *PreQLineEditDoseVal = (QLineEdit *)ui->DetailTable->cellWidget(Tmpi-1,1);
            float CurrentDose = PreQLineEditDoseVal->text().toFloat();

            QDateEdit *CurrentDateEditVal = (QDateEdit *)ui->DetailTable->cellWidget(Tmpi,0);
            int DateLength = PreDateEditVal->date().daysTo(CurrentDateEditVal->date());
            int SegUse = DateLength * CurrentDose;
            TotalUsed += SegUse;
            //qDebug()<<QString("第%1次调量-->第%2次调量,相差%3天,期间用量%4,期间用药%5").arg(Tmpi).arg(Tmpi+1).arg(DateLength).arg(CurrentDose).arg(SegUse);
        }
    }

    int LastMeet2LastChangeUse = 0;
    int LastMeet2LastChangeSegUse = 0;
    float Res  = 0;
    int ToDays = 0;
    QString ToDateStr;

    if(MeetCount == 0){
        Res = OriginNumber - (OriginDate.daysTo(LastMeetDate) * OriginDose);
        ToDays = Res / OriginDose;
    }else{
        QDateEdit *DateEditValLastChange = (QDateEdit *)ui->DetailTable->cellWidget(MeetCount-1,0);
        QLineEdit *UseEditValLastChange = (QLineEdit *)ui->DetailTable->cellWidget(MeetCount - 1,1);

        int LastMeet2LastChangeDays = DateEditValLastChange->date().daysTo(LastMeetDate);
        if(MorningOrNightFlag == 0){
            LastMeet2LastChangeDays -=1;
        }
        //qDebug()<<"最后调整用量到最后一次访视的天数："<<LastMeet2LastChangeDays;
        LastMeet2LastChangeUse = UseEditValLastChange->text().toFloat();
        //qDebug()<<"最后调整用量到最后一次访视的用量："<<LastMeet2LastChangeUse;
        LastMeet2LastChangeSegUse = LastMeet2LastChangeDays * LastMeet2LastChangeUse;
        //qDebug()<<"最后调整用量到最后一次访视的累计用量："<<LastMeet2LastChangeSegUse;
        Res =  OriginNumber - TotalUsed - LastMeet2LastChangeSegUse;
        ToDays = Res / LastMeet2LastChangeUse;
    }

     //qDebug()<<"剩余药量："<<Res;

     ui->label_ResidueData->setNum(Res);
     ToDateStr = LastMeetDate.addDays(ToDays).toString("yyyy年MM月dd日");

     //qDebug()<<"剩余可用天数："<<ToDays;
     ui->label_NoDate->setText(ToDateStr);

}

void MainWindow::on_pushButtonReset_clicked()
{
    ui->radioButtonNight->setChecked(false);
    ui->radioButtonMorning->setChecked(true);

    ui->dateEdit_OriDate->setDate(CurrentDataTime.date());
    ui->dateEdit_LastDate->setDate(CurrentDataTime.date());
    ui->lineEdit_0Count->clear();
    ui->lineEdit_0Use->clear();
    ui->label_ResidueData->clear();
    ui->label_NoDate->clear();

    int MeetCount = ui->DetailTable->rowCount();
    for (int Tmpi = 0; Tmpi < MeetCount; Tmpi++) {
        delete(ui->DetailTable->cellWidget(Tmpi,0));
        delete(ui->DetailTable->cellWidget(Tmpi,1));

    }
    ui->DetailTable->setRowCount(0);
    ui->DetailTable->clearContents();
}

void MainWindow::on_radioButtonMorning_clicked()
{
    MorningOrNightFlag = 0;
}

void MainWindow::on_radioButtonNight_clicked()
{
    MorningOrNightFlag = 1;
}
