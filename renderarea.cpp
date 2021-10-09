#include "renderarea.h"
#include <QPaintEvent>
#include <QSize>
#include <QPainter>

// constructor of class RenderArea
RenderArea::RenderArea(QWidget *parent) : QWidget(parent), mBackgroundColor(0, 0, 255), mShapeColor(255, 255, 255), mShape(Asteroid)
{
    on_shape_changed();     // ensures that there is asteroid present by default
}

// all are member functions of class RenderArea

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(150, 100);
}

void RenderArea::on_shape_changed()
{
    switch (mShape) {
    case Asteroid:
        mScale =40;
        mIntervalLength=2*M_PI;
        mStepCount=256;
        break;
    case Cycloid:
        mScale =4;
        mIntervalLength=6*M_PI;
        mStepCount=128;
        break;
    case HuygensCycloid:
        mScale =4;
        mIntervalLength=4*M_PI;
        mStepCount=256;
        break;
    case HypoCycloid:
        mScale =15;
        mIntervalLength=2*M_PI;
        mStepCount=256;
        break;
    case Line:
        mScale=100; // length of line in pixels
        mIntervalLength=1; // not really necessary
        mStepCount=128;
        break;
    default:
        break;
    }
}

QPointF RenderArea::compute(float t)
{
    switch (mShape) {
    case Asteroid:
        return compute_astroid(t);
        break;
    case Cycloid:
        return compute_cycloid(t);
        break;
    case HuygensCycloid:
        return compute_huygens(t);
        break;
    case HypoCycloid:
        return compute_hypo(t);
        break;
    case Line:
        return compute_line(t);
    default:
        break;
    }
    return QPointF(0,0);
}

QPointF RenderArea::compute_astroid(float t){
    // We will compute astroid here
    float cos_t = cos(t);
    float sin_t = sin(t);
    float x = 2 * pow(cos_t, 3);
    float y = 2 * pow(sin_t, 3);
    return QPointF (x, y);
};

QPointF RenderArea::compute_cycloid(float t)
{
    return QPointF(
                1.5*(1-cos(t)),    // x co-ordinate
                1.5*(t-sin(t))    // y co-ordinate
                );
};

QPointF RenderArea::compute_huygens(float t)
{
    return QPointF(
                4*(3*cos(t) - cos(3*t)),    // x co-ordinate
                4*(3*sin(t) - sin(3*t))     // y co-ordinate
                );
};

QPointF RenderArea::compute_hypo(float t)
{
    return QPointF(
                1.5*(2*cos(t)+cos(2*t)),
                1.5*(2*sin(t)-sin(2*t))
                );

};

QPointF RenderArea::compute_line(float t)
{
    return QPointF(1-t, 1-t);
}


// paintEvent is method/function of class RenderArea and is used to draw the objects in render area
void RenderArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setBrush(mBackgroundColor);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(mShapeColor);
    // drawing area

    painter.drawRect(this->rect());
    QPointF center= this->rect().center();


    QPointF previousPoint = compute(0);
    QPoint previousPixel;
    previousPixel.setX(previousPoint.x() * mScale + center.x());
    previousPixel.setY(previousPoint.y() * mScale + center.y());

    float step= mIntervalLength/mStepCount;
    for (float t=0; t<mIntervalLength; t+=step)
    {
        QPointF point = compute (t);
        QPoint pixel;
        pixel.setX(point.x() * mScale + center.x());
        pixel.setY(point.y() * mScale + center.y());

        painter.drawLine(pixel, previousPixel);
        previousPixel=pixel;
    }

//    painter.drawLine(this->rect().topLeft(), this->rect().bottomRight());
}
