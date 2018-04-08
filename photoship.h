#ifndef PHOTOSHIP_H
#define PHOTOSHIP_H

#include <QMainWindow>
#include <rgbdialog.h>

typedef struct
{
    int Shadow;
    float Midtones;
    int Highlight;
    int OutShadow;
    int OutHighlight;
}ColorLevelItem, *PColorLevelItem;

//原文使用的结构体
typedef struct
{
    ColorLevelItem Blue;
    ColorLevelItem Green;
    ColorLevelItem Red;
    ColorLevelItem RGB;
}ColorLevelData, *PColorLevelData;

namespace Ui {
class PhotoShip;
}

class PhotoShip : public QMainWindow
{
    Q_OBJECT

public:
    explicit PhotoShip(QWidget *parent = 0);
    ~PhotoShip();

private slots:
    void on_inputBtn_clicked();
    void on_saveBtn_clicked();
    void mouseMoveEvent(QMouseEvent *event);

    void on_grayBtn_clicked();

    void on_otsuBtn_clicked();

    void on_thre1Input_textEdited(const QString &arg1);

    void on_thre2Input_textEdited(const QString &arg1);

    void on_thre1Slider_valueChanged(int value);

    void on_thre2Slider_valueChanged(int value);

    void on_confThreBtn_clicked();

    void on_addBtn_clicked();

    void on_subBtn_clicked();

    void on_mul1Btn_clicked();

    void on_mul2Btn_clicked();

    void on_cutBtn_clicked();

    void on_scaleNearBtn_clicked();

    void on_scaleLinearBtn_clicked();

    void on_rotaNearBtn_clicked();

    void on_rotaLinearBtn_clicked();

    void on_contLinearBtn_clicked();

    void on_partLinearBtn_clicked();

    void on_midBtn_clicked();

    void on_averageBtn_clicked();

    void on_gaussianBtn_clicked();

    void on_rgbBtn_clicked();

private:
    Ui::PhotoShip *ui;
    //RgbDialog *rgb;
    void initial();
    void initialThre();
    int otsu(QImage img);
    QImage binary(int thre1,int thre2);
    QImage* openPhoto();
    QImage erose(QImage src,int temp_w,int temp_h,int* temp_data);
    QImage dilate(QImage src,int temp_w,int temp_h,int* temp_data);
    QImage thin(QImage src,const int iterations);

    QImage erose_gray(QImage src);
    QImage dilate_gray(QImage src);

    float calcEuclideanDiatance(int x1, int y1, int x2, int y2);
    //计算棋盘距离的函数
    int calcChessboardDistance(int x1, int y1, int x2, int y2);
    //计算麦哈顿距离(街区距离)
    int calcBlockDistance(int x1, int y1, int x2, int y2);

    bool getColorLevelTable(PColorLevelItem item,unsigned char* clTable);
    bool checkColorLevelData(PColorLevelData clData,unsigned char clTables[][256]);
    QImage imageColorLevel(QImage source,PColorLevelData clData);
    int initColorLevelData(PColorLevelData clData);

private slots:
    void receiveData(QList<int>);
    void receiveHueData(int);
    void receiveSaturData(int);
    void receiveBriData(int);
    void on_hueBtn_clicked();
    void on_saturBtn_clicked();
    void on_brightBtn_clicked();
    void on_sobelbtn_clicked();
    void on_laplBtn_clicked();
    void on_equalBtn_clicked();
    void on_graphBtn_clicked();
    void on_cannyBtn_clicked();
    void on_erosBtn_clicked();
    void on_expoBtn_clicked();
    void on_logaBtn_clicked();
    void on_dilateBtn_clicked();
    void on_openBtn_clicked();
    void on_closeBtn_clicked();
    void on_thinBtn_clicked();
    void on_thickBtn_clicked();
    void on_distanBtn_clicked();
    void on_eroGrayBtn_clicked();
    void on_dilaGrayBtn_clicked();
    void on_openGrayBtn_clicked();
    void on_closeGrayBtn_clicked();
    void on_bandBtn_clicked();
};

#endif // PHOTOSHIP_H
