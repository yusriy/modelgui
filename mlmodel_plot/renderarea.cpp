#include <QtGui>
#include "renderarea.h"
#include "widget.h"
#include <iostream>
#include <cmath>

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    axisset = false;
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(220, 220);
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 400);
}

void RenderArea::setAxis(double xmin, double xmax, double ymin, double ymax)
{
    axisset = true;
    this->xmin = xmin;
    this->xmax = xmax;
    this->ymin = ymin;
    this->ymax = ymax;
    update();
}

double RenderArea::transfx(double xreal, double xscale, double xmin)
{
    double xwidget = ((xreal-xmin)*xscale) + plotmargin;
    return (xwidget);
}

double RenderArea::transfy(double yreal, double yscale, double ymin)
{
    int pwidget_height = geometry().height();
    double ywidget = pwidget_height -plotmargin - ((yreal-ymin)*yscale);
    return (ywidget);
}
// BVS

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    // Size of widget (pixels)
    int pwidget_width = geometry().width();
    int pwidget_height = geometry().height();
    // Size of plotable area within widget (pixels)
    double plotwidth = pwidget_width - 2 * plotmargin;
    double plotheight = pwidget_height - 2 * plotmargin;

    if (axisset == false){
        this->xmin = 0;
        this->xmax = 10;
        this->ymin = 0;
        this->ymax = 1;
    }

    double xstep = 0.1;                        // x-interval for loop
    double yscale = plotheight / (ymax-ymin);  // scaling factor for f(real-coordinate to Widget-coordinate)
    double xscale = plotwidth / (xmax-xmin);   // scaling factor for f(real-coordinate to Widget-coordinate)

    QPainter paint(this);
    QPen pen(Qt::black, 1, Qt::SolidLine);
    paint.setPen(pen);

    // Extra line through y=zero for negative y-values
    if (ymin < 0){
        paint.setPen(Qt::DashLine);
        paint.drawLine(transfx(xmin,xscale,xmin),transfy(0,yscale,ymin),transfx(xmax,xscale,xmin),transfy(0,yscale,ymin));
    }

    // Draw X and Y axis
    paint.setPen(Qt::SolidLine);
    paint.drawLine(plotmargin,pwidget_height - plotmargin,pwidget_width-plotmargin,pwidget_height-plotmargin);
    paint.drawLine(plotmargin,pwidget_height - plotmargin,plotmargin,plotmargin);  // Draw Y-axis

    // Draw labels Y-axis
    paint.drawText(5,plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax));
    paint.drawText(5,(plotheight*0.25)+plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax-((ymax-ymin)*0.25)));
    paint.drawText(5,(plotheight*0.5)+plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax-((ymax-ymin)*0.50)));
    paint.drawText(5,(plotheight*0.75)+plotmargin-5,40,13,Qt::AlignRight, QString::number(ymax-((ymax-ymin)*0.75)));
    paint.drawText(5,pwidget_height-60,40,13,Qt::AlignRight, QString::number(ymin));

    // Draw labels X-axis
    paint.drawText(30,pwidget_height-45,40,13,Qt::AlignCenter, QString::number(xmin));
    paint.drawText((plotwidth*0.75)+plotmargin-20,pwidget_height-45,40,13,Qt::AlignCenter, QString::number((xmax-((xmax-xmin)*0.25))));
    paint.drawText((plotwidth*0.5)+plotmargin-20,pwidget_height-45,40,13,Qt::AlignCenter, QString::number((xmax-((xmax-xmin)*0.5))));
    paint.drawText((plotwidth*0.25)+plotmargin-20,pwidget_height-45,40,13,Qt::AlignCenter, QString::number((xmax-((xmax-xmin)*0.75))));
    paint.drawText(pwidget_width-70,pwidget_height-45,40,13,Qt::AlignCenter, QString::number(xmax));

    // Draw title
    paint.drawText((pwidget_width/2)-100,(plotmargin/2)-10,200,13,Qt::AlignCenter, "Cosine(X), Sine(X)");

    // Hereafter; clip data plot .
    paint.setClipping(true);
    paint.setClipRect(plotmargin,plotmargin,plotwidth,plotheight);

    // Demo data
    for (double i=xmin; i<xmax; i+=xstep)
    {
      paint.setPen( Qt::blue );
      paint.drawLine(transfx(i,xscale,xmin),transfy(cos(i),yscale,ymin),transfx((i+xstep),xscale,xmin),transfy(cos(i+xstep),yscale,ymin));
      paint.setPen( Qt::red );
      paint.drawLine(transfx(i,xscale,xmin),transfy(sin(i),yscale,ymin),transfx((i+xstep),xscale,xmin),transfy(sin(i+xstep),yscale,ymin));
    }
}