#ifndef GETCOORDINATES_H
#define GETCOORDINATES_H

#include <QObject>

class GetCoordinates : public QObject
{
    Q_OBJECT
public:
    explicit GetCoordinates(QObject *parent = 0);
    ~GetCoordinates();
    
signals:
    
public slots:
    
};

#endif // GETCOORDINATES_H
