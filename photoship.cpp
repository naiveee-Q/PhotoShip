#include "photoship.h"
#include "ui_photoship.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QScreen>
#include <QtMath>
#include <rgbdialog.h>
#include "huedialog.h"
#include "saturdialog.h"
#include "brightdialog.h"
#include "graphdialog.h"
#include "paintedwidget.h"
#include <vector>
using namespace std;

#define label_width_ 800
#define label_height_ 600

float scale = 1;
float width_ = 0.0;
float height_ = 0.0;

QImage org_img;

PhotoShip::PhotoShip(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PhotoShip)
{
    ui->setupUi(this);
    ui->photo->setScaledContents(true);
    ui->photo->setMouseTracking(true);
    this->centralWidget()->setMouseTracking(true);
    ui->widget->setMouseTracking(true);
    this->setMouseTracking(true);

    initial();
}

PhotoShip::~PhotoShip()
{
    delete ui;
}

void PhotoShip::receiveData(QList<int> list){
    QImage img = org_img;
    //ui->brightLabel->setText(QString("%1").arg(list[0]));
    for (int i = 0; i < img.width(); ++i) {
        for (int j = 0; j < img.height(); ++j) {
            QRgb rgb = img.pixel(i,j);
            int red = qRed(rgb);
            int green = qGreen(rgb);
            int blue = qBlue(rgb);
            if(list[0] <= 49){
                red = red * 0.02 * list[0];
            }
            if(list[0] > 49){
                red = red + (255 - red) * (list[0] - 49) * 0.02;
            }
            if(list[1] <= 49){
                green = green * 0.02 * list[1];
            }
            if(list[1] > 49){
                green = green + (255 - green) * (list[1] - 49) * 0.02;
            }
            if(list[2] <= 49){
                blue = blue * 0.02 * list[2];
            }
            if(list[0] > 49){
                blue = blue + (255 - blue) * (list[2] - 49) * 0.02;
            }

            img.setPixel(i,j,QColor(red,green,blue).rgb());
        }
    }
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::receiveHueData(int value){
    if(value < -100 || value > 100){
        return;
    }

    QImage img = ui->photo->pixmap()->toImage();
    if(value > 0){
        for(int i = 0;i < img.width();++i){
            for(int j = 0;j < img.height();++j){
                QColor color = org_img.pixelColor(i,j);
                int H_cur = color.hslHue();
                color.setHsl(H_cur + (360 - H_cur) * value * 0.01,color.hslSaturation(),color.lightness());
                img.setPixel(i,j,color.rgb());
            }
        }
    }else if(value < 0){
        for(int i = 0;i < img.width();++i){
            for(int j = 0;j < img.height();++j){
                QColor color = org_img.pixelColor(i,j);
                int H_cur = color.hslHue();
                color.setHsl(H_cur + H_cur * value * 0.01,color.hslSaturation(),color.lightness());
                img.setPixel(i,j,color.rgb());
            }
        }
    }
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::receiveSaturData(int value){
    QImage img = ui->photo->pixmap()->toImage();
    if(value > 0){
        for(int i = 0;i < img.width();++i){
            for(int j = 0;j < img.height();++j){
                QColor color = org_img.pixelColor(i,j);
                int S_cur = color.hslSaturation();
                color.setHsl(color.hslHue(),S_cur + (255 - S_cur) * value * 0.01,color.lightness());
                img.setPixel(i,j,color.rgb());
            }
        }
    }else if(value < 0){
        for(int i = 0;i < img.width();++i){
            for(int j = 0;j < img.height();++j){
                QColor color = org_img.pixelColor(i,j);
                int S_cur = color.hslSaturation();
                color.setHsl(color.hslHue(),S_cur + S_cur * value * 0.01,color.lightness());
                img.setPixel(i,j,color.rgb());
            }
        }
    }
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::receiveBriData(int value){
    QImage img = ui->photo->pixmap()->toImage();
    if(value > 0){
        for(int i = 0;i < img.width();++i){
            for(int j = 0;j < img.height();++j){
                QColor color = org_img.pixelColor(i,j);
                int L_cur = color.lightness();
                color.setHsl(color.hslHue(),color.hslSaturation(),L_cur + (255 - L_cur) * value * 0.01);
                img.setPixel(i,j,color.rgb());
            }
        }
    }else if(value < 0){
        for(int i = 0;i < img.width();++i){
            for(int j = 0;j < img.height();++j){
                QColor color = org_img.pixelColor(i,j);
                int L_cur = color.lightness();
                color.setHsl(color.hslHue(),color.hslSaturation(),L_cur + L_cur * value * 0.01);
                img.setPixel(i,j,color.rgb());
            }
        }
    }
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::initialThre(){
    ui->thre1Input->setText("0");
    ui->thre1Slider->setValue(0);
    ui->thre1Slider->setRange(0,255);

    ui->thre2Input->setText("255");
    ui->thre2Slider->setValue(255);
    ui->thre2Slider->setRange(0,255);
}

void PhotoShip::initial(){
    initialThre();
}

QImage* PhotoShip::openPhoto(){
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择图像"),
        "",
        tr("Images (*.png *.bmp *.jpg *.tif *.GIF)")); //选择路径
    if(filename.isEmpty())
    {
        return (new QImage);
    }
    else
    {
        initial();
        QImage* img=new QImage;
        if(!(img->load(filename))) //加载图像
        {
            QMessageBox::information(this,
            tr("打开图像失败"),
            tr("打开图像失败!"));
            return (new QImage);
        }
        return img;
    }
}

void PhotoShip::mouseMoveEvent(QMouseEvent *event){
    QPoint m = mapFromGlobal(QCursor::pos());
    float x = (m.x()*1.0-ui->widget->x()-ui->photo->x()) * 1.0;
    float y = (m.y()*1.0-ui->widget->y()- 2 * ui->photo->y()) * 1.0;
    if(ui->photo->text() == "input photo first"){
        return;
    }
    QImage img = ui->photo->pixmap()->toImage();
    if(x>=0 && y>=0 && x<=width_- 2 * ui->photo->x() && y<=height_- 2 * ui->photo->y() && x < img.width() && y < img.height()){
        QRgb rgb = img.pixel(x * 1.0 /scale,y * 1.0/scale);
        ui->mousePos->setText(QString("(%1,%2)").arg(x).arg(y));
        ui->mousePix->setText(QString("(%1,%2,%3)").arg(qRed(rgb)).arg(qGreen(rgb)).arg(qBlue(rgb)));
    }else{
        ui->mousePos->setText(QString("not exist"));
        ui->mousePix->setText(QString("not exist"));
    }
}

void PhotoShip::on_inputBtn_clicked()
{
    QImage* img = openPhoto();

    if(!img->isNull()){
        float scale_w = (label_width_*1.0) / (img->width());  //宽的比例
        float scale_h = (label_height_*1.0) / (img->height());  //高的比例

        float new_width, new_height;  //新的宽和高
        if ((label_width_ >= img->width()) && (label_height_ >= img->height()))  //图片的原始宽和高均小于显示控件的宽和高
        {
            new_width = img->width();
            new_height = img->height();
            scale = 1 * (new_width - 2 * ui->photo->x()) * 1.0 / new_width;
        }
        else if (scale_w > scale_h)
        {
            new_width = img->width() * scale_h;
            new_height = img->height() * scale_h;
            scale = scale_h * (new_height - 2 * ui->photo->y()) * 1.0 / new_height;
        }
        else
        {
            new_width = img->width() * scale_w;
            new_height = img->height() * scale_w;
            scale = scale_w * (new_width - 2 * ui->photo->x()) * 1.0 / new_width;
        }
        width_ = new_width;
        height_ = new_height;
        org_img = *img;
        ui->widget->resize(new_width ,new_height);
        ui->photo->setPixmap(QPixmap::fromImage(*img)); //显示图像
    }
}

void PhotoShip::on_saveBtn_clicked()
{
    if(ui->photo->text() == "input photo first"){
        QMessageBox::information(this,
        tr("Save Photo Error"),
        tr("Please input first!"));
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Image"),
        "",
        tr("Images (*.png *.bmp *.jpg *.tif *.GIF)")); //选择路径
    QImage img = ui->photo->pixmap()->toImage();
    img.save(filename);
}


void PhotoShip::on_grayBtn_clicked()
{
    if(ui->photo->text() == "input photo first"){
        return;
    }
    QImage img = org_img;

    for(int i =0;i<img.width();i++)
    {
         for(int j=0;j<img.height();j++)
         {
          QRgb rgb=img.pixel(i,j);
          int r=qRed(rgb);
          int g=qGreen(rgb);
          int b=qBlue(rgb);
          int gray = r * 0.3 + g * 0.59 + b * 0.11;
          img.setPixel(i,j,QColor(gray,gray,gray).rgb());
         }
    }
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}


int PhotoShip::otsu(QImage img){
    int threshold;
    int pixelCount[256];
    float pixelPro[256];

    //initial
    for(int i = 0;i < 256;i++){
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    //count the number of pixels of every gray level
    for(int i = 0;i < img.width();i++){
        for(int j = 0;j < img.height();j++){
            QRgb rgb = img.pixel(i,j);
            int gray = qRed(rgb) * 0.3 + qGreen(rgb) * 0.59 + qBlue(rgb) * 0.11;
            pixelCount[gray]++;
        }
    }

    //calculate the proportion of every gray level
    for(int i = 0;i < 256;i++){
        pixelPro[i] = pixelCount[i] * 1.0 / (img.width() * img.height() * 1.0);
    }

    //calculate the best threshold value
    float w0, w1, u0tmp, u1tmp, u0, u1, u,deltaTmp, deltaMax = 0;
    for(int i = 0;i < 256;i++){
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for(int j = 0;j < 256;j++){
            if(j <= i){//background
                w0 += pixelPro[j];
                u0tmp += j * pixelPro[j];
            }else{//foreground
                w1 += pixelPro[j];
                u1tmp += j * pixelPro[j];
            }
        }
        u0 = u0tmp / w0;        //第一类的平均灰度
        u1 = u1tmp / w1;        //第二类的平均灰度
        u = u0tmp + u1tmp;      //整幅图像的平均灰度

        //计算类间方差
        deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);
        //找出最大类间方差以及对应的阈值
        if(deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = i;
        }
    }
    return threshold;
}

void PhotoShip::on_otsuBtn_clicked()
{
    if(ui->photo->text() == "input photo first"){
        return;
    }

    QImage img = org_img;
    int threshold = otsu(img);

    for(int i = 0;i < img.width();i++){
        for(int j = 0;j < img.height();j++){
            QRgb rgb = img.pixel(i,j);
            int gray = qRed(rgb) * 0.3 + qGreen(rgb) * 0.59 + qBlue(rgb) * 0.11;
            if(gray <= threshold){
                img.setPixel(i,j,QColor(0,0,0).rgb());
            }else{
                img.setPixel(i,j,QColor(255,255,255).rgb());
            }
        }
    }
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

QImage PhotoShip::binary(int thre1, int thre2){
    QImage img = org_img;

    for(int i = 0;i < img.width();i++){
        for(int j = 0;j < img.height();j++){
            QRgb rgb = img.pixel(i,j);
            int gray = qRed(rgb) * 0.3 + qGreen(rgb) * 0.59 + qBlue(rgb) * 0.11;
            if(gray > thre1 && gray < thre2){
                img.setPixel(i,j,QColor(255,255,255).rgb());
            }else{
                img.setPixel(i,j,QColor(0,0,0).rgb());
            }
        }
    }
    return img;
}

void PhotoShip::on_thre1Input_textEdited(const QString &arg1)
{
    int thre1 = QString(arg1).toInt();
    int thre2 = QString(ui->thre2Input->text()).toInt();
    ui->thre1Slider->setValue(thre1);

    if(thre2 < thre1){
        QMessageBox::information(this,
        tr("thre2 must > thre1"),
        tr("thre2 must > thre1!"));
        return;
    }

    //check photo
    if(ui->photo->text() == "input photo first"){
        return;
    }

    QImage img = binary(thre1,thre2);
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_thre2Input_textEdited(const QString &arg1)
{
    int thre2 = QString(arg1).toInt();
    int thre1 = QString(ui->thre1Input->text()).toInt();
    ui->thre2Slider->setValue(thre1);

    if(thre2 < thre1){
        QMessageBox::information(this,
        tr("thre2 must > thre1"),
        tr("thre2 must > thre1!"));
        return;
    }

    //check photo
    if(ui->photo->text() == "input photo first"){
        return;
    }

    QImage img = binary(thre1,thre2);
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_thre1Slider_valueChanged(int value)
{
    ui->thre1Input->setText(QString("%1").arg(value));
    int thre1 = value;
    int thre2 = QString(ui->thre2Input->text()).toInt();

    if(thre2 < thre1){
        QMessageBox::information(this,
        tr("thre2 must > thre1"),
        tr("thre2 must > thre1!"));
        return;
    }

    //check photo
    if(ui->photo->text() == "input photo first"){
        return;
    }

    QImage img = binary(thre1,thre2);
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_thre2Slider_valueChanged(int value)
{
    ui->thre2Input->setText(QString("%1").arg(value));
    int thre2 = value;
    int thre1 = QString(ui->thre1Input->text()).toInt();

    if(thre2 < thre1){
        QMessageBox::information(this,
        tr("thre2 must > thre1"),
        tr("thre2 must > thre1!"));
        return;
    }

    //check photo
    if(ui->photo->text() == "input photo first"){
        return;
    }

    QImage img = binary(thre1,thre2);
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_confThreBtn_clicked()
{
    org_img = ui->photo->pixmap()->toImage();
    initialThre();
    ui->photo->setPixmap(QPixmap::fromImage(org_img));
}

void PhotoShip::on_addBtn_clicked()
{
    QImage *add_img = openPhoto();
    QImage img = org_img;

    for(int i = 0;i < add_img->width() && i < img.width();++i){
        for(int j = 0;j < add_img->height() && j < img.height();++j){
            QRgb rgb = img.pixel(i,j);
            QRgb add_rgb = add_img->pixel(i,j);
            img.setPixel(i,j,QColor((qRed(rgb) + qRed(add_rgb)) * 0.5, (qGreen(rgb) + qGreen(add_rgb)) * 0.5, (qBlue(rgb) + qBlue(add_rgb)) * 0.5).rgb());
        }
    }
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(org_img));
}

void PhotoShip::on_subBtn_clicked()
{
    QImage *sub_img = openPhoto();
    QImage img = org_img;

    for(int i = 0;i < sub_img->width() && i < img.width();++i){
        for(int j = 0;j < sub_img->height() && j < img.height();++j){
            QRgb rgb = img.pixel(i,j);
            QRgb sub_rgb = sub_img->pixel(i,j);
            img.setPixel(i,j,QColor((qRed(rgb) - qRed(sub_rgb)), (qGreen(rgb) - qGreen(sub_rgb)), (qBlue(rgb) - qBlue(sub_rgb))).rgb());
        }
    }
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(org_img));
}

void PhotoShip::on_mul1Btn_clicked()
{
    QImage *mul_img = openPhoto();
    QImage img = org_img;
    *mul_img = mul_img->scaled(img.height(),img.width());

    for(int i = 0;i < mul_img->height() && i < img.width();++i){
        for(int s = 0;s < mul_img->height() && s < img.width();++s){
            int tmp_red = 0;
            int tmp_green = 0;
            int tmp_blue = 0;
        for(int j = 0;j < mul_img->width() && j < img.height();++j){
            QRgb rgb = img.pixel(i,j);
            QRgb mul_rgb = mul_img->pixel(j,s);
            tmp_red += qRed(rgb) * qRed(mul_rgb);
            tmp_green += qGreen(rgb) * qGreen(mul_rgb);
            tmp_blue += qBlue(rgb) * qBlue(mul_rgb);
        }
        if(tmp_red = tmp_red * 1.0 / (160 * img.height()) > 255){
            tmp_red = 255;
        }
        if(tmp_green = tmp_green * 1.0 / (160 * img.height()) > 255){
            tmp_green = 255;
        }
        if(tmp_blue =tmp_blue * 1.0 / (160 * img.height()) > 255){
            tmp_blue = 255;
        }
        img.setPixel(i,s,QColor(tmp_red,tmp_green,tmp_blue).rgb());
        }
    }
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(org_img));
}

void PhotoShip::on_mul2Btn_clicked()
{
    QImage *mul_img = openPhoto();
    QImage img = org_img;
    *mul_img = mul_img->scaled(img.width(),img.height());

    for(int i = 0;i < mul_img->width() && i < img.width();++i){
        for(int j = 0;j < mul_img->height() && j < img.height();++j){
            QRgb rgb = img.pixel(i,j);
            QRgb mul_rgb = mul_img->pixel(i,j);

            int tmp_red = qRed(rgb) * qRed(mul_rgb);
            int tmp_green = qGreen(rgb) * qGreen(mul_rgb);
            int tmp_blue = qBlue(rgb) * qBlue(mul_rgb);

            tmp_red = tmp_red * 1.0 / 255;
            tmp_green = tmp_green * 1.0 / 255;
            tmp_blue =tmp_blue * 1.0 / 255;

            img.setPixel(i,j,QColor(tmp_red,tmp_green,tmp_blue).rgb());
        }
    }
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(org_img));
}

void PhotoShip::on_cutBtn_clicked()
{
    int x1 = ui->cut1xValue->text().toInt();
    int x2 = ui->cut2xValue->text().toInt();
    int y1 = ui->cut1yValue->text().toInt();
    int y2 = ui->cut2yValue->text().toInt();

    if(x1 > x2 || y1 > y2){
        return;
    }

    //ui->blueLabel->setText(QString("%1 %2 %3 %4").arg(x1).arg(x2).arg(y1).arg(y2));

    QImage img = ui->photo->pixmap()->toImage();
    img = img.copy(x1 * 1.0 / scale,y1 * 1.0 / scale,x2 * 1.0 / scale,y2 * 1.0 / scale);

    float scale_w = (label_width_*1.0) / (img.width());  //宽的比例
    float scale_h = (label_height_*1.0) / (img.height());  //高的比例

    float new_width, new_height;  //新的宽和高
    if ((label_width_ >= img.width()) && (label_height_ >= img.height()))  //图片的原始宽和高均小于显示控件的宽和高
    {
        new_width = img.width();
        new_height = img.height();
        scale = 1 * (new_width - 2 * ui->photo->x()) * 1.0 / new_width;
    }
    else if (scale_w > scale_h)
    {
        new_width = img.width() * scale_h;
        new_height = img.height() * scale_h;
        scale = scale_h * (new_height - 2 * ui->photo->y()) * 1.0 / new_height;
    }
    else
    {
        new_width = img.width() * scale_w;
        new_height = img.height() * scale_w;
        scale = scale_w * (new_width - 2 * ui->photo->x()) * 1.0 / new_width;
    }
    width_ = new_width;
    height_ = new_height;
    org_img = img;
    ui->widget->resize(new_width,new_height);
    ui->photo->setPixmap(QPixmap::fromImage(img)); //显示图像
}

void PhotoShip::on_scaleNearBtn_clicked()
{
    float new_width = ui->widthInput->text().toFloat();
    float new_height = ui->heightInput->text().toFloat();

    //ui->blueLabel->setText(QString("%1 %2").arg(new_width).arg(new_height));

    float w_scale_rate = new_width * 1.0 / org_img.width();
    float h_scale_rate = new_height * 1.0 / org_img.height();



    QImage img = QImage(new_width,new_height,QImage::Format_RGB32);

    for(int i = 0; i < new_width;++i){
        for(int j = 0;j < new_height;++j){
            float org_i = i * 1.0 / w_scale_rate;
            float org_j = j * 1.0 / h_scale_rate;

            img.setPixel(i,j,org_img.pixel(org_i,org_j));
        }
    }
    scale = 1;
    org_img = img;
    ui->widget->resize(new_width + 24,new_height + 24);
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_scaleLinearBtn_clicked()
{
    float new_width = ui->widthInput->text().toFloat();
    float new_height = ui->heightInput->text().toFloat();

    float w_scale_rate = org_img.width() * 1.0 / new_width;
    float h_scale_rate = org_img.height() * 1.0 / new_height;



    QImage img = QImage(new_width,new_height,QImage::Format_RGB32);

    for (int org_i = 0; org_i < new_height; org_i++){
        for (int org_j = 0; org_j < new_width; org_j++){
                float i_scale = h_scale_rate * org_i;
                float j_scale = w_scale_rate * org_j;
                //cout << "i_scale: " << i_scale <<" j_scale: "<< j_scale << endl;

                int i = i_scale;
                int j = j_scale;
                float u = i_scale - i;
                float v = j_scale - j;

                //注意处理边界问题，容易越界
                if (i + 1 >= org_img.height() || j + 1 >= org_img.width())
                {
                    img.setPixel(org_j,org_i,org_img.pixel(j,i));
                }else{

                QRgb x1 = org_img.pixel(j,i);  //f(i,j)
                QRgb x2 = org_img.pixel(j+1,i);  //f(i,j+1)
                QRgb x3 = org_img.pixel(j,i+1);   //(i+1,j)
                QRgb x4 = org_img.pixel(j+1,i+1);  //f(i+1,j+1)

                img.setPixel(org_j,org_i,QColor((1.0 *(1-u)*(1-v)*qRed(x1)+(1-u)*v*qRed(x2)+u*(1-v)*qRed(x3)+u*v*qRed(x4)),
                                        (1.0 *(1-u)*(1-v)*qGreen(x1)+(1-u)*v*qGreen(x2)+u*(1-v)*qGreen(x3)+u*v*qGreen(x4)),
                                        (1.0 *(1-u)*(1-v)*qBlue(x1)+(1-u)*v*qBlue(x2)+u*(1-v)*qBlue(x3)+u*v*qBlue(x4))).rgb());
                }
        }
    }
    scale = 1;
    org_img = img;
    ui->widget->resize(new_width + 24,new_height + 24);
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_rotaNearBtn_clicked()
{

    float rotaValue = ui->rotaInput->text().toInt() * 1.0 / 180 * 3.14;

    float new_width = org_img.width() * qCos(rotaValue) + org_img.height() * qSin(rotaValue);
    float new_height = org_img.width() * qSin(rotaValue) + org_img.width() * qCos(rotaValue);

    QImage img = QImage(new_width,new_height,QImage::Format_RGB32);
    for(int i = 0;i < new_width - 1;++i){
        for(int j = 0;j < new_height - 1;++j){
            int x = i - 0.5 * new_width;
            int y = j - 0.5 * new_height;
            img.setPixel(i,j,QColor(0,0,0).rgb());
            int old_x = x * qCos(rotaValue) + y * qSin(rotaValue);
            int old_y = -1 * x * qSin(rotaValue) + y * qCos(rotaValue);
            int old_i = old_x + 0.5 * org_img.width();
            int old_j = old_y + 0.5 * org_img.height();
            if(old_i >= 0 && old_j >= 0 && old_i < org_img.width() && old_j < org_img.height()){
                //ui->blueLabel->setText("hello");
                img.setPixel(i,j,org_img.pixel(old_i,old_j));
            }

        }
    }

    //org_img = img;
    ui->widget->resize(new_width + 24,new_height + 24);
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_rotaLinearBtn_clicked()
{
    float rotaValue = ui->rotaInput->text().toInt() * 1.0 / 180 * 3.141592654;

    float new_width = org_img.width() * qCos(rotaValue) + org_img.height() * qSin(rotaValue);
    float new_height = org_img.width() * qSin(rotaValue) + org_img.width() * qCos(rotaValue);

    QImage img = QImage(new_width,new_height,QImage::Format_RGB32);
    for(int i = 0;i < new_width - 1;++i){
        for(int j = 0;j < new_height - 1;++j){
            int x = i - 0.5 * new_width;
            int y = j - 0.5 * new_height;
            img.setPixel(i,j,QColor(0,0,0).rgb());
            float old_x = x * 1.0 * qCos(rotaValue) + y * qSin(rotaValue);
            float old_y = -1 * 1.0 * x * qSin(rotaValue) + y * qCos(rotaValue);
            float old_i = old_x + 0.5 * org_img.width();
            float old_j = old_y + 0.5 * org_img.height();

            int o_i = old_i;
            int o_j = old_j;

            if(o_i > 0 && o_j > 0 && o_i < org_img.width() && o_j < org_img.height()){
            float u = old_i - o_i;
            float v = old_j - o_j;

            //注意处理边界问题，容易越界
            if (o_i + 1 >= org_img.width() || o_j + 1 >= org_img.height())
            {
                img.setPixel(i,j,org_img.pixel(o_i,o_j));
            }else{

            QRgb x1 = org_img.pixel(o_i,o_j);  //f(i,j)
            QRgb x2 = org_img.pixel(o_i+1,o_j);  //f(i,j+1)
            QRgb x3 = org_img.pixel(o_i,o_j+1);   //(i+1,j)
            QRgb x4 = org_img.pixel(o_i+1,o_j+1);  //f(i+1,j+1)

            img.setPixel(i,j,QColor((1.0 *(1-u)*(1-v)*qRed(x1)+(1-u)*v*qRed(x2)+u*(1-v)*qRed(x3)+u*v*qRed(x4)),
                                    (1.0 *(1-u)*(1-v)*qGreen(x1)+(1-u)*v*qGreen(x2)+u*(1-v)*qGreen(x3)+u*v*qGreen(x4)),
                                    (1.0 *(1-u)*(1-v)*qBlue(x1)+(1-u)*v*qBlue(x2)+u*(1-v)*qBlue(x3)+u*v*qBlue(x4))).rgb());
            }
            }
        }
    }

    //org_img = img;
    ui->widget->resize(new_width + 24,new_height + 24);
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_contLinearBtn_clicked()
{
    int k = ui->contValue1Input->text().toInt();

    QImage img = org_img;
    for(int i = 0; i < org_img.width();++i){
        for(int j = 0;j < org_img.height();++j){
            int red = qRed(org_img.pixel(i,j)) * 0.01 * k > 255?255:qRed(org_img.pixel(i,j)) * 0.01 * k;
            int green = qGreen(org_img.pixel(i,j)) * 0.01 * k > 255?255:qGreen(org_img.pixel(i,j)) * 0.01 * k;
            int blue = qBlue(org_img.pixel(i,j)) * 0.01 * k > 255?255:qBlue(org_img.pixel(i,j)) * 0.01 * k;
            img.setPixel(i,j,QColor(red, green,blue).rgb());
        }
    }
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_partLinearBtn_clicked()
{
    int k1 = ui->contValue1Input->text().toInt();
    int k2 = ui->contValue2Input->text().toInt();
    int k3 = ui->contValue3Input->text().toInt();

    int x1 = ui->contX1Input->text().toInt();
    int x2 = ui->contX2Input->text().toInt();
    int x3 = ui->contX3Input->text().toInt();

    QImage img = org_img;
    for(int i = 0;i < org_img.width();++i){
        for(int j = 0;j < org_img.height();++j){
            QRgb rgb = org_img.pixel(i,j);
            int red = qRed(rgb);
            int green = qGreen(rgb);
            int blue = qBlue(rgb);

            int gray = red * 0.3 + green * 0.59 + blue * 0.11;

            if(gray < x1){
                int red = qRed(org_img.pixel(i,j)) * 0.01 * k1 > 255?255:qRed(org_img.pixel(i,j)) * 0.01 * k1;
                int green = qGreen(org_img.pixel(i,j)) * 0.01 * k1 > 255?255:qGreen(org_img.pixel(i,j)) * 0.01 * k1;
                int blue = qBlue(org_img.pixel(i,j)) * 0.01 * k1 > 255?255:qBlue(org_img.pixel(i,j)) * 0.01 * k1;
                img.setPixel(i,j,QColor(red, green,blue).rgb());
            }else if(gray < x2){
                int red = qRed(org_img.pixel(i,j)) * 0.01 * k2 + k1 * x1 > 255?255:qRed(org_img.pixel(i,j)) * 0.01 * k2 + k1 * x1;
                int green = qGreen(org_img.pixel(i,j)) * 0.01 * k2 + k1 * x1 > 255?255:qGreen(org_img.pixel(i,j)) * 0.01 * k2 + k1 * x1;
                int blue = qBlue(org_img.pixel(i,j)) * 0.01 * k2 + k1 * x1 > 255?255:qBlue(org_img.pixel(i,j)) * 0.01 * k2 + k1 * x1;
                img.setPixel(i,j,QColor(red, green,blue).rgb());
            }else{
                int red = qRed(org_img.pixel(i,j)) * 0.01 * k3 + k2 * (x2 - x1) + k1 * x1 > 255?255:qRed(org_img.pixel(i,j)) * 0.01 * k3 + k2 * (x2 - x1) + k1 * x1;
                int green = qGreen(org_img.pixel(i,j)) * 0.01 * k3 + k2 * (x2 - x1) + k1 * x1 > 255?255:qGreen(org_img.pixel(i,j)) * 0.01 * k3 + k2 * (x2 - x1) + k1 * x1;
                int blue = qBlue(org_img.pixel(i,j)) * 0.01 * k3 + k2 * (x2 - x1) + k1 * x1 > 255?255:qBlue(org_img.pixel(i,j)) * 0.01 * k3 + k2 * (x2 - x1) + k1 * x1;
                img.setPixel(i,j,QColor(red, green,blue).rgb());
            }
        }
    }
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_midBtn_clicked()
{
    int value = ui->contValue1Input->text().toInt();
    QImage img = org_img;

    for(int i = (value - 1) / 2; i < img.width() - (value - 1) / 2; ++i){
        for(int j = (value - 1)/2;j < img.height()-(value -1)/2;++j){
            QList<float> list_gray;
            QList<QList<float>> list_rgb;
            for(int n = 0;n < value;++n){
                for(int m = 0;m < value;++m){
                    QRgb rgb = org_img.pixel(i - (value - 1) / 2 + n,j - (value - 1) / 2 + m);
                    QList<float> cur_rgb;
                    float red = qRed(rgb);
                    float green = qGreen(rgb);
                    float blue = qBlue(rgb);
                    float gray = red*0.3+green*0.59+blue*0.11;
                    cur_rgb.append(gray);
                    cur_rgb.append(red);
                    cur_rgb.append(green);
                    cur_rgb.append(blue);
                    list_gray.append(gray);
                    list_rgb.append(cur_rgb);
                }
            }
            qSort(list_gray);

            float red = 0;
            float green = 0;
            float blue = 0;
            for(int n = 0;n < list_rgb.size();++n){
                if(list_rgb[n][0] == list_gray[(value - 1) / 2]){
                    red = list_rgb[n][1];
                    green = list_rgb[n][2];
                    blue = list_rgb[n][3];
                }
            }

            img.setPixel(i,j,QColor(red,green,blue).rgb());
        }
    }
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_averageBtn_clicked()
{
    int value = ui->contValue1Input->text().toInt();
    QImage img = org_img;

    //int cont = 0;
    for(int i = (value - 1) / 2; i < img.width() - (value - 1) / 2; ++i){
        for(int j = (value - 1)/2;j < img.height()-(value -1)/2;++j){
            float red = 0;
            float green = 0;
            float blue = 0;
            //cont = 0;
            for(int n = 0;n < value;++n){
                for(int m = 0;m < value;++m){
                    //ui->blueLabel->setText(QString("%1 %2").arg(n).arg(m));
                    QRgb rgb = org_img.pixel(i - (value - 1) / 2 + n,j - (value - 1) / 2 + m);
                    red += qRed(rgb);
                    green += qGreen(rgb);
                    blue += qBlue(rgb);
                    /*if(i == 130 && j == 130){
                        ui->blueLabel->setText(QString("%1 %2 %3").arg(red).arg(green).arg(blue));
                    }*/
                }
            }
            red = 1.0 * red / (value * value);
            green = 1.0 * green / (value * value);
            blue = 1.0 * blue / (value * value);
            img.setPixel(i,j,QColor(red,green,blue).rgb());
        }
    }
    //ui->blueLabel->setText(QString("%1").arg(img.width() - (value - 1) / 2));
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_gaussianBtn_clicked()
{
    int temp[25] = {1,4,7,4,1,
                    4,16,26,16,4,
                    7,26,41,26,7,
                    4,16,26,16,4,
                    1,4,7,4,1};
    QImage img = org_img;
    for(int i = 0;i < img.width() - 2;i++){
        for(int j = 0;j <img.height() - 2;j++){
            int sum_red = 0;
            int sum_green = 0;
            int sum_blue = 0;
            int index = 0;
            for(int m = i - 2;m < i + 3;m++){
                for(int n = j - 2;n < j+3;n++){
                    QRgb rgb = img.pixel(m,n);
                    sum_red += qRed(rgb) * temp[index];
                    sum_green += qGreen(rgb) * temp[index];
                    sum_blue += qBlue(rgb) *temp[index++];
                }
            }
            sum_red /= 273;
            sum_green /= 273;
            sum_blue /= 273;

            if(sum_red > 255){
                sum_red = 255;
            }
            if(sum_green > 255){
                sum_green = 255;
            }
            if(sum_blue > 255){
                sum_blue = 255;
            }

            img.setPixel(i,j,QColor(sum_red,sum_green,sum_blue).rgb());
        }
    }
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_rgbBtn_clicked()
{
    // slot
    RgbDialog *rgb = new RgbDialog(this);
    connect(rgb, SIGNAL(sendData(QList<int>)), this, SLOT(receiveData(QList<int>)));
    rgb->setModal(true);
    rgb->show();
}

void PhotoShip::on_hueBtn_clicked()
{
    // slot
    HueDialog *hue = new HueDialog(this);
    connect(hue, SIGNAL(sendHueData(int)), this, SLOT(receiveHueData(int)));
    hue->setModal(true);
    hue->show();
}

void PhotoShip::on_saturBtn_clicked()
{
    // slot
    SaturDialog *satur = new SaturDialog(this);
    connect(satur, SIGNAL(sendSaturData(int)), this, SLOT(receiveSaturData(int)));
    satur->setModal(true);
    satur->show();
}

void PhotoShip::on_brightBtn_clicked()
{
    BrightDialog *bright = new BrightDialog(this);
    connect(bright, SIGNAL(sendBriData(int)), this, SLOT(receiveBriData(int)));
    bright->setModal(true);
    bright->show();
}

void PhotoShip::on_sobelbtn_clicked()
{
    QImage result = org_img;
    //double scale = 0.8;
    int level;
    int aHr, aHg, aHb;  // horizontal
    int aVr, aVg, aVb;  // vertical
    int aH, aV, a;

    for(int x = 1; x < org_img.width()-1; x ++){
        for(int y = 1; y < org_img.height()-1; y++){
            QRgb left = org_img.pixel(x -1,y);
            QRgb right = org_img.pixel(x+1,y);
            QRgb left_top = org_img.pixel(x-1,y-1);
            QRgb left_buttom = org_img.pixel(x-1,y+1);
            QRgb right_top = org_img.pixel(x+1,y-1);
            QRgb right_buttom = org_img.pixel(x+1,y+1);
            QRgb top = org_img.pixel(x,y-1);
            QRgb buttom = org_img.pixel(x,y+1);

            // 3x3
            aHr = abs(
                    (qRed(left_top) + qRed(top)*2 + qRed(right_top))
                    -
                    (qRed(left_buttom) + qRed(buttom)*2 + qRed(right_buttom))
                    );
            aVr = abs(
                    (qRed(left_top) +qRed(left)*2 + qRed(left_buttom))
                    -
                    (qRed(right_top) + qRed(right)*2 + qRed(right_buttom))
                    );
            aHg = abs(
                    (qGreen(left_top) + qGreen(top)*2 + qGreen(right_top))
                    -
                    (qGreen(left_buttom) + qGreen(buttom)*2 + qGreen(right_buttom))
                    );
            aVg = abs(
                    (qGreen(left_top) +qGreen(left)*2 + qGreen(left_buttom))
                    -
                    (qGreen(right_top) + qGreen(right)*2 + qGreen(right_buttom))
                    );
            aHb = abs(
                    (qBlue(left_top) + qBlue(top)*2 + qBlue(right_top))
                    -
                    (qBlue(left_buttom) + qBlue(buttom)*2 + qBlue(right_buttom))
                    );
            aVb = abs(
                    (qBlue(left_top) +qBlue(left)*2 + qBlue(left_buttom))
                    -
                    (qBlue(right_top) + qBlue(right)*2 + qBlue(right_buttom))
                    );
            // calculate for 3 color seperatly
            aH = (aHr * 0.3 + aHg * 0.59 + aHb * 0.11) ;
            aV = (aVr * 0.3 + aVg * 0.59 + aVb * 0.11) ;
            /*if(aH > aV){
                level = aH;
            }else{
                level = aV;
            }*/
            level = abs(aH) + abs(aV);
            //level = level * scale;
            a = level > 255? 255:level;
            //a = a*scale;
            if(a==255){
                result.setPixel(x,y,QColor(255,255,255).rgb());
            }else{
                result.setPixel(x,y,QColor(0,0,0).rgb());
            }

        }
    }
    org_img = result;
    ui->photo->setPixmap(QPixmap::fromImage(result));
}

void PhotoShip::on_laplBtn_clicked()
{
    QImage result = org_img;

    for(int x = 1; x < result.width() - 1; x ++){
        for(int y = 1; y < result.height() - 1; y++){
            QRgb left_rgb = org_img.pixel(x-1,y);
            QRgb right_rgb = org_img.pixel(x+1,y);
            QRgb top_rgb = org_img.pixel(x,y-1);
            QRgb buttom_rgb = org_img.pixel(x,y+1);
            QRgb center_rgb = org_img.pixel(x,y);

            int center = qRed(center_rgb) * 0.3 + qGreen(center_rgb) * 0.59 + qBlue(center_rgb) * 0.11;
            int left = qRed(left_rgb) * 0.3 + qGreen(left_rgb) * 0.59 + qBlue(left_rgb) * 0.11;
            int right = qRed(right_rgb) * 0.3 + qGreen(right_rgb) * 0.59 + qBlue(right_rgb) * 0.11;
            int top = qRed(top_rgb) * 0.3 + qGreen(top_rgb) * 0.59 + qBlue(top_rgb) * 0.11;
            int buttom = qRed(buttom_rgb) * 0.3 + qGreen(buttom_rgb) * 0.59 + qBlue(buttom_rgb) * 0.11;


            // f2x = right - 2center + left
            // f2y = top - 2center + buttom
            // delta2f = f2x + f2y
            int delta2f = abs(4 * center - (top + left + right + buttom));


            // 3x
            //int a = delta2f > ? 255:delta2f;
            if(delta2f > 50){
                result.setPixel(x,y,QColor(255,255,255).rgb());
            }else{
                result.setPixel(x,y,QColor(0,0,0).rgb());
            }

        }
    }
    org_img = result;
    ui->photo->setPixmap(QPixmap::fromImage(result));
}

void PhotoShip::on_equalBtn_clicked()
{
    int gray[256] = { 0 };  //记录每个灰度级别下的像素个数
    double gray_prob[256] = { 0 };  //记录灰度分布密度
    double gray_distribution[256] = { 0 };  //记录累计密度
    int gray_equal[256] = { 0 };  //均衡化后的灰度值

    int gray_sum = 0;  //像素总数

    QImage output = org_img;
    gray_sum = org_img.width() * org_img.height();

    //统计每个灰度下的像素个数
    unsigned char* grayData = org_img.bits();
    for (int i = 0; i < org_img.width(); i++)
    {
        for (int j = 0; j < org_img.height(); j++)
        {
            int index = int(*grayData);
            gray[index] = gray[index]+1;
            grayData += 3;
        }
    }


    //统计灰度频率
    for (int i = 0; i < 256; i++)
    {
        gray_prob[i] = ((double)gray[i] / gray_sum);
    }

    //计算累计密度
    gray_distribution[0] = gray_prob[0];
    for (int i = 1; i < 256; i++)
    {
        gray_distribution[i] = gray_distribution[i-1] +gray_prob[i];
    }

    //重新计算均衡化后的灰度值，四舍五入。参考公式：(N-1)*T+0.5
    for (int i = 0; i < 256; i++)
    {
        gray_equal[i] = (255 * gray_distribution[i] + 0.5);
    }


    //直方图均衡化,更新原图每个点的像素值
    for (int i = 0; i < org_img.width(); i++)
    {
        for (int j = 0; j < org_img.height(); j++)
        {
            // output[i][j] = gray_equal(output[i][j])
            QRgb rgb = org_img.pixel(i,j);
            int index = qRed(rgb) * 0.3 + qGreen(rgb)*0.59 + qBlue(rgb)*0.11;
            int gray = gray_equal[index];
            output.setPixel(i,j,QColor(gray,gray,gray).rgb());
        }
    }

    org_img = output;
    ui->photo->setPixmap(QPixmap::fromImage(output));
}

void PhotoShip::on_graphBtn_clicked()
{
    GraphDialog *gd = new GraphDialog(this, &org_img);
    gd->show();
    //PaintedWidget *pw = new PaintedWidget(this, &org_img);
    //pw->show();
}

void PhotoShip::on_cannyBtn_clicked()
{
    int temp[25] = {1,4,7,4,1,
                    4,16,26,16,4,
                    7,26,41,26,7,
                    4,16,26,16,4,
                    1,4,7,4,1};
    QImage img = org_img;
    QImage p = org_img;
    QImage q = org_img;
    for(int i = 2;i < img.width() - 2;i++){
        for(int j = 2;j <img.height() - 2;j++){
            int sum_red = 0;
            int sum_green = 0;
            int sum_blue = 0;
            int index = 0;
            for(int m = i - 2;m < i + 3;m++){
                for(int n = j - 2;n < j+3;n++){
                    QRgb rgb = img.pixel(m,n);
                    sum_red += qRed(rgb) * temp[index];
                    sum_green += qGreen(rgb) * temp[index];
                    sum_blue += qBlue(rgb) *temp[index++];
                }
            }
            sum_red /= 273;
            sum_green /= 273;
            sum_blue /= 273;

            if(sum_red > 255){
                sum_red = 255;
            }
            if(sum_green > 255){
                sum_green = 255;
            }
            if(sum_blue > 255){
                sum_blue = 255;
            }
            img.setPixel(i,j,QColor(sum_red,sum_green,sum_blue).rgb());
        }
    }

    //QList<QList<float>> m;
    QList<float> pointDrection;
    QImage temp_img = img;

    for(int i = 1;i < org_img.width() - 1;i++){
        //QList<float> temp;
        for(int j = 1;j < org_img.height() - 1;j++){
            QRgb x1 = temp_img.pixel(i,j+1);
            QRgb x2 = temp_img.pixel(i,j);
            QRgb x3 = temp_img.pixel(i+1,j+1);
            QRgb x4 = temp_img.pixel(i+1,j);

            int x1_gray = qRed(x1)*0.3 + qGreen(x1)*0.59 + qBlue(x1)*0.11;
            int x2_gray = qRed(x2)*0.3 + qGreen(x2)*0.59 + qBlue(x2)*0.11;
            int x3_gray = qRed(x3)*0.3 + qGreen(x3)*0.59 + qBlue(x3)*0.11;
            int x4_gray = qRed(x4)*0.3 + qGreen(x4)*0.59 + qBlue(x4)*0.11;

            float p_gray = (x1_gray - x2_gray + x3_gray - x4_gray)/2;
            float q_gray = (x2_gray-x4_gray+x1_gray-x3_gray)/2;

            float m_gray = sqrt(p_gray * p_gray + q_gray * q_gray);
            //temp.append(m_gray);
            img.setPixel(i,j,QColor(m_gray,m_gray,m_gray).rgb());
            pointDrection.append(qAtan(1.0 * q_gray/p_gray));
        }
        //m.append(temp);
    }

    //QList<QList<int>> s;
    int highThreshold = 225;
    int lowThreshold = 0;
    int k = 0;
    /*
    temp_img = img;
    for(int i = 1;i<org_img.width() -1;++i){
        //QList<int> temp;
        for(int j = 1;j<org_img.height() - 1;++j){
            int value00=qRed(temp_img.pixel(i-1,j-1));
            int value01=qRed(temp_img.pixel(i-1,j));
            int value02=qRed(temp_img.pixel(i-1,j+1));
            int value10=qRed(temp_img.pixel(i,j-1));
            int value11=qRed(temp_img.pixel(i,j));
            int value12=qRed(temp_img.pixel(i,j+1));
            int value20=qRed(temp_img.pixel(i+1,j-1));
            int value21=qRed(temp_img.pixel(i+1,j));
            int value22=qRed(temp_img.pixel(i+1,j+1));
            //temp.append(value11);

            if(pointDrection[k]>0&&pointDrection[k]<=45)
            {
                if(value11<=(value12+(value02-value12)*qTan(pointDrection[i*org_img.width()+j]))||(value11<=(value10+(value20-value10)*qTan(pointDrection[i*org_img.width()+j]))))
                {
                    img.setPixel(i,j,QColor(0,0,0).rgb());
                }
            }
            if(pointDrection[k]>45&&pointDrection[k]<=90)

            {
                if(value11<=(value01+(value02-value01)/qTan(pointDrection[i*org_img.height()+j]))||value11<=(value21+(value20-value21)/qTan(pointDrection[i*org_img.height()+j])))
                {
                    img.setPixel(i,j,QColor(0,0,0).rgb());

                }
            }
            if(pointDrection[k]>90&&pointDrection[k]<=135)
            {
                if(value11<=(value01+(value00-value01)/qTan(180-pointDrection[i*org_img.width()+j]))||value11<=(value21+(value22-value21)/qTan(180-pointDrection[i*org_img.height()+j])))
                {
                    img.setPixel(i,j,QColor(0,0,0).rgb());
                }
            }
            if(pointDrection[k]>135&&pointDrection[k]<=180)
            {
                if(value11<=(value10+(value00-value10)*qTan(180-pointDrection[i*org_img.height()+j]))||value11<=(value12+(value22-value11)*qTan(180-pointDrection[i*org_img.height()+j])))
                {
                    img.setPixel(i,j,QColor(0,0,0).rgb());
                }
            }
            k++;

            if(temp[j-1]>highThreshold)
            {
                img.setPixel(i,j,QColor(255,255,255).rgb());
            }
            if(temp[j-1]<lowThreshold)
            {
                img.setPixel(i,j,QColor(0,0,0).rgb());
            }
        }
        //s.append(temp);
    }
*/
    /*for(int i = 1;i<org_img.width()-1;i++){
        for(int j=1;j<org_img.height()-1;j++){
            img.setPixel(i,j,QColor(s[i-1][j-1],s[i-1][j-1],s[i-1][j-1]).rgb());
        }
    }*/

    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

int mat_min(vector<vector<int>> kernal, vector<vector<int>> back_image){
    int min = 255;
    int width = kernal[0].size();
    int height = kernal.size();
    for(int i = 0; i< height; i++){
        for(int j = 0; j < width; j++){
            if(kernal[i][j] == 1){
                min = min < back_image[i][j] ? min : back_image[i][j];
            }
        }
    }
}

int mat_max(vector<vector<int>> kernal, vector<vector<int>> back_image){
    int max = 0;
    int width = kernal[0].size();
    int height = kernal.size();
    for(int i = 0; i< height; i++){
        for(int j = 0; j < width; j++){
            if(kernal[i][j] == 1){
                max = max > back_image[i][j] ? max : back_image[i][j];
            }
        }
    }
}

// src can be both black_white graph or gray graph
vector<vector<int>> erosion(QImage *src, vector<vector<int>> kernal, pair<int, int> kernal_size){
    int kernal_width = kernal_size.first;
    int kernal_height = kernal_size.second;
    vector<vector<int>> result;
    vector<vector<int>> back_image;
    vector<int> row;
    for(int i = 0; i < src->width(); i++){
        result.push_back(row);
        for(int j = 0; j < src->height(); j ++){
            result[i].push_back(0);
        }
    }

    for(int  i = 0; i < (src->width()-kernal_width+1); i++){
        for(int j = 0; j < (src->height()-kernal_height+1); j++){
            // construct back_image
            for(int k = 0; k < kernal_height; k++){
                back_image.push_back(row);
                for(int h = 0; h < kernal_width; h++){
                    int gray_value;
                    int x = i+h;
                    int y = j+k;
                    QRgb pixel=src->pixel(x, y);
                    gray_value = qGray(qRed(pixel), qGreen(pixel), qBlue(pixel));
                    back_image[k].push_back(gray_value);
                }
            }
            result[i][j] = mat_min(kernal, back_image);
            back_image.clear();
        }
    }
    return result;
}

QImage PhotoShip::erose(QImage src,int temp_w,int temp_h,int* temp_data){
    QImage dst = src;

    //int tmpdata[5][5]={{0,0,0,0,0},{0,1,1,1,0},{0,1,1,1,0},{0,1,1,1,0},{0,0,0,0,0}};
    for(int i=temp_h/2;i<src.height();i++){
        for(int j=temp_w/2;j<src.width();j++){
            bool flag=true;
            for(int k=-temp_h/2;k<=temp_h/2;k++){
                for(int l=-temp_w/2;l<=temp_w/2;l++){
                    if(temp_data[(k+temp_h/2)*temp_w+l+temp_w/2]){
                        QRgb rgb = src.pixel(j+l,i+k);
                        if(qRed(rgb) == 0 && qGreen(rgb) ==0 &&qBlue(rgb) == 0)
                            flag=false;
                    }
                }
                if(flag)
                    dst.setPixel(j,i,QColor(255,255,255).rgb());
                else
                    dst.setPixel(j,i,QColor(0,0,0).rgb());
            }
        }

    }
    return dst;
}

QImage PhotoShip::dilate(QImage src,int temp_w, int temp_h, int *temp_data){
    int *tempMask=new int[temp_w*temp_h];
    for(int i=0;i<src.width();i++){
        for(int j=0;j<src.height();j++){

            if(qRed(src.pixel(i,j)) == 0)
                src.setPixel(i,j,QColor(255,255,255).rgb());
            else
                src.setPixel(i,j,QColor(0,0,0).rgb());

        }
    }

    QImage dst = src;

    for(int k=0;k<temp_h;k++)
    {
       for(int l=0;l<temp_w;l++)
       {
           tempMask[k*temp_w+l]=temp_data[(temp_h-1-k)*temp_w+temp_w-1-l];
       }
    }
    //int tmpdata[5][5]={{0,0,0,0,0},{0,1,1,1,0},{0,1,1,1,0},{0,1,1,1,0},{0,0,0,0,0}};
    for(int i=temp_h/2;i<src.height();i++){
        for(int j=temp_w/2;j<src.width();j++){
            bool flag=true;
            for(int k=-temp_h/2;k<=temp_h/2;k++){
                for(int l=-temp_w/2;l<=temp_w/2;l++){
                    if(tempMask[(k+temp_h/2)*temp_w+l+temp_w/2]){
                        if(!qRed(src.pixel(j+l,i+k)))
                            flag=false;
                    }
                }
                if(flag)
                    dst.setPixel(j,i,QColor(0,0,0).rgb());
                else
                    dst.setPixel(j,i,QColor(255,255,255).rgb());
            }
        }

    }
    return dst;
}

void PhotoShip::on_erosBtn_clicked()
{
    QImage src = org_img;
    int threshold = otsu(src);

    for(int i = 0;i < src.width();i++){
        for(int j = 0;j < src.height();j++){
            QRgb rgb = src.pixel(i,j);
            int gray = qRed(rgb) * 0.3 + qGreen(rgb) * 0.59 + qBlue(rgb) * 0.11;
            if(gray <= threshold){
                src.setPixel(i,j,QColor(0,0,0).rgb());
            }else{
                src.setPixel(i,j,QColor(255,255,255).rgb());
            }
        }
    }
    int tempH=ui->contValue1Input->text().toInt();
    int tempW=ui->contValue2Input->text().toInt();
    int* tmpdata=(int*)malloc(tempH*tempW*sizeof(int));
    for(int i=0;i<tempH;i++)
        for(int j=0;j<tempW;j++){
            if(i>(tempH-1)/2 && i<tempH-(tempH-1)/2 &&j>(tempW-1)/2 && j<tempW-(tempW-1)/2)
                tmpdata[i*tempW+j]=1;
        }
    QImage img = erose(src,tempW,tempH,tmpdata);
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_expoBtn_clicked()
{
    //Mat a=imread(fileName.toStdString());
    QImage a = org_img;
    float gamma=ui->contValue1Input->text().toFloat();
    QImage b = a;
    for(int i=0;i<a.width();i++){
        for(int j=0;j<a.height();j++){
            QRgb rgb = a.pixel(i,j);
            int red = qPow(qRed(rgb),gamma)>255?255:qPow(qRed(rgb),gamma);
            int green = qPow(qGreen(rgb),gamma)>255?255:qPow(qGreen(rgb),gamma);
            int blue = qPow(qBlue(rgb),gamma)>255?255:qPow(qBlue(rgb),gamma);
            b.setPixel(i,j,QColor(red,green,blue).rgb());
        }
    }
    org_img = b;
    ui->photo->setPixmap(QPixmap::fromImage(b));
}

void PhotoShip::on_logaBtn_clicked()
{
    //Mat a=imread(fileName.toStdString());
    QImage a = org_img;
    float gamma=ui->contValue1Input->text().toFloat();
    QImage b = a;
    for(int i=0;i<a.width();i++){
        for(int j=0;j<a.height();j++){
            QRgb rgb = a.pixel(i,j);
            int red = (1.0 * log(1.0 * 1+qRed(rgb)*gamma)/1.0 *log(gamma-1));
            red = red>255?255:red;
            red = red<0?0:red;
            int green = (1.0 * log(1.0 * 1+qGreen(rgb)*gamma)/1.0 * log(gamma-1));
            green = green>255?255:green;
            green = green<0?0:green;
            int blue = (1.0 * log(1.0 * 1+qBlue(rgb)*gamma)/1.0 *log(1.0 * gamma-1));
            blue = blue>255?255:blue;
            blue = blue<0?0:blue;
            b.setPixel(i,j,QColor(red,green,blue).rgb());

        }
    }
    //org_img = b;
    ui->photo->setPixmap(QPixmap::fromImage(b));
}

void PhotoShip::on_dilateBtn_clicked()
{
    QImage src = org_img;
    int threshold = otsu(src);

    for(int i = 0;i < src.width();i++){
        for(int j = 0;j < src.height();j++){
            QRgb rgb = src.pixel(i,j);
            int gray = qRed(rgb) * 0.3 + qGreen(rgb) * 0.59 + qBlue(rgb) * 0.11;
            if(gray <= threshold){
                src.setPixel(i,j,QColor(0,0,0).rgb());
            }else{
                src.setPixel(i,j,QColor(255,255,255).rgb());
            }
        }
    }
    int tempH=ui->contValue1Input->text().toInt();
    int tempW=ui->contValue2Input->text().toInt();
    int* tmpdata=(int*)malloc(tempH*tempW*sizeof(int));
    for(int i=0;i<tempH;i++)
        for(int j=0;j<tempW;j++){
            if(i>(tempH-1)/2 && i<tempH-(tempH-1)/2 &&j>(tempW-1)/2 && j<tempW-(tempW-1)/2)
                tmpdata[i*tempW+j]=1;
        }

    QImage img = dilate(src,tempW,tempH,tmpdata);

    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_openBtn_clicked()
{
    QImage src = org_img;
    int threshold = otsu(src);

    for(int i = 0;i < src.width();i++){
        for(int j = 0;j < src.height();j++){
            QRgb rgb = src.pixel(i,j);
            int gray = qRed(rgb) * 0.3 + qGreen(rgb) * 0.59 + qBlue(rgb) * 0.11;
            if(gray <= threshold){
                src.setPixel(i,j,QColor(0,0,0).rgb());
            }else{
                src.setPixel(i,j,QColor(255,255,255).rgb());
            }
        }
    }
    int tempH=ui->contValue1Input->text().toInt();
    int tempW=ui->contValue2Input->text().toInt();
    int* tmpdata=(int*)malloc(tempH*tempW*sizeof(int));
    for(int i=0;i<tempH;i++)
        for(int j=0;j<tempW;j++){
            if(i>(tempH-1)/2 && i<tempH-(tempH-1)/2 &&j>(tempW-1)/2 && j<tempW-(tempW-1)/2)
                tmpdata[i*tempW+j]=1;
        }

    QImage img = erose(src,tempW,tempH,tmpdata);
    img = dilate(img,tempW,tempH,tmpdata);

    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_closeBtn_clicked()
{
    QImage src = org_img;
    int threshold = otsu(src);

    for(int i = 0;i < src.width();i++){
        for(int j = 0;j < src.height();j++){
            QRgb rgb = src.pixel(i,j);
            int gray = qRed(rgb) * 0.3 + qGreen(rgb) * 0.59 + qBlue(rgb) * 0.11;
            if(gray <= threshold){
                src.setPixel(i,j,QColor(0,0,0).rgb());
            }else{
                src.setPixel(i,j,QColor(255,255,255).rgb());
            }
        }
    }
    int tempH=ui->contValue1Input->text().toInt();
    int tempW=ui->contValue2Input->text().toInt();
    int* tmpdata=(int*)malloc(tempH*tempW*sizeof(int));
    for(int i=0;i<tempH;i++)
        for(int j=0;j<tempW;j++){
            if(i>(tempH-1)/2 && i<tempH-(tempH-1)/2 &&j>(tempW-1)/2 && j<tempW-(tempW-1)/2)
                tmpdata[i*tempW+j]=1;
        }

    QImage img = dilate(src,tempW,tempH,tmpdata);
    img = erose(img,tempW,tempH,tmpdata);

    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

QImage PhotoShip::thin(QImage src, const int iterations){
    const int height =src.height() -1;
    const int width  =src.width() -1;
    //拷贝一个数组给另一个数组
    QImage dst = src;
    int n = 0,i = 0,j = 0;
    QImage tmpImg;
    uchar *pU, *pC, *pD;
    bool isFinished =false;
    for(n=0; n<iterations; n++){
        tmpImg = dst;
        isFinished =false;
        for(i=1; i<height;  i++) {
            //pU = tmpImg.ptr<uchar>(i-1);
            //pC = tmpImg.ptr<uchar>(i);
            //pD = tmpImg.ptr<uchar>(i+1);
            for(int j=1; j<width; j++){
                if(qRed(tmpImg.pixel(j,i)) > 0){
                    int ap=0;
                    int p2 = (qRed(tmpImg.pixel(j,i-1))>0);
                    int p3 = (qRed(tmpImg.pixel(j+1,i-1))>0);
                    if (p2==0 && p3==1)
                        ap++;
                    int p4 = (qRed(tmpImg.pixel(j+1,i)) >0);
                    if(p3==0 && p4==1)
                        ap++;
                    int p5 = (qRed(tmpImg.pixel(j+1,i+1)) >0);
                    if(p4==0 && p5==1)
                        ap++;
                    int p6 = (qRed(tmpImg.pixel(j,i+1)) >0);
                    if(p5==0 && p6==1)
                        ap++;
                    int p7 = (qRed(tmpImg.pixel(j-1,i+1)) >0);
                    if(p6==0 && p7==1)
                        ap++;
                    int p8 = (qRed(tmpImg.pixel(j-1,i)) >0);
                    if(p7==0 && p8==1)
                        ap++;
                    int p9 = (qRed(tmpImg.pixel(j-1,i-1)) >0);
                    if(p8==0 && p9==1)
                        ap++;
                    if(p9==0 && p2==1)
                        ap++;
                    if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7){
                        if(ap==1){
                            if((p2*p4*p6==0)&&(p4*p6*p8==0)){
                                dst.setPixel(j,i,QColor(0,0,0).rgb());
                                isFinished =true;
                            }
                        }
                    }
                }

            }
            tmpImg = dst;
            for(i=1; i<height;  i++){
                //pU = tmpImg.ptr<uchar>(i-1);
                //pC = tmpImg.ptr<uchar>(i);
                //pD = tmpImg.ptr<uchar>(i+1);
                for(int j=1; j<width; j++){
                    if(qRed(tmpImg.pixel(j,i)) > 0){
                        int ap=0;
                        int p2 = (qRed(tmpImg.pixel(j,i-1))>0);
                        int p3 = (qRed(tmpImg.pixel(j+1,i-1))>0);
                        if (p2==0 && p3==1)
                            ap++;
                        int p4 = (qRed(tmpImg.pixel(j+1,i)) >0);
                        if(p3==0 && p4==1)
                            ap++;
                        int p5 = (qRed(tmpImg.pixel(j+1,i+1)) >0);
                        if(p4==0 && p5==1)
                            ap++;
                        int p6 = (qRed(tmpImg.pixel(j,i+1)) >0);
                        if(p5==0 && p6==1)
                            ap++;
                        int p7 = (qRed(tmpImg.pixel(j-1,i+1)) >0);
                        if(p6==0 && p7==1)
                            ap++;
                        int p8 = (qRed(tmpImg.pixel(j-1,i)) >0);
                        if(p7==0 && p8==1)
                            ap++;
                        int p9 = (qRed(tmpImg.pixel(j-1,i-1)) >0);
                        if(p8==0 && p9==1)
                            ap++;
                        if(p9==0 && p2==1)
                            ap++;
                        if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7){
                            if(ap==1){
                                if((p2*p4*p8==0)&&(p2*p6*p8==0)){
                                    dst.setPixel(j,i,QColor(0,0,0).rgb());
                                    isFinished =true;
                                }
                            }
                        }
                    }
                }
            }
            if(isFinished ==false)
                break;
        }
    }
    return dst;
}

void PhotoShip::on_thinBtn_clicked()
{
    QImage src = org_img;
    int threshold = otsu(src);

    for(int i = 0;i < src.width();i++){
        for(int j = 0;j < src.height();j++){
            QRgb rgb = src.pixel(i,j);
            int gray = qRed(rgb) * 0.3 + qGreen(rgb) * 0.59 + qBlue(rgb) * 0.11;
            if(gray <= threshold){
                src.setPixel(i,j,QColor(0,0,0).rgb());
            }else{
                src.setPixel(i,j,QColor(255,255,255).rgb());
            }
        }
    }

    QImage img = thin(src,8);
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

void PhotoShip::on_thickBtn_clicked()
{
    QImage src = org_img;
    int threshold = otsu(src);

    for(int i = 0;i < src.width();i++){
        for(int j = 0;j < src.height();j++){
            QRgb rgb = src.pixel(i,j);
            int gray = qRed(rgb) * 0.3 + qGreen(rgb) * 0.59 + qBlue(rgb) * 0.11;
            if(gray <= threshold){
                src.setPixel(i,j,QColor(0,0,0).rgb());
            }else{
                src.setPixel(i,j,QColor(255,255,255).rgb());
            }
        }
    }
    for(int i=0;i<src.width();i++){
        for(int j=0;j<src.height();j++){
            //qDebug()<<data[i*src.step+j];
            if(qRed(src.pixel(i,j)) == 0)
                src.setPixel(i,j,QColor(255,255,255).rgb());
            else
                src.setPixel(i,j,QColor(0,0,0).rgb());

        }
    }

    QImage img = thin(src,8);
    for(int i=0;i<img.width();i++){
        for(int j=0;j<img.height();j++){
            //qDebug()<<data[i*src.step+j];
            if(qRed(img.pixel(i,j)) == 0)
                img.setPixel(i,j,QColor(255,255,255).rgb());
            else
                img.setPixel(i,j,QColor(0,0,0).rgb());

        }
    }
    org_img = img;
    ui->photo->setPixmap(QPixmap::fromImage(img));
}

float PhotoShip::calcEuclideanDiatance(int x1, int y1, int x2, int y2)
{
    return qSqrt(float((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)));
}

//计算棋盘距离的函数
int PhotoShip::calcChessboardDistance(int x1, int y1, int x2, int y2)
{
    return qMax(qAbs(x1 - x2), qAbs(y1 - y2));
}

//计算麦哈顿距离(街区距离)
int PhotoShip::calcBlockDistance(int x1, int y1, int x2, int y2)
{
    return qAbs(x1 - x2) + qAbs(y1 - y2);
}
void PhotoShip::on_distanBtn_clicked()
{
    QImage src = org_img;
    int threshold = otsu(src);

    for(int i = 0;i < src.width();i++){
        for(int j = 0;j < src.height();j++){
            QRgb rgb = src.pixel(i,j);
            int gray = qRed(rgb) * 0.3 + qGreen(rgb) * 0.59 + qBlue(rgb) * 0.11;
            if(gray <= threshold){
                src.setPixel(i,j,QColor(0,0,0).rgb());
            }else{
                src.setPixel(i,j,QColor(255,255,255).rgb());
            }
        }
    }
    QImage dst = src;

    int rows = src.height();
    int cols = src.width();
    //uchar *pDataOne;
    //uchar *pDataTwo;
    float disPara = 0;
    float fDisMIn = 0;
    //第一遍遍历图像，使用左模板更新像素值
        for (int i = 1; i < rows - 1; i++)
        {
            //图像的行指针的获取
            for (int j = 1; j < cols; j++)
            {
                //分别计算左模板掩码的相关距离
                //PL  PL
                //PL  P
                //PL
                //pDataTwo = dst.ptr<uchar>(i - 1);
                disPara = calcBlockDistance(i, j, i - 1, j - 1);
                fDisMIn = qMin((float)qRed(src.pixel(j,i)), disPara + qRed(src.pixel(j-1,i-1)));
                disPara = calcBlockDistance(i, j, i - 1, j);
                fDisMIn = qMin(fDisMIn, disPara + qRed(src.pixel(j,i-1)));
                //pDataTwo =dst.ptr<uchar>(i);
                disPara = calcBlockDistance(i, j, i, j - 1);
                fDisMIn = qMin(fDisMIn, disPara + qRed(src.pixel(j-1,i)));
                //pDataTwo =dst.ptr<uchar>(i+1);
                disPara = calcBlockDistance(i, j, i+1,j-1);
                fDisMIn = qMin(fDisMIn, disPara + qRed(src.pixel(j-1,i+1)));
                dst.setPixel(j,i,QColor(int(fDisMIn)*1.05>255?255:int(fDisMIn)*1.05,
                                        int(fDisMIn)*1.05>255?255:int(fDisMIn)*1.05,
                                        int(fDisMIn)*1.05>255?255:int(fDisMIn)*1.05).rgb());
            }
        }

        //第二遍遍历图像，使用右模板更新像素值
        for (int i = rows - 2; i > 0; i--)
        {
            //pDataOne = dst.ptr<uchar>(i);
            for (int j = cols - 1; j >= 0; j--)
            {
                //分别计算右模板掩码的相关距离
                //pR  pR
                //pR  p
                //pR
                //pDataTwo = dst.ptr<uchar>(i + 1);
                disPara = calcBlockDistance(i, j, i + 1, j);
                fDisMIn = qMin((float)qRed(src.pixel(j,i)), disPara + qRed(src.pixel(j,i+1)));
                disPara = calcBlockDistance(i, j, i + 1, j + 1);

                fDisMIn = qMin(fDisMIn, disPara + qRed(src.pixel(j+1,i+1)));
                //pDataTwo = dst.ptr<uchar>(i);
                disPara = calcBlockDistance(i, j, i, j +1);
                fDisMIn = qMin(fDisMIn, disPara + qRed(src.pixel(j+1,i)));
                //pDataTwo = dst.ptr<uchar>(i - 1);
                disPara = calcBlockDistance(i, j, i - 1, j + 1);
                fDisMIn = qMin(fDisMIn, disPara + qRed(src.pixel(j+1,i-1)));
                dst.setPixel(j,i,QColor(int(fDisMIn)*1.05>255?255:int(fDisMIn)*1.05,
                                        int(fDisMIn)*1.05>255?255:int(fDisMIn)*1.05,
                                        int(fDisMIn)*1.05>255?255:int(fDisMIn)*1.05).rgb());

            }
        }
    //distanceTransform(dst2, dst2, CV_DIST_L2, CV_DIST_MASK_PRECISE);
    //displayMat3(dst,ui->label_2);
    //displayMat3(dst2,ui->label_8);
    //saveg=dst2;
        org_img = dst;
        ui->photo->setPixmap(QPixmap::fromImage(dst));
}

QImage PhotoShip::erose_gray(QImage src){
    QImage dst = src;
    int * tmp=new int[9];

    for(int i=1;i<src.height()-1;i++){
        for(int j=1;j<src.width()-1;j++){
            int minx=255;
            tmp[0]=qRed(src.pixel(j-1,i-1));
            //qDebug()<<tmp[0]<<endl;
            minx=tmp[0]<minx?tmp[0]:minx;
            tmp[1]=qRed(src.pixel(j,i-1));
            minx=tmp[1]<minx?tmp[1]:minx;
            tmp[2]=qRed(src.pixel(j+1,i-1));
            minx=tmp[2]<minx?tmp[2]:minx;
            tmp[3]=qRed(src.pixel(j-1,i));
            minx=tmp[3]<minx?tmp[3]:minx;
            tmp[4]=qRed(src.pixel(j,i));
            minx=tmp[4]<minx?tmp[4]:minx;
            tmp[5]=qRed(src.pixel(j+1,i));
            minx=tmp[5]<minx?tmp[5]:minx;
            tmp[6]=qRed(src.pixel(j-1,i+1));
            minx=tmp[6]<minx?tmp[6]:minx;
            tmp[7]=qRed(src.pixel(j,i+1));
            minx=tmp[7]<minx?tmp[7]:minx;
            tmp[8]=qRed(src.pixel(j+1,i+1));
            minx=tmp[8]<minx?tmp[8]:minx;

            dst.setPixel(j,i,QColor(minx,minx,minx).rgb());
            //qDebug()<<minx<<endl;
        }
    }
    return dst;
}

QImage PhotoShip::dilate_gray(QImage src){
    QImage dst = src;
    int * tmp=new int[9];

    for(int i=1;i<src.height()-1;i++){
        for(int j=1;j<src.width()-1;j++){
            int maxx=0;
            tmp[0]=qRed(src.pixel(j-1,i-1));
            //qDebug()<<tmp[0]<<endl;
            maxx=tmp[0]>maxx?tmp[0]:maxx;
            tmp[1]=qRed(src.pixel(j,i-1));
            maxx=tmp[1]>maxx?tmp[1]:maxx;
            tmp[2]=qRed(src.pixel(j+1,i-1));
            maxx=tmp[2]>maxx?tmp[2]:maxx;
            tmp[3]=qRed(src.pixel(j-1,i));
            maxx=tmp[3]>maxx?tmp[3]:maxx;
            tmp[4]=qRed(src.pixel(j,i));
            maxx=tmp[4]>maxx?tmp[4]:maxx;
            tmp[5]=qRed(src.pixel(j+1,i));
            maxx=tmp[5]>maxx?tmp[5]:maxx;
            tmp[6]=qRed(src.pixel(j-1,i+1));
            maxx=tmp[6]>maxx?tmp[6]:maxx;
            tmp[7]=qRed(src.pixel(j,i+1));
            maxx=tmp[7]>maxx?tmp[7]:maxx;
            tmp[8]=qRed(src.pixel(j+1,i+1));
            maxx=tmp[8]>maxx?tmp[8]:maxx;

            dst.setPixel(j,i,QColor(maxx,maxx,maxx).rgb());
            //qDebug()<<minx<<endl;
        }
    }
    return dst;
}

void PhotoShip::on_eroGrayBtn_clicked()
{
    QImage src = org_img;
    for(int i=0;i<src.width();i++){
        for(int j=0;j<src.height();j++){
            QRgb rgb = src.pixel(i,j);
            int gray = qRed(rgb)*0.3+qGreen(rgb)*0.59+qBlue(rgb)*0.11;
            src.setPixel(i,j,QColor(gray,gray,gray).rgb());
        }
    }
    src = erose_gray(src);
    org_img = src;
    ui->photo->setPixmap(QPixmap::fromImage(src));
}

void PhotoShip::on_dilaGrayBtn_clicked()
{
    QImage src = org_img;
    for(int i=0;i<src.width();i++){
        for(int j=0;j<src.height();j++){
            QRgb rgb = src.pixel(i,j);
            int gray = qRed(rgb)*0.3+qGreen(rgb)*0.59+qBlue(rgb)*0.11;
            src.setPixel(i,j,QColor(gray,gray,gray).rgb());
        }
    }
    src = dilate_gray(src);
    org_img = src;
    ui->photo->setPixmap(QPixmap::fromImage(src));
}

void PhotoShip::on_openGrayBtn_clicked()
{
    QImage src = org_img;
    for(int i=0;i<src.width();i++){
        for(int j=0;j<src.height();j++){
            QRgb rgb = src.pixel(i,j);
            int gray = qRed(rgb)*0.3+qGreen(rgb)*0.59+qBlue(rgb)*0.11;
            src.setPixel(i,j,QColor(gray,gray,gray).rgb());
        }
    }
    src = erose_gray(src);
    src = dilate_gray(src);
    org_img = src;
    ui->photo->setPixmap(QPixmap::fromImage(src));
}



void PhotoShip::on_closeGrayBtn_clicked()
{
    QImage src = org_img;
    for(int i=0;i<src.width();i++){
        for(int j=0;j<src.height();j++){
            QRgb rgb = src.pixel(i,j);
            int gray = qRed(rgb)*0.3+qGreen(rgb)*0.59+qBlue(rgb)*0.11;
            src.setPixel(i,j,QColor(gray,gray,gray).rgb());
        }
    }
    src = dilate_gray(src);
    src = erose_gray(src);

    org_img = src;
    ui->photo->setPixmap(QPixmap::fromImage(src));
}

bool PhotoShip::getColorLevelTable(PColorLevelItem item,unsigned char* clTable)
{
    int diff = (int)(item->Highlight - item->Shadow);
    int outDiff = (int)(item->OutHighlight - item->OutShadow);

    if (!((item->Highlight <= 255 && diff < 255 && diff >= 2) ||
        (item->OutShadow <= 255 && item->OutHighlight <= 255 && outDiff < 255) ||
        (!(item->Midtones > 9.99 && item->Midtones > 0.1) && item->Midtones != 1.0)))
        return false;

    double coef = 255.0 / diff;
    double outCoef = outDiff / 255.0;
    double exponent = 1.0 / item->Midtones;

    for (int i = 0; i < 256; i++){
        int v;
        if (clTable[i] <= (unsigned char)item->Shadow)
            v = 0;
        else{
            v = (int)((clTable[i] - item->Shadow) * coef + 0.5);
            if (v > 255)
                v = 255;
        }
        v = (int)(pow(v / 255.0, exponent) * 255.0 + 0.5);
        clTable[i] = (unsigned char)(v * outCoef + item->OutShadow + 0.5);
    }

    return true;
}

bool PhotoShip::checkColorLevelData(PColorLevelData clData,unsigned char clTables[][256])
{
    bool result = false;
    int i, j;
    for (i = 0; i < 3; i++){
        for (j = 0; j < 256; j++)
            clTables[i][j] = (unsigned char)j;
    }

    PColorLevelItem item = &clData->Blue;
    //分别获取Blue->Green->Red三个通道的色阶表
    for (i = 0; i < 3; i++, item++){
        if (getColorLevelTable(item, clTables[i]))
            result = true;
    }
    //获取RGB通道的色阶表
    for (i = 0; i < 3; i++){
        if (!getColorLevelTable(item, clTables[i]))
            break;
        result = true;
    }

    return result;
}

QImage PhotoShip::imageColorLevel(QImage source,PColorLevelData clData)
{
    int nRet;
    int width, height;

    QImage dst = source;

    nRet = 0;
    width = source.width();
    height = source.height();

    //色阶表替换法替换三个通道的数据
    unsigned char clTables[3][256];
    if (checkColorLevelData(clData, clTables)){
        for (unsigned int y = 0; y < height; y++){
            for (unsigned int x = 0; x < width; x++){
                dst.setPixel(x,y,QColor(clTables[0][qRed(source.pixel(x,y))],
                        clTables[0][qGreen(source.pixel(x,y))],
                        clTables[0][qBlue(source.pixel(x,y))]).rgb());

            }
        }
    }else{
        dst = source;
    }

    return dst;
}

int PhotoShip::initColorLevelData(PColorLevelData clData)
{
    int nRet = 0;

    PColorLevelItem item = &clData->Blue;
    for (int i = 0; i < 4; i++, item++){
        item->Shadow = item->OutShadow = 0;
        item->Highlight = item->OutHighlight = 255;
        item->Midtones = 1.0;
    }

    return nRet;
}

void PhotoShip::on_bandBtn_clicked()
{
    QImage src = org_img;
    ColorLevelData clData;
    initColorLevelData(&clData);
    clData.RGB.Shadow = ui->contValue1Input->text().toInt();
    clData.RGB.Midtones = ui->contValue2Input->text().toInt();
    clData.RGB.Highlight = ui->contValue3Input->text().toInt();
    clData.RGB.OutShadow = ui->contX1Input->text().toInt();
    clData.RGB.OutHighlight = ui->contX2Input->text().toInt();

    src = imageColorLevel(src, &clData);
    //org_img = src;
    ui->photo->setPixmap(QPixmap::fromImage(src));
}
