#include "graphdialog.h"
#include "ui_graphdialog.h"
#include <vector>
using namespace std;
GraphDialog::GraphDialog(QWidget *parent, QImage* i) :
    QDialog(parent),
    ui(new Ui::GraphDialog)
{
    ui->setupUi(this);
    image = i;
}

GraphDialog::~GraphDialog()
{
    delete ui;
}

static vector<int> Histogram(QImage *img){
    unsigned char* graydata = img->bits();
    vector<int> hist(256);
    for(int i=0; i!=img->width(); i++){
        for(int j = 0; j != img->height(); j++){
            int index = int(*graydata);
            hist[index] = hist[index]+1;
            graydata += 3;
        }
    }
    graydata = NULL;
    return hist;
}

void GraphDialog::paintEvent(QPaintEvent *e){
    QPainter p(this);
    p.setBrush(QBrush(QColor(121,121,121)));
    p.drawRect(0,0,this->width(),this->height());

    p.setBrush(QBrush(QColor(255,255,255)));
    p.drawRect(0,0,this->width(),this->height());
    std::vector<int> count = Histogram(image);
    std::vector<int> sortcount = count;
    std::sort(sortcount.begin(),sortcount.end());
    int maxcount = sortcount[sortcount.size()-1];


    QImage *hist = new QImage(this->width(),this->height(),QImage::Format_RGB888);
    hist->fill(qRgb(255,255,255));
    p.translate(0,hist->height());


    p.drawLine(0,0,100,100);

    int wid=hist->width();
    int hei=hist->height();

    p.drawLine(10,-10,wid-10,-10);// 横轴
    p.drawLine(10,-10,10,-hei+10);//纵轴

    float xstep = float(wid-20) / 256;
    float ystep = float(hei-20) / maxcount;

    for (int i=0;i!=256;i++)
    {
        if(i!=255)
        {
            QColor color(i,255-i,0);
            //p.setPen(Qt::blue);
            //p.setBrush(Qt::blue);
            p.setBrush(color);
            p.setPen(color);
            //p.drawLine(QPointF(10+(i+0.5)*xstep,-10-ystep*count[i]),QPointF(10+(i+1.5)*xstep,-10-ystep*count[i+1]));
            p.drawRect(10+i*xstep,-10,xstep,-10-ystep*count[i]*5);
        }

        if(i % 32 == 0||i==255)
        {
            p.drawText(QPointF(10+(i-0.5)*xstep,0),QString::number(i));
        }

    }
}
